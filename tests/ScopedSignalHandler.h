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

class ScopedSignalHandler {
 public:
  ScopedSignalHandler(int signal_number, void (*handler)(int)) : signal_number_(signal_number) {
    sigemptyset(&action_.sa_mask);
    action_.sa_flags = 0;
    action_.sa_handler = handler;
    sigaction(signal_number_, &action_, &old_action_);
  }

  ~ScopedSignalHandler() {
    sigaction(signal_number_, &old_action_, NULL);
  }

 private:
  struct sigaction action_;
  struct sigaction old_action_;
  const int signal_number_;
};

#endif // _BIONIC_TESTS_SCOPED_SIGNAL_HANDLER_H
