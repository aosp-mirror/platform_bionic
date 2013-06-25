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
  char empty[0];
  char one[1];
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
              testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, sprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(myfoo.a, "%s", source_buf),
              testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, sprintf2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  ASSERT_EXIT(sprintf(myfoo.a, "0123456789"),
              testing::KilledBySignal(SIGABRT), "");
}

#if __BIONIC__
// zero sized target with "\0" source (should fail)
TEST(Fortify2_DeathTest, strcpy_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("");
  ASSERT_EXIT(strcpy(myfoo.empty, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
}

// zero sized target with longer source (should fail)
TEST(Fortify2_DeathTest, strcpy2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("1");
  ASSERT_EXIT(strcpy(myfoo.empty, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
}

// one byte target with longer source (should fail)
TEST(Fortify2_DeathTest, strcpy3_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("12");
  ASSERT_EXIT(strcpy(myfoo.one, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
}

TEST(Fortify2_DeathTest, strchr_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a));
  myfoo.b[0] = '\0';
  ASSERT_EXIT(printf("%s", strchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strrchr_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", 10);
  memcpy(myfoo.b, "01234", 6);
  ASSERT_EXIT(printf("%s", strrchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strlcpy_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "01");
  size_t n = strlen(myfoo.a);
  ASSERT_EXIT(strlcpy(myfoo.one, myfoo.a, n),
              testing::KilledBySignal(SIGABRT), "");
}

#endif

TEST(Fortify2_DeathTest, strncat_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  size_t n = atoi("10"); // avoid compiler optimizations
  strncpy(myfoo.a, "012345678", n);
  ASSERT_EXIT(strncat(myfoo.a, "9", n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strncat2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  myfoo.a[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(myfoo.a, "0123456789", n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strncat3_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a)); // unterminated string
  myfoo.b[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(myfoo.b, myfoo.a, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strcat_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char src[11];
  strcpy(src, "0123456789");
  foo myfoo;
  myfoo.a[0] = '\0';
  ASSERT_EXIT(strcat(myfoo.a, src), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strcat2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a)); // unterminated string
  myfoo.b[0] = '\0';
  ASSERT_EXIT(strcat(myfoo.b, myfoo.a), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, snprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "012345678");
  size_t n = strlen(myfoo.a) + 2;
  ASSERT_EXIT(snprintf(myfoo.b, n, "a%s", myfoo.a), testing::KilledBySignal(SIGABRT), "");
}

/***********************************************************/
/* TESTS BELOW HERE DUPLICATE TESTS FROM fortify1_test.cpp */
/***********************************************************/

#if __BIONIC__
// multibyte target where we over fill (should fail)
TEST(Fortify2_DeathTest, strcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char *orig = strdup("0123456789");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

// zero sized target with "\0" source (should fail)
TEST(Fortify2_DeathTest, strcpy2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[0];
  char *orig = strdup("");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

// zero sized target with longer source (should fail)
TEST(Fortify2_DeathTest, strcpy3_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[0];
  char *orig = strdup("1");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

// one byte target with longer source (should fail)
TEST(Fortify2_DeathTest, strcpy4_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[1];
  char *orig = strdup("12");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

TEST(Fortify2_DeathTest, strlen_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%d", strlen(buf)), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strchr(buf, 'a')), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strrchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strrchr(buf, 'a')), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strlcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strlcpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

#endif

TEST(Fortify2_DeathTest, sprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(buf, "%s", source_buf), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, sprintf2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[5];
  ASSERT_EXIT(sprintf(buf, "aaaaa"), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strncat_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  size_t n = atoi("10"); // avoid compiler optimizations
  strncpy(buf, "012345678", n);
  ASSERT_EXIT(strncat(buf, "9", n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strncat2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  buf[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(buf, "0123456789", n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strcat_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char src[11];
  strcpy(src, "0123456789");
  char buf[10];
  buf[0] = '\0';
  ASSERT_EXIT(strcat(buf, src), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, memmove_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[20];
  strcpy(buf, "0123456789");
  size_t n = atoi("10");
  ASSERT_EXIT(memmove(buf + 11, buf, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, memcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[10];
  char bufb[10];
  strcpy(bufa, "012345678");
  size_t n = atoi("11");
  ASSERT_EXIT(memcpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, strncpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strncpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(Fortify2_DeathTest, snprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "0123456789");
  size_t n = strlen(bufa) + 1;
  ASSERT_EXIT(snprintf(bufb, n, "%s", bufa), testing::KilledBySignal(SIGABRT), "");
}
