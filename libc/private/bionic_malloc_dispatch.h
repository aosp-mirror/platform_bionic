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
#include "private/bionic_config.h"

/* Entry in malloc dispatch table. */
typedef void* (*MallocDebugCalloc)(size_t, size_t);
typedef void (*MallocDebugFree)(void*);
typedef struct mallinfo (*MallocDebugMallinfo)();
typedef void* (*MallocDebugMalloc)(size_t);
typedef size_t (*MallocDebugMallocUsableSize)(const void*);
typedef void* (*MallocDebugMemalign)(size_t, size_t);
typedef int (*MallocDebugPosixMemalign)(void**, size_t, size_t);
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
typedef void* (*MallocDebugPvalloc)(size_t);
#endif
typedef void* (*MallocDebugRealloc)(void*, size_t);
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
typedef void* (*MallocDebugValloc)(size_t);
#endif

struct MallocDebug {
  MallocDebugCalloc calloc;
  MallocDebugFree free;
  MallocDebugMallinfo mallinfo;
  MallocDebugMalloc malloc;
  MallocDebugMallocUsableSize malloc_usable_size;
  MallocDebugMemalign memalign;
  MallocDebugPosixMemalign posix_memalign;
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  MallocDebugPvalloc pvalloc;
#endif
  MallocDebugRealloc realloc;
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  MallocDebugValloc valloc;
#endif
} __attribute__((aligned(32)));

#endif
