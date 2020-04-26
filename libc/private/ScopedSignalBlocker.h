/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <signal.h>

#include "platform/bionic/macros.h"

// This code needs to really block all the signals, not just the user-visible
// ones. We call __rt_sigprocmask(2) directly so we don't mask out our own
// signals (https://issuetracker.google.com/153624226 was a pthread_exit(3)
// crash because a request to dump the thread's stack came in as it was exiting).
extern "C" int __rt_sigprocmask(int, const sigset64_t*, sigset64_t*, size_t);

class ScopedSignalBlocker {
 public:
  // Block all signals.
  explicit ScopedSignalBlocker() {
    sigset64_t set;
    sigfillset64(&set);
    __rt_sigprocmask(SIG_BLOCK, &set, &old_set_, sizeof(sigset64_t));
  }

  // Block just the specified signal.
  explicit ScopedSignalBlocker(int signal) {
    sigset64_t set = {};
    sigaddset64(&set, signal);
    __rt_sigprocmask(SIG_BLOCK, &set, &old_set_, sizeof(sigset64_t));
  }

  ~ScopedSignalBlocker() {
    reset();
  }

  void reset() {
    __rt_sigprocmask(SIG_SETMASK, &old_set_, nullptr, sizeof(sigset64_t));
  }

  sigset64_t old_set_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(ScopedSignalBlocker);
};
