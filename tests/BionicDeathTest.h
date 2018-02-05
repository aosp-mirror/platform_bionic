/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef BIONIC_TESTS_BIONIC_DEATH_TEST_H_
#define BIONIC_TESTS_BIONIC_DEATH_TEST_H_

#include <signal.h>

#include <gtest/gtest.h>

#if !defined(__BIONIC__)
#define sigaction64 sigaction
#endif

class BionicDeathTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // Suppress debuggerd stack traces. Too slow.
    for (int signo : { SIGABRT, SIGBUS, SIGSEGV, SIGSYS }) {
      struct sigaction64 action = { .sa_handler = SIG_DFL };
      sigaction64(signo, &action, &previous_);
    }
  }

  virtual void TearDown() {
    for (int signo : { SIGABRT, SIGBUS, SIGSEGV, SIGSYS }) {
      sigaction64(signo, &previous_, nullptr);
    }
  }

 private:
  struct sigaction64 previous_;
};

#endif // BIONIC_TESTS_BIONIC_DEATH_TEST_H_
