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

#include <gtest/gtest.h>

#include <sys/prctl.h>

class BionicDeathTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // Suppress debuggerd stack traces. Too slow.
    old_dumpable_ = prctl(PR_GET_DUMPABLE, 0, 0, 0, 0);
    prctl(PR_SET_DUMPABLE, 0, 0, 0, 0);
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  }

  virtual void TearDown() {
    prctl(PR_SET_DUMPABLE, old_dumpable_, 0, 0, 0, 0);
  }

 private:
  int old_dumpable_;
};

#endif // BIONIC_TESTS_BIONIC_DEATH_TEST_H_
