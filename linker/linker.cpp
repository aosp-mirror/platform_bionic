/*
 * Copyright (C) 2008, 2009 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/atomics.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// Private C library headers.
#include "private/bionic_tls.h"
#include "private/KernelArgumentBlock.h"
#include "private/ScopedPthreadMutexLocker.h"

#include "linker.h"
#include "linker_debug.h"
#include "linker_environ.h"
#include "linker_phdr.h"

/* >>> IMPORTANT NOTE - READ ME BEFORE MODIFYING <<<
 *
 * Do NOT use malloc() and friends or pthread_*() code here.
 * Don't use printf() either; it's caused mysterious memory
 * corruption in the past.
 * The linker runs before we bring up libc and it's easiest
 * to make sure it does not depend on any complex libc features
 *
 * open issues / todo:
 *
 * - are we doing everything we should for ARM_COPY relocations?
 * - cleaner error reporting
 * - after linking, set as much stuff as possible to READONLY
 *   and NOEXEC
 */

static bool soinfo_link_image(soinfo* si);
static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf);

// We can't use malloc(3) in the dynamic linker. We use a linked list of anonymous
// maps, each a single page in size. The pages are broken up into as many struct soinfo
// objects as will fit, and they're all threaded together on a free list.
#define SOINFO_PER_POOL ((PAGE_SIZE - sizeof(soinfo_pool_t*)) / sizeof(soinfo))
struct soinfo_pool_t {
  soinfo_pool_t* next;
  soinfo info[SOINFO_PER_POOL];
};
static struct soinfo_pool_t* gSoInfoPools = NULL;
static soinfo* gSoInfoFreeList = NULL;

static soinfo* solist = &libdl_info;
static soinfo* sonext = &libdl_info;
static soinfo* somain; /* main process, always the one after libdl_info */

static const char* const gDefaultLdPaths[] = {
#if defined(__LP64__)
  "/vendor/lib64",
  "/system/lib64",
#else
  "/vendor/lib",
  "/system/lib",
#endif
  NULL
};

#define LDPATH_BUFSIZE (LDPATH_MAX*64)
#define LDPATH_MAX 8

#define LDPRELOAD_BUFSIZE (LDPRELOAD_MAX*64)
#define LDPRELOAD_MAX 8

static char gLdPathsBuffer[LDPATH_BUFSIZE];
static const char* gLdPaths[LDPATH_MAX + 1];

static char gLdPreloadsBuffer[LDPRELOAD_BUFSIZE];
static const char* gLdPreloadNames[LDPRELOAD_MAX + 1];

static soinfo* gLdPreloads[LDPRELOAD_MAX + 1];

__LIBC_HIDDEN__ int gLdDebugVerbosity;

__LIBC_HIDDEN__ abort_msg_t* gAbortMessage = NULL; // For debuggerd.

enum RelocationKind {
    kRelocAbsolute = 0,
    kRelocRelative,
    kRelocCopy,
    kRelocSymbol,
    kRelocMax
};

#if STATS
struct linker_stats_t {
    int count[kRelocMax];
};

static linker_stats_t linker_stats;

static void count_relocation(RelocationKind kind) {
    ++linker_stats.count[kind];
}
#else
static void count_relocation(RelocationKind) {
}
#endif

#if COUNT_PAGES
static unsigned bitmask[4096];
#if defined(__LP64__)
#define MARK(offset) \
    do { \
        if ((((offset) >> 12) >> 5) < 4096) \
            bitmask[((offset) >> 12) >> 5] |= (1 << (((offset) >> 12) & 31)); \
    } while (0)
#else
#define MARK(offset) \
    do { \
        bitmask[((offset) >> 12) >> 3] |= (1 << (((offset) >> 12) & 7)); \
    } while (0)
#endif
#else
#define MARK(x) do {} while (0)
#endif

// You shouldn't try to call memory-allocating functions in the dynamic linker.
// Guard against the most obvious ones.
#define DISALLOW_ALLOCATION(return_type, name, ...) \
    return_type name __VA_ARGS__ \
    { \
        const char* msg = "ERROR: " #name " called from the dynamic linker!\n"; \
        __libc_format_log(ANDROID_LOG_FATAL, "linker", "%s", msg); \
        write(2, msg, strlen(msg)); \
        abort(); \
    }
#define UNUSED __attribute__((unused))
DISALLOW_ALLOCATION(void*, malloc, (size_t u UNUSED));
DISALLOW_ALLOCATION(void, free, (void* u UNUSED));
DISALLOW_ALLOCATION(void*, realloc, (void* u1 UNUSED, size_t u2 UNUSED));
DISALLOW_ALLOCATION(void*, calloc, (size_t u1 UNUSED, size_t u2 UNUSED));

static char tmp_err_buf[768];
static char __linker_dl_err_buf[768];

char* linker_get_error_buffer() {
  return &__linker_dl_err_buf[0];
}

size_t linker_get_error_buffer_size() {
  return sizeof(__linker_dl_err_buf);
}

/*
 * This function is an empty stub where GDB locates a breakpoint to get notified
 * about linker activity.
 */
extern "C" void __attribute__((noinline)) __attribute__((visibility("default"))) rtld_db_dlactivity();

static r_debug _r_debug = {1, NULL, reinterpret_cast<uintptr_t>(&rtld_db_dlactivity), r_debug::RT_CONSISTENT, 0};
static link_map* r_debug_tail = 0;

static pthread_mutex_t gDebugMutex = PTHREAD_MUTEX_INITIALIZER;

static void insert_soinfo_into_debug_map(soinfo* info) {
    // Copy the necessary fields into the debug structure.
    link_map* map = &(info->link_map_head);
    map->l_addr = info->load_bias;
    map->l_name = reinterpret_cast<char*>(info->name);
    map->l_ld = info->dynamic;

    /* Stick the new library at the end of the list.
     * gdb tends to care more about libc than it does
     * about leaf libraries, and ordering it this way
     * reduces the back-and-forth over the wire.
     */
    if (r_debug_tail) {
        r_debug_tail->l_next = map;
        map->l_prev = r_debug_tail;
        map->l_next = 0;
    } else {
        _r_debug.r_map = map;
        map->l_prev = 0;
        map->l_next = 0;
    }
    r_debug_tail = map;
}

static void remove_soinfo_from_debug_map(soinfo* info) {
    link_map* map = &(info->link_map_head);

    if (r_debug_tail == map) {
        r_debug_tail = map->l_prev;
    }

    if (map->l_prev) {
        map->l_prev->l_next = map->l_next;
    }
    if (map->l_next) {
        map->l_next->l_prev = map->l_prev;
    }
}

static void notify_gdb_of_load(soinfo* info) {
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    ScopedPthreadMutexLocker locker(&gDebugMutex);

    _r_debug.r_state = r_debug::RT_ADD;
    rtld_db_dlactivity();

    insert_soinfo_into_debug_map(info);

    _r_debug.r_state = r_debug::RT_CONSISTENT;
    rtld_db_dlactivity();
}

static void notify_gdb_of_unload(soinfo* info) {
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    ScopedPthreadMutexLocker locker(&gDebugMutex);

    _r_debug.r_state = r_debug::RT_DELETE;
    rtld_db_dlactivity();

    remove_soinfo_from_debug_map(info);

    _r_debug.r_state = r_debug::RT_CONSISTENT;
    rtld_db_dlactivity();
}

void notify_gdb_of_libraries() {
  _r_debug.r_state = r_debug::RT_ADD;
  rtld_db_dlactivity();
  _r_debug.r_state = r_debug::RT_CONSISTENT;
  rtld_db_dlactivity();
}

static bool ensure_free_list_non_empty() {
  if (gSoInfoFreeList != NULL) {
    return true;
  }

  // Allocate a new pool.
  soinfo_pool_t* pool = reinterpret_cast<soinfo_pool_t*>(mmap(NULL, sizeof(*pool),
                                                              PROT_READ|PROT_WRITE,
                                                              MAP_PRIVATE|MAP_ANONYMOUS, 0, 0));
  if (pool == MAP_FAILED) {
    return false;
  }

  // Add the pool to our list of pools.
  pool->next = gSoInfoPools;
  gSoInfoPools = pool;

  // Chain the entries in the new pool onto the free list.
  gSoInfoFreeList = &pool->info[0];
  soinfo* next = NULL;
  for (int i = SOINFO_PER_POOL - 1; i >= 0; --i) {
    pool->info[i].next = next;
    next = &pool->info[i];
  }

  return true;
}

static void set_soinfo_pool_protection(int protection) {
  for (soinfo_pool_t* p = gSoInfoPools; p != NULL; p = p->next) {
    if (mprotect(p, sizeof(*p), protection) == -1) {
      abort(); // Can't happen.
    }
  }
}

static soinfo* soinfo_alloc(const char* name) {
  if (strlen(name) >= SOINFO_NAME_LEN) {
    DL_ERR("library name \"%s\" too long", name);
    return NULL;
  }

  if (!ensure_free_list_non_empty()) {
    DL_ERR("out of memory when loading \"%s\"", name);
    return NULL;
  }

  // Take the head element off the free list.
  soinfo* si = gSoInfoFreeList;
  gSoInfoFreeList = gSoInfoFreeList->next;

  // Initialize the new element.
  memset(si, 0, sizeof(soinfo));
  strlcpy(si->name, name, sizeof(si->name));
  sonext->next = si;
  sonext = si;

  TRACE("name %s: allocated soinfo @ %p", name, si);
  return si;
}

static void soinfo_free(soinfo* si) {
    if (si == NULL) {
        return;
    }

    soinfo *prev = NULL, *trav;

    TRACE("name %s: freeing soinfo @ %p", si->name, si);

    for (trav = solist; trav != NULL; trav = trav->next) {
        if (trav == si)
            break;
        prev = trav;
    }
    if (trav == NULL) {
        /* si was not in solist */
        DL_ERR("name \"%s\" is not in solist!", si->name);
        return;
    }

    /* prev will never be NULL, because the first entry in solist is
       always the static libdl_info.
    */
    prev->next = si->next;
    if (si == sonext) {
        sonext = prev;
    }
    si->next = gSoInfoFreeList;
    gSoInfoFreeList = si;
}


