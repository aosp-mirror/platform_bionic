/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <android-base/silent_death_test.h>

#include "private/bionic_constants.h"

using scs_DeathTest = SilentDeathTest;

int recurse2(int count);

__attribute__((weak, noinline)) int recurse1(int count) {
  if (count != 0) return recurse2(count - 1) + 1;
  return 0;
}

__attribute__((weak, noinline)) int recurse2(int count) {
  if (count != 0) return recurse1(count - 1) + 1;
  return 0;
}

TEST_F(scs_DeathTest, stack_overflow) {
#if defined(__aarch64__) || defined(__riscv)
  ASSERT_EXIT(recurse1(SCS_SIZE), testing::KilledBySignal(SIGSEGV), "");
#else
  GTEST_SKIP() << "no SCS on this architecture";
#endif
}
