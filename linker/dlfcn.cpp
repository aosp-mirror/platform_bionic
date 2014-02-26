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

#include "linker.h"

#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <bionic/pthread_internal.h>
#include "private/bionic_tls.h"
#include "private/ScopedPthreadMutexLocker.h"
#include "private/ThreadLocalBuffer.h"

/* This file hijacks the symbols stubbed out in libdl.so. */

static pthread_mutex_t gDlMutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;

static const char* __bionic_set_dlerror(char* new_value) {
  char** dlerror_slot = &reinterpret_cast<char**>(__get_tls())[TLS_SLOT_DLERROR];

  const char* old_value = *dlerror_slot;
  *dlerror_slot = new_value;
  return old_value;
}

static void __bionic_format_dlerror(const char* msg, const char* detail) {
  char* buffer = __get_thread()->dlerror_buffer;
  strlcpy(buffer, msg, __BIONIC_DLERROR_BUFFER_SIZE);
  if (detail != NULL) {
    strlcat(buffer, ": ", __BIONIC_DLERROR_BUFFER_SIZE);
    strlcat(buffer, detail, __BIONIC_DLERROR_BUFFER_SIZE);
  }

  __bionic_set_dlerror(buffer);
}

const char* dlerror() {
  const char* old_value = __bionic_set_dlerror(NULL);
  return old_value;
}

void android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  ScopedPthreadMutexLocker locker(&gDlMutex);
  do_android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  ScopedPthreadMutexLocker locker(&gDlMutex);
  do_android_update_LD_LIBRARY_PATH(ld_library_path);
}

void* dlopen(const char* filename, int flags) {
  ScopedPthreadMutexLocker locker(&gDlMutex);
  soinfo* result = do_dlopen(filename, flags);
  if (result == NULL) {
    __bionic_format_dlerror("dlopen failed", linker_get_error_buffer());
    return NULL;
  }
  return result;
}

void* dlsym(void* handle, const char* symbol) {
  ScopedPthreadMutexLocker locker(&gDlMutex);

  if (handle == NULL) {
    __bionic_format_dlerror("dlsym library handle is null", NULL);
    return NULL;
  }
  if (symbol == NULL) {
    __bionic_format_dlerror("dlsym symbol name is null", NULL);
    return NULL;
  }

  soinfo* found = NULL;
  ElfW(Sym)* sym = NULL;
  if (handle == RTLD_DEFAULT) {
    sym = dlsym_linear_lookup(symbol, &found, NULL);
  } else if (handle == RTLD_NEXT) {
    void* ret_addr = __builtin_return_address(0);
    soinfo* si = find_containing_library(ret_addr);

    sym = NULL;
    if (si && si->next) {
      sym = dlsym_linear_lookup(symbol, &found, si->next);
    }
  } else {
    found = reinterpret_cast<soinfo*>(handle);
    sym = dlsym_handle_lookup(found, symbol);
  }

  if (sym != NULL) {
    unsigned bind = ELF_ST_BIND(sym->st_info);

    if (bind == STB_GLOBAL && sym->st_shndx != 0) {
      return reinterpret_cast<void*>(sym->st_value + found->load_bias);
    }

    __bionic_format_dlerror("symbol found but not global", symbol);
    return NULL;
  } else {
    __bionic_format_dlerror("undefined symbol", symbol);
    return NULL;
  }
}

int dladdr(const void* addr, Dl_info* info) {
  ScopedPthreadMutexLocker locker(&gDlMutex);

  // Determine if this address can be found in any library currently mapped.
  soinfo* si = find_containing_library(addr);
  if (si == NULL) {
    return 0;
  }

  memset(info, 0, sizeof(Dl_info));

  info->dli_fname = si->name;
  // Address at which the shared object is loaded.
  info->dli_fbase = reinterpret_cast<void*>(si->base);

  // Determine if any symbol in the library contains the specified address.
  ElfW(Sym)* sym = dladdr_find_symbol(si, addr);
  if (sym != NULL) {
    info->dli_sname = si->strtab + sym->st_name;
    info->dli_saddr = reinterpret_cast<void*>(si->load_bias + sym->st_value);
  }

  return 1;
}

int dlclose(void* handle) {
  ScopedPthreadMutexLocker locker(&gDlMutex);
  return do_dlclose(reinterpret_cast<soinfo*>(handle));
}

// name_offset: starting index of the name in libdl_info.strtab
#define ELF32_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      reinterpret_cast<Elf32_Addr>(reinterpret_cast<void*>(value)), \
      /* st_size */ 0, \
      (shndx == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx, \
    }

#define ELF64_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      (shndx == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx, \
      reinterpret_cast<Elf64_Addr>(reinterpret_cast<void*>(value)), \
      /* st_size */ 0, \
    }

#if defined(__arm__)
  // 0000000 00011111 111112 22222222 2333333 3333444444444455555555556666666 6667777777777888888888899999 9999900000000001 1
  // 0123456 78901234 567890 12345678 9012345 6789012345678901234567890123456 7890123456789012345678901234 5678901234567890 1