static void parse_path(const char* path, const char* delimiters,
                       const char** array, char* buf, size_t buf_size, size_t max_count) {
  if (path == NULL) {
    return;
  }

  size_t len = strlcpy(buf, path, buf_size);

  size_t i = 0;
  char* buf_p = buf;
  while (i < max_count && (array[i] = strsep(&buf_p, delimiters))) {
    if (*array[i] != '\0') {
      ++i;
    }
  }

  // Forget the last path if we had to truncate; this occurs if the 2nd to
  // last char isn't '\0' (i.e. wasn't originally a delimiter).
  if (i > 0 && len >= buf_size && buf[buf_size - 2] != '\0') {
    array[i - 1] = NULL;
  } else {
    array[i] = NULL;
  }
}

static void parse_LD_LIBRARY_PATH(const char* path) {
  parse_path(path, ":", gLdPaths,
             gLdPathsBuffer, sizeof(gLdPathsBuffer), LDPATH_MAX);
}

static void parse_LD_PRELOAD(const char* path) {
  // We have historically supported ':' as well as ' ' in LD_PRELOAD.
  parse_path(path, " :", gLdPreloadNames,
             gLdPreloadsBuffer, sizeof(gLdPreloadsBuffer), LDPRELOAD_MAX);
}

#if defined(__arm__)

/* For a given PC, find the .so that it belongs to.
 * Returns the base address of the .ARM.exidx section
 * for that .so, and the number of 8-byte entries
 * in that section (via *pcount).
 *
 * Intended to be called by libc's __gnu_Unwind_Find_exidx().
 *
 * This function is exposed via dlfcn.cpp and libdl.so.
 */
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) {
    unsigned addr = (unsigned)pc;

    for (soinfo* si = solist; si != 0; si = si->next) {
        if ((addr >= si->base) && (addr < (si->base + si->size))) {
            *pcount = si->ARM_exidx_count;
            return (_Unwind_Ptr)si->ARM_exidx;
        }
    }
    *pcount = 0;
    return NULL;
}

#endif

/* Here, we only have to provide a callback to iterate across all the
 * loaded libraries. gcc_eh does the rest. */
int dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data) {
    int rv = 0;
    for (soinfo* si = solist; si != NULL; si = si->next) {
        dl_phdr_info dl_info;
        dl_info.dlpi_addr = si->link_map_head.l_addr;
        dl_info.dlpi_name = si->link_map_head.l_name;
        dl_info.dlpi_phdr = si->phdr;
        dl_info.dlpi_phnum = si->phnum;
        rv = cb(&dl_info, sizeof(dl_phdr_info), data);
        if (rv != 0) {
            break;
        }
    }
    return rv;
}

static ElfW(Sym)* soinfo_elf_lookup(soinfo* si, unsigned hash, const char* name) {
  ElfW(Sym)* symtab = si->symtab;
  const char* strtab = si->strtab;

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p %x %zd",
             name, si->name, reinterpret_cast<void*>(si->base), hash, hash % si->nbucket);

  for (unsigned n = si->bucket[hash % si->nbucket]; n != 0; n = si->chain[n]) {
    ElfW(Sym)* s = symtab + n;
    if (strcmp(strtab + s->st_name, name)) continue;

    /* only concern ourselves with global and weak symbol definitions */
    switch (ELF_ST_BIND(s->st_info)) {
      case STB_GLOBAL:
      case STB_WEAK:
        if (s->st_shndx == SHN_UNDEF) {
        continue;
      }

      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
                 name, si->name, reinterpret_cast<void*>(s->st_value),
                 static_cast<size_t>(s->st_size));
      return s;
    }
  }

  return NULL;
}

static unsigned elfhash(const char* _name) {
    const unsigned char* name = reinterpret_cast<const unsigned char*>(_name);
    unsigned h = 0, g;

    while (*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}

static ElfW(Sym)* soinfo_do_lookup(soinfo* si, const char* name, soinfo** lsi, soinfo* needed[]) {
    unsigned elf_hash = elfhash(name);
    ElfW(Sym)* s = NULL;

    if (si != NULL && somain != NULL) {
        /*
         * Local scope is executable scope. Just start looking into it right away
         * for the shortcut.
         */

        if (si == somain) {
            s = soinfo_elf_lookup(si, elf_hash, name);
            if (s != NULL) {
                *lsi = si;
                goto done;
            }
        } else {
            /* Order of symbol lookup is controlled by DT_SYMBOLIC flag */

            /*
             * If this object was built with symbolic relocations disabled, the
             * first place to look to resolve external references is the main
             * executable.
             */

            if (!si->has_DT_SYMBOLIC) {
                DEBUG("%s: looking up %s in executable %s",
                      si->name, name, somain->name);
                s = soinfo_elf_lookup(somain, elf_hash, name);
                if (s != NULL) {
                    *lsi = somain;
                    goto done;
                }
            }

            /* Look for symbols in the local scope (the object who is
             * searching). This happens with C++ templates on x86 for some
             * reason.
             *
             * Notes on weak symbols:
             * The ELF specs are ambiguous about treatment of weak definitions in
             * dynamic linking.  Some systems return the first definition found
             * and some the first non-weak definition.   This is system dependent.
             * Here we return the first definition found for simplicity.  */

            s = soinfo_elf_lookup(si, elf_hash, name);
            if (s != NULL) {
                *lsi = si;
                goto done;
            }

            /*
             * If this object was built with -Bsymbolic and symbol is not found
             * in the local scope, try to find the symbol in the main executable.
             */

            if (si->has_DT_SYMBOLIC) {
                DEBUG("%s: looking up %s in executable %s after local scope",
                      si->name, name, somain->name);
                s = soinfo_elf_lookup(somain, elf_hash, name);
                if (s != NULL) {
                    *lsi = somain;
                    goto done;
                }
            }
        }
    }

    /* Next, look for it in the preloads list */
    for (int i = 0; gLdPreloads[i] != NULL; i++) {
        s = soinfo_elf_lookup(gLdPreloads[i], elf_hash, name);
        if (s != NULL) {
            *lsi = gLdPreloads[i];
            goto done;
        }
    }

    for (int i = 0; needed[i] != NULL; i++) {
        DEBUG("%s: looking up %s in %s",
              si->name, name, needed[i]->name);
        s = soinfo_elf_lookup(needed[i], elf_hash, name);
        if (s != NULL) {
            *lsi = needed[i];
            goto done;
        }
    }

done:
    if (s != NULL) {
        TRACE_TYPE(LOOKUP, "si %s sym %s s->st_value = %p, "
                   "found in %s, base = %p, load bias = %p",
                   si->name, name, reinterpret_cast<void*>(s->st_value),
                   (*lsi)->name, reinterpret_cast<void*>((*lsi)->base),
                   reinterpret_cast<void*>((*lsi)->load_bias));
        return s;
    }

    return NULL;
}

/* This is used by dlsym(3).  It performs symbol lookup only within the
   specified soinfo object and not in any of its dependencies.

   TODO: Only looking in the specified soinfo seems wrong. dlsym(3) says
   that it should do a breadth first search through the dependency
   tree. This agrees with the ELF spec (aka System V Application
   Binary Interface) where in Chapter 5 it discuss resolving "Shared
   Object Dependencies" in breadth first search order.
 */
ElfW(Sym)* dlsym_handle_lookup(soinfo* si, const char* name) {
    return soinfo_elf_lookup(si, elfhash(name), name);
}

/* This is used by dlsym(3) to performs a global symbol lookup. If the
   start value is null (for RTLD_DEFAULT), the search starts at the
   beginning of the global solist. Otherwise the search starts at the
   specified soinfo (for RTLD_NEXT).
 */
ElfW(Sym)* dlsym_linear_lookup(const char* name, soinfo** found, soinfo* start) {
  unsigned elf_hash = elfhash(name);

  if (start == NULL) {
    start = solist;
  }

  ElfW(Sym)* s = NULL;
  for (soinfo* si = start; (s == NULL) && (si != NULL); si = si->next) {
    s = soinfo_elf_lookup(si, elf_hash, name);
    if (s != NULL) {
      *found = si;
      break;
    }
  }

  if (s != NULL) {
    TRACE_TYPE(LOOKUP, "%s s->st_value = %p, found->base = %p",
               name, reinterpret_cast<void*>(s->st_value), reinterpret_cast<void*>((*found)->base));
  }

  return s;
}

soinfo* find_containing_library(const void* p) {
  ElfW(Addr) address = reinterpret_cast<ElfW(Addr)>(p);
  for (soinfo* si = solist; si != NULL; si = si->next) {
    if (address >= si->base && address - si->base < si->size) {
      return si;
    }
  }
  return NULL;
}

ElfW(Sym)* dladdr_find_symbol(soinfo* si, const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - si->base;

  // Search the library's symbol table for any defined symbol which
  // contains this address.
  for (size_t i = 0; i < si->nchain; ++i) {
    ElfW(Sym)* sym = &si->symtab[i];
    if (sym->st_shndx != SHN_UNDEF &&
        soaddr >= sym->st_value &&
        soaddr < sym->st_value + sym->st_size) {
      return sym;
    }
  }

  return NULL;
}

static int open_library_on_path(const char* name, const char* const paths[]) {
  char buf[512];
  for (size_t i = 0; paths[i] != NULL; ++i) {
    int n = __libc_format_buffer(buf, sizeof(buf), "%s/%s", paths[i], name);
    if (n < 0 || n >= static_cast<int>(sizeof(buf))) {
      PRINT("Warning: ignoring very long library path: %s/%s", paths[i], name);
      continue;
    }
    int fd = TEMP_FAILURE_RETRY(open(buf, O_RDONLY | O_CLOEXEC));
    if (fd != -1) {
      return fd;
    }
  }
  return -1;
}

static int open_library(const char* name) {
  TRACE("[ opening %s ]", name);

  // If the name contains a slash, we should attempt to open it directly and not search the paths.
  if (strchr(name, '/') != NULL) {
    int fd = TEMP_FAILURE_RETRY(open(name, O_RDONLY | O_CLOEXEC));
    if (fd != -1) {
      return fd;
    }
    // ...but nvidia binary blobs (at least) rely on this behavior, so fall through for now.
  }

  // Otherwise we try LD_LIBRARY_PATH first, and fall back to the built-in well known paths.
  int fd = open_library_on_path(name, gLdPaths);
  if (fd == -1) {
    fd = open_library_on_path(name, gDefaultLdPaths);
  }
  return fd;
}

static soinfo* load_library(const char* name) {
    // Open the file.
    int fd = open_library(name);
    if (fd == -1) {
        DL_ERR("library \"%s\" not found", name);
        return NULL;
    }

    // Read the ELF header and load the segments.
    ElfReader elf_reader(name, fd);
    if (!elf_reader.Load()) {
        return NULL;
    }

    const char* bname = strrchr(name, '/');
    soinfo* si = soinfo_alloc(bname ? bname + 1 : name);
    if (si == NULL) {
        return NULL;
    }
    si->base = elf_reader.load_start();
    si->size = elf_reader.load_size();
    si->load_bias = elf_reader.load_bias();
    si->flags = 0;
    si->entry = 0;
    si->dynamic = NULL;
    si->phnum = elf_reader.phdr_count();
    si->phdr = elf_reader.loaded_phdr();
    return si;
}

static soinfo *find_loaded_library(const char* name) {
    // TODO: don't use basename only for determining libraries
    // http://code.google.com/p/android/issues/detail?id=6670

    const char* bname = strrchr(name, '/');
    bname = bname ? bname + 1 : name;

    for (soinfo* si = solist; si != NULL; si = si->next) {
        if (!strcmp(bname, si->name)) {
            return si;
        }
    }
    return NULL;
}

static soinfo* find_library_internal(const char* name) {
  if (name == NULL) {
    return somain;
  }

  soinfo* si = find_loaded_library(name);
  if (si != NULL) {
    if (si->flags & FLAG_LINKED) {
      return si;
    }
    DL_ERR("OOPS: recursive link to \"%s\"", si->name);
    return NULL;
  }

  TRACE("[ '%s' has not been loaded yet.  Locating...]", name);
  si = load_library(name);
  if (si == NULL) {
    return NULL;
  }

  // At this point we know that whatever is loaded @ base is a valid ELF
  // shared library whose segments are properly mapped in.
  TRACE("[ find_library_internal base=%p size=%zu name='%s' ]",
        reinterpret_cast<void*>(si->base), si->size, si->name);

  if (!soinfo_link_image(si)) {
    munmap(reinterpret_cast<void*>(si->base), si->size);
    soinfo_free(si);
    return NULL;
  }

  return si;
}

static soinfo* find_library(const char* name) {
  soinfo* si = find_library_internal(name);
  if (si != NULL) {
    si->ref_count++;
  }
  return si;
}

static int soinfo_unload(soinfo* si) {
  if (si->ref_count == 1) {
    TRACE("unloading '%s'", si->name);
    si->CallDestructors();

    for (ElfW(Dyn)* d = si->dynamic; d->d_tag != DT_NULL; ++d) {
      if (d->d_tag == DT_NEEDED) {
        const char* library_name = si->strtab + d->d_un.d_val;
        TRACE("%s needs to unload %s", si->name, library_name);
        soinfo_unload(find_loaded_library(library_name));
      }
    }

    munmap(reinterpret_cast<void*>(si->base), si->size);
    notify_gdb_of_unload(si);
    soinfo_free(si);
    si->ref_count = 0;
  } else {
    si->ref_count--;
    TRACE("not unloading '%s', decrementing ref_count to %zd", si->name, si->ref_count);
  }
  return 0;
}

void do_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  snprintf(buffer, buffer_size, "%s:%s", gDefaultLdPaths[0], gDefaultLdPaths[1]);
}

