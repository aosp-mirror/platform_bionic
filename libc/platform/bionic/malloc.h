/*
 * Copyright (C) 2018 The Android Open Source Project
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

#pragma once

#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>

// Structures for android_mallopt.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
typedef struct {
  // Pointer to the buffer allocated by a call to M_GET_MALLOC_LEAK_INFO.
  uint8_t* buffer;
  // The size of the "info" buffer.
  size_t overall_size;
  // The size of a single entry.
  size_t info_size;
  // The sum of all allocations that have been tracked. Does not include
  // any heap overhead.
  size_t total_memory;
  // The maximum number of backtrace entries.
  size_t backtrace_size;
} android_mallopt_leak_info_t;
#pragma clang diagnostic pop
// Opcodes for android_mallopt.

enum {
  // Marks the calling process as a profileable zygote child, possibly
  // initializing profiling infrastructure.
  M_INIT_ZYGOTE_CHILD_PROFILING = 1,
#define M_INIT_ZYGOTE_CHILD_PROFILING M_INIT_ZYGOTE_CHILD_PROFILING
  M_RESET_HOOKS = 2,
#define M_RESET_HOOKS M_RESET_HOOKS
  // Set an upper bound on the total size in bytes of all allocations made
  // using the memory allocation APIs.
  //   arg = size_t*
  //   arg_size = sizeof(size_t)
  M_SET_ALLOCATION_LIMIT_BYTES = 3,
#define M_SET_ALLOCATION_LIMIT_BYTES M_SET_ALLOCATION_LIMIT_BYTES
  // Called after the zygote forks to indicate this is a child.
  M_SET_ZYGOTE_CHILD = 4,
#define M_SET_ZYGOTE_CHILD M_SET_ZYGOTE_CHILD

  // Options to dump backtraces of allocations. These options only
  // work when malloc debug has been enabled.

  // Writes the backtrace information of all current allocations to a file.
  // NOTE: arg_size has to be sizeof(FILE*) because FILE is an opaque type.
  //   arg = FILE*
  //   arg_size = sizeof(FILE*)
  M_WRITE_MALLOC_LEAK_INFO_TO_FILE = 5,
#define M_WRITE_MALLOC_LEAK_INFO_TO_FILE M_WRITE_MALLOC_LEAK_INFO_TO_FILE
  // Get information about the backtraces of all
  //   arg = android_mallopt_leak_info_t*
  //   arg_size = sizeof(android_mallopt_leak_info_t)
  M_GET_MALLOC_LEAK_INFO = 6,
#define M_GET_MALLOC_LEAK_INFO M_GET_MALLOC_LEAK_INFO
  // Free the memory allocated and returned by M_GET_MALLOC_LEAK_INFO.
  //   arg = android_mallopt_leak_info_t*
  //   arg_size = sizeof(android_mallopt_leak_info_t)
  M_FREE_MALLOC_LEAK_INFO = 7,
#define M_FREE_MALLOC_LEAK_INFO M_FREE_MALLOC_LEAK_INFO
  // Query whether the current process is considered to be profileable by the
  // Android platform. Result is assigned to the arg pointer's destination.
  //   arg = bool*
  //   arg_size = sizeof(bool)
  M_GET_PROCESS_PROFILEABLE = 9,
#define M_GET_PROCESS_PROFILEABLE M_GET_PROCESS_PROFILEABLE
  // Maybe enable GWP-ASan. Set *arg to force GWP-ASan to be turned on,
  // otherwise this mallopt() will internally decide whether to sample the
  // process. The program must be single threaded at the point when the
  // android_mallopt function is called.
  //   arg = android_mallopt_gwp_asan_options_t*
  //   arg_size = sizeof(android_mallopt_gwp_asan_options_t)
  M_INITIALIZE_GWP_ASAN = 10,
#define M_INITIALIZE_GWP_ASAN M_INITIALIZE_GWP_ASAN
  // Query whether memtag stack is enabled for this process.
  M_MEMTAG_STACK_IS_ON = 11,
#define M_MEMTAG_STACK_IS_ON M_MEMTAG_STACK_IS_ON
  // Query whether the current process has the decay time enabled so that
  // the memory from allocations are not immediately released to the OS.
  // Result is assigned to the arg pointer's destination.
  //   arg = bool*
  //   arg_size = sizeof(bool)
  M_GET_DECAY_TIME_ENABLED = 12,
#define M_GET_DECAY_TIME_ENABLED M_GET_DECAY_TIME_ENABLED
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
typedef struct {
  // The null-terminated name that the zygote is spawning. Because native
  // SpecializeCommon (where the GWP-ASan mallopt() is called from) happens
  // before argv[0] is set, we need the zygote to tell us the new app name.
  const char* program_name = nullptr;

  // An android_mallopt(M_INITIALIZE_GWP_ASAN) is always issued on process
  // startup and app startup, regardless of whether GWP-ASan is desired or not.
  // This allows the process/app's desire to be overwritten by the
  // "libc.debug.gwp_asan.*.app_default" or "libc.debug.gwp_asan.*.<name>"
  // system properties, as well as the "GWP_ASAN_*" environment variables.
  //
  // Worth noting, the "libc.debug.gwp_asan.*.app_default" sysprops *do not*
  // apply to system apps. They use the "libc.debug.gwp_asan.*.system_default"
  // sysprops.
  enum Action {
    // Enable GWP-ASan. This is used by apps that have `gwpAsanMode=always` in
    // the manifest.
    TURN_ON_FOR_APP,
    // Enable GWP-ASan, but only a small percentage of the time. This is used by
    // system processes and system apps, and we use a lottery to determine which
    // processes have GWP-ASan enabled. This allows us to mitigate system-wide
    // memory overhead concerns, as each GWP-ASan enabled process uses ~70KiB of
    // extra memory.
    TURN_ON_WITH_SAMPLING,
    // Don't enable GWP-ASan, unless overwritten by a system property or
    // environment variable. This is used by apps that have `gwpAsanMode=never`
    // in the manifest. Prior to Android 14, this also was used by non-system
    // apps that didn't specify a `gwpAsanMode` in their manifest.
    DONT_TURN_ON_UNLESS_OVERRIDDEN,
    // Enable GWP-ASan, but only a small percentage of the time, and enable it
    // in the non-crashing ("recoverable") mode. In Android 14, this is used by
    // apps that don't specify `gwpAsanMode` (or use `gwpAsanMode=default`) in
    // their manifest. GWP-ASan will detect heap memory safety bugs in this
    // mode, and bug reports will be created by debuggerd, however the process
    // will recover and continue to function as if the memory safety bug wasn't
    // detected.
    TURN_ON_FOR_APP_SAMPLED_NON_CRASHING,
  };

  Action desire = DONT_TURN_ON_UNLESS_OVERRIDDEN;
} android_mallopt_gwp_asan_options_t;
#pragma clang diagnostic pop
// Manipulates bionic-specific handling of memory allocation APIs such as
// malloc. Only for use by the Android platform and APEXes.
//
// On success, returns true. On failure, returns false and sets errno.
extern "C" bool android_mallopt(int opcode, void* _Nullable arg, size_t arg_size);
