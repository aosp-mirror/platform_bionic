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
// is set to a non-zero value.

#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#include <private/bionic_config.h>
#include <platform/bionic/malloc.h>

#include "gwp_asan_wrappers.h"
#include "heap_tagging.h"
#include "malloc_common.h"
#include "malloc_limit.h"
#include "malloc_tagged_pointers.h"

// =============================================================================
// Global variables instantations.
// =============================================================================

// Malloc hooks globals.
void* (*volatile __malloc_hook)(size_t, const void*);
void* (*volatile __realloc_hook)(void*, size_t, const void*);
void (*volatile __free_hook)(void*, const void*);
void* (*volatile __memalign_hook)(size_t, size_t, const void*);
// =============================================================================

// =============================================================================
// Allocation functions
// =============================================================================
extern "C" void* calloc(size_t n_elements, size_t elem_size) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return MaybeTagPointer(dispatch_table->calloc(n_elements, elem_size));
  }
  void* result = Malloc(calloc)(n_elements, elem_size);
  if (__predict_false(result == nullptr)) {
    warning_log("calloc(%zu, %zu) failed: returning null pointer", n_elements, elem_size);
  }
  return MaybeTagPointer(result);
}

extern "C" void free(void* mem) {
  auto dispatch_table = GetDispatchTable();
  mem = MaybeUntagAndCheckPointer(mem);
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

extern "C" int malloc_info(int options, FILE* fp) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc_info(options, fp);
  }
  return Malloc(malloc_info)(options, fp);
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
  void *result;
  if (__predict_false(dispatch_table != nullptr)) {
    result = dispatch_table->malloc(bytes);
  } else {
    result = Malloc(malloc)(bytes);
  }
  if (__predict_false(result == nullptr)) {
    warning_log("malloc(%zu) failed: returning null pointer", bytes);
    return nullptr;
  }
  return MaybeTagPointer(result);
}

extern "C" size_t malloc_usable_size(const void* mem) {
  auto dispatch_table = GetDispatchTable();
  mem = MaybeUntagAndCheckPointer(mem);
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc_usable_size(mem);
  }
  return Malloc(malloc_usable_size)(mem);
}

extern "C" void* memalign(size_t alignment, size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return MaybeTagPointer(dispatch_table->memalign(alignment, bytes));
  }
  void* result = Malloc(memalign)(alignment, bytes);
  if (__predict_false(result == nullptr)) {
    warning_log("memalign(%zu, %zu) failed: returning null pointer", alignment, bytes);
  }
  return MaybeTagPointer(result);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
  auto dispatch_table = GetDispatchTable();
  int result;
  if (__predict_false(dispatch_table != nullptr)) {
    result = dispatch_table->posix_memalign(memptr, alignment, size);
  } else {
    result = Malloc(posix_memalign)(memptr, alignment, size);
  }
  if (result == 0) {
    *memptr = MaybeTagPointer(*memptr);
  }
  return result;
}

extern "C" void* aligned_alloc(size_t alignment, size_t size) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return MaybeTagPointer(dispatch_table->aligned_alloc(alignment, size));
  }
  void* result = Malloc(aligned_alloc)(alignment, size);
  if (__predict_false(result == nullptr)) {
    warning_log("aligned_alloc(%zu, %zu) failed: returning null pointer", alignment, size);
  }
  return MaybeTagPointer(result);
}

extern "C" __attribute__((__noinline__)) void* realloc(void* old_mem, size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  old_mem = MaybeUntagAndCheckPointer(old_mem);
  if (__predict_false(dispatch_table != nullptr)) {
    return MaybeTagPointer(dispatch_table->realloc(old_mem, bytes));
  }
  void* result = Malloc(realloc)(old_mem, bytes);
  if (__predict_false(result == nullptr && bytes != 0)) {
    warning_log("realloc(%p, %zu) failed: returning null pointer", old_mem, bytes);
  }
  return MaybeTagPointer(result);
}

extern "C" void* reallocarray(void* old_mem, size_t item_count, size_t item_size) {
  size_t new_size;
  if (__builtin_mul_overflow(item_count, item_size, &new_size)) {
    warning_log("reallocaray(%p, %zu, %zu) failed: returning null pointer",
                old_mem, item_count, item_size);
    errno = ENOMEM;
    return nullptr;
  }
  return realloc(old_mem, new_size);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return MaybeTagPointer(dispatch_table->pvalloc(bytes));
  }
  void* result = Malloc(pvalloc)(bytes);
  if (__predict_false(result == nullptr)) {
    warning_log("pvalloc(%zu) failed: returning null pointer", bytes);
  }
  return MaybeTagPointer(result);
}

