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

#include <pthread.h>

#include <private/bionic_config.h>
#include <private/bionic_globals.h>
#include <private/bionic_malloc_dispatch.h>

#include "jemalloc.h"
#define Malloc(function)  je_ ## function

static constexpr MallocDispatch __libc_malloc_default_dispatch
  __attribute__((unused)) = {
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
    Malloc(iterate),
    Malloc(malloc_disable),
    Malloc(malloc_enable),
    Malloc(mallopt),
  };

// In a VM process, this is set to 1 after fork()ing out of zygote.
int gMallocLeakZygoteChild = 0;

// =============================================================================
// Allocation functions
// =============================================================================
extern "C" void* calloc(size_t n_elements, size_t elem_size) {
  auto _calloc = __libc_globals->malloc_dispatch.calloc;
  if (__predict_false(_calloc != nullptr)) {
    return _calloc(n_elements, elem_size);
  }
  return Malloc(calloc)(n_elements, elem_size);
}

extern "C" void free(void* mem) {
  auto _free = __libc_globals->malloc_dispatch.free;
  if (__predict_false(_free != nullptr)) {
    _free(mem);
  } else {
    Malloc(free)(mem);
  }
}

extern "C" struct mallinfo mallinfo() {
  auto _mallinfo = __libc_globals->malloc_dispatch.mallinfo;
  if (__predict_false(_mallinfo != nullptr)) {
    return _mallinfo();
  }
  return Malloc(mallinfo)();
}

extern "C" int mallopt(int param, int value) {
  auto _mallopt = __libc_globals->malloc_dispatch.mallopt;
  if (__predict_false(_mallopt != nullptr)) {
    return _mallopt(param, value);
  }
  return Malloc(mallopt)(param, value);
}

extern "C" void* malloc(size_t bytes) {
  auto _malloc = __libc_globals->malloc_dispatch.malloc;
  if (__predict_false(_malloc != nullptr)) {
    return _malloc(bytes);
  }
  return Malloc(malloc)(bytes);
}

extern "C" size_t malloc_usable_size(const void* mem) {
  auto _malloc_usable_size = __libc_globals->malloc_dispatch.malloc_usable_size;
  if (__predict_false(_malloc_usable_size != nullptr)) {
    return _malloc_usable_size(mem);
  }
  return Malloc(malloc_usable_size)(mem);
}

extern "C" void* memalign(size_t alignment, size_t bytes) {
  auto _memalign = __libc_globals->malloc_dispatch.memalign;
  if (__predict_false(_memalign != nullptr)) {
    return _memalign(alignment, bytes);
  }
  return Malloc(memalign)(alignment, bytes);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
  auto _posix_memalign = __libc_globals->malloc_dispatch.posix_memalign;
  if (__predict_false(_posix_memalign != nullptr)) {
    return _posix_memalign(memptr, alignment, size);
  }
  return Malloc(posix_memalign)(memptr, alignment, size);
}

extern "C" void* realloc(void* old_mem, size_t bytes) {
  auto _realloc = __libc_globals->malloc_dispatch.realloc;
  if (__predict_false(_realloc != nullptr)) {
    return _realloc(old_mem, bytes);
  }
  return Malloc(realloc)(old_mem, bytes);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t bytes) {
  auto _pvalloc = __libc_globals->malloc_dispatch.pvalloc;
  if (__predict_false(_pvalloc != nullptr)) {
    return _pvalloc(bytes);
  }
  return Malloc(pvalloc)(bytes);
}

extern "C" void* valloc(size_t bytes) {
  auto _valloc = __libc_globals->malloc_dispatch.valloc;
  if (__predict_false(_valloc != nullptr)) {
    return _valloc(bytes);
  }
  return Malloc(valloc)(bytes);
}
#endif

// We implement malloc debugging only in libc.so, so the code below
// must be excluded if we compile this file for static libc.a
#if !defined(LIBC_STATIC)

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <private/libc_logging.h>
#include <sys/system_properties.h>

