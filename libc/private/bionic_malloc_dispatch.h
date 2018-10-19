/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef _PRIVATE_BIONIC_MALLOC_DISPATCH_H
#define _PRIVATE_BIONIC_MALLOC_DISPATCH_H

#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>
#include <private/bionic_config.h>

// Entry in malloc dispatch table.
typedef void* (*MallocCalloc)(size_t, size_t);
typedef void (*MallocFree)(void*);
typedef struct mallinfo (*MallocMallinfo)();
typedef void* (*MallocMalloc)(size_t);
typedef size_t (*MallocMallocUsableSize)(const void*);
typedef void* (*MallocMemalign)(size_t, size_t);
typedef int (*MallocPosixMemalign)(void**, size_t, size_t);
typedef void* (*MallocRealloc)(void*, size_t);
typedef int (*MallocIterate)(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*);
typedef void (*MallocMallocDisable)();
typedef void (*MallocMallocEnable)();
typedef int (*MallocMallopt)(int, int);
typedef void* (*MallocAlignedAlloc)(size_t, size_t);

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
typedef void* (*MallocPvalloc)(size_t);
typedef void* (*MallocValloc)(size_t);
#endif

struct MallocDispatch {
  _Atomic MallocCalloc calloc;
  _Atomic MallocFree free;
  _Atomic MallocMallinfo mallinfo;
  _Atomic MallocMalloc malloc;
  _Atomic MallocMallocUsableSize malloc_usable_size;
  _Atomic MallocMemalign memalign;
  _Atomic MallocPosixMemalign posix_memalign;
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  _Atomic MallocPvalloc pvalloc;
#endif
  _Atomic MallocRealloc realloc;
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  _Atomic MallocValloc valloc;
#endif
  _Atomic MallocIterate iterate;
  _Atomic MallocMallocDisable malloc_disable;
  _Atomic MallocMallocEnable malloc_enable;
  _Atomic MallocMallopt mallopt;
  _Atomic MallocAlignedAlloc aligned_alloc;
} __attribute__((aligned(32)));

#endif
