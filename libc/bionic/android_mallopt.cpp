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

#include <errno.h>
#include <stdatomic.h>

#include <platform/bionic/malloc.h>
#include <private/bionic_globals.h>

#include "gwp_asan_wrappers.h"
#include "malloc_limit.h"

#if !defined(LIBC_STATIC)
#include <stdio.h>

#include <private/bionic_defs.h>

#include "malloc_heapprofd.h"

extern bool gZygoteChild;
extern _Atomic bool gZygoteChildProfileable;

bool WriteMallocLeakInfo(FILE* fp);
bool GetMallocLeakInfo(android_mallopt_leak_info_t* leak_info);
bool FreeMallocLeakInfo(android_mallopt_leak_info_t* leak_info);
#endif

// =============================================================================
// Platform-internal mallopt variant.
// =============================================================================
#if !defined(LIBC_STATIC)
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
#endif
extern "C" bool android_mallopt(int opcode, void* arg, size_t arg_size) {
  // Functionality available in both static and dynamic libc.
  if (opcode == M_GET_DECAY_TIME_ENABLED) {
    if (arg == nullptr || arg_size != sizeof(bool)) {
      errno = EINVAL;
      return false;
    }
    *reinterpret_cast<bool*>(arg) = atomic_load(&__libc_globals->decay_time_enabled);
    return true;
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
    *reinterpret_cast<bool*>(arg) = atomic_load(&__libc_memtag_stack);
    return true;
  }
  if (opcode == M_SET_ALLOCATION_LIMIT_BYTES) {
    return LimitEnable(arg, arg_size);
  }

#if defined(LIBC_STATIC)
  errno = ENOTSUP;
  return false;
#else
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
  // Try heapprofd's mallopt, as it handles options not covered here.
  return HeapprofdMallopt(opcode, arg, arg_size);
#endif
}
