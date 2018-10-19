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

#include <pthread.h>
#include <stdatomic.h>

#include <private/bionic_config.h>
#include <private/bionic_globals.h>
#include <private/bionic_malloc_dispatch.h>

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
#include <sanitizer/hwasan_interface.h>
#define Malloc(function)  __sanitizer_ ## function

#else // __has_feature(hwaddress_sanitizer)
#include "jemalloc.h"
#define Malloc(function)  je_ ## function
#endif

template <typename T>
static T* RemoveConst(const T* x) {
  return const_cast<T*>(x);
}

// RemoveConst is a workaround for bug in current libcxx. Fix in
// https://reviews.llvm.org/D47613
#define atomic_load_explicit_const(obj, order) atomic_load_explicit(RemoveConst(obj), order)

static constexpr memory_order default_read_memory_order = memory_order_acquire;

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
  auto _calloc = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.calloc,
      default_read_memory_order);
  if (__predict_false(_calloc != nullptr)) {
    return _calloc(n_elements, elem_size);
  }
  return Malloc(calloc)(n_elements, elem_size);
}

extern "C" void free(void* mem) {
  auto _free = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.free,
      default_read_memory_order);
  if (__predict_false(_free != nullptr)) {
    _free(mem);
  } else {
    Malloc(free)(mem);
  }
}

extern "C" struct mallinfo mallinfo() {
  auto _mallinfo = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.mallinfo,
      default_read_memory_order);
  if (__predict_false(_mallinfo != nullptr)) {
    return _mallinfo();
  }
  return Malloc(mallinfo)();
}

extern "C" int mallopt(int param, int value) {
  auto _mallopt = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.mallopt,
      default_read_memory_order);
  if (__predict_false(_mallopt != nullptr)) {
    return _mallopt(param, value);
  }
  return Malloc(mallopt)(param, value);
}

extern "C" void* malloc(size_t bytes) {
  auto _malloc = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.malloc,
      default_read_memory_order);
  if (__predict_false(_malloc != nullptr)) {
    return _malloc(bytes);
  }
  return Malloc(malloc)(bytes);
}

extern "C" size_t malloc_usable_size(const void* mem) {
  auto _malloc_usable_size = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.malloc_usable_size,
      default_read_memory_order);
  if (__predict_false(_malloc_usable_size != nullptr)) {
    return _malloc_usable_size(mem);
  }
  return Malloc(malloc_usable_size)(mem);
}

extern "C" void* memalign(size_t alignment, size_t bytes) {
  auto _memalign = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.memalign,
      default_read_memory_order);
  if (__predict_false(_memalign != nullptr)) {
    return _memalign(alignment, bytes);
  }
  return Malloc(memalign)(alignment, bytes);
}

extern "C" int posix_memalign(void** memptr, size_t alignment, size_t size) {
  auto _posix_memalign = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.posix_memalign,
      default_read_memory_order);
  if (__predict_false(_posix_memalign != nullptr)) {
    return _posix_memalign(memptr, alignment, size);
  }
  return Malloc(posix_memalign)(memptr, alignment, size);
}

extern "C" void* aligned_alloc(size_t alignment, size_t size) {
  auto _aligned_alloc = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.aligned_alloc,
      default_read_memory_order);
  if (__predict_false(_aligned_alloc != nullptr)) {
    return _aligned_alloc(alignment, size);
  }
  return Malloc(aligned_alloc)(alignment, size);
}

extern "C" void* realloc(void* old_mem, size_t bytes) {
  auto _realloc = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.realloc,
      default_read_memory_order);
  if (__predict_false(_realloc != nullptr)) {
    return _realloc(old_mem, bytes);
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
  auto _pvalloc = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.pvalloc,
      default_read_memory_order);
  if (__predict_false(_pvalloc != nullptr)) {
    return _pvalloc(bytes);
  }
  return Malloc(pvalloc)(bytes);
}

extern "C" void* valloc(size_t bytes) {
  auto _valloc = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.valloc,
      default_read_memory_order);
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

static const char* HEAPPROFD_SHARED_LIB = "heapprofd_client.so";
static const char* HEAPPROFD_PREFIX = "heapprofd";
static const int HEAPPROFD_SIGNAL = __SIGRTMIN + 4;

enum FunctionEnum : uint8_t {
  FUNC_INITIALIZE,
  FUNC_FINALIZE,
  FUNC_GET_MALLOC_LEAK_INFO,
  FUNC_FREE_MALLOC_LEAK_INFO,
  FUNC_MALLOC_BACKTRACE,
  FUNC_WRITE_LEAK_INFO,
  FUNC_LAST,
};
static void* g_functions[FUNC_LAST];

typedef void (*finalize_func_t)();
typedef bool (*init_func_t)(const MallocDispatch*, int*, const char*);
typedef void (*get_malloc_leak_info_func_t)(uint8_t**, size_t*, size_t*, size_t*, size_t*);
typedef void (*free_malloc_leak_info_func_t)(uint8_t*);
typedef bool (*write_malloc_leak_info_func_t)(FILE*);
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

extern "C" void write_malloc_leak_info(FILE* fp) {
  if (fp == nullptr) {
    error_log("write_malloc_leak_info called with a nullptr");
    return;
  }

  void* func = g_functions[FUNC_WRITE_LEAK_INFO];
  bool written = false;
  if (func != nullptr) {
    written = reinterpret_cast<write_malloc_leak_info_func_t>(func)(fp);
  }

  if (!written) {
    fprintf(fp, "Native heap dump not available. To enable, run these commands (requires root):\n");
    fprintf(fp, "# adb shell stop\n");
    fprintf(fp, "# adb shell setprop libc.debug.malloc.options backtrace\n");
    fprintf(fp, "# adb shell start\n");
  }
}

