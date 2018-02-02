/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef _BIONIC_TESTS_SCOPED_SIGNAL_HANDLER_H
#define _BIONIC_TESTS_SCOPED_SIGNAL_HANDLER_H

#include <signal.h>
#include <string.h>

#if defined(__GLIBC__)
#define posix_spawnattr_getsigdefault64 posix_spawnattr_getsigdefault
#define posix_spawnattr_getsigmask64 posix_spawnattr_getsigmask
#define posix_spawnattr_setsigdefault64 posix_spawnattr_setsigdefault
#define posix_spawnattr_setsigmask64 posix_spawnattr_setsigmask
#define pthread_sigmask64 pthread_sigmask
#define sigaction64 sigaction
#define sigaddset64 sigaddset
#define sigdelset64 sigdelset
#define sigemptyset64 sigemptyset
#define sigfillset64 sigfillset
#define sigismember64 sigismember
#define sigpending64 sigpending
#define sigprocmask64 sigprocmask
#define sigset64_t sigset_t
#define sigsuspend64 sigsuspend
#define sigtimedwait64 sigtimedwait
#define sigwait64 sigwait
#define sigwaitinfo64 sigwaitinfo
#endif

class ScopedSignalHandler {
 public:
  ScopedSignalHandler(int signal_number, void (*handler)(int), int sa_flags = 0)
      : signal_number_(signal_number) {
    memset(&action_, 0, sizeof(action_));
    action_.sa_flags = sa_flags;
    action_.sa_handler = handler;
    sigaction64(signal_number_, &action_, &old_action_);
  }

  ScopedSignalHandler(int signal_number, void (*action)(int, siginfo_t*, void*),
                      int sa_flags = SA_SIGINFO)
      : signal_number_(signal_number) {
    memset(&action_, 0, sizeof(action_));
    action_.sa_flags = sa_flags;
    action_.sa_sigaction = action;
    sigaction64(signal_number_, &action_, &old_action_);
  }

  ScopedSignalHandler(int signal_number) : signal_number_(signal_number) {
    sigaction64(signal_number, nullptr, &old_action_);
  }

  ~ScopedSignalHandler() {
    sigaction64(signal_number_, &old_action_, NULL);
  }

 private:
  struct sigaction64 action_;
  struct sigaction64 old_action_;
  const int signal_number_;
};

class SignalMaskRestorer {
 public:
  SignalMaskRestorer() {
    sigprocmask64(SIG_SETMASK, nullptr, &old_mask_);
  }

  ~SignalMaskRestorer() {
    sigprocmask64(SIG_SETMASK, &old_mask_, nullptr);
  }

 private:
  sigset64_t old_mask_;
};

#endif // _BIONIC_TESTS_SCOPED_SIGNAL_HANDLER_H
