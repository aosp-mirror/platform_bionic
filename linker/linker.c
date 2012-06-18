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

#include <linux/auxvec.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include <pthread.h>

#include <sys/mman.h>

#include <sys/atomics.h>

/* special private C library header - see Android.mk */
#include <bionic_tls.h>

#include "linker.h"
#include "linker_debug.h"
#include "linker_environ.h"
#include "linker_format.h"
#include "linker_phdr.h"

#define ALLOW_SYMBOLS_FROM_MAIN 1
#define SO_MAX 128

/* Assume average path length of 64 and max 8 paths */
#define LDPATH_BUFSIZE 512
#define LDPATH_MAX 8

#define LDPRELOAD_BUFSIZE 512
#define LDPRELOAD_MAX 8

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
 * - linker hardcodes PAGE_SIZE and PAGE_MASK because the kernel
 *   headers provide versions that are negative...
 * - allocate space for soinfo structs dynamically instead of
 *   having a hard limit (64)
*/


static int soinfo_link_image(soinfo *si, unsigned wr_offset);

static int socount = 0;
static soinfo sopool[SO_MAX];
static soinfo *freelist = NULL;
static soinfo *solist = &libdl_info;
static soinfo *sonext = &libdl_info;
#if ALLOW_SYMBOLS_FROM_MAIN
static soinfo *somain; /* main process, always the one after libdl_info */
#endif


static inline int validate_soinfo(soinfo *si)
{
    return (si >= sopool && si < sopool + SO_MAX) ||
        si == &libdl_info;
}

static char ldpaths_buf[LDPATH_BUFSIZE];
static const char *ldpaths[LDPATH_MAX + 1];

static char ldpreloads_buf[LDPRELOAD_BUFSIZE];
static const char *ldpreload_names[LDPRELOAD_MAX + 1];

static soinfo *preloads[LDPRELOAD_MAX + 1];

#if LINKER_DEBUG
int debug_verbosity;
#endif

static int pid;

/* This boolean is set if the program being loaded is setuid */
static int program_is_setuid;

#if STATS
struct _link_stats linker_stats;
#endif

#if COUNT_PAGES
unsigned bitmask[4096];
#endif

#define HOODLUM(name, ret, ...)                                               \
    ret name __VA_ARGS__                                                      \
    {                                                                         \
        char errstr[] = "ERROR: " #name " called from the dynamic linker!\n"; \
        write(2, errstr, sizeof(errstr));                                     \
        abort();                                                              \
    }
HOODLUM(malloc, void *, (size_t size));
HOODLUM(free, void, (void *ptr));
HOODLUM(realloc, void *, (void *ptr, size_t size));
HOODLUM(calloc, void *, (size_t cnt, size_t size));