#  define ANDROID_LIBDL_STRTAB \
    "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0android_update_LD_LIBRARY_PATH\0android_get_LD_LIBRARY_PATH\0dl_iterate_phdr\0dl_unwind_find_exidx\0"
#elif defined(__aarch64__) || defined(__i386__) || defined(__mips__) || defined(__x86_64__)
  // 0000000 00011111 111112 22222222 2333333 3333444444444455555555556666666 6667777777777888888888899999 9999900000000001 1
  // 0123456 78901234 567890 12345678 9012345 6789012345678901234567890123456 7890123456789012345678901234 5678901234567890 1
#  define ANDROID_LIBDL_STRTAB \
    "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0android_update_LD_LIBRARY_PATH\0android_get_LD_LIBRARY_PATH\0dl_iterate_phdr\0"
#else
#  error Unsupported architecture. Only arm, arm64, mips, mips64, x86 and x86_64 are presently supported.
#endif

static ElfW(Sym) gLibDlSymtab[] = {
  // Total length of libdl_info.strtab, including trailing 0.
  // This is actually the STH_UNDEF entry. Technically, it's
  // supposed to have st_name == 0, but instead, it points to an index
  // in the strtab with a \0 to make iterating through the symtab easier.
  ELFW(SYM_INITIALIZER)(sizeof(ANDROID_LIBDL_STRTAB) - 1, NULL, 0),
  ELFW(SYM_INITIALIZER)(  0, &dlopen, 1),
  ELFW(SYM_INITIALIZER)(  7, &dlclose, 1),
  ELFW(SYM_INITIALIZER)( 15, &dlsym, 1),
  ELFW(SYM_INITIALIZER)( 21, &dlerror, 1),
  ELFW(SYM_INITIALIZER)( 29, &dladdr, 1),
  ELFW(SYM_INITIALIZER)( 36, &android_update_LD_LIBRARY_PATH, 1),
  ELFW(SYM_INITIALIZER)( 67, &android_get_LD_LIBRARY_PATH, 1),
  ELFW(SYM_INITIALIZER)( 95, &dl_iterate_phdr, 1),
#if defined(__arm__)
  ELFW(SYM_INITIALIZER)(111, &dl_unwind_find_exidx, 1),
#endif
};

// Fake out a hash table with a single bucket.
//
// A search of the hash table will look through gLibDlSymtab starting with index 1, then
// use gLibDlChains to find the next index to look at. gLibDlChains should be set up to
// walk through every element in gLibDlSymtab, and then end with 0 (sentinel value).
//
// That is, gLibDlChains should look like { 0, 2, 3, ... N, 0 } where N is the number
// of actual symbols, or nelems(gLibDlSymtab)-1 (since the first element of gLibDlSymtab is not
// a real symbol). (See soinfo_elf_lookup().)
//
// Note that adding any new symbols here requires stubbing them out in libdl.
static unsigned gLibDlBuckets[1] = { 1 };
#if defined(__arm__)
static unsigned gLibDlChains[] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
#else
static unsigned gLibDlChains[] = { 0, 2, 3, 4, 5, 6, 7, 8, 0 };
#endif

// This is used by the dynamic linker. Every process gets these symbols for free.
soinfo libdl_info = {
    "libdl.so",

    .phdr = 0,
    .phnum = 0,
    .entry = 0,
    .base = 0,
    .size = 0,

#if !defined(__LP64__)
    .unused1 = 0,
#endif

    .dynamic = 0,

#if !defined(__LP64__)
    .unused2 = 0, .unused3 = 0,
#endif

    .next = 0,

    .flags = FLAG_LINKED,

    .strtab = ANDROID_LIBDL_STRTAB,
    .symtab = gLibDlSymtab,

    .nbucket = sizeof(gLibDlBuckets)/sizeof(unsigned),
    .nchain = sizeof(gLibDlChains)/sizeof(unsigned),
    .bucket = gLibDlBuckets,
    .chain = gLibDlChains,

#if defined(USE_RELA)
    .plt_rela = 0,
    .plt_rela_count = 0,
    .rela = 0,
    .rela_count = 0,
#else
    .plt_got = 0,
    .plt_rel = 0,
    .plt_rel_count = 0,
    .rel = 0,
    .rel_count = 0,
#endif

    .preinit_array = 0,
    .preinit_array_count = 0,

    .init_array = 0,
    .init_array_count = 0,

    .fini_array = 0,
    .fini_array_count = 0,

    .init_func = 0,
    .fini_func = 0,

#if defined(__arm__)
    .ARM_exidx = 0,
    .ARM_exidx_count = 0,
#elif defined(__mips__)
    .mips_symtabno = 0,
    .mips_local_gotno = 0,
    .mips_gotsym = 0,
#endif

    .ref_count = 0,
    { .l_addr = 0, .l_name = 0, .l_ld = 0, .l_next = 0, .l_prev = 0, },
    .constructors_called = false,
    .load_bias = 0,
#if !defined(__LP64__)
    .has_text_relocations = false,
#endif
    .has_DT_SYMBOLIC = true,
};
