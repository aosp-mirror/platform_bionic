/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>

#include <private/ScopedPthreadMutexLocker.h>

#include "linker.h"
#include "linker_format.h"

/* This file hijacks the symbols stubbed out in libdl.so. */

static char dl_err_buf[1024];
static const char* dl_err_str;

#define likely(expr)   __builtin_expect (expr, 1)
#define unlikely(expr) __builtin_expect (expr, 0)

pthread_mutex_t dl_lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;

static void set_dlerror(const char* msg, const char* detail) {
  if (detail != NULL) {
    format_buffer(dl_err_buf, sizeof(dl_err_buf), "%s: %s", msg, detail);
  } else {
    format_buffer(dl_err_buf, sizeof(dl_err_buf), "%s", msg);
  }
  dl_err_str = (const char*) &dl_err_buf[0];
}

void *dlopen(const char* filename, int flag) {
  ScopedPthreadMutexLocker locker(&dl_lock);
  soinfo* result = find_library(filename);
  if (result == NULL) {
    set_dlerror("dlopen failed", linker_get_error());
    return NULL;
  }
  soinfo_call_constructors(result);
  result->refcount++;
  return result;
}

const char* dlerror() {
  const char* old_value = dl_err_str;
  dl_err_str = NULL;
  return (const char*) old_value;
}

void* dlsym(void* handle, const char* symbol) {
  ScopedPthreadMutexLocker locker(&dl_lock);

  if (unlikely(handle == 0)) {
    set_dlerror("dlsym library handle is null", NULL);
    return NULL;
  }
  if (unlikely(symbol == 0)) {
    set_dlerror("dlsym symbol name is null", NULL);
    return NULL;
  }

  soinfo* found = NULL;
  Elf32_Sym* sym = NULL;
  if (handle == RTLD_DEFAULT) {
    sym = lookup(symbol, &found, NULL);
  } else if (handle == RTLD_NEXT) {
    void* ret_addr = __builtin_return_address(0);
    soinfo* si = find_containing_library(ret_addr);

    sym = NULL;
    if (si && si->next) {
      sym = lookup(symbol, &found, si->next);
    }
  } else {
    found = (soinfo*) handle;
    sym = soinfo_lookup(found, symbol);
  }

  if (likely(sym != 0)) {
    unsigned bind = ELF32_ST_BIND(sym->st_info);

    if (likely((bind == STB_GLOBAL) && (sym->st_shndx != 0))) {
      unsigned ret = sym->st_value + found->load_bias;
      return (void*) ret;
    }

    set_dlerror("symbol found but not global", symbol);
    return NULL;
  } else {
    set_dlerror("undefined symbol", symbol);
    return NULL;
  }
}

int dladdr(const void* addr, Dl_info* info) {
  ScopedPthreadMutexLocker locker(&dl_lock);

  // Determine if this address can be found in any library currently mapped.
  soinfo* si = find_containing_library(addr);
  if (si == NULL) {
    return 0;
  }

  memset(info, 0, sizeof(Dl_info));

  info->dli_fname = si->name;
  // Address at which the shared object is loaded.
  info->dli_fbase = (void*) si->base;

  // Determine if any symbol in the library contains the specified address.
  Elf32_Sym *sym = soinfo_find_symbol(si, addr);
  if (sym != NULL) {
    info->dli_sname = si->strtab + sym->st_name;
    info->dli_saddr = (void*)(si->load_bias + sym->st_value);
  }

  return 1;
}

int dlclose(void* handle) {
  ScopedPthreadMutexLocker locker(&dl_lock);
  return soinfo_unload((soinfo*) handle);
}

#if defined(ANDROID_ARM_LINKER)
//                     0000000 00011111 111112 22222222 2333333 333344444444445555555
//                     0123456 78901234 567890 12345678 9012345 678901234567890123456
#define ANDROID_LIBDL_STRTAB \
                      "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0dl_unwind_find_exidx\0"