static char tmp_err_buf[768];
static char __linker_dl_err_buf[768];
#define DL_ERR(fmt, x...)                                                     \
    do {                                                                      \
        format_buffer(__linker_dl_err_buf, sizeof(__linker_dl_err_buf),            \
                 "%s[%d]: " fmt, __func__, __LINE__, ##x);                    \
        ERROR(fmt "\n", ##x);                                                      \
    } while(0)

const char *linker_get_error(void)
{
    return (const char *)&__linker_dl_err_buf[0];
}

/*
 * This function is an empty stub where GDB locates a breakpoint to get notified
 * about linker activity.
 */
extern void __attribute__((noinline)) rtld_db_dlactivity(void);

static struct r_debug _r_debug = {1, NULL, &rtld_db_dlactivity,
                                  RT_CONSISTENT, 0};
static struct link_map *r_debug_tail = 0;

static pthread_mutex_t _r_debug_lock = PTHREAD_MUTEX_INITIALIZER;

static void insert_soinfo_into_debug_map(soinfo * info)
{
    struct link_map * map;

    /* Copy the necessary fields into the debug structure.
     */
    map = &(info->linkmap);
    map->l_addr = info->base;
    map->l_name = (char*) info->name;
    map->l_ld = (uintptr_t)info->dynamic;

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

static void remove_soinfo_from_debug_map(soinfo * info)
{
    struct link_map * map = &(info->linkmap);

    if (r_debug_tail == map)
        r_debug_tail = map->l_prev;

    if (map->l_prev) map->l_prev->l_next = map->l_next;
    if (map->l_next) map->l_next->l_prev = map->l_prev;
}

void notify_gdb_of_load(soinfo * info)
{
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    pthread_mutex_lock(&_r_debug_lock);

    _r_debug.r_state = RT_ADD;
    rtld_db_dlactivity();

    insert_soinfo_into_debug_map(info);

    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();

    pthread_mutex_unlock(&_r_debug_lock);
}

void notify_gdb_of_unload(soinfo * info)
{
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    pthread_mutex_lock(&_r_debug_lock);

    _r_debug.r_state = RT_DELETE;
    rtld_db_dlactivity();

    remove_soinfo_from_debug_map(info);

    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();

    pthread_mutex_unlock(&_r_debug_lock);
}

void notify_gdb_of_libraries()
{
    _r_debug.r_state = RT_ADD;
    rtld_db_dlactivity();
    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();
}

static soinfo *soinfo_alloc(const char *name)
{
    soinfo *si;

    if(strlen(name) >= SOINFO_NAME_LEN) {
        DL_ERR("%5d library name %s too long", pid, name);
        return NULL;
    }

    /* The freelist is populated when we call soinfo_free(), which in turn is
       done only by dlclose(), which is not likely to be used.
    */
    if (!freelist) {
        if(socount == SO_MAX) {
            DL_ERR("%5d too many libraries when loading %s", pid, name);
            return NULL;
        }
        freelist = sopool + socount++;
        freelist->next = NULL;
    }

    si = freelist;
    freelist = freelist->next;

    /* Make sure we get a clean block of soinfo */
    memset(si, 0, sizeof(soinfo));
    strlcpy((char*) si->name, name, sizeof(si->name));
    sonext->next = si;
    si->next = NULL;
    si->refcount = 0;
    sonext = si;

    TRACE("%5d name %s: allocated soinfo @ %p\n", pid, name, si);
    return si;
}

static void soinfo_free(soinfo *si)
{
    soinfo *prev = NULL, *trav;

    TRACE("%5d name %s: freeing soinfo @ %p\n", pid, si->name, si);

    for(trav = solist; trav != NULL; trav = trav->next){
        if (trav == si)
            break;
        prev = trav;
    }
    if (trav == NULL) {
        /* si was not ni solist */
        DL_ERR("%5d name %s is not in solist!", pid, si->name);
        return;
    }

    /* prev will never be NULL, because the first entry in solist is
       always the static libdl_info.
    */
    prev->next = si->next;
    if (si == sonext) sonext = prev;
    si->next = freelist;
    freelist = si;
}

const char *addr_to_name(unsigned addr)
{
    soinfo *si;

    for(si = solist; si != 0; si = si->next){
        if((addr >= si->base) && (addr < (si->base + si->size))) {
            return si->name;
        }
    }

    return "";
}

/* For a given PC, find the .so that it belongs to.
 * Returns the base address of the .ARM.exidx section
 * for that .so, and the number of 8-byte entries
 * in that section (via *pcount).
 *
 * Intended to be called by libc's __gnu_Unwind_Find_exidx().
 *
 * This function is exposed via dlfcn.c and libdl.so.
 */
#ifdef ANDROID_ARM_LINKER
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int *pcount)
{
    soinfo *si;
    unsigned addr = (unsigned)pc;

    for (si = solist; si != 0; si = si->next){
        if ((addr >= si->base) && (addr < (si->base + si->size))) {
            *pcount = si->ARM_exidx_count;
            return (_Unwind_Ptr)si->ARM_exidx;
        }
    }
   *pcount = 0;
    return NULL;
}
#elif defined(ANDROID_X86_LINKER)
/* Here, we only have to provide a callback to iterate across all the
 * loaded libraries. gcc_eh does the rest. */
int
dl_iterate_phdr(int (*cb)(struct dl_phdr_info *info, size_t size, void *data),
                void *data)
{
    soinfo *si;
    struct dl_phdr_info dl_info;
    int rv = 0;

    for (si = solist; si != NULL; si = si->next) {
        dl_info.dlpi_addr = si->linkmap.l_addr;
        dl_info.dlpi_name = si->linkmap.l_name;
        dl_info.dlpi_phdr = si->phdr;
        dl_info.dlpi_phnum = si->phnum;
        rv = cb(&dl_info, sizeof (struct dl_phdr_info), data);
        if (rv != 0)
            break;
    }
    return rv;
}
#endif

static Elf32_Sym *soinfo_elf_lookup(soinfo *si, unsigned hash, const char *name)
{
    Elf32_Sym *s;
    Elf32_Sym *symtab = si->symtab;
    const char *strtab = si->strtab;
    unsigned n;

    TRACE_TYPE(LOOKUP, "%5d SEARCH %s in %s@0x%08x %08x %d\n", pid,
               name, si->name, si->base, hash, hash % si->nbucket);
    n = hash % si->nbucket;

    for(n = si->bucket[hash % si->nbucket]; n != 0; n = si->chain[n]){
        s = symtab + n;
        if(strcmp(strtab + s->st_name, name)) continue;

            /* only concern ourselves with global and weak symbol definitions */
        switch(ELF32_ST_BIND(s->st_info)){
        case STB_GLOBAL:
        case STB_WEAK:
                /* no section == undefined */
            if(s->st_shndx == 0) continue;

            TRACE_TYPE(LOOKUP, "%5d FOUND %s in %s (%08x) %d\n", pid,
                       name, si->name, s->st_value, s->st_size);
            return s;
        }
    }

    return NULL;
}

static unsigned elfhash(const char *_name)
{
    const unsigned char *name = (const unsigned char *) _name;
    unsigned h = 0, g;

    while(*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}

static Elf32_Sym *
soinfo_do_lookup(soinfo *si, const char *name, Elf32_Addr *offset)
{
    unsigned elf_hash = elfhash(name);
    Elf32_Sym *s;
    unsigned *d;
    soinfo *lsi = si;
    int i;

    /* Look for symbols in the local scope (the object who is
     * searching). This happens with C++ templates on i386 for some
     * reason.
     *
     * Notes on weak symbols:
     * The ELF specs are ambigious about treatment of weak definitions in
     * dynamic linking.  Some systems return the first definition found
     * and some the first non-weak definition.   This is system dependent.
     * Here we return the first definition found for simplicity.  */

    s = soinfo_elf_lookup(si, elf_hash, name);
    if(s != NULL)
        goto done;

    /* Next, look for it in the preloads list */
    for(i = 0; preloads[i] != NULL; i++) {
        lsi = preloads[i];
        s = soinfo_elf_lookup(lsi, elf_hash, name);
        if(s != NULL)
            goto done;
    }

    for(d = si->dynamic; *d; d += 2) {
        if(d[0] == DT_NEEDED){
            lsi = (soinfo *)d[1];
            if (!validate_soinfo(lsi)) {
                DL_ERR("%5d bad DT_NEEDED pointer in %s",
                       pid, si->name);
                return NULL;
            }

            DEBUG("%5d %s: looking up %s in %s\n",
                  pid, si->name, name, lsi->name);
            s = soinfo_elf_lookup(lsi, elf_hash, name);
            if ((s != NULL) && (s->st_shndx != SHN_UNDEF))
                goto done;
        }
    }

#if ALLOW_SYMBOLS_FROM_MAIN
    /* If we are resolving relocations while dlopen()ing a library, it's OK for
     * the library to resolve a symbol that's defined in the executable itself,
     * although this is rare and is generally a bad idea.
     */
    if (somain) {
        lsi = somain;
        DEBUG("%5d %s: looking up %s in executable %s\n",
              pid, si->name, name, lsi->name);
        s = soinfo_elf_lookup(lsi, elf_hash, name);
    }
#endif

done:
    if(s != NULL) {
        TRACE_TYPE(LOOKUP, "%5d si %s sym %s s->st_value = 0x%08x, "
                   "found in %s, base = 0x%08x, load bias = 0x%08x\n",
                   pid, si->name, name, s->st_value,
                   lsi->name, lsi->base, lsi->load_bias);
        *offset = lsi->load_bias;
        return s;
    }

    return NULL;
}

/* This is used by dl_sym().  It performs symbol lookup only within the
   specified soinfo object and not in any of its dependencies.
 */
Elf32_Sym *soinfo_lookup(soinfo *si, const char *name)
{
    return soinfo_elf_lookup(si, elfhash(name), name);
}

/* This is used by dl_sym().  It performs a global symbol lookup.
 */
Elf32_Sym *lookup(const char *name, soinfo **found, soinfo *start)
{
    unsigned elf_hash = elfhash(name);
    Elf32_Sym *s = NULL;
    soinfo *si;

    if(start == NULL) {
        start = solist;
    }

    for(si = start; (s == NULL) && (si != NULL); si = si->next)
    {
        if(si->flags & FLAG_ERROR)
            continue;
        s = soinfo_elf_lookup(si, elf_hash, name);
        if (s != NULL) {
            *found = si;
            break;
        }
    }

    if(s != NULL) {
        TRACE_TYPE(LOOKUP, "%5d %s s->st_value = 0x%08x, "
                   "si->base = 0x%08x\n", pid, name, s->st_value, si->base);
        return s;
    }

    return NULL;
}

soinfo *find_containing_library(const void *addr)
{
    soinfo *si;

    for(si = solist; si != NULL; si = si->next)
    {
        if((unsigned)addr >= si->base && (unsigned)addr - si->base < si->size) {
            return si;
        }
    }

    return NULL;
}

Elf32_Sym *soinfo_find_symbol(soinfo* si, const void *addr)
{
    unsigned int i;
    unsigned soaddr = (unsigned)addr - si->base;

    /* Search the library's symbol table for any defined symbol which
     * contains this address */
    for(i=0; i<si->nchain; i++) {
        Elf32_Sym *sym = &si->symtab[i];

        if(sym->st_shndx != SHN_UNDEF &&
           soaddr >= sym->st_value &&
           soaddr < sym->st_value + sym->st_size) {
            return sym;
        }
    }

    return NULL;
}

#if 0
static void dump(soinfo *si)
{
    Elf32_Sym *s = si->symtab;
    unsigned n;

    for(n = 0; n < si->nchain; n++) {
        TRACE("%5d %04d> %08x: %02x %04x %08x %08x %s\n", pid, n, s,
               s->st_info, s->st_shndx, s->st_value, s->st_size,
               si->strtab + s->st_name);
        s++;
    }
}
#endif

static const char * const sopaths[] = {
    "/vendor/lib",
    "/system/lib",
    0
};

static int _open_lib(const char *name)
{
    int fd;
    struct stat filestat;

    if ((stat(name, &filestat) >= 0) && S_ISREG(filestat.st_mode)) {
        if ((fd = TEMP_FAILURE_RETRY(open(name, O_RDONLY))) >= 0)
            return fd;
    }

    return -1;
}

static int open_library(const char *name)
{
    int fd;
    char buf[512];
    const char * const*path;
    int n;

    TRACE("[ %5d opening %s ]\n", pid, name);

    if(name == 0) return -1;
    if(strlen(name) > 256) return -1;

    if ((name[0] == '/') && ((fd = _open_lib(name)) >= 0))
        return fd;

    for (path = ldpaths; *path; path++) {
        n = format_buffer(buf, sizeof(buf), "%s/%s", *path, name);
        if (n < 0 || n >= (int)sizeof(buf)) {
            WARN("Ignoring very long library path: %s/%s\n", *path, name);
            continue;
        }
        if ((fd = _open_lib(buf)) >= 0)
            return fd;
    }
    for (path = sopaths; *path; path++) {
        n = format_buffer(buf, sizeof(buf), "%s/%s", *path, name);
        if (n < 0 || n >= (int)sizeof(buf)) {
            WARN("Ignoring very long library path: %s/%s\n", *path, name);
            continue;
        }
        if ((fd = _open_lib(buf)) >= 0)
            return fd;
    }

    return -1;
}

typedef struct {
    long mmap_addr;
    char tag[4]; /* 'P', 'R', 'E', ' ' */
} prelink_info_t;

/* Returns the requested base address if the library is prelinked,
 * and 0 otherwise.  */
static unsigned long
is_prelinked(int fd, const char *name)
{
    off_t sz;
    prelink_info_t info;

    sz = lseek(fd, -sizeof(prelink_info_t), SEEK_END);
    if (sz < 0) {
        DL_ERR("lseek() failed!");
        return 0;
    }

    if (TEMP_FAILURE_RETRY(read(fd, &info, sizeof(info)) != sizeof(info))) {
        WARN("Could not read prelink_info_t structure for `%s`\n", name);
        return 0;
    }

    if (memcmp(info.tag, "PRE ", 4)) {
        WARN("`%s` is not a prelinked library\n", name);
        return 0;
    }

    return (unsigned long)info.mmap_addr;
}

/* verify_elf_header
 *      Verifies the content of an ELF header.
 *
 * Args:
 *
 * Returns:
 *       0 on success
 *      -1 if no valid ELF object is found @ base.
 */
static int
verify_elf_header(const Elf32_Ehdr* hdr)
{
    if (hdr->e_ident[EI_MAG0] != ELFMAG0) return -1;
    if (hdr->e_ident[EI_MAG1] != ELFMAG1) return -1;
    if (hdr->e_ident[EI_MAG2] != ELFMAG2) return -1;
    if (hdr->e_ident[EI_MAG3] != ELFMAG3) return -1;

    /* TODO: Should we verify anything else in the header? */
#ifdef ANDROID_ARM_LINKER
    if (hdr->e_machine != EM_ARM) return -1;
#elif defined(ANDROID_X86_LINKER)
    if (hdr->e_machine != EM_386) return -1;
#endif
    return 0;
}


static soinfo *
load_library(const char *name)
{
    int fd = open_library(name);
    int ret, cnt;
    unsigned ext_sz;
    unsigned req_base;
    const char *bname;
    struct stat sb;
    soinfo *si = NULL;
    Elf32_Ehdr  header[1];
    int         phdr_count;
    void*       phdr_mmap = NULL;
    Elf32_Addr  phdr_size;
    const Elf32_Phdr* phdr_table;

    void*       load_start = NULL;
    Elf32_Addr  load_size = 0;
    Elf32_Addr  load_bias = 0;

    if (fd == -1) {
        DL_ERR("Library '%s' not found", name);
        return NULL;
    }

    /* Read the ELF header first */
    ret = TEMP_FAILURE_RETRY(read(fd, (void*)header, sizeof(header)));
    if (ret < 0) {
        DL_ERR("%5d can't read file %s: %s", pid, name, strerror(errno));
        goto fail;
    }
    if (ret != (int)sizeof(header)) {
        DL_ERR("%5d too small to be an ELF executable: %s", pid, name);
        goto fail;
    }
    if (verify_elf_header(header) < 0) {
        DL_ERR("%5d not a valid ELF executable: %s", pid, name);
        goto fail;
    }

    /* Then read the program header table */
    ret = phdr_table_load(fd, header->e_phoff, header->e_phnum,
                          &phdr_mmap, &phdr_size, &phdr_table);
    if (ret < 0) {
        DL_ERR("%5d can't load program header table: %s: %s", pid,
               name, strerror(errno));
        goto fail;
    }
    phdr_count = header->e_phnum;

    /* Get the load extents and the prelinked load address, if any */
    ext_sz = phdr_table_get_load_size(phdr_table, phdr_count);
    if (ext_sz == 0) {
        DL_ERR("%5d no loadable segments in file: %s", pid, name);
        goto fail;
    }

    req_base = (unsigned) is_prelinked(fd, name);
    if (req_base == (unsigned)-1) {
        DL_ERR("%5d can't read end of library: %s: %s", pid, name,
               strerror(errno));
        goto fail;
    }
    if (req_base != 0) {
        TRACE("[ %5d - Prelinked library '%s' requesting base @ 0x%08x ]\n",
              pid, name, req_base);
    } else {
        TRACE("[ %5d - Non-prelinked library '%s' found. ]\n", pid, name);
    }

    TRACE("[ %5d - '%s' (%s) wants base=0x%08x sz=0x%08x ]\n", pid, name,
          (req_base ? "prelinked" : "not pre-linked"), req_base, ext_sz);

    /* Now configure the soinfo struct where we'll store all of our data
     * for the ELF object. If the loading fails, we waste the entry, but
     * same thing would happen if we failed during linking. Configuring the
     * soinfo struct here is a lot more convenient.
     */
    bname = strrchr(name, '/');
    si = soinfo_alloc(bname ? bname + 1 : name);
    if (si == NULL)
        goto fail;

    /* Reserve address space for all loadable segments */
    ret = phdr_table_reserve_memory(phdr_table,
                                    phdr_count,
                                    req_base,
                                    &load_start,
                                    &load_size,
                                    &load_bias);
    if (ret < 0) {
        DL_ERR("%5d Can't reserve %d bytes from 0x%08x in address space for %s: %s",
               pid, ext_sz, req_base, name, strerror(errno));
        goto fail;
    }

    TRACE("[ %5d allocated memory for %s @ %p (0x%08x) ]\n",
          pid, name, load_start, load_size);

    /* Map all the segments in our address space with default protections */
    ret = phdr_table_load_segments(phdr_table,
                                   phdr_count,
                                   load_start,
                                   load_size,
                                   load_bias,
                                   fd);
    if (ret < 0) {
        DL_ERR("%5d Can't map loadable segments for %s: %s",
               pid, name, strerror(errno));
        goto fail;
    }

    /* Unprotect the segments, i.e. make them writable, to allow
     * relocations to work properly. We will later call
     * phdr_table_protect_segments() after all of them are applied
     * and all constructors are run.
     */
    ret = phdr_table_unprotect_segments(phdr_table,
                                        phdr_count,
                                        load_bias);
    if (ret < 0) {
        DL_ERR("%5d Can't unprotect loadable segments for %s: %s",
               pid, name, strerror(errno));
        goto fail;
    }

    si->base = (Elf32_Addr) load_start;
    si->size = load_size;
    si->load_bias = load_bias;
    si->flags = 0;
    si->entry = 0;
    si->dynamic = (unsigned *)-1;
    si->phnum = phdr_count;
    si->phdr = phdr_table_get_loaded_phdr(phdr_table, phdr_count, load_bias);
    if (si->phdr == NULL) {
        DL_ERR("%5d Can't find loaded PHDR for %s",
               pid, name);
        goto fail;
    }

    phdr_table_unload(phdr_mmap, phdr_size);
    close(fd);
    return si;

fail:
    if (si) soinfo_free(si);
    if (phdr_mmap != NULL) {
        phdr_table_unload(phdr_mmap, phdr_size);
    }
    close(fd);
    return NULL;
}

static soinfo *
init_library(soinfo *si)
{
    unsigned wr_offset = 0xffffffff;

    /* At this point we know that whatever is loaded @ base is a valid ELF
     * shared library whose segments are properly mapped in. */
    TRACE("[ %5d init_library base=0x%08x sz=0x%08x name='%s') ]\n",
          pid, si->base, si->size, si->name);

    if(soinfo_link_image(si, wr_offset)) {
            /* We failed to link.  However, we can only restore libbase
            ** if no additional libraries have moved it since we updated it.
            */
        munmap((void *)si->base, si->size);
        return NULL;
    }

    return si;
}

soinfo *find_library(const char *name)
{
    soinfo *si;
    const char *bname;

#if ALLOW_SYMBOLS_FROM_MAIN
    if (name == NULL)
        return somain;
#else
    if (name == NULL)
        return NULL;
#endif

    bname = strrchr(name, '/');
    bname = bname ? bname + 1 : name;

    for(si = solist; si != 0; si = si->next){
        if(!strcmp(bname, si->name)) {
            if(si->flags & FLAG_ERROR) {
                DL_ERR("%5d '%s' failed to load previously", pid, bname);
                return NULL;
            }
            if(si->flags & FLAG_LINKED) return si;
            DL_ERR("OOPS: %5d recursive link to '%s'", pid, si->name);
            return NULL;
        }
    }

    TRACE("[ %5d '%s' has not been loaded yet.  Locating...]\n", pid, name);
    si = load_library(name);
    if(si == NULL)
        return NULL;
    return init_library(si);
}

/* TODO:
 *   notify gdb of unload
 *   for non-prelinked libraries, find a way to decrement libbase
 */
static void call_destructors(soinfo *si);
unsigned soinfo_unload(soinfo *si)
{
    unsigned *d;
    if (si->refcount == 1) {
        TRACE("%5d unloading '%s'\n", pid, si->name);
        call_destructors(si);

        /*
         * Make sure that we undo the PT_GNU_RELRO protections we added
         * in soinfo_link_image. This is needed to undo the DT_NEEDED hack below.
         */
        if (phdr_table_unprotect_gnu_relro(si->phdr, si->phnum,
                                           si->load_bias) < 0) {
            DL_ERR("%5d %s: could not undo GNU_RELRO protections. "
                    "Expect a crash soon. errno=%d (%s)",
                    pid, si->name, errno, strerror(errno));
        }

        for(d = si->dynamic; *d; d += 2) {
            if(d[0] == DT_NEEDED){
                soinfo *lsi = (soinfo *)d[1];

                // The next line will segfault if the we don't undo the
                // PT_GNU_RELRO protections (see comments above and in
                // soinfo_link_image().
                d[1] = 0;

                if (validate_soinfo(lsi)) {
                    TRACE("%5d %s needs to unload %s\n", pid,
                          si->name, lsi->name);
                    soinfo_unload(lsi);
                }
                else
                    DL_ERR("%5d %s: could not unload dependent library",
                           pid, si->name);
            }
        }

        munmap((char *)si->base, si->size);
        notify_gdb_of_unload(si);
        soinfo_free(si);
        si->refcount = 0;
    }
    else {
        si->refcount--;
        PRINT("%5d not unloading '%s', decrementing refcount to %d\n",
              pid, si->name, si->refcount);
    }
    return si->refcount;
}

/* TODO: don't use unsigned for addrs below. It works, but is not
 * ideal. They should probably be either uint32_t, Elf32_Addr, or unsigned
 * long.
 */
static int soinfo_relocate(soinfo *si, Elf32_Rel *rel, unsigned count)
{
    Elf32_Sym *symtab = si->symtab;
    const char *strtab = si->strtab;
    Elf32_Sym *s;
    unsigned base;
    Elf32_Addr offset;
    Elf32_Rel *start = rel;
    unsigned idx;

    for (idx = 0; idx < count; ++idx) {
        unsigned type = ELF32_R_TYPE(rel->r_info);
        unsigned sym = ELF32_R_SYM(rel->r_info);
        unsigned reloc = (unsigned)(rel->r_offset + si->load_bias);
        unsigned sym_addr = 0;
        char *sym_name = NULL;

        DEBUG("%5d Processing '%s' relocation at index %d\n", pid,
              si->name, idx);
        if(sym != 0) {
            sym_name = (char *)(strtab + symtab[sym].st_name);
            s = soinfo_do_lookup(si, sym_name, &offset);
            if(s == NULL) {
                /* We only allow an undefined symbol if this is a weak
                   reference..   */
                s = &symtab[sym];
                if (ELF32_ST_BIND(s->st_info) != STB_WEAK) {
                    DL_ERR("%5d cannot locate '%s'...\n", pid, sym_name);
                    return -1;
                }

                /* IHI0044C AAELF 4.5.1.1:

                   Libraries are not searched to resolve weak references.
                   It is not an error for a weak reference to remain
                   unsatisfied.

                   During linking, the value of an undefined weak reference is:
                   - Zero if the relocation type is absolute
                   - The address of the place if the relocation is pc-relative
                   - The address of nominial base address if the relocation
                     type is base-relative.
                  */

                switch (type) {
#if defined(ANDROID_ARM_LINKER)
                case R_ARM_JUMP_SLOT:
                case R_ARM_GLOB_DAT:
                case R_ARM_ABS32:
                case R_ARM_RELATIVE:    /* Don't care. */
                case R_ARM_NONE:        /* Don't care. */
#elif defined(ANDROID_X86_LINKER)
                case R_386_JUMP_SLOT:
                case R_386_GLOB_DAT:
                case R_386_32:
                case R_386_RELATIVE:    /* Dont' care. */
#endif /* ANDROID_*_LINKER */
                    /* sym_addr was initialized to be zero above or relocation
                       code below does not care about value of sym_addr.
                       No need to do anything.  */
                    break;

#if defined(ANDROID_X86_LINKER)
                case R_386_PC32:
                    sym_addr = reloc;
                    break;
#endif /* ANDROID_X86_LINKER */

#if defined(ANDROID_ARM_LINKER)
                case R_ARM_COPY:
                    /* Fall through.  Can't really copy if weak symbol is
                       not found in run-time.  */
#endif /* ANDROID_ARM_LINKER */
                default:
                    DL_ERR("%5d unknown weak reloc type %d @ %p (%d)\n",
                                 pid, type, rel, (int) (rel - start));
                    return -1;
                }
            } else {
                /* We got a definition.  */
#if 0
            if((base == 0) && (si->base != 0)){
                    /* linking from libraries to main image is bad */
                DL_ERR("%5d cannot locate '%s'...",
                       pid, strtab + symtab[sym].st_name);
                return -1;
            }
#endif
                sym_addr = (unsigned)(s->st_value + offset);
	    }
            COUNT_RELOC(RELOC_SYMBOL);
        } else {
            s = NULL;
        }

/* TODO: This is ugly. Split up the relocations by arch into
 * different files.
 */
        switch(type){
#if defined(ANDROID_ARM_LINKER)
        case R_ARM_JUMP_SLOT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO JMP_SLOT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_ARM_GLOB_DAT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO GLOB_DAT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_ARM_ABS32:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO ABS %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) += sym_addr;
            break;
        case R_ARM_REL32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO REL32 %08x <- %08x - %08x %s\n", pid,
                       reloc, sym_addr, rel->r_offset, sym_name);
            *((unsigned*)reloc) += sym_addr - rel->r_offset;
            break;
#elif defined(ANDROID_X86_LINKER)
        case R_386_JUMP_SLOT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO JMP_SLOT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_386_GLOB_DAT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO GLOB_DAT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
#endif /* ANDROID_*_LINKER */

#if defined(ANDROID_ARM_LINKER)
        case R_ARM_RELATIVE:
#elif defined(ANDROID_X86_LINKER)
        case R_386_RELATIVE:
#endif /* ANDROID_*_LINKER */
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            if(sym){
                DL_ERR("%5d odd RELATIVE form...", pid);
                return -1;
            }
            TRACE_TYPE(RELO, "%5d RELO RELATIVE %08x <- +%08x\n", pid,
                       reloc, si->base);
            *((unsigned*)reloc) += si->base;
            break;

#if defined(ANDROID_X86_LINKER)
        case R_386_32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);

            TRACE_TYPE(RELO, "%5d RELO R_386_32 %08x <- +%08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned *)reloc) += (unsigned)sym_addr;
            break;

        case R_386_PC32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO R_386_PC32 %08x <- "
                       "+%08x (%08x - %08x) %s\n", pid, reloc,
                       (sym_addr - reloc), sym_addr, reloc, sym_name);
            *((unsigned *)reloc) += (unsigned)(sym_addr - reloc);
            break;
#endif /* ANDROID_X86_LINKER */

#ifdef ANDROID_ARM_LINKER
        case R_ARM_COPY:
            COUNT_RELOC(RELOC_COPY);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO %08x <- %d @ %08x %s\n", pid,
                       reloc, s->st_size, sym_addr, sym_name);
            memcpy((void*)reloc, (void*)sym_addr, s->st_size);
            break;
        case R_ARM_NONE:
            break;
#endif /* ANDROID_ARM_LINKER */

        default:
            DL_ERR("%5d unknown reloc type %d @ %p (%d)",
                  pid, type, rel, (int) (rel - start));
            return -1;
        }
        rel++;
    }
    return 0;
}

