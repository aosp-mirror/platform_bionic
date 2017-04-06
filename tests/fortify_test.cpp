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

// -Werror is on whether we like it or not, and we're intentionally doing awful
// things in this file. GCC is dumb and doesn't have a specific error class for
// the fortify failures (it's just -Werror), so we can't use anything more
// constrained than disabling all the warnings in the file :( It also won't let
// us use system_header in a .cpp file, so we have to #include this from
// fortify_test_main.cpp.
#pragma GCC system_header

#include <gtest/gtest.h>
#include "BionicDeathTest.h"

#include <fcntl.h>
#include <malloc.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if __BIONIC__
#define ASSERT_FORTIFY(expr) ASSERT_EXIT(expr, testing::KilledBySignal(SIGABRT), "FORTIFY")
#else
#define ASSERT_FORTIFY(expr) ASSERT_EXIT(expr, testing::KilledBySignal(SIGABRT), "")
#endif

// Fortify test code needs to run multiple times, so TEST_NAME macro is used to
// distinguish different tests. TEST_NAME is defined in compilation command.
#define DEATHTEST_PASTER(name) name##_DeathTest
#define DEATHTEST_EVALUATOR(name) DEATHTEST_PASTER(name)
#define DEATHTEST DEATHTEST_EVALUATOR(TEST_NAME)

class DEATHTEST : public BionicDeathTest {};

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
  foo myfoo;
  int copy_amt = atoi("11");
  ASSERT_FORTIFY(stpncpy(myfoo.a, "01234567890", copy_amt));
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, stpncpy2_fortified2) {
  foo myfoo;
  memset(&myfoo, 0, sizeof(myfoo));
  myfoo.one[0] = 'A'; // not null terminated string
  ASSERT_FORTIFY(stpncpy(myfoo.b, myfoo.one, sizeof(myfoo.b)));
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strncpy_fortified2) {
  foo myfoo;
  int copy_amt = atoi("11");
  ASSERT_FORTIFY(strncpy(myfoo.a, "01234567890", copy_amt));
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strncpy2_fortified2) {
  foo myfoo;
  memset(&myfoo, 0, sizeof(myfoo));
  myfoo.one[0] = 'A'; // not null terminated string
  ASSERT_FORTIFY(strncpy(myfoo.b, myfoo.one, sizeof(myfoo.b)));
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, sprintf_fortified2) {
  foo myfoo;
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_FORTIFY(sprintf(myfoo.a, "%s", source_buf));
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, sprintf2_fortified2) {
  foo myfoo;
  ASSERT_FORTIFY(sprintf(myfoo.a, "0123456789"));
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
  ASSERT_FORTIFY(vsprintf_helper2("%s", "0123456789"));
}

TEST_F(DEATHTEST, vsprintf2_fortified2) {
  ASSERT_FORTIFY(vsprintf_helper2("0123456789"));
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
  ASSERT_FORTIFY(vsnprintf_helper2("%s", "0123456789"));
}

TEST_F(DEATHTEST, vsnprintf2_fortified2) {
  ASSERT_FORTIFY(vsnprintf_helper2("0123456789"));
}
#endif

