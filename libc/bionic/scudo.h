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

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>

#include <private/bionic_config.h>

__BEGIN_DECLS

void* scudo_aligned_alloc(size_t, size_t);
void* scudo_calloc(size_t, size_t);
void scudo_free(void*);
struct mallinfo scudo_mallinfo();
void* scudo_malloc(size_t);
int scudo_malloc_info(int, FILE*);
size_t scudo_malloc_usable_size(const void*);
int scudo_mallopt(int, int);
void* scudo_memalign(size_t, size_t);
void* scudo_realloc(void*, size_t);
int scudo_posix_memalign(void**, size_t, size_t);
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* scudo_pvalloc(size_t);
void* scudo_valloc(size_t);
#endif

int scudo_malloc_iterate(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*);
void scudo_malloc_disable();
void scudo_malloc_enable();

void* scudo_svelte_aligned_alloc(size_t, size_t);
void* scudo_svelte_calloc(size_t, size_t);
void scudo_svelte_free(void*);
struct mallinfo scudo_svelte_mallinfo();
void* scudo_svelte_malloc(size_t);
int scudo_svelte_malloc_info(int, FILE*);
size_t scudo_svelte_malloc_usable_size(const void*);
int scudo_svelte_mallopt(int, int);
void* scudo_svelte_memalign(size_t, size_t);
void* scudo_svelte_realloc(void*, size_t);
int scudo_svelte_posix_memalign(void**, size_t, size_t);
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* scudo_svelte_pvalloc(size_t);
void* scudo_svelte_valloc(size_t);
#endif

int scudo_svelte_malloc_iterate(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*);
void scudo_svelte_malloc_disable();
void scudo_svelte_malloc_enable();

__END_DECLS
