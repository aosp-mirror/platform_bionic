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

#if defined(LIBC_STATIC)
#error This file should not be compiled for static targets.
#endif

// Contains a thin layer that calls whatever real native allocator
// has been defined. For the libc shared library, this allows the
// implementation of a debug malloc that can intercept all of the allocation
// calls and add special debugging code to attempt to catch allocation
// errors. All of the debugging code is implemented in a separate shared
// library that is only loaded when the property "libc.debug.malloc.options"
// is set to a non-zero value. There are three functions exported to
// allow ddms, or other external users to get information from the debug
// allocation.
//   get_malloc_leak_info: Returns information about all of the known native
//                         allocations that are currently in use.
//   free_malloc_leak_info: Frees the data allocated by the call to
//                          get_malloc_leak_info.
//   write_malloc_leak_info: Writes the leak info data to a file.

#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <android/dlext.h>

#include <platform/bionic/malloc.h>
#include <private/ScopedPthreadMutexLocker.h>
#include <private/bionic_config.h>
#include <private/bionic_defs.h>
#include <private/bionic_malloc_dispatch.h>

#include <sys/system_properties.h>

#include "gwp_asan_wrappers.h"
#include "heap_tagging.h"
#include "heap_zero_init.h"
#include "malloc_common.h"
#include "malloc_common_dynamic.h"
#include "malloc_heapprofd.h"
#include "malloc_limit.h"

// =============================================================================
// Global variables instantations.
// =============================================================================
pthread_mutex_t gGlobalsMutateLock = PTHREAD_MUTEX_INITIALIZER;

_Atomic bool gGlobalsMutating = false;

static bool gZygoteChild = false;

// In a Zygote child process, this is set to true if profiling of this process
// is allowed. Note that this is set at a later time than gZygoteChild. The
// latter is set during the fork (while still in zygote's SELinux domain). While
// this bit is set after the child is specialized (and has transferred SELinux
// domains if applicable). These two flags are read by the
// BIONIC_SIGNAL_PROFILER handler, which does nothing if the process is not
// profileable.
static _Atomic bool gZygoteChildProfileable = false;

// =============================================================================

static constexpr char kHooksSharedLib[] = "libc_malloc_hooks.so";
static constexpr char kHooksPrefix[] = "hooks";
static constexpr char kHooksPropertyEnable[] = "libc.debug.hooks.enable";
static constexpr char kHooksEnvEnable[] = "LIBC_HOOKS_ENABLE";

static constexpr char kDebugSharedLib[] = "libc_malloc_debug.so";
static constexpr char kDebugPrefix[] = "debug";
static constexpr char kDebugPropertyOptions[] = "libc.debug.malloc.options";
static constexpr char kDebugPropertyProgram[] = "libc.debug.malloc.program";
static constexpr char kDebugEnvOptions[] = "LIBC_DEBUG_MALLOC_OPTIONS";

typedef void (*finalize_func_t)();
typedef bool (*init_func_t)(const MallocDispatch*, bool*, const char*);
typedef void (*get_malloc_leak_info_func_t)(uint8_t**, size_t*, size_t*, size_t*, size_t*);
typedef void (*free_malloc_leak_info_func_t)(uint8_t*);
typedef bool (*write_malloc_leak_info_func_t)(FILE*);
typedef ssize_t (*malloc_backtrace_func_t)(void*, uintptr_t*, size_t);

enum FunctionEnum : uint8_t {
  FUNC_INITIALIZE,
  FUNC_FINALIZE,
  FUNC_GET_MALLOC_LEAK_INFO,
  FUNC_FREE_MALLOC_LEAK_INFO,
  FUNC_MALLOC_BACKTRACE,
  FUNC_WRITE_LEAK_INFO,
  FUNC_LAST,
};
static void* gFunctions[FUNC_LAST];

