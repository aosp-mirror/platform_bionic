/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <signal.h>

#include <async_safe/log.h>
#include <platform/bionic/malloc.h>
#include <platform/bionic/reserved_signals.h>

#include "malloc_heapprofd.h"

// This file defines the handler for the reserved signal sent by the Android
// platform's profilers. The accompanying signal value discriminates between
// specific requestors:
//  0: heapprofd heap profiler.
static constexpr int kHeapprofdSignalValue = 0;

static void HandleProfilingSignal(int, siginfo_t*, void*);

// Called during dynamic libc preinit.
__LIBC_HIDDEN__ void __libc_init_profiling_handlers() {
  struct sigaction action = {};
  action.sa_flags = SA_SIGINFO | SA_RESTART;
  action.sa_sigaction = HandleProfilingSignal;
  sigaction(BIONIC_SIGNAL_PROFILER, &action, nullptr);
}

static void HandleProfilingSignal(int /*signal_number*/, siginfo_t* info, void* /*ucontext*/) {
  if (info->si_code != SI_QUEUE)
    return;

  int signal_value = info->si_value.sival_int;
  async_safe_format_log(ANDROID_LOG_WARN, "libc", "%s: received profiling signal with si_value: %d",
                        getprogname(), signal_value);

  // Proceed only if the process is considered profileable.
  bool profileable = false;
  android_mallopt(M_GET_PROCESS_PROFILEABLE, &profileable, sizeof(profileable));
  if (!profileable) {
    async_safe_write_log(ANDROID_LOG_ERROR, "libc", "profiling signal rejected (not profileable)");
    return;
  }

  if (signal_value == kHeapprofdSignalValue) {
    HandleHeapprofdSignal();
  } else {
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "unrecognized profiling signal si_value: %d",
                          signal_value);
  }
}
