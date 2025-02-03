// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <dlfcn.h>

#include "header_checks.h"

static void dlfcn_h() {
  MACRO(RTLD_LAZY);
  MACRO(RTLD_NOW);
  MACRO(RTLD_GLOBAL);
  MACRO(RTLD_LOCAL);

#if !defined(__GLIBC__)  // Our glibc is too old.
  TYPE(Dl_info);
  STRUCT_MEMBER(Dl_info, const char*, dli_fname);
  STRUCT_MEMBER(Dl_info, void*, dli_fbase);
  STRUCT_MEMBER(Dl_info, const char*, dli_sname);
  STRUCT_MEMBER(Dl_info, void*, dli_saddr);
#endif

#if !defined(__GLIBC__)  // Our glibc is too old.
  FUNCTION(dladdr, int (*f)(const void*, Dl_info*));
#endif
  FUNCTION(dlclose, int (*f)(void*));
  FUNCTION(dlerror, char* (*f)(void));
  FUNCTION(dlopen, void* (*f)(const char*, int));
  FUNCTION(dlsym, void* (*f)(void*, const char*));
}
