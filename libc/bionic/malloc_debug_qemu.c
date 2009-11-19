/*
 * Copyright (C) 2009 The Android Open Source Project
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

/*
 * Contains implementation of memeory allocation routines instrumented for
 * usage in the emulator to detect memory allocation violations, such as
 * memory leaks, buffer overruns, etc.
 */

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "dlmalloc.h"
#include "logd.h"

// This file should be included into the build only when
// MALLOC_QEMU_INSTRUMENT macro is defined.
#ifndef MALLOC_QEMU_INSTRUMENT
#error MALLOC_QEMU_INSTRUMENT is not defined.
#endif  // MALLOC_QEMU_INSTRUMENT

// =============================================================================
// log functions
// =============================================================================

#define debug_log(format, ...)  \
    __libc_android_log_print(ANDROID_LOG_DEBUG, "malloc_qemu", (format), ##__VA_ARGS__ )
#define error_log(format, ...)  \
    __libc_android_log_print(ANDROID_LOG_ERROR, "malloc_qemu", (format), ##__VA_ARGS__ )
#define info_log(format, ...)  \
    __libc_android_log_print(ANDROID_LOG_INFO, "malloc_qemu", (format), ##__VA_ARGS__ )

void* qemu_instrumented_malloc(size_t bytes)
{
    return dlmalloc(bytes);
}

void  qemu_instrumented_free(void* mem)
{
    dlfree(mem);
}

void* qemu_instrumented_calloc(size_t n_elements, size_t elem_size)
{
    return dlcalloc(n_elements, elem_size);
}

void* qemu_instrumented_realloc(void* mem, size_t bytes)
{
    return dlrealloc(mem, bytes);
}

void* qemu_instrumented_memalign(size_t alignment, size_t bytes)
{
    return dlmemalign(alignment, bytes);
}