#elif defined(ANDROID_X86_LINKER) || defined(ANDROID_MIPS_LINKER)
//                     0000000 00011111 111112 22222222 2333333 3333444444444455
//                     0123456 78901234 567890 12345678 9012345 6789012345678901
#define ANDROID_LIBDL_STRTAB \
                      "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0dl_iterate_phdr\0"
#else
#error Unsupported architecture. Only ARM, MIPS, and x86 are presently supported.
#endif

// name_offset: starting index of the name in libdl_info.strtab
#define ELF32_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      reinterpret_cast<Elf32_Addr>(reinterpret_cast<void*>(value)), \
      /* st_size */ 0, \
      (shndx == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx }

static Elf32_Sym libdl_symtab[] = {
  // Total length of libdl_info.strtab, including trailing 0.
  // This is actually the STH_UNDEF entry. Technically, it's
  // supposed to have st_name == 0, but instead, it points to an index
  // in the strtab with a \0 to make iterating through the symtab easier.
  ELF32_SYM_INITIALIZER(sizeof(ANDROID_LIBDL_STRTAB) - 1, NULL, 0),
  ELF32_SYM_INITIALIZER( 0, &dlopen, 1),
  ELF32_SYM_INITIALIZER( 7, &dlclose, 1),
  ELF32_SYM_INITIALIZER(15, &dlsym, 1),
  ELF32_SYM_INITIALIZER(21, &dlerror, 1),
  ELF32_SYM_INITIALIZER(29, &dladdr, 1),
#if defined(ANDROID_ARM_LINKER)
  ELF32_SYM_INITIALIZER(36, &dl_unwind_find_exidx, 1),
#elif defined(ANDROID_X86_LINKER) || defined(ANDROID_MIPS_LINKER)
  ELF32_SYM_INITIALIZER(36, &dl_iterate_phdr, 1),
#endif
};

/* Fake out a hash table with a single bucket.
 * A search of the hash table will look through
 * libdl_symtab starting with index [1], then
 * use libdl_chains to find the next index to
 * look at.  libdl_chains should be set up to
 * walk through every element in libdl_symtab,
 * and then end with 0 (sentinel value).
 *
 * I.e., libdl_chains should look like
 * { 0, 2, 3, ... N, 0 } where N is the number
 * of actual symbols, or nelems(libdl_symtab)-1
 * (since the first element of libdl_symtab is not
 * a real symbol).
 *
 * (see _elf_lookup())
 *
 * Note that adding any new symbols here requires
 * stubbing them out in libdl.
 */
static unsigned libdl_buckets[1] = { 1 };
static unsigned libdl_chains[7] = { 0, 2, 3, 4, 5, 6, 0 };

soinfo libdl_info = {
    name: "libdl.so",

    phdr: 0, phnum: 0,
    entry: 0, base: 0, size: 0,
    unused: 0, dynamic: 0, unused2: 0, unused3: 0,
    next: 0,

    flags: FLAG_LINKED,

    strtab: ANDROID_LIBDL_STRTAB,
    symtab: libdl_symtab,

    nbucket: 1,
    nchain: 7,
    bucket: libdl_buckets,
    chain: libdl_chains,

    plt_got: 0, plt_rel: 0, plt_rel_count: 0, rel: 0, rel_count: 0,
    preinit_array: 0, preinit_array_count: 0, init_array: 0, init_array_count: 0,
    fini_array: 0, fini_array_count: 0, init_func: 0, fini_func: 0,

#if defined(ANDROID_ARM_LINKER)
    ARM_exidx: 0, ARM_exidx_count: 0,
#elif defined(ANDROID_MIPS_LINKER)
    mips_symtabno: 0, mips_local_gotno: 0, mips_gotsym: 0,
#endif

    refcount: 0,
    { l_addr: 0, l_name: 0, l_ld: 0, l_next: 0, l_prev: 0, },
    constructors_called: 0, load_bias: 0, has_text_relocations: 0,
};