void do_android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  if (!get_AT_SECURE()) {
    parse_LD_LIBRARY_PATH(ld_library_path);
  }
}

soinfo* do_dlopen(const char* name, int flags) {
  if ((flags & ~(RTLD_NOW|RTLD_LAZY|RTLD_LOCAL|RTLD_GLOBAL)) != 0) {
    DL_ERR("invalid flags to dlopen: %x", flags);
    return NULL;
  }
  set_soinfo_pool_protection(PROT_READ | PROT_WRITE);
  soinfo* si = find_library(name);
  if (si != NULL) {
    si->CallConstructors();
  }
  set_soinfo_pool_protection(PROT_READ);
  return si;
}

int do_dlclose(soinfo* si) {
  set_soinfo_pool_protection(PROT_READ | PROT_WRITE);
  int result = soinfo_unload(si);
  set_soinfo_pool_protection(PROT_READ);
  return result;
}

#if defined(USE_RELA)
static int soinfo_relocate(soinfo* si, ElfW(Rela)* rela, unsigned count, soinfo* needed[]) {
  ElfW(Sym)* s;
  soinfo* lsi;

  for (size_t idx = 0; idx < count; ++idx, ++rela) {
    unsigned type = ELFW(R_TYPE)(rela->r_info);
    unsigned sym = ELFW(R_SYM)(rela->r_info);
    ElfW(Addr) reloc = static_cast<ElfW(Addr)>(rela->r_offset + si->load_bias);
    ElfW(Addr) sym_addr = 0;
    const char* sym_name = NULL;

    DEBUG("Processing '%s' relocation at index %zd", si->name, idx);
    if (type == 0) { // R_*_NONE
      continue;
    }
    if (sym != 0) {
      sym_name = reinterpret_cast<const char*>(si->strtab + si->symtab[sym].st_name);
      s = soinfo_do_lookup(si, sym_name, &lsi, needed);
      if (s == NULL) {
        // We only allow an undefined symbol if this is a weak reference...
        s = &si->symtab[sym];
        if (ELF_ST_BIND(s->st_info) != STB_WEAK) {
          DL_ERR("cannot locate symbol \"%s\" referenced by \"%s\"...", sym_name, si->name);
          return -1;
        }

        /* IHI0044C AAELF 4.5.1.1:

           Libraries are not searched to resolve weak references.
           It is not an error for a weak reference to remain unsatisfied.

           During linking, the value of an undefined weak reference is:
           - Zero if the relocation type is absolute
           - The address of the place if the relocation is pc-relative
           - The address of nominal base address if the relocation
             type is base-relative.
         */

        switch (type) {
#if defined(__aarch64__)
        case R_AARCH64_JUMP_SLOT:
        case R_AARCH64_GLOB_DAT:
        case R_AARCH64_ABS64:
        case R_AARCH64_ABS32:
        case R_AARCH64_ABS16:
        case R_AARCH64_RELATIVE:
          /*
           * The sym_addr was initialized to be zero above, or the relocation
           * code below does not care about value of sym_addr.
           * No need to do anything.
           */
          break;
#elif defined(__x86_64__)
        case R_X86_64_JUMP_SLOT:
        case R_X86_64_GLOB_DAT:
        case R_X86_64_32:
        case R_X86_64_RELATIVE:
          // No need to do anything.
          break;
        case R_X86_64_PC32:
          sym_addr = reloc;
          break;
#endif
        default:
          DL_ERR("unknown weak reloc type %d @ %p (%zu)", type, rela, idx);
          return -1;
        }
      } else {
        // We got a definition.
        sym_addr = static_cast<ElfW(Addr)>(s->st_value + lsi->load_bias);
      }
      count_relocation(kRelocSymbol);
    } else {
      s = NULL;
    }

    switch (type) {
#if defined(__aarch64__)
    case R_AARCH64_JUMP_SLOT:
        count_relocation(kRelocAbsolute);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO JMP_SLOT %16llx <- %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + rela->r_addend);
        break;
    case R_AARCH64_GLOB_DAT:
        count_relocation(kRelocAbsolute);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO GLOB_DAT %16llx <- %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + rela->r_addend);
        break;
    case R_AARCH64_ABS64:
        count_relocation(kRelocAbsolute);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO ABS64 %16llx <- %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + rela->r_addend);
        break;
    case R_AARCH64_ABS32:
        count_relocation(kRelocAbsolute);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO ABS32 %16llx <- %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), sym_name);
        if ((static_cast<ElfW(Addr)>(INT32_MIN) <= (*reinterpret_cast<ElfW(Addr)*>(reloc) + (sym_addr + rela->r_addend))) &&
            ((*reinterpret_cast<ElfW(Addr)*>(reloc) + (sym_addr + rela->r_addend)) <= static_cast<ElfW(Addr)>(UINT32_MAX))) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + rela->r_addend);
        } else {
            DL_ERR("0x%016llx out of range 0x%016llx to 0x%016llx",
                   (*reinterpret_cast<ElfW(Addr)*>(reloc) + (sym_addr + rela->r_addend)),
                   static_cast<ElfW(Addr)>(INT32_MIN),
                   static_cast<ElfW(Addr)>(UINT32_MAX));
            return -1;
        }
        break;
    case R_AARCH64_ABS16:
        count_relocation(kRelocAbsolute);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO ABS16 %16llx <- %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), sym_name);
        if ((static_cast<ElfW(Addr)>(INT16_MIN) <= (*reinterpret_cast<ElfW(Addr)*>(reloc) + (sym_addr + rela->r_addend))) &&
            ((*reinterpret_cast<ElfW(Addr)*>(reloc) + (sym_addr + rela->r_addend)) <= static_cast<ElfW(Addr)>(UINT16_MAX))) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + rela->r_addend);
        } else {
            DL_ERR("0x%016llx out of range 0x%016llx to 0x%016llx",
                   (*reinterpret_cast<ElfW(Addr)*>(reloc) + (sym_addr + rela->r_addend)),
                   static_cast<ElfW(Addr)>(INT16_MIN),
                   static_cast<ElfW(Addr)>(UINT16_MAX));
            return -1;
        }
        break;
    case R_AARCH64_PREL64:
        count_relocation(kRelocRelative);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO REL64 %16llx <- %16llx - %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), rela->r_offset, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + rela->r_addend) - rela->r_offset;
        break;
    case R_AARCH64_PREL32:
        count_relocation(kRelocRelative);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO REL32 %16llx <- %16llx - %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), rela->r_offset, sym_name);
        if ((static_cast<ElfW(Addr)>(INT32_MIN) <= (*reinterpret_cast<ElfW(Addr)*>(reloc) + ((sym_addr + rela->r_addend) - rela->r_offset))) &&
            ((*reinterpret_cast<ElfW(Addr)*>(reloc) + ((sym_addr + rela->r_addend) - rela->r_offset)) <= static_cast<ElfW(Addr)>(UINT32_MAX))) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += ((sym_addr + rela->r_addend) - rela->r_offset);
        } else {
            DL_ERR("0x%016llx out of range 0x%016llx to 0x%016llx",
                   (*reinterpret_cast<ElfW(Addr)*>(reloc) + ((sym_addr + rela->r_addend) - rela->r_offset)),
                   static_cast<ElfW(Addr)>(INT32_MIN),
                   static_cast<ElfW(Addr)>(UINT32_MAX));
            return -1;
        }
        break;
    case R_AARCH64_PREL16:
        count_relocation(kRelocRelative);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO REL16 %16llx <- %16llx - %16llx %s\n",
                   reloc, (sym_addr + rela->r_addend), rela->r_offset, sym_name);
        if ((static_cast<ElfW(Addr)>(INT16_MIN) <= (*reinterpret_cast<ElfW(Addr)*>(reloc) + ((sym_addr + rela->r_addend) - rela->r_offset))) &&
            ((*reinterpret_cast<ElfW(Addr)*>(reloc) + ((sym_addr + rela->r_addend) - rela->r_offset)) <= static_cast<ElfW(Addr)>(UINT16_MAX))) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += ((sym_addr + rela->r_addend) - rela->r_offset);
        } else {
            DL_ERR("0x%016llx out of range 0x%016llx to 0x%016llx",
                   (*reinterpret_cast<ElfW(Addr)*>(reloc) + ((sym_addr + rela->r_addend) - rela->r_offset)),
                   static_cast<ElfW(Addr)>(INT16_MIN),
                   static_cast<ElfW(Addr)>(UINT16_MAX));
            return -1;
        }
        break;

    case R_AARCH64_RELATIVE:
        count_relocation(kRelocRelative);
        MARK(rela->r_offset);
        if (sym) {
            DL_ERR("odd RELATIVE form...");
            return -1;
        }
        TRACE_TYPE(RELO, "RELO RELATIVE %16llx <- %16llx\n",
                   reloc, (si->base + rela->r_addend));
        *reinterpret_cast<ElfW(Addr)*>(reloc) = (si->base + rela->r_addend);
        break;

    case R_AARCH64_COPY:
        if ((si->flags & FLAG_EXE) == 0) {
            /*
              * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0044d/IHI0044D_aaelf.pdf
              *
              * Section 4.7.1.10 "Dynamic relocations"
              * R_AARCH64_COPY may only appear in executable objects where e_type is
              * set to ET_EXEC.
              *
              * FLAG_EXE is set for both ET_DYN and ET_EXEC executables.
              * We should explicitly disallow ET_DYN executables from having
              * R_AARCH64_COPY relocations.
              */
            DL_ERR("%s R_AARCH64_COPY relocations only supported for ET_EXEC", si->name);
            return -1;
        }
        count_relocation(kRelocCopy);
        MARK(rela->r_offset);
        TRACE_TYPE(RELO, "RELO COPY %16llx <- %lld @ %16llx %s\n",
                   reloc,
                   s->st_size,
                   (sym_addr + rela->r_addend),
                   sym_name);
        if (reloc == (sym_addr + rela->r_addend)) {
            ElfW(Sym)* src = soinfo_do_lookup(NULL, sym_name, &lsi, needed);

            if (src == NULL) {
                DL_ERR("%s R_AARCH64_COPY relocation source cannot be resolved", si->name);
                return -1;
            }
            if (lsi->has_DT_SYMBOLIC) {
                DL_ERR("%s invalid R_AARCH64_COPY relocation against DT_SYMBOLIC shared "
                       "library %s (built with -Bsymbolic?)", si->name, lsi->name);
                return -1;
            }
            if (s->st_size < src->st_size) {
                DL_ERR("%s R_AARCH64_COPY relocation size mismatch (%lld < %lld)",
                       si->name, s->st_size, src->st_size);
                return -1;
            }
            memcpy(reinterpret_cast<void*>(reloc),
                   reinterpret_cast<void*>(src->st_value + lsi->load_bias), src->st_size);
        } else {
            DL_ERR("%s R_AARCH64_COPY relocation target cannot be resolved", si->name);
            return -1;
        }
        break;
    case R_AARCH64_TLS_TPREL64:
        TRACE_TYPE(RELO, "RELO TLS_TPREL64 *** %16llx <- %16llx - %16llx\n",
                   reloc, (sym_addr + rela->r_addend), rela->r_offset);
        break;
    case R_AARCH64_TLS_DTPREL32:
        TRACE_TYPE(RELO, "RELO TLS_DTPREL32 *** %16llx <- %16llx - %16llx\n",
                   reloc, (sym_addr + rela->r_addend), rela->r_offset);
        break;