// =============================================================================

template<typename FunctionType>
static bool InitMallocFunction(void* malloc_impl_handler, _Atomic(FunctionType)* func, const char* prefix, const char* suffix) {
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
  // We initialize free first to prevent the following situation:
  // Heapprofd's MallocMalloc is installed, and an allocation is observed
  // and logged to the heap dump. The corresponding free happens before
  // heapprofd's MallocFree is installed, and is not logged in the heap
  // dump. This leads to the allocation wrongly being active in the heap
  // dump indefinitely.
  if (!InitMallocFunction<MallocFree>(impl_handler, &table->free, prefix, "free")) {
    return false;
  }
  if (!InitMallocFunction<MallocCalloc>(impl_handler, &table->calloc, prefix, "calloc")) {
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
    "write_malloc_leak_info",
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

static void install_hooks(libc_globals* globals, const char* options,
                          const char* prefix, const char* shared_lib) {
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

extern "C" void InstallInitHeapprofdHook(int);

// Initializes memory allocation framework once per process.
static void malloc_init_impl(libc_globals* globals) {
  struct sigaction action = {};
  action.sa_handler = InstallInitHeapprofdHook;
  sigaction(HEAPPROFD_SIGNAL, &action, nullptr);

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
  install_hooks(globals, options, prefix, shared_lib);
}

// Initializes memory allocation framework.
// This routine is called from __libc_init routines in libc_init_dynamic.cpp.
__LIBC_HIDDEN__ void __libc_init_malloc(libc_globals* globals) {
  malloc_init_impl(globals);
}

// The logic for triggering heapprofd below is as following.
// 1. HEAPPROFD_SIGNAL is received by the process.
// 2a. If the signal is currently being handled (g_heapprofd_init_in_progress
//     is true), no action is taken.
// 2b. Otherwise, The signal handler (InstallInitHeapprofdHook) installs a
//     temporary malloc hook (InitHeapprofdHook).
// 3. When this hook gets run the first time, it uninstalls itself and spawns
//    a thread running InitHeapprofd that loads heapprofd.so and installs the
//    hooks within.
//
// This roundabout way is needed because we are running non AS-safe code, so
// we cannot run it directly in the signal handler. The other approach of
// running a standby thread and signalling through write(2) and read(2) would
// significantly increase the number of active threads in the system.

static _Atomic bool g_heapprofd_init_in_progress = false;
static _Atomic bool g_init_heapprofd_ran = false;

static void* InitHeapprofd(void*) {
  __libc_globals.mutate([](libc_globals* globals) {
    install_hooks(globals, nullptr, HEAPPROFD_PREFIX, HEAPPROFD_SHARED_LIB);
  });
  atomic_store(&g_heapprofd_init_in_progress, false);
  return nullptr;
}

static void* InitHeapprofdHook(size_t bytes) {
  if (!atomic_exchange(&g_init_heapprofd_ran, true)) {
    __libc_globals.mutate([](libc_globals* globals) {
      atomic_store(&globals->malloc_dispatch.malloc, nullptr);
    });

    pthread_t thread_id;
    if (pthread_create(&thread_id, nullptr, InitHeapprofd, nullptr) == -1)
      error_log("%s: heapprofd: failed to pthread_create.", getprogname());
    else if (pthread_detach(thread_id) == -1)
      error_log("%s: heapprofd: failed to pthread_detach", getprogname());
    if (pthread_setname_np(thread_id, "heapprofdinit") == -1)
      error_log("%s: heapprod: failed to pthread_setname_np", getprogname());
  }
  return Malloc(malloc)(bytes);
}

extern "C" void InstallInitHeapprofdHook(int) {
  if (!atomic_exchange(&g_heapprofd_init_in_progress, true)) {
    __libc_globals.mutate([](libc_globals* globals) {
      globals->malloc_dispatch.malloc = InitHeapprofdHook;
    });
  }
}

#endif  // !LIBC_STATIC

// =============================================================================
// Exported for use by libmemunreachable.
// =============================================================================

// Calls callback for every allocation in the anonymous heap mapping
// [base, base+size).  Must be called between malloc_disable and malloc_enable.
extern "C" int malloc_iterate(uintptr_t base, size_t size,
    void (*callback)(uintptr_t base, size_t size, void* arg), void* arg) {
  auto _iterate = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.iterate,
      default_read_memory_order);
  if (__predict_false(_iterate != nullptr)) {
    return _iterate(base, size, callback, arg);
  }
  return Malloc(iterate)(base, size, callback, arg);
}

// Disable calls to malloc so malloc_iterate gets a consistent view of
// allocated memory.
extern "C" void malloc_disable() {
  auto _malloc_disable = atomic_load_explicit_const(
     & __libc_globals->malloc_dispatch.malloc_disable,
      default_read_memory_order);
  if (__predict_false(_malloc_disable != nullptr)) {
    return _malloc_disable();
  }
  return Malloc(malloc_disable)();
}

// Re-enable calls to malloc after a previous call to malloc_disable.
extern "C" void malloc_enable() {
  auto _malloc_enable = atomic_load_explicit_const(
      &__libc_globals->malloc_dispatch.malloc_enable,
      default_read_memory_order);
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