/* Please read the "Initialization and Termination functions" functions.
 * of the linker design note in bionic/linker/README.TXT to understand
 * what the following code is doing.
 *
 * The important things to remember are:
 *
 *   DT_PREINIT_ARRAY must be called first for executables, and should
 *   not appear in shared libraries.
 *
 *   DT_INIT should be called before DT_INIT_ARRAY if both are present
 *
 *   DT_FINI should be called after DT_FINI_ARRAY if both are present
 *
 *   DT_FINI_ARRAY must be parsed in reverse order.
 */

static void call_array(unsigned *ctor, int count, int reverse)
{
    int n, inc = 1;

    if (reverse) {
        ctor += (count-1);
        inc   = -1;
    }

    for(n = count; n > 0; n--) {
        TRACE("[ %5d Looking at %s *0x%08x == 0x%08x ]\n", pid,
              reverse ? "dtor" : "ctor",
              (unsigned)ctor, (unsigned)*ctor);
        void (*func)() = (void (*)()) *ctor;
        ctor += inc;
        if(((int) func == 0) || ((int) func == -1)) continue;
        TRACE("[ %5d Calling func @ 0x%08x ]\n", pid, (unsigned)func);
        func();
    }
}

void soinfo_call_constructors(soinfo *si)
{
    if (si->constructors_called)
        return;

    // Set this before actually calling the constructors, otherwise it doesn't
    // protect against recursive constructor calls. One simple example of
    // constructor recursion is the libc debug malloc, which is implemented in
    // libc_malloc_debug_leak.so:
    // 1. The program depends on libc, so libc's constructor is called here.
    // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
    // 3. dlopen() calls soinfo_call_constructors() with the newly created
    //    soinfo for libc_malloc_debug_leak.so.
    // 4. The debug so depends on libc, so soinfo_call_constructors() is
    //    called again with the libc soinfo. If it doesn't trigger the early-
    //    out above, the libc constructor will be called again (recursively!).
    si->constructors_called = 1;

    if (si->flags & FLAG_EXE) {
        TRACE("[ %5d Calling preinit_array @ 0x%08x [%d] for '%s' ]\n",
              pid, (unsigned)si->preinit_array, si->preinit_array_count,
              si->name);
        call_array(si->preinit_array, si->preinit_array_count, 0);
        TRACE("[ %5d Done calling preinit_array for '%s' ]\n", pid, si->name);
    } else {
        if (si->preinit_array) {
            DL_ERR("%5d Shared library '%s' has a preinit_array table @ 0x%08x."
                   " This is INVALID.", pid, si->name,
                   (unsigned)si->preinit_array);
        }
    }

    if (si->dynamic) {
        unsigned *d;
        for(d = si->dynamic; *d; d += 2) {
            if(d[0] == DT_NEEDED){
                soinfo* lsi = (soinfo *)d[1];
                if (!validate_soinfo(lsi)) {
                    DL_ERR("%5d bad DT_NEEDED pointer in %s",
                           pid, si->name);
                } else {
                    soinfo_call_constructors(lsi);
                }
            }
        }
    }

    if (si->init_func) {
        TRACE("[ %5d Calling init_func @ 0x%08x for '%s' ]\n", pid,
              (unsigned)si->init_func, si->name);
        si->init_func();
        TRACE("[ %5d Done calling init_func for '%s' ]\n", pid, si->name);
    }

    if (si->init_array) {
        TRACE("[ %5d Calling init_array @ 0x%08x [%d] for '%s' ]\n", pid,
              (unsigned)si->init_array, si->init_array_count, si->name);
        call_array(si->init_array, si->init_array_count, 0);
        TRACE("[ %5d Done calling init_array for '%s' ]\n", pid, si->name);
    }

}