#elif defined(__x86_64__)
    case R_X86_64_JUMP_SLOT:
      count_relocation(kRelocAbsolute);
      MARK(rela->r_offset);
      TRACE_TYPE(RELO, "RELO JMP_SLOT %08zx <- %08zx %s", static_cast<size_t>(reloc),
                 static_cast<size_t>(sym_addr + rela->r_addend), sym_name);
      *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + rela->r_addend;
      break;
    case R_X86_64_GLOB_DAT:
      count_relocation(kRelocAbsolute);
      MARK(rela->r_offset);
      TRACE_TYPE(RELO, "RELO GLOB_DAT %08zx <- %08zx %s", static_cast<size_t>(reloc),
                 static_cast<size_t>(sym_addr + rela->r_addend), sym_name);
      *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + rela->r_addend;
      break;
    case R_X86_64_RELATIVE:
      count_relocation(kRelocRelative);
      MARK(rela->r_offset);
      if (sym) {
        DL_ERR("odd RELATIVE form...");
        return -1;
      }
      TRACE_TYPE(RELO, "RELO RELATIVE %08zx <- +%08zx", static_cast<size_t>(reloc),
                 static_cast<size_t>(si->base));
      *reinterpret_cast<ElfW(Addr)*>(reloc) = si->base + rela->r_addend;
      break;
    case R_X86_64_32:
      count_relocation(kRelocRelative);
      MARK(rela->r_offset);
      TRACE_TYPE(RELO, "RELO R_X86_64_32 %08zx <- +%08zx %s", static_cast<size_t>(reloc),
                 static_cast<size_t>(sym_addr), sym_name);
      *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + rela->r_addend;
      break;
    case R_X86_64_64:
      count_relocation(kRelocRelative);
      MARK(rela->r_offset);
      TRACE_TYPE(RELO, "RELO R_X86_64_64 %08zx <- +%08zx %s", static_cast<size_t>(reloc),
                 static_cast<size_t>(sym_addr), sym_name);
      *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + rela->r_addend;
      break;
    case R_X86_64_PC32:
      count_relocation(kRelocRelative);
      MARK(rela->r_offset);
      TRACE_TYPE(RELO, "RELO R_X86_64_PC32 %08zx <- +%08zx (%08zx - %08zx) %s",
                 static_cast<size_t>(reloc), static_cast<size_t>(sym_addr - reloc),
                 static_cast<size_t>(sym_addr), static_cast<size_t>(reloc), sym_name);
      *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + rela->r_addend - reloc;
      break;
#endif

    default:
      DL_ERR("unknown reloc type %d @ %p (%zu)", type, rela, idx);
      return -1;
    }
  }
  return 0;
}

#else // REL, not RELA.

