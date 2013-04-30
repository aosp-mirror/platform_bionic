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

#undef _FORTIFY_SOURCE
#define _FORTIFY_SOURCE 1

#include <gtest/gtest.h>
#include <string.h>

#if __BIONIC__
// We have to say "DeathTest" here so gtest knows to run this test (which exits)
// in its own process.
TEST(Fortify1_DeathTest, strcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char *orig = strdup("0123456789");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGSEGV), "");
  free(orig);
}

TEST(Fortify1_DeathTest, strlen_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%d", strlen(buf)), testing::KilledBySignal(SIGSEGV), "");
}

TEST(Fortify1_DeathTest, strchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strchr(buf, 'a')), testing::KilledBySignal(SIGSEGV), "");
}

TEST(Fortify1_DeathTest, strrchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strrchr(buf, 'a')), testing::KilledBySignal(SIGSEGV), "");
}
#endif

TEST(Fortify1_DeathTest, sprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(buf, "%s", source_buf), testing::KilledBySignal(SIGSEGV), "");
}