static void call_destructors(soinfo *si)
{
    if (si->fini_array) {
        TRACE("[ %5d Calling fini_array @ 0x%08x [%d] for '%s' ]\n", pid,
              (unsigned)si->fini_array, si->fini_array_count, si->name);
        call_array(si->fini_array, si->fini_array_count, 1);
        TRACE("[ %5d Done calling fini_array for '%s' ]\n", pid, si->name);
    }

    if (si->fini_func) {
        TRACE("[ %5d Calling fini_func @ 0x%08x for '%s' ]\n", pid,
              (unsigned)si->fini_func, si->name);
        si->fini_func();
        TRACE("[ %5d Done calling fini_func for '%s' ]\n", pid, si->name);
    }
}

/* Force any of the closed stdin, stdout and stderr to be associated with
   /dev/null. */
static int nullify_closed_stdio (void)
{
    int dev_null, i, status;
    int return_value = 0;

    dev_null = TEMP_FAILURE_RETRY(open("/dev/null", O_RDWR));
    if (dev_null < 0) {
        DL_ERR("Cannot open /dev/null.");
        return -1;
    }
    TRACE("[ %5d Opened /dev/null file-descriptor=%d]\n", pid, dev_null);

    /* If any of the stdio file descriptors is valid and not associated
       with /dev/null, dup /dev/null to it.  */
    for (i = 0; i < 3; i++) {
        /* If it is /dev/null already, we are done. */
        if (i == dev_null)
            continue;

        TRACE("[ %5d Nullifying stdio file descriptor %d]\n", pid, i);
        /* The man page of fcntl does not say that fcntl(..,F_GETFL)
           can be interrupted but we do this just to be safe. */
        do {
          status = fcntl(i, F_GETFL);
        } while (status < 0 && errno == EINTR);

        /* If file is openned, we are good. */
        if (status >= 0)
          continue;

        /* The only error we allow is that the file descriptor does not
           exist, in which case we dup /dev/null to it. */
        if (errno != EBADF) {
            DL_ERR("nullify_stdio: unhandled error %s", strerror(errno));
            return_value = -1;
            continue;
        }

        /* Try dupping /dev/null to this stdio file descriptor and
           repeat if there is a signal.  Note that any errors in closing
           the stdio descriptor are lost.  */
        do {
            status = dup2(dev_null, i);
        } while (status < 0 && errno == EINTR);

        if (status < 0) {
            DL_ERR("nullify_stdio: dup2 error %s", strerror(errno));
            return_value = -1;
            continue;
        }
    }

    /* If /dev/null is not one of the stdio file descriptors, close it. */
    if (dev_null > 2) {
        TRACE("[ %5d Closing /dev/null file-descriptor=%d]\n", pid, dev_null);
        do {
            status = close(dev_null);
        } while (status < 0 && errno == EINTR);

        if (status < 0) {
            DL_ERR("nullify_stdio: close error %s", strerror(errno));
            return_value = -1;
        }
    }

    return return_value;
}