static int soinfo_relocate(soinfo* si, ElfW(Rel)* rel, unsigned count, soinfo* needed[]) {
    ElfW(Sym)* s;
    soinfo* lsi;

    for (size_t idx = 0; idx < count; ++idx, ++rel) {
        unsigned type = ELFW(R_TYPE)(rel->r_info);
        // TODO: don't use unsigned for 'sym'. Use uint32_t or ElfW(Addr) instead.
        unsigned sym = ELFW(R_SYM)(rel->r_info);
        ElfW(Addr) reloc = static_cast<ElfW(Addr)>(rel->r_offset + si->load_bias);
        ElfW(Addr) sym_addr = 0;
        const char* sym_name = NULL;

        DEBUG("Processing '%s' relocation at index %zd", si->name, idx);
        if (type == 0) { // R_*_NONE
            continue;
        }
        if (sym != 0) {
            sym_name = reinterpret_cast<const char*>(si->strtab + si->symtab[sym].st_name);
            s = soinfo_do_lookup(si, sym_name, &lsi, needed);
            if (s == NULL) {
                // We only allow an undefined symbol if this is a weak reference...
                s = &si->symtab[sym];
                if (ELF_ST_BIND(s->st_info) != STB_WEAK) {
                    DL_ERR("cannot locate symbol \"%s\" referenced by \"%s\"...", sym_name, si->name);
                    return -1;
                }

                /* IHI0044C AAELF 4.5.1.1:

                   Libraries are not searched to resolve weak references.
                   It is not an error for a weak reference to remain
                   unsatisfied.

                   During linking, the value of an undefined weak reference is:
                   - Zero if the relocation type is absolute
                   - The address of the place if the relocation is pc-relative
                   - The address of nominal base address if the relocation
                     type is base-relative.
                  */

                switch (type) {
#if defined(__arm__)
                case R_ARM_JUMP_SLOT:
                case R_ARM_GLOB_DAT:
                case R_ARM_ABS32:
                case R_ARM_RELATIVE:    /* Don't care. */
                    // sym_addr was initialized to be zero above or relocation
                    // code below does not care about value of sym_addr.
                    // No need to do anything.
                    break;
#elif defined(__i386__)
                case R_386_JMP_SLOT:
                case R_386_GLOB_DAT:
                case R_386_32:
                case R_386_RELATIVE:    /* Don't care. */
                    // sym_addr was initialized to be zero above or relocation
                    // code below does not care about value of sym_addr.
                    // No need to do anything.
                    break;
                case R_386_PC32:
                    sym_addr = reloc;
                    break;
#endif

#if defined(__arm__)
                case R_ARM_COPY:
                    // Fall through. Can't really copy if weak symbol is not found at run-time.
#endif
                default:
                    DL_ERR("unknown weak reloc type %d @ %p (%zu)", type, rel, idx);
                    return -1;
                }
            } else {
                // We got a definition.
                sym_addr = static_cast<ElfW(Addr)>(s->st_value + lsi->load_bias);
            }
            count_relocation(kRelocSymbol);
        } else {
            s = NULL;
        }

        switch (type) {
#if defined(__arm__)
        case R_ARM_JUMP_SLOT:
            count_relocation(kRelocAbsolute);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO JMP_SLOT %08x <- %08x %s", reloc, sym_addr, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr;
            break;
        case R_ARM_GLOB_DAT:
            count_relocation(kRelocAbsolute);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO GLOB_DAT %08x <- %08x %s", reloc, sym_addr, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr;
            break;
        case R_ARM_ABS32:
            count_relocation(kRelocAbsolute);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO ABS %08x <- %08x %s", reloc, sym_addr, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr;
            break;
        case R_ARM_REL32:
            count_relocation(kRelocRelative);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO REL32 %08x <- %08x - %08x %s",
                       reloc, sym_addr, rel->r_offset, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr - rel->r_offset;
            break;
        case R_ARM_COPY:
            if ((si->flags & FLAG_EXE) == 0) {
                /*
                 * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0044d/IHI0044D_aaelf.pdf
                 *
                 * Section 4.7.1.10 "Dynamic relocations"
                 * R_ARM_COPY may only appear in executable objects where e_type is
                 * set to ET_EXEC.
                 *
                 * TODO: FLAG_EXE is set for both ET_DYN and ET_EXEC executables.
                 * We should explicitly disallow ET_DYN executables from having
                 * R_ARM_COPY relocations.
                 */
                DL_ERR("%s R_ARM_COPY relocations only supported for ET_EXEC", si->name);
                return -1;
            }
            count_relocation(kRelocCopy);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO %08x <- %d @ %08x %s", reloc, s->st_size, sym_addr, sym_name);
            if (reloc == sym_addr) {
                ElfW(Sym)* src = soinfo_do_lookup(NULL, sym_name, &lsi, needed);

                if (src == NULL) {
                    DL_ERR("%s R_ARM_COPY relocation source cannot be resolved", si->name);
                    return -1;
                }
                if (lsi->has_DT_SYMBOLIC) {
                    DL_ERR("%s invalid R_ARM_COPY relocation against DT_SYMBOLIC shared "
                           "library %s (built with -Bsymbolic?)", si->name, lsi->name);
                    return -1;
                }
                if (s->st_size < src->st_size) {
                    DL_ERR("%s R_ARM_COPY relocation size mismatch (%d < %d)",
                           si->name, s->st_size, src->st_size);
                    return -1;
                }
                memcpy(reinterpret_cast<void*>(reloc),
                       reinterpret_cast<void*>(src->st_value + lsi->load_bias), src->st_size);
            } else {
                DL_ERR("%s R_ARM_COPY relocation target cannot be resolved", si->name);
                return -1;
            }
            break;
#elif defined(__i386__)
        case R_386_JMP_SLOT:
            count_relocation(kRelocAbsolute);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO JMP_SLOT %08x <- %08x %s", reloc, sym_addr, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr;
            break;
        case R_386_GLOB_DAT:
            count_relocation(kRelocAbsolute);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO GLOB_DAT %08x <- %08x %s", reloc, sym_addr, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr;
            break;
        case R_386_32:
            count_relocation(kRelocRelative);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO R_386_32 %08x <- +%08x %s", reloc, sym_addr, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr;
            break;
        case R_386_PC32:
            count_relocation(kRelocRelative);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO R_386_PC32 %08x <- +%08x (%08x - %08x) %s",
                       reloc, (sym_addr - reloc), sym_addr, reloc, sym_name);
            *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr - reloc);
            break;
#elif defined(__mips__)
        case R_MIPS_REL32:
#if defined(__LP64__)
            // MIPS Elf64_Rel entries contain compound relocations
            // We only handle the R_MIPS_NONE|R_MIPS_64|R_MIPS_REL32 case
            if (ELF64_R_TYPE2(rel->r_info) != R_MIPS_64 ||
                ELF64_R_TYPE3(rel->r_info) != R_MIPS_NONE) {
                DL_ERR("Unexpected compound relocation type:%d type2:%d type3:%d @ %p (%zu)",
                       type, (unsigned)ELF64_R_TYPE2(rel->r_info),
                       (unsigned)ELF64_R_TYPE3(rel->r_info), rel, idx);
                return -1;
            }
#endif
            count_relocation(kRelocAbsolute);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "RELO REL32 %08zx <- %08zx %s", static_cast<size_t>(reloc),
                       static_cast<size_t>(sym_addr), sym_name ? sym_name : "*SECTIONHDR*");
            if (s) {
                *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr;
            } else {
                *reinterpret_cast<ElfW(Addr)*>(reloc) += si->base;
            }
            break;
#endif

#if defined(__arm__)
        case R_ARM_RELATIVE:
#elif defined(__i386__)
        case R_386_RELATIVE:
#endif
            count_relocation(kRelocRelative);
            MARK(rel->r_offset);
            if (sym) {
                DL_ERR("odd RELATIVE form...");
                return -1;
            }
            TRACE_TYPE(RELO, "RELO RELATIVE %p <- +%p",
                       reinterpret_cast<void*>(reloc), reinterpret_cast<void*>(si->base));
            *reinterpret_cast<ElfW(Addr)*>(reloc) += si->base;
            break;

        default:
            DL_ERR("unknown reloc type %d @ %p (%zu)", type, rel, idx);
            return -1;
        }
    }
    return 0;
}
#endif

#if defined(__mips__)
static bool mips_relocate_got(soinfo* si, soinfo* needed[]) {
    ElfW(Addr)** got = si->plt_got;
    if (got == NULL) {
        return true;
    }
    unsigned local_gotno = si->mips_local_gotno;
    unsigned gotsym = si->mips_gotsym;
    unsigned symtabno = si->mips_symtabno;
    ElfW(Sym)* symtab = si->symtab;

    // got[0] is the address of the lazy resolver function.
    // got[1] may be used for a GNU extension.
    // Set it to a recognizable address in case someone calls it (should be _rtld_bind_start).
    // FIXME: maybe this should be in a separate routine?
    if ((si->flags & FLAG_LINKER) == 0) {
        size_t g = 0;
        got[g++] = reinterpret_cast<ElfW(Addr)*>(0xdeadbeef);
        if (reinterpret_cast<intptr_t>(got[g]) < 0) {
            got[g++] = reinterpret_cast<ElfW(Addr)*>(0xdeadfeed);
        }
        // Relocate the local GOT entries.
        for (; g < local_gotno; g++) {
            got[g] = reinterpret_cast<ElfW(Addr)*>(reinterpret_cast<uintptr_t>(got[g]) + si->load_bias);
        }
    }

    // Now for the global GOT entries...
    ElfW(Sym)* sym = symtab + gotsym;
    got = si->plt_got + local_gotno;
    for (size_t g = gotsym; g < symtabno; g++, sym++, got++) {
        // This is an undefined reference... try to locate it.
        const char* sym_name = si->strtab + sym->st_name;
        soinfo* lsi;
        ElfW(Sym)* s = soinfo_do_lookup(si, sym_name, &lsi, needed);
        if (s == NULL) {
            // We only allow an undefined symbol if this is a weak reference.
            s = &symtab[g];
            if (ELF_ST_BIND(s->st_info) != STB_WEAK) {
                DL_ERR("cannot locate \"%s\"...", sym_name);
                return false;
            }
            *got = 0;
        } else {
            // FIXME: is this sufficient?
            // For reference see NetBSD link loader
            // http://cvsweb.netbsd.org/bsdweb.cgi/src/libexec/ld.elf_so/arch/mips/mips_reloc.c?rev=1.53&content-type=text/x-cvsweb-markup
            *got = reinterpret_cast<ElfW(Addr)*>(lsi->load_bias + s->st_value);
        }
    }
    return true;
}
#endif

