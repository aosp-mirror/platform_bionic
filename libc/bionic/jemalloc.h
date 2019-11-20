/*
 * Copyright (C) 2012 The Android Open Source Project
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

#pragma once

#include <jemalloc/jemalloc.h>
#include <malloc.h>  // For struct mallinfo.

// Need to wrap memalign since je_memalign fails on non-power of 2 alignments.
#define je_memalign je_memalign_round_up_boundary

// Need to wrap aligned_alloc since je_aligned_alloc does not enforce
// that size is a multiple of alignment.
#define je_aligned_alloc je_aligned_alloc_wrapper

__BEGIN_DECLS

void* je_aligned_alloc_wrapper(size_t, size_t);
int je_malloc_iterate(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*);
int je_mallctl(const char *name, void *oldp, size_t *oldlenp, void *newp, size_t newlen) __attribute__((nothrow));
struct mallinfo je_mallinfo();
void je_malloc_disable();
void je_malloc_enable();
int je_malloc_info(int options, FILE* fp);
int je_mallopt(int, int);
void* je_memalign_round_up_boundary(size_t, size_t);
void* je_pvalloc(size_t);

__END_DECLS