static int soinfo_link_image(soinfo *si, unsigned wr_offset)
{
    unsigned *d;
    /* "base" might wrap around UINT32_MAX. */
    Elf32_Addr base = si->load_bias;
    const Elf32_Phdr *phdr = si->phdr;
    int phnum = si->phnum;
    int relocating_linker = (si->flags & FLAG_LINKER) != 0;

    /* We can't debug anything until the linker is relocated */
    if (!relocating_linker) {
        INFO("[ %5d linking %s ]\n", pid, si->name);
        DEBUG("%5d si->base = 0x%08x si->flags = 0x%08x\n", pid,
            si->base, si->flags);
    }

    /* Extract dynamic section */
    si->dynamic = phdr_table_get_dynamic_section(phdr, phnum, base);
    if (si->dynamic == NULL) {
        if (!relocating_linker) {
            DL_ERR("%5d missing PT_DYNAMIC?!", pid);
        }
        goto fail;
    } else {
        if (!relocating_linker) {
            DEBUG("%5d dynamic = %p\n", pid, si->dynamic);
        }
    }

#ifdef ANDROID_ARM_LINKER
    (void) phdr_table_get_arm_exidx(phdr, phnum, base,
                                    &si->ARM_exidx, &si->ARM_exidx_count);
#endif

    if (si->flags & (FLAG_EXE | FLAG_LINKER)) {
        if (phdr_table_unprotect_segments(si->phdr,
                                          si->phnum,
                                          si->load_bias) < 0) {
            /* We can't call DL_ERR if the linker's relocations haven't
             * been performed yet */
            if (!relocating_linker) {
                DL_ERR("%5d Can't unprotect segments for %s: %s",
                       pid, si->name, strerror(errno));
            }
            goto fail;
        }
    }

    /* extract useful information from dynamic section */
    for(d = si->dynamic; *d; d++){
        DEBUG("%5d d = %p, d[0] = 0x%08x d[1] = 0x%08x\n", pid, d, d[0], d[1]);
        switch(*d++){
        case DT_HASH:
            si->nbucket = ((unsigned *) (base + *d))[0];
            si->nchain = ((unsigned *) (base + *d))[1];
            si->bucket = (unsigned *) (base + *d + 8);
            si->chain = (unsigned *) (base + *d + 8 + si->nbucket * 4);
            break;
        case DT_STRTAB:
            si->strtab = (const char *) (base + *d);
            break;
        case DT_SYMTAB:
            si->symtab = (Elf32_Sym *) (base + *d);
            break;
        case DT_PLTREL:
            if(*d != DT_REL) {
                DL_ERR("DT_RELA not supported");
                goto fail;
            }
            break;
        case DT_JMPREL:
            si->plt_rel = (Elf32_Rel*) (base + *d);
            break;
        case DT_PLTRELSZ:
            si->plt_rel_count = *d / 8;
            break;
        case DT_REL:
            si->rel = (Elf32_Rel*) (base + *d);
            break;
        case DT_RELSZ:
            si->rel_count = *d / 8;
            break;
        case DT_PLTGOT:
            /* Save this in case we decide to do lazy binding. We don't yet. */
            si->plt_got = (unsigned *)(base + *d);
            break;
        case DT_DEBUG:
            // Set the DT_DEBUG entry to the addres of _r_debug for GDB
            *d = (int) &_r_debug;
            break;
         case DT_RELA:
            DL_ERR("%5d DT_RELA not supported", pid);
            goto fail;
        case DT_INIT:
            si->init_func = (void (*)(void))(base + *d);
            DEBUG("%5d %s constructors (init func) found at %p\n",
                  pid, si->name, si->init_func);
            break;
        case DT_FINI:
            si->fini_func = (void (*)(void))(base + *d);
            DEBUG("%5d %s destructors (fini func) found at %p\n",
                  pid, si->name, si->fini_func);
            break;
        case DT_INIT_ARRAY:
            si->init_array = (unsigned *)(base + *d);
            DEBUG("%5d %s constructors (init_array) found at %p\n",
                  pid, si->name, si->init_array);
            break;
        case DT_INIT_ARRAYSZ:
            si->init_array_count = ((unsigned)*d) / sizeof(Elf32_Addr);
            break;
        case DT_FINI_ARRAY:
            si->fini_array = (unsigned *)(base + *d);
            DEBUG("%5d %s destructors (fini_array) found at %p\n",
                  pid, si->name, si->fini_array);
            break;
        case DT_FINI_ARRAYSZ:
            si->fini_array_count = ((unsigned)*d) / sizeof(Elf32_Addr);
            break;
        case DT_PREINIT_ARRAY:
            si->preinit_array = (unsigned *)(base + *d);
            DEBUG("%5d %s constructors (preinit_array) found at %p\n",
                  pid, si->name, si->preinit_array);
            break;
        case DT_PREINIT_ARRAYSZ:
            si->preinit_array_count = ((unsigned)*d) / sizeof(Elf32_Addr);
            break;
        case DT_TEXTREL:
            /* TODO: make use of this. */
            /* this means that we might have to write into where the text
             * segment was loaded during relocation... Do something with
             * it.
             */
            DEBUG("%5d Text segment should be writable during relocation.\n",
                  pid);
            break;
        }
    }

    DEBUG("%5d si->base = 0x%08x, si->strtab = %p, si->symtab = %p\n",
           pid, si->base, si->strtab, si->symtab);

    if((si->strtab == 0) || (si->symtab == 0)) {
        DL_ERR("%5d missing essential tables", pid);
        goto fail;
    }

    /* if this is the main executable, then load all of the preloads now */
    if(si->flags & FLAG_EXE) {
        int i;
        memset(preloads, 0, sizeof(preloads));
        for(i = 0; ldpreload_names[i] != NULL; i++) {
            soinfo *lsi = find_library(ldpreload_names[i]);
            if(lsi == 0) {
                strlcpy(tmp_err_buf, linker_get_error(), sizeof(tmp_err_buf));
                DL_ERR("%5d could not load needed library '%s' for '%s' (%s)",
                       pid, ldpreload_names[i], si->name, tmp_err_buf);
                goto fail;
            }
            lsi->refcount++;
            preloads[i] = lsi;
        }
    }

    for(d = si->dynamic; *d; d += 2) {
        if(d[0] == DT_NEEDED){
            DEBUG("%5d %s needs %s\n", pid, si->name, si->strtab + d[1]);
            soinfo *lsi = find_library(si->strtab + d[1]);
            if(lsi == 0) {
                strlcpy(tmp_err_buf, linker_get_error(), sizeof(tmp_err_buf));
                DL_ERR("%5d could not load needed library '%s' for '%s' (%s)",
                       pid, si->strtab + d[1], si->name, tmp_err_buf);
                goto fail;
            }
            /* Save the soinfo of the loaded DT_NEEDED library in the payload
               of the DT_NEEDED entry itself, so that we can retrieve the
               soinfo directly later from the dynamic segment.  This is a hack,
               but it allows us to map from DT_NEEDED to soinfo efficiently
               later on when we resolve relocations, trying to look up a symbol
               with dlsym().
            */
            d[1] = (unsigned)lsi;
            lsi->refcount++;
        }
    }

    if(si->plt_rel) {
        DEBUG("[ %5d relocating %s plt ]\n", pid, si->name );
        if(soinfo_relocate(si, si->plt_rel, si->plt_rel_count))
            goto fail;
    }
    if(si->rel) {
        DEBUG("[ %5d relocating %s ]\n", pid, si->name );
        if(soinfo_relocate(si, si->rel, si->rel_count))
            goto fail;
    }

    si->flags |= FLAG_LINKED;
    DEBUG("[ %5d finished linking %s ]\n", pid, si->name);

    /* All relocations are done, we can protect our segments back to
     * read-only. */
    if (phdr_table_protect_segments(si->phdr, si->phnum, si->load_bias) < 0) {
        DL_ERR("%5d Can't protect segments for %s: %s",
               pid, si->name, strerror(errno));
        goto fail;
    }

    /* We can also turn on GNU RELRO protection */
    if (phdr_table_protect_gnu_relro(si->phdr, si->phnum, si->load_bias) < 0) {
        DL_ERR("%5d Can't enable GNU RELRO protection for %s: %s",
               pid, si->name, strerror(errno));
        goto fail;
    }

    /* If this is a SET?ID program, dup /dev/null to opened stdin,
       stdout and stderr to close a security hole described in:

    ftp://ftp.freebsd.org/pub/FreeBSD/CERT/advisories/FreeBSD-SA-02:23.stdio.asc

     */
    if (program_is_setuid)
        nullify_closed_stdio ();
    notify_gdb_of_load(si);
    return 0;

fail:
    ERROR("failed to link %s\n", si->name);
    si->flags |= FLAG_ERROR;
    return -1;
}

