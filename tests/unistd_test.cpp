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

#include <gtest/gtest.h>

#include <stdint.h>
#include <unistd.h>

TEST(unistd, sysconf_SC_MONOTONIC_CLOCK) {
  ASSERT_GT(sysconf(_SC_MONOTONIC_CLOCK), 0);
}

TEST(unistd, sbrk) {
  void* initial_break = sbrk(0);

  void* new_break = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(initial_break) + 2000);
  ASSERT_EQ(0, brk(new_break));

  void* final_break = sbrk(0);
  ASSERT_EQ(final_break, new_break);
}
