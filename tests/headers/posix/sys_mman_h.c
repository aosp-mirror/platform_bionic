// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include "header_checks.h"

static void sys_mman_h() {
  MACRO(PROT_EXEC);
  MACRO(PROT_NONE);
  MACRO(PROT_READ);
  MACRO(PROT_WRITE);

  MACRO(MAP_FIXED);
  MACRO(MAP_PRIVATE);
  MACRO(MAP_SHARED);

  MACRO(MS_ASYNC);
  MACRO(MS_INVALIDATE);
  MACRO(MS_SYNC);

  MACRO(MCL_CURRENT);
  MACRO(MCL_FUTURE);

  void* p;
  p = MAP_FAILED;

  MACRO(POSIX_MADV_DONTNEED);
  MACRO(POSIX_MADV_NORMAL);
  MACRO(POSIX_MADV_RANDOM);
  MACRO(POSIX_MADV_SEQUENTIAL);
  MACRO(POSIX_MADV_WILLNEED);

#if !defined(__linux__)
  MACRO(POSIX_TYPED_MEM_ALLOCATE);
  MACRO(POSIX_TYPED_MEM_ALLOCATE_CONTIG);
  MACRO(POSIX_TYPED_MEM_ALLOCATABLE);
#endif

  TYPE(mode_t);
  TYPE(off_t);
  TYPE(size_t);

#if !defined(__linux__)
  TYPE(struct posix_typed_mem_info);
  STRUCT_MEMBER(struct posix_typed_mem_info, size_t, posix_tmi_length);
#endif

  FUNCTION(mlock, int (*f)(const void*, size_t));
  FUNCTION(mlockall, int (*f)(int));
  FUNCTION(mmap, void* (*f)(void*, size_t, int, int, int, off_t));
  FUNCTION(mprotect, int (*f)(void*, size_t, int));
  FUNCTION(msync, int (*f)(void*, size_t, int));
  FUNCTION(munlock, int (*f)(const void*, size_t));
  FUNCTION(munlockall, int (*f)(void));
  FUNCTION(munmap, int (*f)(void*, size_t));
  FUNCTION(posix_madvise, int (*f)(void*, size_t, int));
#if !defined(__linux__)
  FUNCTION(posix_mem_offset, int (*f)(const void*, size_t, off_t*, size_t*, int*));
  FUNCTION(posix_typed_mem_get_info, int (*f)(int, struct posix_typed_mem_info*));
  FUNCTION(posix_typed_mem_open, int (*f)(const char*, int, int));
#endif
#if !defined(__BIONIC__) // Disallowed by SELinux, so not implemented.
  FUNCTION(shm_open, int (*f)(const char*, int, mode_t));
  FUNCTION(shm_unlink, int (*f)(const char*));
#endif
}