static void parse_library_path(const char *path, char *delim)
{
    size_t len;
    char *ldpaths_bufp = ldpaths_buf;
    int i = 0;

    len = strlcpy(ldpaths_buf, path, sizeof(ldpaths_buf));

    while (i < LDPATH_MAX && (ldpaths[i] = strsep(&ldpaths_bufp, delim))) {
        if (*ldpaths[i] != '\0')
            ++i;
    }

    /* Forget the last path if we had to truncate; this occurs if the 2nd to
     * last char isn't '\0' (i.e. not originally a delim). */
    if (i > 0 && len >= sizeof(ldpaths_buf) &&
            ldpaths_buf[sizeof(ldpaths_buf) - 2] != '\0') {
        ldpaths[i - 1] = NULL;
    } else {
        ldpaths[i] = NULL;
    }
}

static void parse_preloads(const char *path, char *delim)
{
    size_t len;
    char *ldpreloads_bufp = ldpreloads_buf;
    int i = 0;

    len = strlcpy(ldpreloads_buf, path, sizeof(ldpreloads_buf));

    while (i < LDPRELOAD_MAX && (ldpreload_names[i] = strsep(&ldpreloads_bufp, delim))) {
        if (*ldpreload_names[i] != '\0') {
            ++i;
        }
    }

    /* Forget the last path if we had to truncate; this occurs if the 2nd to
     * last char isn't '\0' (i.e. not originally a delim). */
    if (i > 0 && len >= sizeof(ldpreloads_buf) &&
            ldpreloads_buf[sizeof(ldpreloads_buf) - 2] != '\0') {
        ldpreload_names[i - 1] = NULL;
    } else {
        ldpreload_names[i] = NULL;
    }
}

