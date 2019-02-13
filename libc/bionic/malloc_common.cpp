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

// Contains a thin layer that calls whatever real native allocator
// has been defined. For the libc shared library, this allows the
// implementation of a debug malloc that can intercept all of the allocation
// calls and add special debugging code to attempt to catch allocation
// errors. All of the debugging code is implemented in a separate shared
// library that is only loaded when the property "libc.debug.malloc.options"
// is set to a non-zero value. There are two functions exported to
// allow ddms, or other external users to get information from the debug
// allocation.
//   get_malloc_leak_info: Returns information about all of the known native
//                         allocations that are currently in use.
//   free_malloc_leak_info: Frees the data allocated by the call to
//                          get_malloc_leak_info.
//   write_malloc_leak_info: Writes the leak info data to a file.

#include <stdint.h>

#include <private/bionic_config.h>

#include "malloc_common.h"

// =============================================================================
// Global variables instantations.
// =============================================================================

// Malloc hooks globals.
void* (*volatile __malloc_hook)(size_t, const void*);
void* (*volatile __realloc_hook)(void*, size_t, const void*);
void (*volatile __free_hook)(void*, const void*);
void* (*volatile __memalign_hook)(size_t, size_t, const void*);

// In a VM process, this is set to 1 after fork()ing out of zygote.
int gMallocLeakZygoteChild = 0;
// =============================================================================

// =============================================================================
// Allocation functions
// =============================================================================
extern "C" void* calloc(size_t n_elements, size_t elem_size) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->calloc(n_elements, elem_size);
  }
  return Malloc(calloc)(n_elements, elem_size);
}

extern "C" void free(void* mem) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    dispatch_table->free(mem);
  } else {
    Malloc(free)(mem);
  }
}

extern "C" struct mallinfo mallinfo() {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->mallinfo();
  }
  return Malloc(mallinfo)();
}

extern "C" int mallopt(int param, int value) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->mallopt(param, value);
  }
  return Malloc(mallopt)(param, value);
}

extern "C" void* malloc(size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc(bytes);
  }
  return Malloc(malloc)(bytes);
}

extern "C" size_t malloc_usable_size(const void* mem) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc_usable_size(mem);
  }
  return Malloc(malloc_usable_size)(mem);
}

extern "C" void* memalign(size_t alignment, size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->memalign(alignment, bytes);
  }
  return Malloc(memalign)(alignment, bytes);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->posix_memalign(memptr, alignment, size);
  }
  return Malloc(posix_memalign)(memptr, alignment, size);
}

extern "C" void* aligned_alloc(size_t alignment, size_t size) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->aligned_alloc(alignment, size);
  }
  return Malloc(aligned_alloc)(alignment, size);
}

extern "C" void* realloc(void* old_mem, size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->realloc(old_mem, bytes);
  }
  return Malloc(realloc)(old_mem, bytes);
}

extern "C" void* reallocarray(void* old_mem, size_t item_count, size_t item_size) {
  size_t new_size;
  if (__builtin_mul_overflow(item_count, item_size, &new_size)) {
    errno = ENOMEM;
    return nullptr;
  }
  return realloc(old_mem, new_size);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->pvalloc(bytes);
  }
  return Malloc(pvalloc)(bytes);
}

extern "C" void* valloc(size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->valloc(bytes);
  }
  return Malloc(valloc)(bytes);
}
#endif
// =============================================================================

// =============================================================================
// Exported for use by libmemunreachable.
// =============================================================================

// Calls callback for every allocation in the anonymous heap mapping
// [base, base+size).  Must be called between malloc_disable and malloc_enable.
extern "C" int malloc_iterate(uintptr_t base, size_t size,
    void (*callback)(uintptr_t base, size_t size, void* arg), void* arg) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->iterate(base, size, callback, arg);
  }
  return Malloc(iterate)(base, size, callback, arg);
}

// Disable calls to malloc so malloc_iterate gets a consistent view of
// allocated memory.
extern "C" void malloc_disable() {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc_disable();
  }
  return Malloc(malloc_disable)();
}

// Re-enable calls to malloc after a previous call to malloc_disable.
extern "C" void malloc_enable() {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc_enable();
  }
  return Malloc(malloc_enable)();
}

#if defined(LIBC_STATIC)
extern "C" ssize_t malloc_backtrace(void*, uintptr_t*, size_t) {
  return 0;
}
#endif

#if __has_feature(hwaddress_sanitizer)
// FIXME: implement these in HWASan allocator.
extern "C" int __sanitizer_iterate(uintptr_t base __unused, size_t size __unused,
                                   void (*callback)(uintptr_t base, size_t size, void* arg) __unused,
                                   void* arg __unused) {
  return 0;
}

extern "C" void __sanitizer_malloc_disable() {
}

extern "C" void __sanitizer_malloc_enable() {
}
#endif
// =============================================================================

// =============================================================================
// Platform-internal mallopt variant.
// =============================================================================
#if defined(LIBC_STATIC)
extern "C" bool android_mallopt(int, void*, size_t) {
  // There are no options supported on static executables.
  errno = ENOTSUP;
  return false;
}
#endif
// =============================================================================