extern "C" int __cxa_atexit(void (*func)(void *), void *arg, void *dso);

static const char* DEBUG_SHARED_LIB = "libc_malloc_debug.so";
static const char* DEBUG_MALLOC_PROPERTY_OPTIONS = "libc.debug.malloc.options";
static const char* DEBUG_MALLOC_PROPERTY_PROGRAM = "libc.debug.malloc.program";
static const char* DEBUG_MALLOC_ENV_OPTIONS = "LIBC_DEBUG_MALLOC_OPTIONS";

static void* libc_malloc_impl_handle = nullptr;

static void (*g_debug_finalize_func)();
static void (*g_debug_get_malloc_leak_info_func)(uint8_t**, size_t*, size_t*, size_t*, size_t*);
static void (*g_debug_free_malloc_leak_info_func)(uint8_t*);
static ssize_t (*g_debug_malloc_backtrace_func)(void*, uintptr_t*, size_t);

// =============================================================================
// Log functions
// =============================================================================
#define error_log(format, ...)  \
    __libc_format_log(ANDROID_LOG_ERROR, "libc", (format), ##__VA_ARGS__ )
#define info_log(format, ...)  \
    __libc_format_log(ANDROID_LOG_INFO, "libc", (format), ##__VA_ARGS__ )
// =============================================================================

// =============================================================================
// Exported for use by ddms.
// =============================================================================

// Retrieve native heap information.
//
// "*info" is set to a buffer we allocate
// "*overall_size" is set to the size of the "info" buffer
// "*info_size" is set to the size of a single entry
// "*total_memory" is set to the sum of all allocations we're tracking; does
//   not include heap overhead
// "*backtrace_size" is set to the maximum number of entries in the back trace
extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overall_size,
    size_t* info_size, size_t* total_memory, size_t* backtrace_size) {
  if (g_debug_get_malloc_leak_info_func == nullptr) {
    return;
  }
  g_debug_get_malloc_leak_info_func(info, overall_size, info_size, total_memory, backtrace_size);
}

extern "C" void free_malloc_leak_info(uint8_t* info) {
  if (g_debug_free_malloc_leak_info_func == nullptr) {
    return;
  }
  g_debug_free_malloc_leak_info_func(info);
}

// =============================================================================

template<typename FunctionType>
static bool InitMallocFunction(void* malloc_impl_handler, FunctionType* func, const char* prefix, const char* suffix) {
  char symbol[128];
  snprintf(symbol, sizeof(symbol), "%s_%s", prefix, suffix);
  *func = reinterpret_cast<FunctionType>(dlsym(malloc_impl_handler, symbol));
  if (*func == nullptr) {
    error_log("%s: dlsym(\"%s\") failed", getprogname(), symbol);
    return false;
  }
  return true;
}

static bool InitMalloc(void* malloc_impl_handler, MallocDispatch* table, const char* prefix) {
  if (!InitMallocFunction<MallocCalloc>(malloc_impl_handler, &table->calloc,
                                        prefix, "calloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocFree>(malloc_impl_handler, &table->free,
                                      prefix, "free")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallinfo>(malloc_impl_handler, &table->mallinfo,
                                          prefix, "mallinfo")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallopt>(malloc_impl_handler, &table->mallopt,
                                         prefix, "mallopt")) {
    return false;
  }
  if (!InitMallocFunction<MallocMalloc>(malloc_impl_handler, &table->malloc,
                                        prefix, "malloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallocUsableSize>(
      malloc_impl_handler, &table->malloc_usable_size, prefix, "malloc_usable_size")) {
    return false;
  }
  if (!InitMallocFunction<MallocMemalign>(malloc_impl_handler, &table->memalign,
                                          prefix, "memalign")) {
    return false;
  }
  if (!InitMallocFunction<MallocPosixMemalign>(malloc_impl_handler, &table->posix_memalign,
                                               prefix, "posix_memalign")) {
    return false;
  }
  if (!InitMallocFunction<MallocRealloc>(malloc_impl_handler, &table->realloc,
                                         prefix, "realloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocIterate>(malloc_impl_handler, &table->iterate,
                                         prefix, "iterate")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallocDisable>(malloc_impl_handler, &table->malloc_disable,
                                         prefix, "malloc_disable")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallocEnable>(malloc_impl_handler, &table->malloc_enable,
                                         prefix, "malloc_enable")) {
    return false;
  }
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  if (!InitMallocFunction<MallocPvalloc>(malloc_impl_handler, &table->pvalloc,
                                         prefix, "pvalloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocValloc>(malloc_impl_handler, &table->valloc,
                                        prefix, "valloc")) {
    return false;
  }
#endif

  return true;
}

static void malloc_fini_impl(void*) {
  // Our BSD stdio implementation doesn't close the standard streams,
  // it only flushes them. Other unclosed FILE*s will show up as
  // malloc leaks, but to avoid the standard streams showing up in
  // leak reports, close them here.
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);

  g_debug_finalize_func();
}

