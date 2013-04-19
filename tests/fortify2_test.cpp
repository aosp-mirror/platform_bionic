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
#define _FORTIFY_SOURCE 2

#include <gtest/gtest.h>
#include <string.h>

struct foo {
  char a[10];
  char b[10];
};

// We have to say "DeathTest" here so gtest knows to run this test (which exits)
// in its own process.
TEST(Fortify2_DeathTest, strncpy_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  int copy_amt = atoi("11");
  ASSERT_EXIT(strncpy(myfoo.a, "01234567890", copy_amt),
              testing::KilledBySignal(SIGSEGV), "");
}

TEST(Fortify2_DeathTest, sprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(myfoo.a, "%s", source_buf),
              testing::KilledBySignal(SIGSEGV), "");
}

#if __BIONIC__
TEST(Fortify2_DeathTest, strchr_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a));
  myfoo.b[0] = '\0';
  ASSERT_EXIT(printf("%s", strchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGSEGV), "");
}

TEST(Fortify2_DeathTest, strrchr_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", 10);
  memcpy(myfoo.b, "01234", 6);
  ASSERT_EXIT(printf("%s", strrchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGSEGV), "");
}
#endif

/***********************************************************/
/* TESTS BELOW HERE DUPLICATE TESTS FROM fortify1_test.cpp */
/***********************************************************/

#if __BIONIC__
TEST(Fortify2_DeathTest, strcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char *orig = strdup("0123456789");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGSEGV), "");
  free(orig);
}

TEST(Fortify2_DeathTest, strlen_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%d", strlen(buf)), testing::KilledBySignal(SIGSEGV), "");
}

TEST(Fortify2_DeathTest, strchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strchr(buf, 'a')), testing::KilledBySignal(SIGSEGV), "");
}

TEST(Fortify2_DeathTest, strrchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strrchr(buf, 'a')), testing::KilledBySignal(SIGSEGV), "");
}
#endif

TEST(Fortify2_DeathTest, sprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(buf, "%s", source_buf), testing::KilledBySignal(SIGSEGV), "");
}
