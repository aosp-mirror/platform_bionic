/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef __DLFCN_H__
#define __DLFCN_H__

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

typedef struct {
  /* Pathname of shared object that contains address. */
  const char* _Nullable dli_fname;
  /* Address at which shared object is loaded. */
  void* _Nullable dli_fbase;
  /* Name of nearest symbol with address lower than addr. */
  const char* _Nullable dli_sname;
  /* Exact address of symbol named in dli_sname. */
  void* _Nullable dli_saddr;
} Dl_info;

void* _Nullable dlopen(const char* _Nullable __filename, int __flag);

/**
 * [dlclose(3)](http://man7.org/linux/man-pages/man3/dlclose.3.html)
 * decrements the reference count for the given shared library (and
 * any libraries brought in by that library's DT_NEEDED entries).
 *
 * If a library's reference count hits zero, it may be unloaded.
 * Code that relies on this is not portable, and may not work on
 * future versions of Android.
 *
 * dlclose() is dangerous because function pointers may or may not
 * be rendered invalid, global data may or may not be rendered invalid,
 * and memory may or may not leak. Code with global constructors is
 * especially problematic. Instead of dlclose, prefer to leave the
 * library open or, if cleanup is necessary, dlopen() the library in
 * a child process which can later be killed by the parent or call
 * exit() itself.
 *
 * Returns 0 on success, and returns -1 on failure, in which case
 * dlerror() can be used to retrieve the specific error.
 */
int dlclose(void* _Nonnull __handle);

char* _Nullable dlerror(void);

/* (RTLD_DEFAULT is null for LP64, but -1 for LP32) */
void* _Nullable dlsym(void* __BIONIC_COMPLICATED_NULLNESS __handle, const char* _Nullable __symbol);
/* (RTLD_DEFAULT is null for LP64, but -1 for LP32) */
void* _Nullable dlvsym(void* __BIONIC_COMPLICATED_NULLNESS __handle, const char* _Nullable __symbol, const char* _Nullable __version) __INTRODUCED_IN(24);
int dladdr(const void* _Nonnull __addr, Dl_info* _Nonnull __info);

#define RTLD_LOCAL    0
#define RTLD_LAZY     0x00001
#define RTLD_NOW      0x00002
#define RTLD_NOLOAD   0x00004
#define RTLD_GLOBAL   0x00100
#define RTLD_NODELETE 0x01000

#if !defined(__LP64__)
/* LP32 is broken for historical reasons. */
#undef RTLD_NOW
#define RTLD_NOW      0x00000
#undef RTLD_GLOBAL
#define RTLD_GLOBAL   0x00002
#endif

#if defined (__LP64__)
#define RTLD_DEFAULT  __BIONIC_CAST(reinterpret_cast, void*, 0)
#define RTLD_NEXT     __BIONIC_CAST(reinterpret_cast, void*, -1L)
#else
#define RTLD_DEFAULT  __BIONIC_CAST(reinterpret_cast, void*, 0xffffffff)
#define RTLD_NEXT     __BIONIC_CAST(reinterpret_cast, void*, 0xfffffffe)
#endif

__END_DECLS

#endif
