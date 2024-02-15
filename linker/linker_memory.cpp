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

#include "private/bionic_allocator.h"

#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>

#include <atomic>

#include <async_safe/log.h>

static BionicAllocator g_bionic_allocator;
static std::atomic<pid_t> fallback_tid(0);

// Used by libdebuggerd_handler to switch allocators during a crash dump, in
// case the linker heap is corrupted. Do not use this function.
extern "C" bool __linker_enable_fallback_allocator() {
  pid_t expected = 0;
  return fallback_tid.compare_exchange_strong(expected, gettid());
}

extern "C" void __linker_disable_fallback_allocator() {
  pid_t previous = fallback_tid.exchange(0);
  if (previous == 0) {
    async_safe_fatal("attempted to disable unused fallback allocator");
  } else if (previous != gettid()) {
    async_safe_fatal("attempted to disable fallback allocator in use by another thread (%d)",
                     previous);
  }
}

static BionicAllocator& get_fallback_allocator() {
  static BionicAllocator fallback_allocator;
  return fallback_allocator;
}

static BionicAllocator& get_allocator() {
  if (__predict_false(fallback_tid) && __predict_false(gettid() == fallback_tid)) {
    return get_fallback_allocator();
  }
  return g_bionic_allocator;
}

void* malloc(size_t byte_count) {
  return get_allocator().alloc(byte_count);
}

void* memalign(size_t alignment, size_t byte_count) {
  return get_allocator().memalign(alignment, byte_count);
}

void* aligned_alloc(size_t alignment, size_t byte_count) {
  return get_allocator().memalign(alignment, byte_count);
}

void* calloc(size_t item_count, size_t item_size) {
  return get_allocator().alloc(item_count*item_size);
}

void* realloc(void* p, size_t byte_count) {
  return get_allocator().realloc(p, byte_count);
}

void* reallocarray(void* p, size_t item_count, size_t item_size) {
  size_t byte_count;
  if (__builtin_mul_overflow(item_count, item_size, &byte_count)) {
    errno = ENOMEM;
    return nullptr;
  }
  return get_allocator().realloc(p, byte_count);
}

void free(void* ptr) {
  get_allocator().free(ptr);
}