/*
 * This code is called after the linker has linked itself and
 * fixed it's own GOT. It is safe to make references to externs
 * and other non-local data at this point.
 */
static unsigned __linker_init_post_relocation(unsigned **elfdata)
{
    static soinfo linker_soinfo;

    int argc = (int) *elfdata;
    char **argv = (char**) (elfdata + 1);
    unsigned *vecs = (unsigned*) (argv + argc + 1);
    unsigned *v;
    soinfo *si;
    struct link_map * map;
    const char *ldpath_env = NULL;
    const char *ldpreload_env = NULL;

    /* NOTE: we store the elfdata pointer on a special location
     *       of the temporary TLS area in order to pass it to
     *       the C Library's runtime initializer.
     *
     *       The initializer must clear the slot and reset the TLS
     *       to point to a different location to ensure that no other
     *       shared library constructor can access it.
     */
    __libc_init_tls(elfdata);

    pid = getpid();

#if TIMING
    struct timeval t0, t1;
    gettimeofday(&t0, 0);
#endif

    /* Initialize environment functions, and get to the ELF aux vectors table */
    vecs = linker_env_init(vecs);

    /* Check auxv for AT_SECURE first to see if program is setuid, setgid,
       has file caps, or caused a SELinux/AppArmor domain transition. */
    for (v = vecs; v[0]; v += 2) {
        if (v[0] == AT_SECURE) {
            /* kernel told us whether to enable secure mode */
            program_is_setuid = v[1];
            goto sanitize;
        }
    }

    /* Kernel did not provide AT_SECURE - fall back on legacy test. */
    program_is_setuid = (getuid() != geteuid()) || (getgid() != getegid());

sanitize:
    /* Sanitize environment if we're loading a setuid program */
    if (program_is_setuid)
        linker_env_secure();

    debugger_init();

    /* Get a few environment variables */
    {
#if LINKER_DEBUG
        const char* env;
        env = linker_env_get("DEBUG"); /* XXX: TODO: Change to LD_DEBUG */
        if (env)
            debug_verbosity = atoi(env);
#endif

        /* Normally, these are cleaned by linker_env_secure, but the test
         * against program_is_setuid doesn't cost us anything */
        if (!program_is_setuid) {
            ldpath_env = linker_env_get("LD_LIBRARY_PATH");
            ldpreload_env = linker_env_get("LD_PRELOAD");
        }
    }

    INFO("[ android linker & debugger ]\n");
    DEBUG("%5d elfdata @ 0x%08x\n", pid, (unsigned)elfdata);

    si = soinfo_alloc(argv[0]);
    if(si == 0) {
        exit(-1);
    }

        /* bootstrap the link map, the main exe always needs to be first */
    si->flags |= FLAG_EXE;
    map = &(si->linkmap);

    map->l_addr = 0;
    map->l_name = argv[0];
    map->l_prev = NULL;
    map->l_next = NULL;

    _r_debug.r_map = map;
    r_debug_tail = map;

        /* gdb expects the linker to be in the debug shared object list,
         * and we need to make sure that the reported load address is zero.
         * Without this, gdb gets the wrong idea of where rtld_db_dlactivity()
         * is.  Don't use soinfo_alloc(), because the linker shouldn't
         * be on the soinfo list.
         */
    strlcpy((char*) linker_soinfo.name, "/system/bin/linker", sizeof linker_soinfo.name);
    linker_soinfo.flags = 0;
    linker_soinfo.base = 0;     // This is the important part; must be zero.
    insert_soinfo_into_debug_map(&linker_soinfo);

        /* extract information passed from the kernel */
    while(vecs[0] != 0){
        switch(vecs[0]){
        case AT_PHDR:
            si->phdr = (Elf32_Phdr*) vecs[1];
            break;
        case AT_PHNUM:
            si->phnum = (int) vecs[1];
            break;
        case AT_ENTRY:
            si->entry = vecs[1];
            break;
        }
        vecs += 2;
    }

    /* Compute the value of si->base. We can't rely on the fact that
     * the first entry is the PHDR because this will not be true
     * for certain executables (e.g. some in the NDK unit test suite)
     */
    int nn;
    si->base = 0;
    si->size = phdr_table_get_load_size(si->phdr, si->phnum);
    si->load_bias = 0;
    for ( nn = 0; nn < si->phnum; nn++ ) {
        if (si->phdr[nn].p_type == PT_PHDR) {
            si->load_bias = (Elf32_Addr)si->phdr - si->phdr[nn].p_vaddr;
            si->base = (Elf32_Addr) si->phdr - si->phdr[nn].p_offset;
            break;
        }
    }
    si->dynamic = (unsigned *)-1;
    si->refcount = 1;

        /* Use LD_LIBRARY_PATH if we aren't setuid/setgid */
    if (ldpath_env)
        parse_library_path(ldpath_env, ":");

    if (ldpreload_env) {
        parse_preloads(ldpreload_env, " :");
    }

    if(soinfo_link_image(si, 0)) {
        char errmsg[] = "CANNOT LINK EXECUTABLE\n";
        write(2, __linker_dl_err_buf, strlen(__linker_dl_err_buf));
        write(2, errmsg, sizeof(errmsg));
        exit(-1);
    }

    soinfo_call_constructors(si);

#if ALLOW_SYMBOLS_FROM_MAIN
    /* Set somain after we've loaded all the libraries in order to prevent
     * linking of symbols back to the main image, which is not set up at that
     * point yet.
     */
    somain = si;
#endif

#if TIMING
    gettimeofday(&t1,NULL);
    PRINT("LINKER TIME: %s: %d microseconds\n", argv[0], (int) (
               (((long long)t1.tv_sec * 1000000LL) + (long long)t1.tv_usec) -
               (((long long)t0.tv_sec * 1000000LL) + (long long)t0.tv_usec)
               ));
#endif
#if STATS
    PRINT("RELO STATS: %s: %d abs, %d rel, %d copy, %d symbol\n", argv[0],
           linker_stats.reloc[RELOC_ABSOLUTE],
           linker_stats.reloc[RELOC_RELATIVE],
           linker_stats.reloc[RELOC_COPY],
           linker_stats.reloc[RELOC_SYMBOL]);
#endif
#if COUNT_PAGES
    {
        unsigned n;
        unsigned i;
        unsigned count = 0;
        for(n = 0; n < 4096; n++){
            if(bitmask[n]){
                unsigned x = bitmask[n];
                for(i = 0; i < 8; i++){
                    if(x & 1) count++;
                    x >>= 1;
                }
            }
        }
        PRINT("PAGES MODIFIED: %s: %d (%dKB)\n", argv[0], count, count * 4);
    }
#endif

#if TIMING || STATS || COUNT_PAGES
    fflush(stdout);
#endif

    TRACE("[ %5d Ready to execute '%s' @ 0x%08x ]\n", pid, si->name,
          si->entry);
    return si->entry;
}

