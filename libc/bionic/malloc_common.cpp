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
    Malloc(aligned_alloc),
  };

// Malloc hooks.
void* (*volatile __malloc_hook)(size_t, const void*);
void* (*volatile __realloc_hook)(void*, size_t, const void*);
void (*volatile __free_hook)(void*, const void*);
void* (*volatile __memalign_hook)(size_t, size_t, const void*);

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

extern "C" void* aligned_alloc(size_t alignment, size_t size) {
  auto _aligned_alloc = __libc_globals->malloc_dispatch.aligned_alloc;
  if (__predict_false(_aligned_alloc != nullptr)) {
    return _aligned_alloc(alignment, size);
  }
  return Malloc(aligned_alloc)(alignment, size);
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

#include <async_safe/log.h>
#include <sys/system_properties.h>

extern "C" int __cxa_atexit(void (*func)(void *), void *arg, void *dso);

static const char* HOOKS_SHARED_LIB = "libc_malloc_hooks.so";
static const char* HOOKS_PROPERTY_ENABLE = "libc.debug.hooks.enable";
static const char* HOOKS_ENV_ENABLE = "LIBC_HOOKS_ENABLE";

static const char* DEBUG_SHARED_LIB = "libc_malloc_debug.so";
static const char* DEBUG_PROPERTY_OPTIONS = "libc.debug.malloc.options";
static const char* DEBUG_PROPERTY_PROGRAM = "libc.debug.malloc.program";
static const char* DEBUG_ENV_OPTIONS = "LIBC_DEBUG_MALLOC_OPTIONS";

enum FunctionEnum : uint8_t {
  FUNC_INITIALIZE,
  FUNC_FINALIZE,
  FUNC_GET_MALLOC_LEAK_INFO,
  FUNC_FREE_MALLOC_LEAK_INFO,
  FUNC_MALLOC_BACKTRACE,
  FUNC_LAST,
};
static void* g_functions[FUNC_LAST];

typedef void (*finalize_func_t)();
typedef bool (*init_func_t)(const MallocDispatch*, int*, const char*);
typedef void (*get_malloc_leak_info_func_t)(uint8_t**, size_t*, size_t*, size_t*, size_t*);
typedef void (*free_malloc_leak_info_func_t)(uint8_t*);
typedef ssize_t (*malloc_backtrace_func_t)(void*, uintptr_t*, size_t);

// =============================================================================
// Log functions
// =============================================================================
#define error_log(format, ...)  \
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", (format), ##__VA_ARGS__ )
#define info_log(format, ...)  \
    async_safe_format_log(ANDROID_LOG_INFO, "libc", (format), ##__VA_ARGS__ )
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
  void* func = g_functions[FUNC_GET_MALLOC_LEAK_INFO];
  if (func == nullptr) {
    return;
  }
  reinterpret_cast<get_malloc_leak_info_func_t>(func)(info, overall_size, info_size, total_memory,
                                                      backtrace_size);
}

extern "C" void free_malloc_leak_info(uint8_t* info) {
  void* func = g_functions[FUNC_FREE_MALLOC_LEAK_INFO];
  if (func == nullptr) {
    return;
  }
  reinterpret_cast<free_malloc_leak_info_func_t>(func)(info);
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

static bool InitMallocFunctions(void* impl_handler, MallocDispatch* table, const char* prefix) {
  if (!InitMallocFunction<MallocCalloc>(impl_handler, &table->calloc, prefix, "calloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocFree>(impl_handler, &table->free, prefix, "free")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallinfo>(impl_handler, &table->mallinfo, prefix, "mallinfo")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallopt>(impl_handler, &table->mallopt, prefix, "mallopt")) {
    return false;
  }
  if (!InitMallocFunction<MallocMalloc>(impl_handler, &table->malloc, prefix, "malloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallocUsableSize>(impl_handler, &table->malloc_usable_size, prefix,
                                                  "malloc_usable_size")) {
    return false;
  }
  if (!InitMallocFunction<MallocMemalign>(impl_handler, &table->memalign, prefix, "memalign")) {
    return false;
  }
  if (!InitMallocFunction<MallocPosixMemalign>(impl_handler, &table->posix_memalign, prefix,
                                               "posix_memalign")) {
    return false;
  }
  if (!InitMallocFunction<MallocAlignedAlloc>(impl_handler, &table->aligned_alloc,
                                              prefix, "aligned_alloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocRealloc>(impl_handler, &table->realloc, prefix, "realloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocIterate>(impl_handler, &table->iterate, prefix, "iterate")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallocDisable>(impl_handler, &table->malloc_disable, prefix,
                                               "malloc_disable")) {
    return false;
  }
  if (!InitMallocFunction<MallocMallocEnable>(impl_handler, &table->malloc_enable, prefix,
                                              "malloc_enable")) {
    return false;
  }
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  if (!InitMallocFunction<MallocPvalloc>(impl_handler, &table->pvalloc, prefix, "pvalloc")) {
    return false;
  }
  if (!InitMallocFunction<MallocValloc>(impl_handler, &table->valloc, prefix, "valloc")) {
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

  reinterpret_cast<finalize_func_t>(g_functions[FUNC_FINALIZE])();
}

static bool CheckLoadMallocHooks(char** options) {
  char* env = getenv(HOOKS_ENV_ENABLE);
  if ((env == nullptr || env[0] == '\0' || env[0] == '0') &&
    (__system_property_get(HOOKS_PROPERTY_ENABLE, *options) == 0 || *options[0] == '\0' || *options[0] == '0')) {
    return false;
  }
  *options = nullptr;
  return true;
}

static bool CheckLoadMallocDebug(char** options) {
  // If DEBUG_MALLOC_ENV_OPTIONS is set then it overrides the system properties.
  char* env = getenv(DEBUG_ENV_OPTIONS);
  if (env == nullptr || env[0] == '\0') {
    if (__system_property_get(DEBUG_PROPERTY_OPTIONS, *options) == 0 || *options[0] == '\0') {
      return false;
    }

    // Check to see if only a specific program should have debug malloc enabled.
    char program[PROP_VALUE_MAX];
    if (__system_property_get(DEBUG_PROPERTY_PROGRAM, program) != 0 &&
        strstr(getprogname(), program) == nullptr) {
      return false;
    }
  } else {
    *options = env;
  }
  return true;
}

static void ClearGlobalFunctions() {
  for (size_t i = 0; i < FUNC_LAST; i++) {
    g_functions[i] = nullptr;
  }
}

static void* LoadSharedLibrary(const char* shared_lib, const char* prefix, MallocDispatch* dispatch_table) {
  void* impl_handle = dlopen(shared_lib, RTLD_NOW | RTLD_LOCAL);
  if (impl_handle == nullptr) {
    error_log("%s: Unable to open shared library %s: %s", getprogname(), shared_lib, dlerror());
    return nullptr;
  }

  static constexpr const char* names[] = {
    "initialize",
    "finalize",
    "get_malloc_leak_info",
    "free_malloc_leak_info",
    "malloc_backtrace",
  };
  for (size_t i = 0; i < FUNC_LAST; i++) {
    char symbol[128];
    snprintf(symbol, sizeof(symbol), "%s_%s", prefix, names[i]);
    g_functions[i] = dlsym(impl_handle, symbol);
    if (g_functions[i] == nullptr) {
      error_log("%s: %s routine not found in %s", getprogname(), symbol, shared_lib);
      dlclose(impl_handle);
      ClearGlobalFunctions();
      return nullptr;
    }
  }

  if (!InitMallocFunctions(impl_handle, dispatch_table, prefix)) {
    dlclose(impl_handle);
    ClearGlobalFunctions();
    return nullptr;
  }

  return impl_handle;
}

// Initializes memory allocation framework once per process.
static void malloc_init_impl(libc_globals* globals) {
  const char* prefix;
  const char* shared_lib;
  char prop[PROP_VALUE_MAX];
  char* options = prop;
  // Prefer malloc debug since it existed first and is a more complete
  // malloc interceptor than the hooks.
  if (CheckLoadMallocDebug(&options)) {
    prefix = "debug";
    shared_lib = DEBUG_SHARED_LIB;
  } else if (CheckLoadMallocHooks(&options)) {
    prefix = "hooks";
    shared_lib = HOOKS_SHARED_LIB;
  } else {
    return;
  }

  MallocDispatch dispatch_table;
  void* impl_handle = LoadSharedLibrary(shared_lib, prefix, &dispatch_table);
  if (impl_handle == nullptr) {
    return;
  }

  init_func_t init_func = reinterpret_cast<init_func_t>(g_functions[FUNC_INITIALIZE]);
  if (!init_func(&__libc_malloc_default_dispatch, &gMallocLeakZygoteChild, options)) {
    dlclose(impl_handle);
    ClearGlobalFunctions();
    return;
  }

  globals->malloc_dispatch = dispatch_table;

  info_log("%s: malloc %s enabled", getprogname(), prefix);

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
  void* func = g_functions[FUNC_MALLOC_BACKTRACE];
  if (func == nullptr) {
    return 0;
  }
  return reinterpret_cast<malloc_backtrace_func_t>(func)(pointer, frames, frame_count);
}
#else
extern "C" ssize_t malloc_backtrace(void*, uintptr_t*, size_t) {
  return 0;
}
#endif