// Initializes memory allocation framework once per process.
static void malloc_init_impl(libc_globals* globals) {
  char value[PROP_VALUE_MAX];

  // If DEBUG_MALLOC_ENV_OPTIONS is set then it overrides the system properties.
  const char* options = getenv(DEBUG_MALLOC_ENV_OPTIONS);
  if (options == nullptr || options[0] == '\0') {
    if (__system_property_get(DEBUG_MALLOC_PROPERTY_OPTIONS, value) == 0 || value[0] == '\0') {
      return;
    }
    options = value;

    // Check to see if only a specific program should have debug malloc enabled.
    char program[PROP_VALUE_MAX];
    if (__system_property_get(DEBUG_MALLOC_PROPERTY_PROGRAM, program) != 0 &&
        strstr(getprogname(), program) == nullptr) {
      return;
    }
  }

  // Load the debug malloc shared library.
  void* malloc_impl_handle = dlopen(DEBUG_SHARED_LIB, RTLD_NOW | RTLD_LOCAL);
  if (malloc_impl_handle == nullptr) {
    error_log("%s: Unable to open debug malloc shared library %s: %s",
              getprogname(), DEBUG_SHARED_LIB, dlerror());
    return;
  }

  // Initialize malloc debugging in the loaded module.
  auto init_func = reinterpret_cast<bool (*)(const MallocDispatch*, int*, const char*)>(
      dlsym(malloc_impl_handle, "debug_initialize"));
  if (init_func == nullptr) {
    error_log("%s: debug_initialize routine not found in %s", getprogname(), DEBUG_SHARED_LIB);
    dlclose(malloc_impl_handle);
    return;
  }

  // Get the syms for the external functions.
  void* finalize_sym = dlsym(malloc_impl_handle, "debug_finalize");
  if (finalize_sym == nullptr) {
    error_log("%s: debug_finalize routine not found in %s", getprogname(), DEBUG_SHARED_LIB);
    dlclose(malloc_impl_handle);
    return;
  }

  void* get_leak_info_sym = dlsym(malloc_impl_handle, "debug_get_malloc_leak_info");
  if (get_leak_info_sym == nullptr) {
    error_log("%s: debug_get_malloc_leak_info routine not found in %s", getprogname(),
              DEBUG_SHARED_LIB);
    dlclose(malloc_impl_handle);
    return;
  }

  void* free_leak_info_sym = dlsym(malloc_impl_handle, "debug_free_malloc_leak_info");
  if (free_leak_info_sym == nullptr) {
    error_log("%s: debug_free_malloc_leak_info routine not found in %s", getprogname(),
              DEBUG_SHARED_LIB);
    dlclose(malloc_impl_handle);
    return;
  }

  void* malloc_backtrace_sym = dlsym(malloc_impl_handle, "debug_malloc_backtrace");
  if (malloc_backtrace_sym == nullptr) {
    error_log("%s: debug_malloc_backtrace routine not found in %s", getprogname(),
              DEBUG_SHARED_LIB);
    dlclose(malloc_impl_handle);
    return;
  }

  if (!init_func(&__libc_malloc_default_dispatch, &gMallocLeakZygoteChild, options)) {
    dlclose(malloc_impl_handle);
    return;
  }

  MallocDispatch malloc_dispatch_table;
  if (!InitMalloc(malloc_impl_handle, &malloc_dispatch_table, "debug")) {
    auto finalize_func = reinterpret_cast<void (*)()>(finalize_sym);
    finalize_func();
    dlclose(malloc_impl_handle);
    return;
  }

  g_debug_finalize_func = reinterpret_cast<void (*)()>(finalize_sym);
  g_debug_get_malloc_leak_info_func = reinterpret_cast<void (*)(
      uint8_t**, size_t*, size_t*, size_t*, size_t*)>(get_leak_info_sym);
  g_debug_free_malloc_leak_info_func = reinterpret_cast<void (*)(uint8_t*)>(free_leak_info_sym);
  g_debug_malloc_backtrace_func = reinterpret_cast<ssize_t (*)(
      void*, uintptr_t*, size_t)>(malloc_backtrace_sym);

  globals->malloc_dispatch = malloc_dispatch_table;
  libc_malloc_impl_handle = malloc_impl_handle;

  info_log("%s: malloc debug enabled", getprogname());

  // Use atexit to trigger the cleanup function. This avoids a problem
  // where another atexit function is used to cleanup allocated memory,
  // but the finalize function was already called. This particular error
  // seems to be triggered by a zygote spawned process calling exit.
  int ret_value = __cxa_atexit(malloc_fini_impl, nullptr, nullptr);
  if (ret_value != 0) {
    error_log("failed to set atexit cleanup function: %d", ret_value);
  }
}

