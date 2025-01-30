/*
 * Copyright (C) 2017 The Android Open Source Project
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
