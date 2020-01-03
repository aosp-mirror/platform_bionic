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

#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/prctl.h>

#include "platform/bionic/macros.h"

#include "scudo.h"

// Disable Scudo's mismatch allocation check, as it is being triggered
// by some third party code.
extern "C" const char *__scudo_default_options() {
  return "DeallocationTypeMismatch=false";
}

static inline bool AllocTooBig(size_t bytes) {
#if defined(__LP64__)
  if (__predict_false(bytes > 0x10000000000ULL)) {
#else
  if (__predict_false(bytes > 0x80000000ULL)) {
#endif
    return true;
  }
  return false;
}

void* scudo_aligned_alloc(size_t alignment, size_t size) {
  if (alignment == 0 || !powerof2(alignment) || (size % alignment) != 0) {
    errno = EINVAL;
    return nullptr;
  }
  if (AllocTooBig(size)) {
    errno = ENOMEM;
    return nullptr;
  }

  return aligned_alloc(alignment, size);
}

void* scudo_calloc(size_t item_count, size_t item_size) {
  size_t total;
  if (__builtin_mul_overflow(item_count, item_size, &total) || AllocTooBig(total)) {
    errno = ENOMEM;
    return nullptr;
  }
  return calloc(item_count, item_size);
}

void scudo_free(void* ptr) {
  free(ptr);
}

extern "C" size_t __sanitizer_get_current_allocated_bytes();
extern "C" size_t __sanitizer_get_heap_size();

struct mallinfo scudo_mallinfo() {
  struct mallinfo info {};
  info.uordblks = __sanitizer_get_current_allocated_bytes();
  info.hblkhd = __sanitizer_get_heap_size();
  info.usmblks = info.hblkhd;
  return info;
}

void* scudo_malloc(size_t byte_count) {
  if (AllocTooBig(byte_count)) {
    errno = ENOMEM;
    return nullptr;
  }
  return malloc(byte_count);
}

size_t scudo_malloc_usable_size(const void* ptr) {
  return malloc_usable_size(ptr);
}

void* scudo_memalign(size_t alignment, size_t byte_count) {
  if (AllocTooBig(byte_count)) {
    errno = ENOMEM;
    return nullptr;
  }
  if (alignment != 0) {
    if (!powerof2(alignment)) {
      alignment = BIONIC_ROUND_UP_POWER_OF_2(alignment);
    }
  } else {
    alignment = 1;
  }
  return memalign(alignment, byte_count);
}

void* scudo_realloc(void* ptr, size_t byte_count) {
  if (AllocTooBig(byte_count)) {
    errno = ENOMEM;
    return nullptr;
  }
  return realloc(ptr, byte_count);
}

int scudo_posix_memalign(void** memptr, size_t alignment, size_t size) {
  if (alignment < sizeof(void*) || !powerof2(alignment)) {
    return EINVAL;
  }
  if (AllocTooBig(size)) {
    return ENOMEM;
  }
  return posix_memalign(memptr, alignment, size);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t);

void* scudo_pvalloc(size_t size) {
  if (AllocTooBig(size)) {
    errno = ENOMEM;
    return nullptr;
  }
  return pvalloc(size);
}

extern "C" void* valloc(size_t);

void* scudo_valloc(size_t size) {
  if (AllocTooBig(size)) {
    errno = ENOMEM;
    return nullptr;
  }
  return valloc(size);
}
#endif

// Do not try and name the scudo maps by overriding __sanitizer::internal_mmap.
// There is already a function called MmapNamed that names the maps.
// Unfortunately, there is no easy way to override MmapNamed because
// too much of the code is not compiled into functions available in the
// library, and the code is complicated.
