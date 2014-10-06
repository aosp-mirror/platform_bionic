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
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/prctl.h>

// We have to say "DeathTest" here so gtest knows to run this test (which exits)
// in its own process. Unfortunately, the C preprocessor doesn't give us an
// easy way to concatenate strings, so we need to use the complicated method
// below. *sigh*
#define DEATHTEST_PASTER(name) name##_DeathTest
#define DEATHTEST_EVALUATOR(name) DEATHTEST_PASTER(name)
#define DEATHTEST DEATHTEST_EVALUATOR(TEST_NAME)

class DEATHTEST : public testing::Test {
 protected:
  virtual void SetUp() {
    old_dumpable_ = prctl(PR_GET_DUMPABLE, 0, 0, 0, 0);
    // Suppress debuggerd stack traces. Too slow.
    prctl(PR_SET_DUMPABLE, 0, 0, 0, 0);
  }

  virtual void TearDown() {
    prctl(PR_SET_DUMPABLE, old_dumpable_, 0, 0, 0, 0);
  }
 private:
  int old_dumpable_;
};

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
TEST_F(DEATHTEST, stpncpy_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  int copy_amt = atoi("11");
  ASSERT_EXIT(stpncpy(myfoo.a, "01234567890", copy_amt),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, stpncpy2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memset(&myfoo, 0, sizeof(myfoo));
  myfoo.one[0] = 'A'; // not null terminated string
  ASSERT_EXIT(stpncpy(myfoo.b, myfoo.one, sizeof(myfoo.b)),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strncpy_fortified2) {
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
TEST_F(DEATHTEST, strncpy2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memset(&myfoo, 0, sizeof(myfoo));
  myfoo.one[0] = 'A'; // not null terminated string
  ASSERT_EXIT(strncpy(myfoo.b, myfoo.one, sizeof(myfoo.b)),
              testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, sprintf_fortified2) {
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
TEST_F(DEATHTEST, sprintf2_fortified2) {
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

TEST_F(DEATHTEST, vsprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsprintf_helper2("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, vsprintf2_fortified2) {
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

TEST_F(DEATHTEST, vsnprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper2("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, vsnprintf2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper2("0123456789"), testing::KilledBySignal(SIGABRT), "");
}
#endif

#ifndef __clang__
// zero sized target with "\0" source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, stpcpy_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("");
  ASSERT_EXIT(stpcpy(myfoo.empty, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// zero sized target with "\0" source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strcpy_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("");
  ASSERT_EXIT(strcpy(myfoo.empty, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// zero sized target with longer source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strcpy2_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("1");
  ASSERT_EXIT(strcpy(myfoo.empty, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// one byte target with longer source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strcpy3_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  char* src = strdup("12");
  ASSERT_EXIT(strcpy(myfoo.one, src),
              testing::KilledBySignal(SIGABRT), "");
  free(src);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strchr_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a));
  myfoo.b[0] = '\0';
  ASSERT_EXIT(printf("%s", strchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strrchr_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", 10);
  memcpy(myfoo.b, "01234", 6);
  ASSERT_EXIT(printf("%s", strrchr(myfoo.a, 'a')),
              testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strlcpy_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "01");
  size_t n = strlen(myfoo.a);
  ASSERT_EXIT(strlcpy(myfoo.one, myfoo.a, n),
              testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strlcat_fortified2) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "01");
  myfoo.one[0] = '\0';
  size_t n = strlen(myfoo.a);
  ASSERT_EXIT(strlcat(myfoo.one, myfoo.a, n),
              testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strncat_fortified2) {
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
TEST_F(DEATHTEST, strncat2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  myfoo.a[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(myfoo.a, "0123456789", n), testing::KilledBySignal(SIGABRT), "");
}
#endif

TEST_F(DEATHTEST, strncat3_fortified2) {
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
TEST_F(DEATHTEST, strcat_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char src[11];
  strcpy(src, "0123456789");
  foo myfoo;
  myfoo.a[0] = '\0';
  ASSERT_EXIT(strcat(myfoo.a, src), testing::KilledBySignal(SIGABRT), "");
}
#endif

TEST_F(DEATHTEST, strcat2_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a)); // unterminated string
  myfoo.b[0] = '\0';
  ASSERT_EXIT(strcat(myfoo.b, myfoo.a), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, snprintf_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  strcpy(myfoo.a, "012345678");
  size_t n = strlen(myfoo.a) + 2;
  ASSERT_EXIT(snprintf(myfoo.b, n, "a%s", myfoo.a), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, bzero_fortified2) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  foo myfoo;
  memcpy(myfoo.b, "0123456789", sizeof(myfoo.b));
  size_t n = atoi("11");
  ASSERT_EXIT(bzero(myfoo.b, n), testing::KilledBySignal(SIGABRT), "");
}

#endif /* defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE=2 */

// multibyte target where we over fill (should fail)
TEST_F(DEATHTEST, strcpy_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char *orig = strdup("0123456789");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

// zero sized target with "\0" source (should fail)
TEST_F(DEATHTEST, strcpy2_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[0];
  char *orig = strdup("");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

// zero sized target with longer source (should fail)
TEST_F(DEATHTEST, strcpy3_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[0];
  char *orig = strdup("1");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

// one byte target with longer source (should fail)
TEST_F(DEATHTEST, strcpy4_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[1];
  char *orig = strdup("12");
  ASSERT_EXIT(strcpy(buf, orig), testing::KilledBySignal(SIGABRT), "");
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strlen_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%zd", strlen(buf)), testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strchr_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strchr(buf, 'a')), testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strrchr_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_EXIT(printf("%s", strrchr(buf, 'a')), testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strlcpy_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strlcpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strlcat_fortified) {
#if defined(__BIONIC__)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  bufb[0] = '\0';
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strlcat(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, sprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_EXIT(sprintf(buf, "%s", source_buf), testing::KilledBySignal(SIGABRT), "");
}

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, sprintf_malloc_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char* buf = (char *) malloc(10);
  char source_buf[11];
  memcpy(source_buf, "1234567890", 11);
  ASSERT_EXIT(sprintf(buf, "%s", source_buf), testing::KilledBySignal(SIGABRT), "");
  free(buf);
}
#endif

TEST_F(DEATHTEST, sprintf2_fortified) {
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

TEST_F(DEATHTEST, vsprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsprintf_helper("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, vsprintf2_fortified) {
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

TEST_F(DEATHTEST, vsnprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper("%s", "0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, vsnprintf2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(vsnprintf_helper("0123456789"), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, strncat_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  size_t n = atoi("10"); // avoid compiler optimizations
  strncpy(buf, "012345678", n);
  ASSERT_EXIT(strncat(buf, "9", n), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, strncat2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  buf[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_EXIT(strncat(buf, "0123456789", n), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, strcat_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char src[11];
  strcpy(src, "0123456789");
  char buf[10];
  buf[0] = '\0';
  ASSERT_EXIT(strcat(buf, src), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, memmove_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[20];
  strcpy(buf, "0123456789");
  size_t n = atoi("10");
  ASSERT_EXIT(memmove(buf + 11, buf, n), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, memcpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[10];
  char bufb[10];
  strcpy(bufa, "012345678");
  size_t n = atoi("11");
  ASSERT_EXIT(memcpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, stpncpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(stpncpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, stpncpy2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char dest[11];
  char src[10];
  memcpy(src, "0123456789", sizeof(src)); // src is not null terminated
  ASSERT_EXIT(stpncpy(dest, src, sizeof(dest)), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, strncpy_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_EXIT(strncpy(bufb, bufa, n), testing::KilledBySignal(SIGABRT), "");
}


TEST_F(DEATHTEST, strncpy2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char dest[11];
  char src[10];
  memcpy(src, "0123456789", sizeof(src)); // src is not null terminated
  ASSERT_EXIT(strncpy(dest, src, sizeof(dest)), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, snprintf_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "0123456789");
  size_t n = strlen(bufa) + 1;
  ASSERT_EXIT(snprintf(bufb, n, "%s", bufa), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, bzero_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  size_t n = atoi("11");
  ASSERT_EXIT(bzero(buf, n), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, umask_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  mode_t mask = atoi("1023");  // 01777 in octal
  ASSERT_EXIT(umask(mask), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, recv_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  size_t data_len = atoi("11"); // suppress compiler optimizations
  char buf[10];
  ASSERT_EXIT(recv(0, buf, data_len, 0), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, FD_ISSET_fortified) {
#if defined(__BIONIC__) // glibc catches this at compile-time.
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  fd_set set;
  memset(&set, 0, sizeof(set));
  ASSERT_EXIT(FD_ISSET(-1, &set), testing::KilledBySignal(SIGABRT), "");
#endif
}

TEST_F(DEATHTEST, FD_ISSET_2_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[1];
  fd_set* set = (fd_set*) buf;
  ASSERT_EXIT(FD_ISSET(0, set), testing::KilledBySignal(SIGABRT), "");
}

// gtest's ASSERT_EXIT needs a valid expression, but glibc has a do-while macro.
static void FD_ZERO_function(fd_set* s) { FD_ZERO(s); }

TEST_F(DEATHTEST, FD_ZERO_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[1];
  fd_set* set = (fd_set*) buf;
  ASSERT_EXIT(FD_ZERO_function(set), testing::KilledBySignal(SIGABRT), "");
}

TEST_F(DEATHTEST, read_fortified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_EXIT(read(fd, buf, ct), testing::KilledBySignal(SIGABRT), "");
  close(fd);
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

TEST(TEST_NAME, stpncpy) {
  char src[10];
  char dst[10];
  memcpy(src, "0123456789", sizeof(src)); // non null terminated string
  stpncpy(dst, src, sizeof(dst));
  ASSERT_EQ('0', dst[0]);
  ASSERT_EQ('1', dst[1]);
  ASSERT_EQ('2', dst[2]);
  ASSERT_EQ('3', dst[3]);
  ASSERT_EQ('4', dst[4]);
  ASSERT_EQ('5', dst[5]);
  ASSERT_EQ('6', dst[6]);
  ASSERT_EQ('7', dst[7]);
  ASSERT_EQ('8', dst[8]);
  ASSERT_EQ('9', dst[9]);
}

TEST(TEST_NAME, stpncpy2) {
  char src[10];
  char dst[15];
  memcpy(src, "012345678\0", sizeof(src));
  stpncpy(dst, src, sizeof(dst));
  ASSERT_EQ('0',  dst[0]);
  ASSERT_EQ('1',  dst[1]);
  ASSERT_EQ('2',  dst[2]);
  ASSERT_EQ('3',  dst[3]);
  ASSERT_EQ('4',  dst[4]);
  ASSERT_EQ('5',  dst[5]);
  ASSERT_EQ('6',  dst[6]);
  ASSERT_EQ('7',  dst[7]);
  ASSERT_EQ('8',  dst[8]);
  ASSERT_EQ('\0', dst[9]);
  ASSERT_EQ('\0', dst[10]);
  ASSERT_EQ('\0', dst[11]);
  ASSERT_EQ('\0', dst[12]);
  ASSERT_EQ('\0', dst[13]);
  ASSERT_EQ('\0', dst[14]);
}

TEST(TEST_NAME, strncpy) {
  char src[10];
  char dst[10];
  memcpy(src, "0123456789", sizeof(src)); // non null terminated string
  strncpy(dst, src, sizeof(dst));
  ASSERT_EQ('0', dst[0]);
  ASSERT_EQ('1', dst[1]);
  ASSERT_EQ('2', dst[2]);
  ASSERT_EQ('3', dst[3]);
  ASSERT_EQ('4', dst[4]);
  ASSERT_EQ('5', dst[5]);
  ASSERT_EQ('6', dst[6]);
  ASSERT_EQ('7', dst[7]);
  ASSERT_EQ('8', dst[8]);
  ASSERT_EQ('9', dst[9]);
}

TEST(TEST_NAME, strncpy2) {
  char src[10];
  char dst[15];
  memcpy(src, "012345678\0", sizeof(src));
  strncpy(dst, src, sizeof(dst));
  ASSERT_EQ('0',  dst[0]);
  ASSERT_EQ('1',  dst[1]);
  ASSERT_EQ('2',  dst[2]);
  ASSERT_EQ('3',  dst[3]);
  ASSERT_EQ('4',  dst[4]);
  ASSERT_EQ('5',  dst[5]);
  ASSERT_EQ('6',  dst[6]);
  ASSERT_EQ('7',  dst[7]);
  ASSERT_EQ('8',  dst[8]);
  ASSERT_EQ('\0', dst[9]);
  ASSERT_EQ('\0', dst[10]);
  ASSERT_EQ('\0', dst[11]);
  ASSERT_EQ('\0', dst[12]);
  ASSERT_EQ('\0', dst[13]);
  ASSERT_EQ('\0', dst[14]);
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

extern "C" char* __stpcpy_chk(char*, const char*, size_t);

TEST(TEST_NAME, stpcpy_chk_max_int_size) {
  char buf[10];
  char* res = __stpcpy_chk(buf, "012345678", (size_t)-1);
  ASSERT_EQ(buf + strlen("012345678"), res);
  ASSERT_STREQ("012345678", buf);
}

extern "C" char* __strcpy_chk(char*, const char*, size_t);

TEST(TEST_NAME, strcpy_chk_max_int_size) {
  char buf[10];
  char* res = __strcpy_chk(buf, "012345678", (size_t)-1);
  ASSERT_EQ(buf, res);
  ASSERT_STREQ("012345678", buf);
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

// Verify that macro expansion is done properly for sprintf/snprintf (which
// are defined as macros in stdio.h under clang).
#define CONTENTS "macro expansion"
#define BUF_AND_SIZE(A) A, sizeof(A)
#define BUF_AND_CONTENTS(A) A, CONTENTS
#define BUF_AND_SIZE_AND_CONTENTS(A) A, sizeof(A), CONTENTS
TEST(TEST_NAME, s_n_printf_macro_expansion) {
  char buf[BUFSIZ];
  snprintf(BUF_AND_SIZE(buf), CONTENTS);
  EXPECT_STREQ(CONTENTS, buf);

  snprintf(BUF_AND_SIZE_AND_CONTENTS(buf));
  EXPECT_STREQ(CONTENTS, buf);

  sprintf(BUF_AND_CONTENTS(buf));
  EXPECT_STREQ(CONTENTS, buf);
}
