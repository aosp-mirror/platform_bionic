/*
 * Copyright (C) 2023 The Android Open Source Project
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
#include <malloc.h>
#include <signal.h>
#include <unistd.h>

#include "Config.h"
#include "LogAllocatorStats.h"
#include "debug_log.h"

namespace LogAllocatorStats {

static std::atomic_bool g_call_mallopt = {};

static void CallMalloptLogStats(int, struct siginfo*, void*) {
  g_call_mallopt = true;
}

void CheckIfShouldLog() {
  bool expected = true;
  if (g_call_mallopt.compare_exchange_strong(expected, false)) {
    info_log("Logging allocator stats...");
    if (mallopt(M_LOG_STATS, 0) == 0) {
      error_log("mallopt(M_LOG_STATS, 0) call failed.");
    }
  }
}

bool Initialize(const Config& config) {
  struct sigaction64 log_stats_act = {};
  log_stats_act.sa_sigaction = CallMalloptLogStats;
  log_stats_act.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
  if (sigaction64(config.log_allocator_stats_signal(), &log_stats_act, nullptr) != 0) {
    error_log("Unable to set up log allocator stats signal function: %s", strerror(errno));
    return false;
  }

  if (config.options() & VERBOSE) {
    info_log("%s: Run: 'kill -%d %d' to log allocator stats.", getprogname(),
             config.log_allocator_stats_signal(), getpid());
  }

  return true;
}

}  // namespace LogAllocatorStats