extern "C" int __cxa_atexit(void (*func)(void *), void *arg, void *dso);

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
  if (!InitMallocFunction<MallocMallocInfo>(impl_handler, &table->malloc_info, prefix,
                                                "malloc_info")) {
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
  if (!InitMallocFunction<MallocIterate>(impl_handler, &table->malloc_iterate, prefix,
                                         "malloc_iterate")) {
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

static void MallocFiniImpl(void*) {
  // Our BSD stdio implementation doesn't close the standard streams,
  // it only flushes them. Other unclosed FILE*s will show up as
  // malloc leaks, but to avoid the standard streams showing up in
  // leak reports, close them here.
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);

  reinterpret_cast<finalize_func_t>(gFunctions[FUNC_FINALIZE])();
}

static bool CheckLoadMallocHooks(char** options) {
  char* env = getenv(kHooksEnvEnable);
  if ((env == nullptr || env[0] == '\0' || env[0] == '0') &&
    (__system_property_get(kHooksPropertyEnable, *options) == 0 || *options[0] == '\0' || *options[0] == '0')) {
    return false;
  }
  *options = nullptr;
  return true;
}

static bool CheckLoadMallocDebug(char** options) {
  // If kDebugMallocEnvOptions is set then it overrides the system properties.
  char* env = getenv(kDebugEnvOptions);
  if (env == nullptr || env[0] == '\0') {
    if (__system_property_get(kDebugPropertyOptions, *options) == 0 || *options[0] == '\0') {
      return false;
    }

    // Check to see if only a specific program should have debug malloc enabled.
    char program[PROP_VALUE_MAX];
    if (__system_property_get(kDebugPropertyProgram, program) != 0 &&
        strstr(getprogname(), program) == nullptr) {
      return false;
    }
  } else {
    *options = env;
  }
  return true;
}

void SetGlobalFunctions(void* functions[]) {
  for (size_t i = 0; i < FUNC_LAST; i++) {
    gFunctions[i] = functions[i];
  }
}

static void ClearGlobalFunctions() {
  for (size_t i = 0; i < FUNC_LAST; i++) {
    gFunctions[i] = nullptr;
  }
}

bool InitSharedLibrary(void* impl_handle, const char* shared_lib, const char* prefix, MallocDispatch* dispatch_table) {
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
    gFunctions[i] = dlsym(impl_handle, symbol);
    if (gFunctions[i] == nullptr) {
      error_log("%s: %s routine not found in %s", getprogname(), symbol, shared_lib);
      ClearGlobalFunctions();
      return false;
    }
  }

  if (!InitMallocFunctions(impl_handle, dispatch_table, prefix)) {
    ClearGlobalFunctions();
    return false;
  }
  return true;
}

extern "C" struct android_namespace_t* android_get_exported_namespace(const char* name);

void* LoadSharedLibrary(const char* shared_lib, const char* prefix, MallocDispatch* dispatch_table) {
  void* impl_handle = nullptr;
  // Try to load the libc_malloc_* libs from the "runtime" namespace and then
  // fall back to dlopen() to load them from the default namespace.
  //
  // The libraries are packaged in the runtime APEX together with libc.so.
  // However, since the libc.so is searched via the symlink in the system
  // partition (/system/lib/libc.so -> /apex/com.android.runtime/bionic/libc.so)
  // libc.so is loaded into the default namespace. If we just dlopen() here, the
  // linker will load the libs found in /system/lib which might be incompatible
  // with libc.so in the runtime APEX. Use android_dlopen_ext to explicitly load
  // the ones in the runtime APEX.
  struct android_namespace_t* runtime_ns = android_get_exported_namespace("com_android_runtime");
  if (runtime_ns != nullptr) {
    const android_dlextinfo dlextinfo = {
      .flags = ANDROID_DLEXT_USE_NAMESPACE,
      .library_namespace = runtime_ns,
    };
    impl_handle = android_dlopen_ext(shared_lib, RTLD_NOW | RTLD_LOCAL, &dlextinfo);
  }

  if (impl_handle == nullptr) {
    impl_handle = dlopen(shared_lib, RTLD_NOW | RTLD_LOCAL);
  }

  if (impl_handle == nullptr) {
    error_log("%s: Unable to open shared library %s: %s", getprogname(), shared_lib, dlerror());
    return nullptr;
  }

  if (!InitSharedLibrary(impl_handle, shared_lib, prefix, dispatch_table)) {
    dlclose(impl_handle);
    impl_handle = nullptr;
  }

  return impl_handle;
}

bool FinishInstallHooks(libc_globals* globals, const char* options, const char* prefix) {
  init_func_t init_func = reinterpret_cast<init_func_t>(gFunctions[FUNC_INITIALIZE]);

  // If GWP-ASan was initialised, we should use it as the dispatch table for
  // heapprofd/malloc_debug/malloc_debug.
  const MallocDispatch* prev_dispatch = GetDefaultDispatchTable();
  if (prev_dispatch == nullptr) {
    prev_dispatch = NativeAllocatorDispatch();
  }

  if (!init_func(prev_dispatch, &gZygoteChild, options)) {
    error_log("%s: failed to enable malloc %s", getprogname(), prefix);
    ClearGlobalFunctions();
    return false;
  }

  // Do a pointer swap so that all of the functions become valid at once to
  // avoid any initialization order problems.
  atomic_store(&globals->default_dispatch_table, &globals->malloc_dispatch_table);
  if (!MallocLimitInstalled()) {
    atomic_store(&globals->current_dispatch_table, &globals->malloc_dispatch_table);
  }

  // Use atexit to trigger the cleanup function. This avoids a problem
  // where another atexit function is used to cleanup allocated memory,
  // but the finalize function was already called. This particular error
  // seems to be triggered by a zygote spawned process calling exit.
  int ret_value = __cxa_atexit(MallocFiniImpl, nullptr, nullptr);
  if (ret_value != 0) {
    // We don't consider this a fatal error.
    warning_log("failed to set atexit cleanup function: %d", ret_value);
  }
  return true;
}

