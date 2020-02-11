/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <stdatomic.h>
#include <stdio.h>

#include <async_safe/log.h>
#include <private/bionic_globals.h>
#include <private/bionic_malloc_dispatch.h>

#if __has_feature(hwaddress_sanitizer)

#include <sanitizer/hwasan_interface.h>

__BEGIN_DECLS

// FIXME: implement these in HWASan allocator.
int __sanitizer_malloc_iterate(uintptr_t base, size_t size,
                               void (*callback)(uintptr_t base, size_t size, void* arg),
                               void* arg);
void __sanitizer_malloc_disable();
void __sanitizer_malloc_enable();
int __sanitizer_malloc_info(int options, FILE* fp);

__END_DECLS

#define Malloc(function)  __sanitizer_ ## function

#else // __has_feature(hwaddress_sanitizer)

#if defined(USE_SCUDO)

#include "scudo.h"
#define Malloc(function)  scudo_ ## function

#elif defined(USE_SCUDO_SVELTE)

#include "scudo.h"
#define Malloc(function)  scudo_svelte_ ## function

#else

#include "jemalloc.h"
#define Malloc(function)  je_ ## function

#endif

#endif

const MallocDispatch* NativeAllocatorDispatch();

static inline const MallocDispatch* GetDispatchTable() {
  return atomic_load_explicit(&__libc_globals->current_dispatch_table, memory_order_acquire);
}

static inline const MallocDispatch* GetDefaultDispatchTable() {
  return atomic_load_explicit(&__libc_globals->default_dispatch_table, memory_order_acquire);
}

// =============================================================================
// Log functions
// =============================================================================
#define error_log(format, ...)  \
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", (format), ##__VA_ARGS__ )
#define info_log(format, ...)  \
    async_safe_format_log(ANDROID_LOG_INFO, "libc", (format), ##__VA_ARGS__ )
#define warning_log(format, ...)  \
    async_safe_format_log(ANDROID_LOG_WARN, "libc", (format), ##__VA_ARGS__ )
// =============================================================================
