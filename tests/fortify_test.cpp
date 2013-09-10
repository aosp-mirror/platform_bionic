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

#include <gtest/gtest.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

// We have to say "DeathTest" here so gtest knows to run this test (which exits)
// in its own process. Unfortunately, the C preprocessor doesn't give us an
// easy way to concatenate strings, so we need to use the complicated method
// below. *sigh*
#define DEATHTEST_PASTER(name) name##_DeathTest
#define DEATHTEST_EVALUATOR(name) DEATHTEST_PASTER(name)
#define DEATHTEST DEATHTEST_EVALUATOR(TEST_NAME)

#if defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE == 2
struct foo {
  char empty[0];
  char one[1];
  char a[10];
  char b[10];
};

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strncpy_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  int copy_amt = atoi("11");
  ASSERT_EXIT(strncpy(myfoo.a, "01234567890", copy_amt),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, sprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(myfoo.a, "%s", source_buf),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, sprintf2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  ASSERT_EXIT(sprintf(myfoo.a, "0123456789"),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// These tests are disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
static int vsprintf_helper2(const char *fmt, ...) {
  foo myfoo;
  va_list va;
  int result;

  va_start(va, fmt);
  result = vsprintf(myfoo.a, fmt, va); // should crash here
  va_end(va);
  return result;
}

TEST(DEATHTEST, vsprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsprintf_helper2("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, vsprintf2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsprintf_helper2("0123456789"), testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// These tests are disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
static int vsnprintf_helper2(const char *fmt, ...) {
  foo myfoo;
  va_list va;
  int result;
  size_t size = atoi("11");

  va_start(va, fmt);
  result = vsnprintf(myfoo.a, size, fmt, va); // should crash here
  va_end(va);
  return result;
}

TEST(DEATHTEST, vsnprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper2("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, vsnprintf2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper2("0123456789"), testing::KilledBySignal(SIGABRT), "");
}
#endif

#if __BIONIC__

#ifndef __clang__
// zero sized target with "\0" source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strcpy_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("");
  ASSERT_EXIT(strcpy(myfoo.empty, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
}
#endif

#ifndef __clang__
// zero sized target with longer source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strcpy2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("1");
  ASSERT_EXIT(strcpy(myfoo.empty, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
}
#endif

#ifndef __clang__
// one byte target with longer source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strcpy3_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("12");
  ASSERT_EXIT(strcpy(myfoo.one, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strchr_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a));
  myfoo.b[0] = '\0';
  ASSERT_EXIT(printf("%s", strchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strrchr_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", 10);
  memcpy(myfoo.b, "01234", 6);
  ASSERT_EXIT(printf("%s", strrchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strlcpy_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "01");
  size_t n = strlen(myfoo.a);
  ASSERT_EXIT(strlcpy(myfoo.one, myfoo.a, n),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strlcat_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "01");
  myfoo.one[0] = '\0';
  size_t n = strlen(myfoo.a);
  ASSERT_EXIT(strlcat(myfoo.one, myfoo.a, n),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#endif /* __BIONIC__ */

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strncat_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  size_t n = atoi("10"); // avoid compiler optimizations
  strncpy(myfoo.a, "012345678", n);
  ASSERT_EXIT(strncat(myfoo.a, "9", n), testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strncat2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  myfoo.a[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(myfoo.a, "0123456789", n), testing::KilledBySignal(SIGABRT), "");
}
#endif

TEST(DEATHTEST, strncat3_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a)); // unterminated string
  myfoo.b[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(myfoo.b, myfoo.a, n), testing::KilledBySignal(SIGABRT), "");
}

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST(DEATHTEST, strcat_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char src[11];
  strcpy(src, "0123456789");
  foo myfoo;
  myfoo.a[0] = '\0';
  ASSERT_EXIT(strcat(myfoo.a, src), testing::KilledBySignal(SIGABRT), "");
}
#endif

TEST(DEATHTEST, strcat2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a)); // unterminated string
  myfoo.b[0] = '\0';
  ASSERT_EXIT(strcat(myfoo.b, myfoo.a), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, snprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "012345678");
  size_t n = strlen(myfoo.a) + 2;
  ASSERT_EXIT(snprintf(myfoo.b, n, "a%s", myfoo.a), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, bzero_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.b, "0123456789", sizeof(myfoo.b));
  size_t n = atoi("11");
  ASSERT_EXIT(bzero(myfoo.b, n), testing::KilledBySignal(SIGABRT), "");
}

#endif /* defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE=2 */

#if __BIONIC__
// multibyte target where we over fill (should fail)
TEST(DEATHTEST, strcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char *orig = strdup("0123456789");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

// zero sized target with "\0" source (should fail)
TEST(DEATHTEST, strcpy2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[0];
  char *orig = strdup("");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

// zero sized target with longer source (should fail)
TEST(DEATHTEST, strcpy3_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[0];
  char *orig = strdup("1");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

// one byte target with longer source (should fail)
TEST(DEATHTEST, strcpy4_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[1];
  char *orig = strdup("12");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
}

TEST(DEATHTEST, strlen_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%d", strlen(buf)), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strchr(buf, 'a')), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strrchr_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strrchr(buf, 'a')), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strlcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strlcpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strlcat_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  bufb[0] = '\0';
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strlcat(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

#endif

TEST(DEATHTEST, sprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(buf, "%s", source_buf), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, sprintf2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[5];
  ASSERT_EXIT(sprintf(buf, "aaaaa"), testing::KilledBySignal(SIGABRT), "");
}

static int vsprintf_helper(const char *fmt, ...) {
  char buf[10];
  va_list va;
  int result;

  va_start(va, fmt);
  result = vsprintf(buf, fmt, va); // should crash here
  va_end(va);
  return result;
}

TEST(DEATHTEST, vsprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsprintf_helper("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, vsprintf2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsprintf_helper("0123456789"), testing::KilledBySignal(SIGABRT), "");
}

static int vsnprintf_helper(const char *fmt, ...) {
  char buf[10];
  va_list va;
  int result;
  size_t size = atoi("11");

  va_start(va, fmt);
  result = vsnprintf(buf, size, fmt, va); // should crash here
  va_end(va);
  return result;
}

TEST(DEATHTEST, vsnprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, vsnprintf2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper("0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strncat_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  size_t n = atoi("10"); // avoid compiler optimizations
  strncpy(buf, "012345678", n);
  ASSERT_EXIT(strncat(buf, "9", n), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strncat2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  buf[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(buf, "0123456789", n), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strcat_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char src[11];
  strcpy(src, "0123456789");
  char buf[10];
  buf[0] = '\0';
  ASSERT_EXIT(strcat(buf, src), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, memmove_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[20];
  strcpy(buf, "0123456789");
  size_t n = atoi("10");
  ASSERT_EXIT(memmove(buf + 11, buf, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, memcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[10];
  char bufb[10];
  strcpy(bufa, "012345678");
  size_t n = atoi("11");
  ASSERT_EXIT(memcpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, strncpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strncpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, snprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "0123456789");
  size_t n = strlen(bufa) + 1;
  ASSERT_EXIT(snprintf(bufb, n, "%s", bufa), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, bzero_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  size_t n = atoi("11");
  ASSERT_EXIT(bzero(buf, n), testing::KilledBySignal(SIGABRT), "");
}

TEST(DEATHTEST, umask_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  mode_t mask = atoi("1023");  // 01777 in octal
  ASSERT_EXIT(umask(mask), testing::KilledBySignal(SIGABRT), "");
}

extern "C" char* __strncat_chk(char*, const char*, size_t, size_t);
extern "C" char* __strcat_chk(char*, const char*, size_t);

TEST(TEST_NAME, strncat) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = __strncat_chk(buf, "01234", sizeof(buf) - strlen(buf) - 1, sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('\0', buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(TEST_NAME, strncat2) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = __strncat_chk(buf, "0123456789", 5, sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('\0', buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(TEST_NAME, strncat3) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = '\0';
  char* res = __strncat_chk(buf, "0123456789", 5, sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('0',  buf[0]);
  ASSERT_EQ('1',  buf[1]);
  ASSERT_EQ('2',  buf[2]);
  ASSERT_EQ('3',  buf[3]);
  ASSERT_EQ('4',  buf[4]);
  ASSERT_EQ('\0', buf[5]);
  ASSERT_EQ('A',  buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(TEST_NAME, strncat4) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[9] = '\0';
  char* res = __strncat_chk(buf, "", 5, sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('A',  buf[0]);
  ASSERT_EQ('A',  buf[1]);
  ASSERT_EQ('A',  buf[2]);
  ASSERT_EQ('A',  buf[3]);
  ASSERT_EQ('A',  buf[4]);
  ASSERT_EQ('A',  buf[5]);
  ASSERT_EQ('A',  buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('\0', buf[9]);
}

TEST(TEST_NAME, strncat5) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = __strncat_chk(buf, "01234567", 8, sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('5', buf[6]);
  ASSERT_EQ('6',  buf[7]);
  ASSERT_EQ('7',  buf[8]);
  ASSERT_EQ('\0',  buf[9]);
}

TEST(TEST_NAME, strncat6) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = __strncat_chk(buf, "01234567", 9, sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('5', buf[6]);
  ASSERT_EQ('6',  buf[7]);
  ASSERT_EQ('7',  buf[8]);
  ASSERT_EQ('\0',  buf[9]);
}


TEST(TEST_NAME, strcat) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = __strcat_chk(buf, "01234", sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('\0', buf[6]);
  ASSERT_EQ('A',  buf[7]);
  ASSERT_EQ('A',  buf[8]);
  ASSERT_EQ('A',  buf[9]);
}

TEST(TEST_NAME, strcat2) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = __strcat_chk(buf, "01234567", sizeof(buf));
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('5', buf[6]);
  ASSERT_EQ('6',  buf[7]);
  ASSERT_EQ('7',  buf[8]);
  ASSERT_EQ('\0',  buf[9]);
}

TEST(TEST_NAME, strcat_chk_max_int_size) {
  char buf[10];
  memset(buf, 'A', sizeof(buf));
  buf[0] = 'a';
  buf[1] = '\0';
  char* res = __strcat_chk(buf, "01234567", (size_t)-1);
  ASSERT_EQ(buf, res);
  ASSERT_EQ('a',  buf[0]);
  ASSERT_EQ('0',  buf[1]);
  ASSERT_EQ('1',  buf[2]);
  ASSERT_EQ('2',  buf[3]);
  ASSERT_EQ('3',  buf[4]);
  ASSERT_EQ('4',  buf[5]);
  ASSERT_EQ('5',  buf[6]);
  ASSERT_EQ('6',  buf[7]);
  ASSERT_EQ('7',  buf[8]);
  ASSERT_EQ('\0', buf[9]);
}

extern "C" char* __strcpy_chk(char*, const char*, size_t);

TEST(TEST_NAME, strcpy_chk_max_int_size) {
  char buf[10];
  char* res = __strcpy_chk(buf, "012345678", (size_t)-1);
  ASSERT_EQ(buf, res);
  ASSERT_EQ('0',  buf[0]);
  ASSERT_EQ('1',  buf[1]);
  ASSERT_EQ('2',  buf[2]);
  ASSERT_EQ('3',  buf[3]);
  ASSERT_EQ('4',  buf[4]);
  ASSERT_EQ('5',  buf[5]);
  ASSERT_EQ('6',  buf[6]);
  ASSERT_EQ('7',  buf[7]);
  ASSERT_EQ('8',  buf[8]);
  ASSERT_EQ('\0', buf[9]);
}
extern "C" void* __memcpy_chk(void*, const void*, size_t, size_t);

TEST(TEST_NAME, memcpy_chk_max_int_size) {
  char buf[10];
  void* res = __memcpy_chk(buf, "012345678", sizeof(buf), (size_t)-1);
  ASSERT_EQ((void*)buf, res);
  ASSERT_EQ('0',  buf[0]);
  ASSERT_EQ('1',  buf[1]);
  ASSERT_EQ('2',  buf[2]);
  ASSERT_EQ('3',  buf[3]);
  ASSERT_EQ('4',  buf[4]);
  ASSERT_EQ('5',  buf[5]);
  ASSERT_EQ('6',  buf[6]);
  ASSERT_EQ('7',  buf[7]);
  ASSERT_EQ('8',  buf[8]);
  ASSERT_EQ('\0', buf[9]);
}