static bool InstallHooks(libc_globals* globals, const char* options, const char* prefix,
                         const char* shared_lib) {
  void* impl_handle = LoadSharedLibrary(shared_lib, prefix, &globals->malloc_dispatch_table);
  if (impl_handle == nullptr) {
    return false;
  }

  if (!FinishInstallHooks(globals, options, prefix)) {
    dlclose(impl_handle);
    return false;
  }
  return true;
}

extern "C" const char* __scudo_get_stack_depot_addr();
extern "C" const char* __scudo_get_region_info_addr();
extern "C" const char* __scudo_get_ring_buffer_addr();
extern "C" size_t __scudo_get_ring_buffer_size();
extern "C" size_t __scudo_get_stack_depot_size();

// Initializes memory allocation framework once per process.
static void MallocInitImpl(libc_globals* globals) {
  char prop[PROP_VALUE_MAX];
  char* options = prop;

  MaybeInitGwpAsanFromLibc(globals);

#if defined(USE_SCUDO)
  __libc_shared_globals()->scudo_stack_depot = __scudo_get_stack_depot_addr();
  __libc_shared_globals()->scudo_region_info = __scudo_get_region_info_addr();
  __libc_shared_globals()->scudo_ring_buffer = __scudo_get_ring_buffer_addr();
  __libc_shared_globals()->scudo_ring_buffer_size = __scudo_get_ring_buffer_size();
  __libc_shared_globals()->scudo_stack_depot_size = __scudo_get_stack_depot_size();
#endif

  // Prefer malloc debug since it existed first and is a more complete
  // malloc interceptor than the hooks.
  bool hook_installed = false;
  if (CheckLoadMallocDebug(&options)) {
    hook_installed = InstallHooks(globals, options, kDebugPrefix, kDebugSharedLib);
  } else if (CheckLoadMallocHooks(&options)) {
    hook_installed = InstallHooks(globals, options, kHooksPrefix, kHooksSharedLib);
  }

  if (!hook_installed) {
    if (HeapprofdShouldLoad()) {
      HeapprofdInstallHooksAtInit(globals);
    }
  } else {
    // Record the fact that incompatible hooks are active, to skip any later
    // heapprofd signal handler invocations.
    HeapprofdRememberHookConflict();
  }
}

// Initializes memory allocation framework.
// This routine is called from __libc_init routines in libc_init_dynamic.cpp.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
__LIBC_HIDDEN__ void __libc_init_malloc(libc_globals* globals) {
  MallocInitImpl(globals);
}

// =============================================================================
// Functions to support dumping of native heap allocations using malloc debug.
// =============================================================================
bool GetMallocLeakInfo(android_mallopt_leak_info_t* leak_info) {
  void* func = gFunctions[FUNC_GET_MALLOC_LEAK_INFO];
  if (func == nullptr) {
    errno = ENOTSUP;
    return false;
  }
  reinterpret_cast<get_malloc_leak_info_func_t>(func)(
      &leak_info->buffer, &leak_info->overall_size, &leak_info->info_size,
      &leak_info->total_memory, &leak_info->backtrace_size);
  return true;
}

bool FreeMallocLeakInfo(android_mallopt_leak_info_t* leak_info) {
  void* func = gFunctions[FUNC_FREE_MALLOC_LEAK_INFO];
  if (func == nullptr) {
    errno = ENOTSUP;
    return false;
  }
  reinterpret_cast<free_malloc_leak_info_func_t>(func)(leak_info->buffer);
  return true;
}

bool WriteMallocLeakInfo(FILE* fp) {
  void* func = gFunctions[FUNC_WRITE_LEAK_INFO];
  bool written = false;
  if (func != nullptr) {
    written = reinterpret_cast<write_malloc_leak_info_func_t>(func)(fp);
  }

  if (!written) {
    fprintf(fp, "Native heap dump not available. To enable, run these commands (requires root):\n");
    fprintf(fp, "# adb shell stop\n");
    fprintf(fp, "# adb shell setprop libc.debug.malloc.options backtrace\n");
    fprintf(fp, "# adb shell start\n");
    errno = ENOTSUP;
  }
  return written;
}
// =============================================================================

// =============================================================================
// Exported for use by libmemunreachable.
// =============================================================================
extern "C" ssize_t malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count) {
  void* func = gFunctions[FUNC_MALLOC_BACKTRACE];
  if (func == nullptr) {
    return 0;
  }
  return reinterpret_cast<malloc_backtrace_func_t>(func)(pointer, frames, frame_count);
}
// =============================================================================