/*
 * Find the value of AT_BASE passed to us by the kernel. This is the load
 * location of the linker.
 */
static unsigned find_linker_base(unsigned **elfdata) {
    int argc = (int) *elfdata;
    char **argv = (char**) (elfdata + 1);
    unsigned *vecs = (unsigned*) (argv + argc + 1);
    while (vecs[0] != 0) {
        vecs++;
    }

    /* The end of the environment block is marked by two NULL pointers */
    vecs++;

    while(vecs[0]) {
        if (vecs[0] == AT_BASE) {
            return vecs[1];
        }
        vecs += 2;
    }

    return 0; // should never happen
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
static Elf32_Addr
get_elf_exec_load_bias(const Elf32_Ehdr* elf)
{
    Elf32_Addr        offset     = elf->e_phoff;
    const Elf32_Phdr* phdr_table = (const Elf32_Phdr*)((char*)elf + offset);
    const Elf32_Phdr* phdr_end   = phdr_table + elf->e_phnum;
    const Elf32_Phdr* phdr;

    for (phdr = phdr_table; phdr < phdr_end; phdr++) {
        if (phdr->p_type == PT_LOAD) {
            return (Elf32_Addr)elf + phdr->p_offset - phdr->p_vaddr;
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
unsigned __linker_init(unsigned **elfdata) {
    unsigned linker_addr = find_linker_base(elfdata);
    Elf32_Ehdr *elf_hdr = (Elf32_Ehdr *) linker_addr;
    Elf32_Phdr *phdr =
        (Elf32_Phdr *)((unsigned char *) linker_addr + elf_hdr->e_phoff);

    soinfo linker_so;
    memset(&linker_so, 0, sizeof(soinfo));

    linker_so.base = linker_addr;
    linker_so.size = phdr_table_get_load_size(phdr, elf_hdr->e_phnum);
    linker_so.load_bias = get_elf_exec_load_bias(elf_hdr);
    linker_so.dynamic = (unsigned *) -1;
    linker_so.phdr = phdr;
    linker_so.phnum = elf_hdr->e_phnum;
    linker_so.flags |= FLAG_LINKER;

    if (soinfo_link_image(&linker_so, 0)) {
        // It would be nice to print an error message, but if the linker
        // can't link itself, there's no guarantee that we'll be able to
        // call write() (because it involves a GOT reference).
        //
        // This situation should never occur unless the linker itself
        // is corrupt.
        exit(-1);
    }

    // We have successfully fixed our own relocations. It's safe to run
    // the main part of the linker now.
    return __linker_init_post_relocation(elfdata);
}