// Initializes memory allocation framework.
// This routine is called from __libc_init routines in libc_init_dynamic.cpp.
__LIBC_HIDDEN__ void __libc_init_malloc(libc_globals* globals) {
  malloc_init_impl(globals);
}
#endif  // !LIBC_STATIC

// =============================================================================
// Exported for use by libmemunreachable.
// =============================================================================

// Calls callback for every allocation in the anonymous heap mapping
// [base, base+size).  Must be called between malloc_disable and malloc_enable.
extern "C" int malloc_iterate(uintptr_t base, size_t size,
    void (*callback)(uintptr_t base, size_t size, void* arg), void* arg) {
  auto _iterate = __libc_globals->malloc_dispatch.iterate;
  if (__predict_false(_iterate != nullptr)) {
    return _iterate(base, size, callback, arg);
  }
  return Malloc(iterate)(base, size, callback, arg);
}

// Disable calls to malloc so malloc_iterate gets a consistent view of
// allocated memory.
extern "C" void malloc_disable() {
  auto _malloc_disable = __libc_globals->malloc_dispatch.malloc_disable;
  if (__predict_false(_malloc_disable != nullptr)) {
    return _malloc_disable();
  }
  return Malloc(malloc_disable)();
}

// Re-enable calls to malloc after a previous call to malloc_disable.
extern "C" void malloc_enable() {
  auto _malloc_enable = __libc_globals->malloc_dispatch.malloc_enable;
  if (__predict_false(_malloc_enable != nullptr)) {
    return _malloc_enable();
  }
  return Malloc(malloc_enable)();
}

#ifndef LIBC_STATIC
extern "C" ssize_t malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count) {
  if (g_debug_malloc_backtrace_func == nullptr) {
    return 0;
  }
  return g_debug_malloc_backtrace_func(pointer, frames, frame_count);
}
#else
extern "C" ssize_t malloc_backtrace(void*, uintptr_t*, size_t) {
  return 0;
}
#endif