#ifndef __clang__
// zero sized target with "\0" source (should fail)
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, stpcpy_fortified2) {
#if defined(__BIONIC__)
  foo myfoo;
  char* src = strdup("");
  ASSERT_FORTIFY(stpcpy(myfoo.empty, src));
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
  foo myfoo;
  char* src = strdup("");
  ASSERT_FORTIFY(strcpy(myfoo.empty, src));
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
  foo myfoo;
  char* src = strdup("1");
  ASSERT_FORTIFY(strcpy(myfoo.empty, src));
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
  foo myfoo;
  char* src = strdup("12");
  ASSERT_FORTIFY(strcpy(myfoo.one, src));
  free(src);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

TEST_F(DEATHTEST, strchr_fortified2) {
#if defined(__BIONIC__)
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a));
  myfoo.b[0] = '\0';
  ASSERT_FORTIFY(printf("%s", strchr(myfoo.a, 'a')));
  ASSERT_FORTIFY(printf("%s", strchr(static_cast<const char*>(myfoo.a), 'a')));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strrchr_fortified2) {
#if defined(__BIONIC__)
  foo myfoo;
  memcpy(myfoo.a, "0123456789", 10);
  memcpy(myfoo.b, "01234", 6);
  ASSERT_FORTIFY(printf("%s", strrchr(myfoo.a, 'a')));
  ASSERT_FORTIFY(printf("%s", strrchr(static_cast<const char*>(myfoo.a), 'a')));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, memchr_fortified2) {
#if defined(__BIONIC__)
  foo myfoo;
  volatile int asize = sizeof(myfoo.a) + 1;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a));
  ASSERT_FORTIFY(printf("%s", memchr(myfoo.a, 'a', asize)));
  ASSERT_FORTIFY(printf("%s", memchr(static_cast<const void*>(myfoo.a), 'a', asize)));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strlcpy_fortified2) {
#if defined(__BIONIC__)
  foo myfoo;
  strcpy(myfoo.a, "01");
  size_t n = strlen(myfoo.a);
  ASSERT_FORTIFY(strlcpy(myfoo.one, myfoo.a, n));
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
  foo myfoo;
  strcpy(myfoo.a, "01");
  myfoo.one[0] = '\0';
  size_t n = strlen(myfoo.a);
  ASSERT_FORTIFY(strlcat(myfoo.one, myfoo.a, n));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strncat_fortified2) {
  foo myfoo;
  size_t n = atoi("10"); // avoid compiler optimizations
  strncpy(myfoo.a, "012345678", n);
  ASSERT_FORTIFY(strncat(myfoo.a, "9", n));
}
#endif

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strncat2_fortified2) {
  foo myfoo;
  myfoo.a[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_FORTIFY(strncat(myfoo.a, "0123456789", n));
}
#endif

TEST_F(DEATHTEST, strncat3_fortified2) {
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a)); // unterminated string
  myfoo.b[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_FORTIFY(strncat(myfoo.b, myfoo.a, n));
}

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, strcat_fortified2) {
  char src[11];
  strcpy(src, "0123456789");
  foo myfoo;
  myfoo.a[0] = '\0';
  ASSERT_FORTIFY(strcat(myfoo.a, src));
}
#endif

TEST_F(DEATHTEST, strcat2_fortified2) {
  foo myfoo;
  memcpy(myfoo.a, "0123456789", sizeof(myfoo.a)); // unterminated string
  myfoo.b[0] = '\0';
  ASSERT_FORTIFY(strcat(myfoo.b, myfoo.a));
}

TEST_F(DEATHTEST, snprintf_fortified2) {
  foo myfoo;
  strcpy(myfoo.a, "012345678");
  size_t n = strlen(myfoo.a) + 2;
  ASSERT_FORTIFY(snprintf(myfoo.b, n, "a%s", myfoo.a));
}

TEST_F(DEATHTEST, bzero_fortified2) {
  foo myfoo;
  memcpy(myfoo.b, "0123456789", sizeof(myfoo.b));
  size_t n = atoi("11");
  ASSERT_FORTIFY(bzero(myfoo.b, n));
}

#endif /* defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE=2 */

