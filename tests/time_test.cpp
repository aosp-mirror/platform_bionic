/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <sys/cdefs.h>
#include <features.h>
#include <gtest/gtest.h>

#include <time.h>

#if defined(__BIONIC__) // mktime_tz is a bionic extension.
#include <libc/private/bionic_time.h>
#endif // __BIONIC__

TEST(time, mktime_tz) {
#if defined(__BIONIC__)
  struct tm epoch;
  memset(&epoch, 0, sizeof(tm));
  epoch.tm_year = 1970 - 1900;
  epoch.tm_mon = 1;
  epoch.tm_mday = 1;

  // Alphabetically first. Coincidentally equivalent to UTC.
  ASSERT_EQ(2678400, mktime_tz(&epoch, "Africa/Abidjan"));

  // Alphabetically last. Coincidentally equivalent to UTC.
  ASSERT_EQ(2678400, mktime_tz(&epoch, "Zulu"));

  // Somewhere in the middle, not UTC.
  ASSERT_EQ(2707200, mktime_tz(&epoch, "America/Los_Angeles"));

  // Missing. Falls back to UTC.
  ASSERT_EQ(2678400, mktime_tz(&epoch, "PST"));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(time, gmtime) {
  time_t t = 0;
  tm* broken_down = gmtime(&t);
  ASSERT_TRUE(broken_down != NULL);
  ASSERT_EQ(0, broken_down->tm_sec);
  ASSERT_EQ(0, broken_down->tm_min);
  ASSERT_EQ(0, broken_down->tm_hour);
  ASSERT_EQ(1, broken_down->tm_mday);
  ASSERT_EQ(0, broken_down->tm_mon);
  ASSERT_EQ(1970, broken_down->tm_year + 1900);
}

TEST(time, mktime_10310929) {
  struct tm t;
  memset(&t, 0, sizeof(tm));
  t.tm_year = 200;
  t.tm_mon = 2;
  t.tm_mday = 10;

#if !defined(__LP64__)
  // 32-bit bionic stupidly had a signed 32-bit time_t.
  ASSERT_EQ(-1, mktime(&t));
#if defined(__BIONIC__)
  ASSERT_EQ(-1, mktime_tz(&t, "UTC"));
#endif
#else
  // Everyone else should be using a signed 64-bit time_t.
  ASSERT_GE(sizeof(time_t) * 8, 64U);

  setenv("TZ", "America/Los_Angeles", 1);
  tzset();
  ASSERT_EQ(static_cast<time_t>(4108348800U), mktime(&t));
#if defined(__BIONIC__)
  ASSERT_EQ(static_cast<time_t>(4108320000U), mktime_tz(&t, "UTC"));
#endif

  setenv("TZ", "UTC", 1);
  tzset();
  ASSERT_EQ(static_cast<time_t>(4108320000U), mktime(&t));
#if defined(__BIONIC__)
  ASSERT_EQ(static_cast<time_t>(4108348800U), mktime_tz(&t, "America/Los_Angeles"));
#endif
#endif
}
