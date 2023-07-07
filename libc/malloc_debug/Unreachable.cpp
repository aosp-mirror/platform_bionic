/*
 * Copyright (C) 2022 The Android Open Source Project
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
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <string>

#include <memunreachable/memunreachable.h>
#include <platform/bionic/macros.h>

#include "Config.h"
#include "Unreachable.h"
#include "debug_log.h"

std::atomic_bool Unreachable::do_check_;

static void EnableUnreachableCheck(int, struct siginfo*, void*) {
  Unreachable::EnableCheck();
}

void Unreachable::CheckIfRequested(const Config& config) {
  if ((config.options() & CHECK_UNREACHABLE_ON_SIGNAL) && do_check_.exchange(false)) {
    info_log("Starting to check for unreachable memory.");
    if (!LogUnreachableMemory(false, 100)) {
      error_log("Unreachable check failed, run setenforce 0 and try again.");
    }
  }
}

bool Unreachable::Initialize(const Config& config) {
  if (!(config.options() & CHECK_UNREACHABLE_ON_SIGNAL)) {
    return true;
  }

  struct sigaction64 unreachable_act = {};
  unreachable_act.sa_sigaction = EnableUnreachableCheck;
  unreachable_act.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
  if (sigaction64(config.check_unreachable_signal(), &unreachable_act, nullptr) != 0) {
    error_log("Unable to set up check unreachable signal function: %s", strerror(errno));
    return false;
  }

  if (config.options() & VERBOSE) {
    info_log("%s: Run: 'kill -%d %d' to check for unreachable memory.", getprogname(),
             config.check_unreachable_signal(), getpid());
  }

  return true;
}