// multibyte target where we over fill (should fail)
TEST_F(DEATHTEST, strcpy_fortified) {
#if defined(__BIONIC__)
  char buf[10];
  char *orig = strdup("0123456789");
  ASSERT_FORTIFY(strcpy(buf, orig));
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

// zero sized target with "\0" source (should fail)
TEST_F(DEATHTEST, strcpy2_fortified) {
#if defined(__BIONIC__)
  char buf[0];
  char *orig = strdup("");
  ASSERT_FORTIFY(strcpy(buf, orig));
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

// zero sized target with longer source (should fail)
TEST_F(DEATHTEST, strcpy3_fortified) {
#if defined(__BIONIC__)
  char buf[0];
  char *orig = strdup("1");
  ASSERT_FORTIFY(strcpy(buf, orig));
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

// one byte target with longer source (should fail)
TEST_F(DEATHTEST, strcpy4_fortified) {
#if defined(__BIONIC__)
  char buf[1];
  char *orig = strdup("12");
  ASSERT_FORTIFY(strcpy(buf, orig));
  free(orig);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strlen_fortified) {
#if defined(__BIONIC__)
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_FORTIFY(printf("%zd", strlen(buf)));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strchr_fortified) {
#if defined(__BIONIC__)
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_FORTIFY(printf("%s", strchr(buf, 'a')));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strrchr_fortified) {
#if defined(__BIONIC__)
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  ASSERT_FORTIFY(printf("%s", strrchr(buf, 'a')));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strlcpy_fortified) {
#if defined(__BIONIC__)
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_FORTIFY(strlcpy(bufb, bufa, n));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, strlcat_fortified) {
#if defined(__BIONIC__)
  char bufa[15];
  char bufb[10];
  bufb[0] = '\0';
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_FORTIFY(strlcat(bufb, bufa, n));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST_F(DEATHTEST, sprintf_fortified) {
  char buf[10];
  char source_buf[15];
  memcpy(source_buf, "12345678901234", 15);
  ASSERT_FORTIFY(sprintf(buf, "%s", source_buf));
}

#ifndef __clang__
// This test is disabled in clang because clang doesn't properly detect
// this buffer overflow. TODO: Fix clang.
TEST_F(DEATHTEST, sprintf_malloc_fortified) {
  char* buf = (char *) malloc(10);
  char source_buf[11];
  memcpy(source_buf, "1234567890", 11);
  ASSERT_FORTIFY(sprintf(buf, "%s", source_buf));
  free(buf);
}
#endif

TEST_F(DEATHTEST, sprintf2_fortified) {
  char buf[5];
  ASSERT_FORTIFY(sprintf(buf, "aaaaa"));
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
  ASSERT_FORTIFY(vsprintf_helper("%s", "0123456789"));
}

TEST_F(DEATHTEST, vsprintf2_fortified) {
  ASSERT_FORTIFY(vsprintf_helper("0123456789"));
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
  ASSERT_FORTIFY(vsnprintf_helper("%s", "0123456789"));
}

TEST_F(DEATHTEST, vsnprintf2_fortified) {
  ASSERT_FORTIFY(vsnprintf_helper("0123456789"));
}

TEST_F(DEATHTEST, strncat_fortified) {
  char buf[10];
  size_t n = atoi("10"); // avoid compiler optimizations
  strncpy(buf, "012345678", n);
  ASSERT_FORTIFY(strncat(buf, "9", n));
}

TEST_F(DEATHTEST, strncat2_fortified) {
  char buf[10];
  buf[0] = '\0';
  size_t n = atoi("10"); // avoid compiler optimizations
  ASSERT_FORTIFY(strncat(buf, "0123456789", n));
}

TEST_F(DEATHTEST, strcat_fortified) {
  char src[11];
  strcpy(src, "0123456789");
  char buf[10];
  buf[0] = '\0';
  ASSERT_FORTIFY(strcat(buf, src));
}

TEST_F(DEATHTEST, memmove_fortified) {
  char buf[20];
  strcpy(buf, "0123456789");
  size_t n = atoi("10");
  ASSERT_FORTIFY(memmove(buf + 11, buf, n));
}

TEST_F(DEATHTEST, memcpy_fortified) {
  char bufa[10];
  char bufb[10];
  strcpy(bufa, "012345678");
  size_t n = atoi("11");
  ASSERT_FORTIFY(memcpy(bufb, bufa, n));
}

TEST_F(DEATHTEST, memset_fortified) {
  char buf[10];
  size_t n = atoi("11");
  ASSERT_FORTIFY(memset(buf, 0, n));
}

TEST_F(DEATHTEST, stpncpy_fortified) {
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_FORTIFY(stpncpy(bufb, bufa, n));
}

TEST_F(DEATHTEST, stpncpy2_fortified) {
  char dest[11];
  char src[10];
  memcpy(src, "0123456789", sizeof(src)); // src is not null terminated
  ASSERT_FORTIFY(stpncpy(dest, src, sizeof(dest)));
}

TEST_F(DEATHTEST, strncpy_fortified) {
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "01234567890123");
  size_t n = strlen(bufa);
  ASSERT_FORTIFY(strncpy(bufb, bufa, n));
}


TEST_F(DEATHTEST, strncpy2_fortified) {
  char dest[11];
  char src[10];
  memcpy(src, "0123456789", sizeof(src)); // src is not null terminated
  ASSERT_FORTIFY(strncpy(dest, src, sizeof(dest)));
}

TEST_F(DEATHTEST, snprintf_fortified) {
  char bufa[15];
  char bufb[10];
  strcpy(bufa, "0123456789");
  size_t n = strlen(bufa) + 1;
  ASSERT_FORTIFY(snprintf(bufb, n, "%s", bufa));
}

TEST_F(DEATHTEST, bzero_fortified) {
  char buf[10];
  memcpy(buf, "0123456789", sizeof(buf));
  size_t n = atoi("11");
  ASSERT_FORTIFY(bzero(buf, n));
}

TEST_F(DEATHTEST, umask_fortified) {
  mode_t mask = atoi("1023");  // 01777 in octal
  ASSERT_FORTIFY(umask(mask));
}

TEST_F(DEATHTEST, recv_fortified) {
  size_t data_len = atoi("11"); // suppress compiler optimizations
  char buf[10];
  ASSERT_FORTIFY(recv(0, buf, data_len, 0));
}

TEST_F(DEATHTEST, send_fortified) {
  size_t data_len = atoi("11"); // suppress compiler optimizations
  char buf[10] = {0};
  ASSERT_FORTIFY(send(0, buf, data_len, 0));
}

TEST_F(DEATHTEST, FD_ISSET_fortified) {
#if defined(__BIONIC__) // glibc catches this at compile-time.
  fd_set set;
  memset(&set, 0, sizeof(set));
  ASSERT_FORTIFY(FD_ISSET(-1, &set));
#endif
}

TEST_F(DEATHTEST, FD_ISSET_2_fortified) {
  char buf[1];
  fd_set* set = (fd_set*) buf;
  ASSERT_FORTIFY(FD_ISSET(0, set));
}

TEST_F(DEATHTEST, getcwd_fortified) {
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  ASSERT_FORTIFY(getcwd(buf, ct));
}

TEST_F(DEATHTEST, pread_fortified) {
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_FORTIFY(pread(fd, buf, ct, 0));
  close(fd);
}

TEST_F(DEATHTEST, pread64_fortified) {
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_FORTIFY(pread64(fd, buf, ct, 0));
  close(fd);
}

TEST_F(DEATHTEST, pwrite_fortified) {
  char buf[1] = {0};
  size_t ct = atoi("2"); // prevent optimizations
  int fd = open("/dev/null", O_WRONLY);
  ASSERT_FORTIFY(pwrite(fd, buf, ct, 0));
  close(fd);
}

TEST_F(DEATHTEST, pwrite64_fortified) {
  char buf[1] = {0};
  size_t ct = atoi("2"); // prevent optimizations
  int fd = open("/dev/null", O_WRONLY);
  ASSERT_FORTIFY(pwrite64(fd, buf, ct, 0));
  close(fd);
}

TEST_F(DEATHTEST, read_fortified) {
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_FORTIFY(read(fd, buf, ct));
  close(fd);
}

TEST_F(DEATHTEST, write_fortified) {
  char buf[1] = {0};
  size_t ct = atoi("2"); // prevent optimizations
  int fd = open("/dev/null", O_WRONLY);
  ASSERT_EXIT(write(fd, buf, ct), testing::KilledBySignal(SIGABRT), "");
  close(fd);
}

TEST_F(DEATHTEST, fread_fortified) {
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  FILE* fp = fopen("/dev/null", "r");
  ASSERT_FORTIFY(fread(buf, 1, ct, fp));
  fclose(fp);
}

TEST_F(DEATHTEST, fwrite_fortified) {
  char buf[1] = {0};
  size_t ct = atoi("2"); // prevent optimizations
  FILE* fp = fopen("/dev/null", "w");
  ASSERT_FORTIFY(fwrite(buf, 1, ct, fp));
  fclose(fp);
}

TEST_F(DEATHTEST, readlink_fortified) {
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  ASSERT_FORTIFY(readlink("/dev/null", buf, ct));
}

TEST_F(DEATHTEST, readlinkat_fortified) {
  char buf[1];
  size_t ct = atoi("2"); // prevent optimizations
  ASSERT_FORTIFY(readlinkat(AT_FDCWD, "/dev/null", buf, ct));
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

TEST_F(DEATHTEST, poll_fortified) {
  nfds_t fd_count = atoi("2"); // suppress compiler optimizations
  pollfd buf[1] = {{0, POLLIN, 0}};
  // Set timeout to zero to prevent waiting in poll when fortify test fails.
  ASSERT_FORTIFY(poll(buf, fd_count, 0));
}

TEST_F(DEATHTEST, ppoll_fortified) {
  nfds_t fd_count = atoi("2"); // suppress compiler optimizations
  pollfd buf[1] = {{0, POLLIN, 0}};
  // Set timeout to zero to prevent waiting in ppoll when fortify test fails.
  timespec timeout;
  timeout.tv_sec = timeout.tv_nsec = 0;
  ASSERT_FORTIFY(ppoll(buf, fd_count, &timeout, NULL));
}
