/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <sys/timex.h>

#include <errno.h>

#include <gtest/gtest.h>

TEST(sys_timex, adjtimex_smoke) {
  timex t;
  memset(&t, 0, sizeof(t));
  // adjtimex/clock_adjtime return the clock state on success, -1 on failure.
  ASSERT_NE(-1, adjtimex(&t));
}

TEST(sys_timex, adjtimex_EFAULT) {
  errno = 0;
  ASSERT_EQ(-1, adjtimex(nullptr));
  ASSERT_EQ(EFAULT, errno);
}

TEST(sys_timex, clock_adjtime_smoke) {
  timex t;
  memset(&t, 0, sizeof(t));
  // adjtimex/clock_adjtime return the clock state on success, -1 on failure.
  ASSERT_NE(-1, clock_adjtime(CLOCK_REALTIME, &t));
}

TEST(sys_timex, clock_adjtime_EFAULT) {
  errno = 0;
  ASSERT_EQ(-1, clock_adjtime(CLOCK_REALTIME, nullptr));
  ASSERT_EQ(EFAULT, errno);
}