void soinfo::CallArray(const char* array_name UNUSED, linker_function_t* functions, size_t count, bool reverse) {
  if (functions == NULL) {
    return;
  }

  TRACE("[ Calling %s (size %zd) @ %p for '%s' ]", array_name, count, functions, name);

  int begin = reverse ? (count - 1) : 0;
  int end = reverse ? -1 : count;
  int step = reverse ? -1 : 1;

  for (int i = begin; i != end; i += step) {
    TRACE("[ %s[%d] == %p ]", array_name, i, functions[i]);
    CallFunction("function", functions[i]);
  }

  TRACE("[ Done calling %s for '%s' ]", array_name, name);
}

void soinfo::CallFunction(const char* function_name UNUSED, linker_function_t function) {
  if (function == NULL || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling %s @ %p for '%s' ]", function_name, function, name);
  function();
  TRACE("[ Done calling %s @ %p for '%s' ]", function_name, function, name);

  // The function may have called dlopen(3) or dlclose(3), so we need to ensure our data structures
  // are still writable. This happens with our debug malloc (see http://b/7941716).
  set_soinfo_pool_protection(PROT_READ | PROT_WRITE);
}

void soinfo::CallPreInitConstructors() {
  // DT_PREINIT_ARRAY functions are called before any other constructors for executables,
  // but ignored in a shared library.
  CallArray("DT_PREINIT_ARRAY", preinit_array, preinit_array_count, false);
}

void soinfo::CallConstructors() {
  if (constructors_called) {
    return;
  }

  // We set constructors_called before actually calling the constructors, otherwise it doesn't
  // protect against recursive constructor calls. One simple example of constructor recursion
  // is the libc debug malloc, which is implemented in libc_malloc_debug_leak.so:
  // 1. The program depends on libc, so libc's constructor is called here.
  // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
  // 3. dlopen() calls the constructors on the newly created
  //    soinfo for libc_malloc_debug_leak.so.
  // 4. The debug .so depends on libc, so CallConstructors is
  //    called again with the libc soinfo. If it doesn't trigger the early-
  //    out above, the libc constructor will be called again (recursively!).
  constructors_called = true;

  if ((flags & FLAG_EXE) == 0 && preinit_array != NULL) {
    // The GNU dynamic linker silently ignores these, but we warn the developer.
    PRINT("\"%s\": ignoring %zd-entry DT_PREINIT_ARRAY in shared library!",
          name, preinit_array_count);
  }

  if (dynamic != NULL) {
    for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
      if (d->d_tag == DT_NEEDED) {
        const char* library_name = strtab + d->d_un.d_val;
        TRACE("\"%s\": calling constructors in DT_NEEDED \"%s\"", name, library_name);
        find_loaded_library(library_name)->CallConstructors();
      }
    }
  }

  TRACE("\"%s\": calling constructors", name);

  // DT_INIT should be called before DT_INIT_ARRAY if both are present.
  CallFunction("DT_INIT", init_func);
  CallArray("DT_INIT_ARRAY", init_array, init_array_count, false);
}

void soinfo::CallDestructors() {
  TRACE("\"%s\": calling destructors", name);

  // DT_FINI_ARRAY must be parsed in reverse order.
  CallArray("DT_FINI_ARRAY", fini_array, fini_array_count, true);

  // DT_FINI should be called after DT_FINI_ARRAY if both are present.
  CallFunction("DT_FINI", fini_func);
}

/* Force any of the closed stdin, stdout and stderr to be associated with
   /dev/null. */
static int nullify_closed_stdio() {
    int dev_null, i, status;
    int return_value = 0;

    dev_null = TEMP_FAILURE_RETRY(open("/dev/null", O_RDWR));
    if (dev_null < 0) {
        DL_ERR("cannot open /dev/null: %s", strerror(errno));
        return -1;
    }
    TRACE("[ Opened /dev/null file-descriptor=%d]", dev_null);

    /* If any of the stdio file descriptors is valid and not associated
       with /dev/null, dup /dev/null to it.  */
    for (i = 0; i < 3; i++) {
        /* If it is /dev/null already, we are done. */
        if (i == dev_null) {
            continue;
        }

        TRACE("[ Nullifying stdio file descriptor %d]", i);
        status = TEMP_FAILURE_RETRY(fcntl(i, F_GETFL));

        /* If file is opened, we are good. */
        if (status != -1) {
            continue;
        }

        /* The only error we allow is that the file descriptor does not
           exist, in which case we dup /dev/null to it. */
        if (errno != EBADF) {
            DL_ERR("fcntl failed: %s", strerror(errno));
            return_value = -1;
            continue;
        }

        /* Try dupping /dev/null to this stdio file descriptor and
           repeat if there is a signal.  Note that any errors in closing
           the stdio descriptor are lost.  */
        status = TEMP_FAILURE_RETRY(dup2(dev_null, i));
        if (status < 0) {
            DL_ERR("dup2 failed: %s", strerror(errno));
            return_value = -1;
            continue;
        }
    }

    /* If /dev/null is not one of the stdio file descriptors, close it. */
    if (dev_null > 2) {
        TRACE("[ Closing /dev/null file-descriptor=%d]", dev_null);
        status = TEMP_FAILURE_RETRY(close(dev_null));
        if (status == -1) {
            DL_ERR("close failed: %s", strerror(errno));
            return_value = -1;
        }
    }

    return return_value;
}

