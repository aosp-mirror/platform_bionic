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
#include <fcntl.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <private/bionic_config.h>
#include <private/bionic_defs.h>
#include <private/bionic_malloc_dispatch.h>

#include <sys/system_properties.h>

#include "malloc_common.h"
#include "malloc_common_dynamic.h"
#include "malloc_heapprofd.h"

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
typedef bool (*init_func_t)(const MallocDispatch*, int*, const char*);
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

void* LoadSharedLibrary(const char* shared_lib, const char* prefix, MallocDispatch* dispatch_table) {
  void* impl_handle = dlopen(shared_lib, RTLD_NOW | RTLD_LOCAL);
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
  if (!init_func(&__libc_malloc_default_dispatch, &gMallocLeakZygoteChild, options)) {
    error_log("%s: failed to enable malloc %s", getprogname(), prefix);
    ClearGlobalFunctions();
    return false;
  }

  // Do a pointer swap so that all of the functions become valid at once to
  // avoid any initialization order problems.
  atomic_store(&globals->current_dispatch_table, &globals->malloc_dispatch_table);

  info_log("%s: malloc %s enabled", getprogname(), prefix);

  // Use atexit to trigger the cleanup function. This avoids a problem
  // where another atexit function is used to cleanup allocated memory,
  // but the finalize function was already called. This particular error
  // seems to be triggered by a zygote spawned process calling exit.
  int ret_value = __cxa_atexit(MallocFiniImpl, nullptr, nullptr);
  if (ret_value != 0) {
    // We don't consider this a fatal error.
    info_log("failed to set atexit cleanup function: %d", ret_value);
  }
  return true;
}

static bool InstallHooks(libc_globals* globals, const char* options, const char* prefix,
                         const char* shared_lib) {
  void* impl_handle = LoadSharedLibrary(shared_lib, prefix, &globals->malloc_dispatch_table);
  if (impl_handle == nullptr) {
    return false;
  }

  init_func_t init_func = reinterpret_cast<init_func_t>(gFunctions[FUNC_INITIALIZE]);
  if (!init_func(&__libc_malloc_default_dispatch, &gMallocLeakZygoteChild, options)) {
    error_log("%s: failed to enable malloc %s", getprogname(), prefix);
    ClearGlobalFunctions();
    return false;
  }

  if (!FinishInstallHooks(globals, options, prefix)) {
    dlclose(impl_handle);
    return false;
  }
  return true;
}

// Initializes memory allocation framework once per process.
static void MallocInitImpl(libc_globals* globals) {
  char prop[PROP_VALUE_MAX];
  char* options = prop;

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

    // Install this last to avoid as many race conditions as possible.
    HeapprofdInstallSignalHandler();
  } else {
    // Install a signal handler that prints an error since we don't support
    // heapprofd and any other hook to be installed at the same time.
    HeapprofdInstallErrorSignalHandler();
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
  void* func = gFunctions[FUNC_GET_MALLOC_LEAK_INFO];
  if (func == nullptr) {
    return;
  }
  reinterpret_cast<get_malloc_leak_info_func_t>(func)(info, overall_size, info_size, total_memory,
                                                      backtrace_size);
}

extern "C" void free_malloc_leak_info(uint8_t* info) {
  void* func = gFunctions[FUNC_FREE_MALLOC_LEAK_INFO];
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
  }
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
extern "C" bool android_mallopt(int opcode, void* arg, size_t arg_size) {
  return HeapprofdMallopt(opcode, arg, arg_size);
}
// =============================================================================