// =============================================================================
// Platform-internal mallopt variant.
// =============================================================================
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
extern "C" bool android_mallopt(int opcode, void* arg, size_t arg_size) {
  if (opcode == M_SET_ZYGOTE_CHILD) {
    if (arg != nullptr || arg_size != 0) {
      errno = EINVAL;
      return false;
    }
    gZygoteChild = true;
    return true;
  }
  if (opcode == M_INIT_ZYGOTE_CHILD_PROFILING) {
    if (arg != nullptr || arg_size != 0) {
      errno = EINVAL;
      return false;
    }
    atomic_store_explicit(&gZygoteChildProfileable, true, memory_order_release);
    // Also check if heapprofd should start profiling from app startup.
    HeapprofdInitZygoteChildProfiling();
    return true;
  }
  if (opcode == M_GET_PROCESS_PROFILEABLE) {
    if (arg == nullptr || arg_size != sizeof(bool)) {
      errno = EINVAL;
      return false;
    }
    // Native processes are considered profileable. Zygote children are considered
    // profileable only when appropriately tagged.
    *reinterpret_cast<bool*>(arg) =
        !gZygoteChild || atomic_load_explicit(&gZygoteChildProfileable, memory_order_acquire);
    return true;
  }
  if (opcode == M_SET_ALLOCATION_LIMIT_BYTES) {
    return LimitEnable(arg, arg_size);
  }
  if (opcode == M_WRITE_MALLOC_LEAK_INFO_TO_FILE) {
    if (arg == nullptr || arg_size != sizeof(FILE*)) {
      errno = EINVAL;
      return false;
    }
    return WriteMallocLeakInfo(reinterpret_cast<FILE*>(arg));
  }
  if (opcode == M_GET_MALLOC_LEAK_INFO) {
    if (arg == nullptr || arg_size != sizeof(android_mallopt_leak_info_t)) {
      errno = EINVAL;
      return false;
    }
    return GetMallocLeakInfo(reinterpret_cast<android_mallopt_leak_info_t*>(arg));
  }
  if (opcode == M_FREE_MALLOC_LEAK_INFO) {
    if (arg == nullptr || arg_size != sizeof(android_mallopt_leak_info_t)) {
      errno = EINVAL;
      return false;
    }
    return FreeMallocLeakInfo(reinterpret_cast<android_mallopt_leak_info_t*>(arg));
  }
  if (opcode == M_INITIALIZE_GWP_ASAN) {
    if (arg == nullptr || arg_size != sizeof(android_mallopt_gwp_asan_options_t)) {
      errno = EINVAL;
      return false;
    }

    return EnableGwpAsan(*reinterpret_cast<android_mallopt_gwp_asan_options_t*>(arg));
  }
  if (opcode == M_MEMTAG_STACK_IS_ON) {
    if (arg == nullptr || arg_size != sizeof(bool)) {
      errno = EINVAL;
      return false;
    }
    *reinterpret_cast<bool*>(arg) = atomic_load(&__libc_globals->memtag_stack);
    return true;
  }
  if (opcode == M_GET_DECAY_TIME_ENABLED) {
    if (arg == nullptr || arg_size != sizeof(bool)) {
      errno = EINVAL;
      return false;
    }
    *reinterpret_cast<bool*>(arg) = atomic_load(&__libc_globals->decay_time_enabled);
    return true;
  }
  // Try heapprofd's mallopt, as it handles options not covered here.
  return HeapprofdMallopt(opcode, arg, arg_size);
}
// =============================================================================

#if !defined(__LP64__) && defined(__arm__)
// =============================================================================
// Old platform only functions that some old 32 bit apps are still using.
// See b/132175052.
// Only compile the functions for 32 bit arm, so that new apps do not use
// these functions.
// =============================================================================
extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size,
                                     size_t* total_memory, size_t* backtrace_size) {
  if (info == nullptr || overall_size == nullptr || info_size == nullptr ||
      total_memory == nullptr || backtrace_size == nullptr) {
    return;
  }

  *info = nullptr;
  *overall_size = 0;
  *info_size = 0;
  *total_memory = 0;
  *backtrace_size = 0;

  android_mallopt_leak_info_t leak_info = {};
  if (android_mallopt(M_GET_MALLOC_LEAK_INFO, &leak_info, sizeof(leak_info))) {
    *info = leak_info.buffer;
    *overall_size = leak_info.overall_size;
    *info_size = leak_info.info_size;
    *total_memory = leak_info.total_memory;
    *backtrace_size = leak_info.backtrace_size;
  }
}

extern "C" void free_malloc_leak_info(uint8_t* info) {
  android_mallopt_leak_info_t leak_info = { .buffer = info };
  android_mallopt(M_FREE_MALLOC_LEAK_INFO, &leak_info, sizeof(leak_info));
}
// =============================================================================
#endif