extern "C" void* valloc(size_t bytes) {
  auto dispatch_table = GetDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return MaybeTagPointer(dispatch_table->valloc(bytes));
  }
  void* result = Malloc(valloc)(bytes);
  if (__predict_false(result == nullptr)) {
    warning_log("valloc(%zu) failed: returning null pointer", bytes);
  }
  return MaybeTagPointer(result);
}
#endif
// =============================================================================

struct CallbackWrapperArg {
  void (*callback)(uintptr_t base, size_t size, void* arg);
  void* arg;
};

void CallbackWrapper(uintptr_t base, size_t size, void* arg) {
  CallbackWrapperArg* wrapper_arg = reinterpret_cast<CallbackWrapperArg*>(arg);
  wrapper_arg->callback(
    reinterpret_cast<uintptr_t>(MaybeTagPointer(reinterpret_cast<void*>(base))),
    size, wrapper_arg->arg);
}

// =============================================================================
// Exported for use by libmemunreachable.
// =============================================================================

// Calls callback for every allocation in the anonymous heap mapping
// [base, base+size). Must be called between malloc_disable and malloc_enable.
// `base` in this can take either a tagged or untagged pointer, but we always
// provide a tagged pointer to the `base` argument of `callback` if the kernel
// supports tagged pointers.
extern "C" int malloc_iterate(uintptr_t base, size_t size,
    void (*callback)(uintptr_t base, size_t size, void* arg), void* arg) {
  auto dispatch_table = GetDispatchTable();
  // Wrap the malloc_iterate callback we were provided, in order to provide
  // pointer tagging support.
  CallbackWrapperArg wrapper_arg;
  wrapper_arg.callback = callback;
  wrapper_arg.arg = arg;
  uintptr_t untagged_base =
      reinterpret_cast<uintptr_t>(UntagPointer(reinterpret_cast<void*>(base)));
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc_iterate(
      untagged_base, size, CallbackWrapper, &wrapper_arg);
  }
  return Malloc(malloc_iterate)(
    untagged_base, size, CallbackWrapper, &wrapper_arg);
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
extern "C" int __sanitizer_malloc_iterate(uintptr_t base __unused, size_t size __unused,
                                          void (*callback)(uintptr_t base, size_t size, void* arg)
                                              __unused,
                                          void* arg __unused) {
  return 0;
}

extern "C" void __sanitizer_malloc_disable() {
}

extern "C" void __sanitizer_malloc_enable() {
}

extern "C" int __sanitizer_malloc_info(int, FILE*) {
  errno = ENOTSUP;
  return -1;
}
#endif
// =============================================================================

// =============================================================================
// Platform-internal mallopt variant.
// =============================================================================
#if defined(LIBC_STATIC)
extern "C" bool android_mallopt(int opcode, void* arg, size_t arg_size) {
  if (opcode == M_SET_ALLOCATION_LIMIT_BYTES) {
    return LimitEnable(arg, arg_size);
  }
  if (opcode == M_SET_HEAP_TAGGING_LEVEL) {
    return SetHeapTaggingLevel(arg, arg_size);
  }
  if (opcode == M_INITIALIZE_GWP_ASAN) {
    if (arg == nullptr || arg_size != sizeof(bool)) {
      errno = EINVAL;
      return false;
    }
    __libc_globals.mutate([&](libc_globals* globals) {
      return MaybeInitGwpAsan(globals, *reinterpret_cast<bool*>(arg));
    });
  }
  errno = ENOTSUP;
  return false;
}
#endif
// =============================================================================

static constexpr MallocDispatch __libc_malloc_default_dispatch __attribute__((unused)) = {
  Malloc(calloc),
  Malloc(free),
  Malloc(mallinfo),
  Malloc(malloc),
  Malloc(malloc_usable_size),
  Malloc(memalign),
  Malloc(posix_memalign),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  Malloc(pvalloc),
#endif
  Malloc(realloc),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  Malloc(valloc),
#endif
  Malloc(malloc_iterate),
  Malloc(malloc_disable),
  Malloc(malloc_enable),
  Malloc(mallopt),
  Malloc(aligned_alloc),
  Malloc(malloc_info),
};

const MallocDispatch* NativeAllocatorDispatch() {
  return &__libc_malloc_default_dispatch;
}