static bool soinfo_link_image(soinfo* si) {
    /* "base" might wrap around UINT32_MAX. */
    ElfW(Addr) base = si->load_bias;
    const ElfW(Phdr)* phdr = si->phdr;
    int phnum = si->phnum;
    bool relocating_linker = (si->flags & FLAG_LINKER) != 0;

    /* We can't debug anything until the linker is relocated */
    if (!relocating_linker) {
        INFO("[ linking %s ]", si->name);
        DEBUG("si->base = %p si->flags = 0x%08x", reinterpret_cast<void*>(si->base), si->flags);
    }

    /* Extract dynamic section */
    size_t dynamic_count;
    ElfW(Word) dynamic_flags;
    phdr_table_get_dynamic_section(phdr, phnum, base, &si->dynamic,
                                   &dynamic_count, &dynamic_flags);
    if (si->dynamic == NULL) {
        if (!relocating_linker) {
            DL_ERR("missing PT_DYNAMIC in \"%s\"", si->name);
        }
        return false;
    } else {
        if (!relocating_linker) {
            DEBUG("dynamic = %p", si->dynamic);
        }
    }

#if defined(__arm__)
    (void) phdr_table_get_arm_exidx(phdr, phnum, base,
                                    &si->ARM_exidx, &si->ARM_exidx_count);
#endif

    // Extract useful information from dynamic section.
    uint32_t needed_count = 0;
    for (ElfW(Dyn)* d = si->dynamic; d->d_tag != DT_NULL; ++d) {
        DEBUG("d = %p, d[0](tag) = %p d[1](val) = %p",
              d, reinterpret_cast<void*>(d->d_tag), reinterpret_cast<void*>(d->d_un.d_val));
        switch (d->d_tag) {
        case DT_HASH:
            si->nbucket = reinterpret_cast<uint32_t*>(base + d->d_un.d_ptr)[0];
            si->nchain = reinterpret_cast<uint32_t*>(base + d->d_un.d_ptr)[1];
            si->bucket = reinterpret_cast<uint32_t*>(base + d->d_un.d_ptr + 8);
            si->chain = reinterpret_cast<uint32_t*>(base + d->d_un.d_ptr + 8 + si->nbucket * 4);
            break;
        case DT_STRTAB:
            si->strtab = reinterpret_cast<const char*>(base + d->d_un.d_ptr);
            break;
        case DT_SYMTAB:
            si->symtab = reinterpret_cast<ElfW(Sym)*>(base + d->d_un.d_ptr);
            break;
#if !defined(__LP64__)
        case DT_PLTREL:
            if (d->d_un.d_val != DT_REL) {
                DL_ERR("unsupported DT_RELA in \"%s\"", si->name);
                return false;
            }
            break;
#endif
        case DT_JMPREL:
#if defined(USE_RELA)
            si->plt_rela = reinterpret_cast<ElfW(Rela)*>(base + d->d_un.d_ptr);
#else
            si->plt_rel = reinterpret_cast<ElfW(Rel)*>(base + d->d_un.d_ptr);
#endif
            break;
        case DT_PLTRELSZ:
#if defined(USE_RELA)
            si->plt_rela_count = d->d_un.d_val / sizeof(ElfW(Rela));
#else
            si->plt_rel_count = d->d_un.d_val / sizeof(ElfW(Rel));
#endif
            break;
#if defined(__mips__)
        case DT_PLTGOT:
            // Used by mips and mips64.
            si->plt_got = reinterpret_cast<ElfW(Addr)**>(base + d->d_un.d_ptr);
            break;
#endif
        case DT_DEBUG:
            // Set the DT_DEBUG entry to the address of _r_debug for GDB
            // if the dynamic table is writable
// FIXME: not working currently for N64
// The flags for the LOAD and DYNAMIC program headers do not agree.
// The LOAD section containng the dynamic table has been mapped as
// read-only, but the DYNAMIC header claims it is writable.
#if !(defined(__mips__) && defined(__LP64__))
            if ((dynamic_flags & PF_W) != 0) {
                d->d_un.d_val = reinterpret_cast<uintptr_t>(&_r_debug);
            }
            break;
#endif
#if defined(USE_RELA)
         case DT_RELA:
            si->rela = reinterpret_cast<ElfW(Rela)*>(base + d->d_un.d_ptr);
            break;
         case DT_RELASZ:
            si->rela_count = d->d_un.d_val / sizeof(ElfW(Rela));
            break;
        case DT_REL:
            DL_ERR("unsupported DT_REL in \"%s\"", si->name);
            return false;
        case DT_RELSZ:
            DL_ERR("unsupported DT_RELSZ in \"%s\"", si->name);
            return false;
#else
        case DT_REL:
            si->rel = reinterpret_cast<ElfW(Rel)*>(base + d->d_un.d_ptr);
            break;
        case DT_RELSZ:
            si->rel_count = d->d_un.d_val / sizeof(ElfW(Rel));
            break;
         case DT_RELA:
            DL_ERR("unsupported DT_RELA in \"%s\"", si->name);
            return false;
#endif
        case DT_INIT:
            si->init_func = reinterpret_cast<linker_function_t>(base + d->d_un.d_ptr);
            DEBUG("%s constructors (DT_INIT) found at %p", si->name, si->init_func);
            break;
        case DT_FINI:
            si->fini_func = reinterpret_cast<linker_function_t>(base + d->d_un.d_ptr);
            DEBUG("%s destructors (DT_FINI) found at %p", si->name, si->fini_func);
            break;
        case DT_INIT_ARRAY:
            si->init_array = reinterpret_cast<linker_function_t*>(base + d->d_un.d_ptr);
            DEBUG("%s constructors (DT_INIT_ARRAY) found at %p", si->name, si->init_array);
            break;
        case DT_INIT_ARRAYSZ:
            si->init_array_count = ((unsigned)d->d_un.d_val) / sizeof(ElfW(Addr));
            break;
        case DT_FINI_ARRAY:
            si->fini_array = reinterpret_cast<linker_function_t*>(base + d->d_un.d_ptr);
            DEBUG("%s destructors (DT_FINI_ARRAY) found at %p", si->name, si->fini_array);
            break;
        case DT_FINI_ARRAYSZ:
            si->fini_array_count = ((unsigned)d->d_un.d_val) / sizeof(ElfW(Addr));
            break;
        case DT_PREINIT_ARRAY:
            si->preinit_array = reinterpret_cast<linker_function_t*>(base + d->d_un.d_ptr);
            DEBUG("%s constructors (DT_PREINIT_ARRAY) found at %p", si->name, si->preinit_array);
            break;
        case DT_PREINIT_ARRAYSZ:
            si->preinit_array_count = ((unsigned)d->d_un.d_val) / sizeof(ElfW(Addr));
            break;
        case DT_TEXTREL:
#if defined(__LP64__)
            DL_ERR("text relocations (DT_TEXTREL) found in 64-bit ELF file \"%s\"", si->name);
            return false;
#else
            si->has_text_relocations = true;
            break;
#endif
        case DT_SYMBOLIC:
            si->has_DT_SYMBOLIC = true;
            break;
        case DT_NEEDED:
            ++needed_count;
            break;
        case DT_FLAGS:
            if (d->d_un.d_val & DF_TEXTREL) {
#if defined(__LP64__)
                DL_ERR("text relocations (DF_TEXTREL) found in 64-bit ELF file \"%s\"", si->name);
                return false;
#else
                si->has_text_relocations = true;
#endif
            }
            if (d->d_un.d_val & DF_SYMBOLIC) {
                si->has_DT_SYMBOLIC = true;
            }
            break;
#if defined(__mips__)
        case DT_STRSZ:
        case DT_SYMENT:
        case DT_RELENT:
             break;
        case DT_MIPS_RLD_MAP:
            // Set the DT_MIPS_RLD_MAP entry to the address of _r_debug for GDB.
            {
              r_debug** dp = reinterpret_cast<r_debug**>(base + d->d_un.d_ptr);
              *dp = &_r_debug;
            }
            break;
        case DT_MIPS_RLD_VERSION:
        case DT_MIPS_FLAGS:
        case DT_MIPS_BASE_ADDRESS:
        case DT_MIPS_UNREFEXTNO:
            break;

        case DT_MIPS_SYMTABNO:
            si->mips_symtabno = d->d_un.d_val;
            break;

        case DT_MIPS_LOCAL_GOTNO:
            si->mips_local_gotno = d->d_un.d_val;
            break;

        case DT_MIPS_GOTSYM:
            si->mips_gotsym = d->d_un.d_val;
            break;
#endif

        default:
            DEBUG("Unused DT entry: type %p arg %p",
                  reinterpret_cast<void*>(d->d_tag), reinterpret_cast<void*>(d->d_un.d_val));
            break;
        }
    }

    DEBUG("si->base = %p, si->strtab = %p, si->symtab = %p",
          reinterpret_cast<void*>(si->base), si->strtab, si->symtab);

    // Sanity checks.
    if (relocating_linker && needed_count != 0) {
        DL_ERR("linker cannot have DT_NEEDED dependencies on other libraries");
        return false;
    }
    if (si->nbucket == 0) {
        DL_ERR("empty/missing DT_HASH in \"%s\" (built with --hash-style=gnu?)", si->name);
        return false;
    }
    if (si->strtab == 0) {
        DL_ERR("empty/missing DT_STRTAB in \"%s\"", si->name);
        return false;
    }
    if (si->symtab == 0) {
        DL_ERR("empty/missing DT_SYMTAB in \"%s\"", si->name);
        return false;
    }

    // If this is the main executable, then load all of the libraries from LD_PRELOAD now.
    if (si->flags & FLAG_EXE) {
        memset(gLdPreloads, 0, sizeof(gLdPreloads));
        size_t preload_count = 0;
        for (size_t i = 0; gLdPreloadNames[i] != NULL; i++) {
            soinfo* lsi = find_library(gLdPreloadNames[i]);
            if (lsi != NULL) {
                gLdPreloads[preload_count++] = lsi;
            } else {
                // As with glibc, failure to load an LD_PRELOAD library is just a warning.
                DL_WARN("could not load library \"%s\" from LD_PRELOAD for \"%s\"; caused by %s",
                        gLdPreloadNames[i], si->name, linker_get_error_buffer());
            }
        }
    }

    soinfo** needed = reinterpret_cast<soinfo**>(alloca((1 + needed_count) * sizeof(soinfo*)));
    soinfo** pneeded = needed;

    for (ElfW(Dyn)* d = si->dynamic; d->d_tag != DT_NULL; ++d) {
        if (d->d_tag == DT_NEEDED) {
            const char* library_name = si->strtab + d->d_un.d_val;
            DEBUG("%s needs %s", si->name, library_name);
            soinfo* lsi = find_library(library_name);
            if (lsi == NULL) {
                strlcpy(tmp_err_buf, linker_get_error_buffer(), sizeof(tmp_err_buf));
                DL_ERR("could not load library \"%s\" needed by \"%s\"; caused by %s",
                       library_name, si->name, tmp_err_buf);
                return false;
            }
            *pneeded++ = lsi;
        }
    }
    *pneeded = NULL;

#if !defined(__LP64__)
    if (si->has_text_relocations) {
        // Make segments writable to allow text relocations to work properly. We will later call
        // phdr_table_protect_segments() after all of them are applied and all constructors are run.
        DL_WARN("%s has text relocations. This is wasting memory and prevents "
                "security hardening. Please fix.", si->name);
        if (phdr_table_unprotect_segments(si->phdr, si->phnum, si->load_bias) < 0) {
            DL_ERR("can't unprotect loadable segments for \"%s\": %s",
                   si->name, strerror(errno));
            return false;
        }
    }
#endif

#if defined(USE_RELA)
    if (si->plt_rela != NULL) {
        DEBUG("[ relocating %s plt ]\n", si->name);
        if (soinfo_relocate(si, si->plt_rela, si->plt_rela_count, needed)) {
            return false;
        }
    }
    if (si->rela != NULL) {
        DEBUG("[ relocating %s ]\n", si->name);
        if (soinfo_relocate(si, si->rela, si->rela_count, needed)) {
            return false;
        }
    }
#else
    if (si->plt_rel != NULL) {
        DEBUG("[ relocating %s plt ]", si->name);
        if (soinfo_relocate(si, si->plt_rel, si->plt_rel_count, needed)) {
            return false;
        }
    }
    if (si->rel != NULL) {
        DEBUG("[ relocating %s ]", si->name);
        if (soinfo_relocate(si, si->rel, si->rel_count, needed)) {
            return false;
        }
    }
#endif

#if defined(__mips__)
    if (!mips_relocate_got(si, needed)) {
        return false;
    }
#endif

    si->flags |= FLAG_LINKED;
    DEBUG("[ finished linking %s ]", si->name);

#if !defined(__LP64__)
    if (si->has_text_relocations) {
        // All relocations are done, we can protect our segments back to read-only.
        if (phdr_table_protect_segments(si->phdr, si->phnum, si->load_bias) < 0) {
            DL_ERR("can't protect segments for \"%s\": %s",
                   si->name, strerror(errno));
            return false;
        }
    }
#endif

    /* We can also turn on GNU RELRO protection */
    if (phdr_table_protect_gnu_relro(si->phdr, si->phnum, si->load_bias) < 0) {
        DL_ERR("can't enable GNU RELRO protection for \"%s\": %s",
               si->name, strerror(errno));
        return false;
    }

    notify_gdb_of_load(si);
    return true;
}

/*
 * This function add vdso to internal dso list.
 * It helps to stack unwinding through signal handlers.
 * Also, it makes bionic more like glibc.
 */
static void add_vdso(KernelArgumentBlock& args UNUSED) {
#if defined(AT_SYSINFO_EHDR)
  ElfW(Ehdr)* ehdr_vdso = reinterpret_cast<ElfW(Ehdr)*>(args.getauxval(AT_SYSINFO_EHDR));
  if (ehdr_vdso == NULL) {
    return;
  }

  soinfo* si = soinfo_alloc("[vdso]");

  si->phdr = reinterpret_cast<ElfW(Phdr)*>(reinterpret_cast<char*>(ehdr_vdso) + ehdr_vdso->e_phoff);
  si->phnum = ehdr_vdso->e_phnum;
  si->base = reinterpret_cast<ElfW(Addr)>(ehdr_vdso);
  si->size = phdr_table_get_load_size(si->phdr, si->phnum);
  si->flags = 0;
  si->load_bias = get_elf_exec_load_bias(ehdr_vdso);

  soinfo_link_image(si);
#endif
}

/*
 * This code is called after the linker has linked itself and
 * fixed it's own GOT. It is safe to make references to externs
 * and other non-local data at this point.
 */
static ElfW(Addr) __linker_init_post_relocation(KernelArgumentBlock& args, ElfW(Addr) linker_base) {
    /* NOTE: we store the args pointer on a special location
     *       of the temporary TLS area in order to pass it to
     *       the C Library's runtime initializer.
     *
     *       The initializer must clear the slot and reset the TLS
     *       to point to a different location to ensure that no other
     *       shared library constructor can access it.
     */
  __libc_init_tls(args);

#if TIMING
    struct timeval t0, t1;
    gettimeofday(&t0, 0);
#endif

    // Initialize environment functions, and get to the ELF aux vectors table.
    linker_env_init(args);

    // If this is a setuid/setgid program, close the security hole described in
    // ftp://ftp.freebsd.org/pub/FreeBSD/CERT/advisories/FreeBSD-SA-02:23.stdio.asc
    if (get_AT_SECURE()) {
        nullify_closed_stdio();
    }

    debuggerd_init();

    // Get a few environment variables.
    const char* LD_DEBUG = linker_env_get("LD_DEBUG");
    if (LD_DEBUG != NULL) {
      gLdDebugVerbosity = atoi(LD_DEBUG);
    }

    // Normally, these are cleaned by linker_env_init, but the test
    // doesn't cost us anything.
    const char* ldpath_env = NULL;
    const char* ldpreload_env = NULL;
    if (!get_AT_SECURE()) {
      ldpath_env = linker_env_get("LD_LIBRARY_PATH");
      ldpreload_env = linker_env_get("LD_PRELOAD");
    }

    INFO("[ android linker & debugger ]");

    soinfo* si = soinfo_alloc(args.argv[0]);
    if (si == NULL) {
        exit(EXIT_FAILURE);
    }

    /* bootstrap the link map, the main exe always needs to be first */
    si->flags |= FLAG_EXE;
    link_map* map = &(si->link_map_head);

    map->l_addr = 0;
    map->l_name = args.argv[0];
    map->l_prev = NULL;
    map->l_next = NULL;

    _r_debug.r_map = map;
    r_debug_tail = map;

    /* gdb expects the linker to be in the debug shared object list.
     * Without this, gdb has trouble locating the linker's ".text"
     * and ".plt" sections. Gdb could also potentially use this to
     * relocate the offset of our exported 'rtld_db_dlactivity' symbol.
     * Don't use soinfo_alloc(), because the linker shouldn't
     * be on the soinfo list.
     */
    {
        static soinfo linker_soinfo;
#if defined(__LP64__)
        strlcpy(linker_soinfo.name, "/system/bin/linker64", sizeof(linker_soinfo.name));
#else
        strlcpy(linker_soinfo.name, "/system/bin/linker", sizeof(linker_soinfo.name));
#endif
        linker_soinfo.flags = 0;
        linker_soinfo.base = linker_base;

        /*
         * Set the dynamic field in the link map otherwise gdb will complain with
         * the following:
         *   warning: .dynamic section for "/system/bin/linker" is not at the
         *   expected address (wrong library or version mismatch?)
         */
        ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_base);
        ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_base + elf_hdr->e_phoff);
        phdr_table_get_dynamic_section(phdr, elf_hdr->e_phnum, linker_base,
                                       &linker_soinfo.dynamic, NULL, NULL);
        insert_soinfo_into_debug_map(&linker_soinfo);
    }

    // Extract information passed from the kernel.
    si->phdr = reinterpret_cast<ElfW(Phdr)*>(args.getauxval(AT_PHDR));
    si->phnum = args.getauxval(AT_PHNUM);
    si->entry = args.getauxval(AT_ENTRY);

    /* Compute the value of si->base. We can't rely on the fact that
     * the first entry is the PHDR because this will not be true
     * for certain executables (e.g. some in the NDK unit test suite)
     */
    si->base = 0;
    si->size = phdr_table_get_load_size(si->phdr, si->phnum);
    si->load_bias = 0;
    for (size_t i = 0; i < si->phnum; ++i) {
      if (si->phdr[i].p_type == PT_PHDR) {
        si->load_bias = reinterpret_cast<ElfW(Addr)>(si->phdr) - si->phdr[i].p_vaddr;
        si->base = reinterpret_cast<ElfW(Addr)>(si->phdr) - si->phdr[i].p_offset;
        break;
      }
    }
    si->dynamic = NULL;
    si->ref_count = 1;

    // Use LD_LIBRARY_PATH and LD_PRELOAD (but only if we aren't setuid/setgid).
    parse_LD_LIBRARY_PATH(ldpath_env);
    parse_LD_PRELOAD(ldpreload_env);

    somain = si;

    if (!soinfo_link_image(si)) {
        __libc_format_fd(2, "CANNOT LINK EXECUTABLE: %s\n", linker_get_error_buffer());
        exit(EXIT_FAILURE);
    }

    add_vdso(args);

    si->CallPreInitConstructors();

    for (size_t i = 0; gLdPreloads[i] != NULL; ++i) {
        gLdPreloads[i]->CallConstructors();
    }

    /* After the link_image, the si->load_bias is initialized.
     * For so lib, the map->l_addr will be updated in notify_gdb_of_load.
     * We need to update this value for so exe here. So Unwind_Backtrace
     * for some arch like x86 could work correctly within so exe.
     */
    map->l_addr = si->load_bias;
    si->CallConstructors();

#if TIMING
    gettimeofday(&t1, NULL);
    PRINT("LINKER TIME: %s: %d microseconds", args.argv[0], (int) (
               (((long long)t1.tv_sec * 1000000LL) + (long long)t1.tv_usec) -
               (((long long)t0.tv_sec * 1000000LL) + (long long)t0.tv_usec)));
#endif
#if STATS
    PRINT("RELO STATS: %s: %d abs, %d rel, %d copy, %d symbol", args.argv[0],
           linker_stats.count[kRelocAbsolute],
           linker_stats.count[kRelocRelative],
           linker_stats.count[kRelocCopy],
           linker_stats.count[kRelocSymbol]);
#endif
#if COUNT_PAGES
    {
        unsigned n;
        unsigned i;
        unsigned count = 0;
        for (n = 0; n < 4096; n++) {
            if (bitmask[n]) {
                unsigned x = bitmask[n];
#if defined(__LP64__)
                for (i = 0; i < 32; i++) {
#else
                for (i = 0; i < 8; i++) {
#endif
                    if (x & 1) {
                        count++;
                    }
                    x >>= 1;
                }
            }
        }
        PRINT("PAGES MODIFIED: %s: %d (%dKB)", args.argv[0], count, count * 4);
    }
#endif

#if TIMING || STATS || COUNT_PAGES
    fflush(stdout);
#endif

    TRACE("[ Ready to execute '%s' @ %p ]", si->name, reinterpret_cast<void*>(si->entry));
    return si->entry;
}

/* Compute the load-bias of an existing executable. This shall only
 * be used to compute the load bias of an executable or shared library
 * that was loaded by the kernel itself.
 *
 * Input:
 *    elf    -> address of ELF header, assumed to be at the start of the file.
 * Return:
 *    load bias, i.e. add the value of any p_vaddr in the file to get
 *    the corresponding address in memory.
 */
static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf) {
  ElfW(Addr) offset = elf->e_phoff;
  const ElfW(Phdr)* phdr_table = reinterpret_cast<const ElfW(Phdr)*>(reinterpret_cast<uintptr_t>(elf) + offset);
  const ElfW(Phdr)* phdr_end = phdr_table + elf->e_phnum;

  for (const ElfW(Phdr)* phdr = phdr_table; phdr < phdr_end; phdr++) {
    if (phdr->p_type == PT_LOAD) {
      return reinterpret_cast<ElfW(Addr)>(elf) + phdr->p_offset - phdr->p_vaddr;
    }
  }
  return 0;
}

/*
 * This is the entry point for the linker, called from begin.S. This
 * method is responsible for fixing the linker's own relocations, and
 * then calling __linker_init_post_relocation().
 *
 * Because this method is called before the linker has fixed it's own
 * relocations, any attempt to reference an extern variable, extern
 * function, or other GOT reference will generate a segfault.
 */
extern "C" ElfW(Addr) __linker_init(void* raw_args) {
  KernelArgumentBlock args(raw_args);

  ElfW(Addr) linker_addr = args.getauxval(AT_BASE);
  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_addr);
  ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_addr + elf_hdr->e_phoff);

  soinfo linker_so;
  memset(&linker_so, 0, sizeof(soinfo));

  strcpy(linker_so.name, "[dynamic linker]");
  linker_so.base = linker_addr;
  linker_so.size = phdr_table_get_load_size(phdr, elf_hdr->e_phnum);
  linker_so.load_bias = get_elf_exec_load_bias(elf_hdr);
  linker_so.dynamic = NULL;
  linker_so.phdr = phdr;
  linker_so.phnum = elf_hdr->e_phnum;
  linker_so.flags |= FLAG_LINKER;

  if (!soinfo_link_image(&linker_so)) {
    // It would be nice to print an error message, but if the linker
    // can't link itself, there's no guarantee that we'll be able to
    // call write() (because it involves a GOT reference). We may as
    // well try though...
    const char* msg = "CANNOT LINK EXECUTABLE: ";
    write(2, msg, strlen(msg));
    write(2, __linker_dl_err_buf, strlen(__linker_dl_err_buf));
    write(2, "\n", 1);
    _exit(EXIT_FAILURE);
  }

  // We have successfully fixed our own relocations. It's safe to run
  // the main part of the linker now.
  args.abort_message_ptr = &gAbortMessage;
  ElfW(Addr) start_address = __linker_init_post_relocation(args, linker_addr);

  set_soinfo_pool_protection(PROT_READ);

  // Return the address that the calling assembly stub should jump to.
  return start_address;
}
