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

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

#include <string>
#include <thread>
#include <vector>

#include <android-base/file.h>
#include <android-base/silent_death_test.h>
#include <android-base/strings.h>
#include <android-base/test_utils.h>
#include <android-base/unique_fd.h>

#include "utils.h"

// This #include is actually a test too. We have to duplicate the
// definitions of the RENAME_ constants because <linux/fs.h> also contains
// pollution such as BLOCK_SIZE which conflicts with lots of user code.
// Important to check that we have matching definitions.
// There's no _MAX to test that we have all the constants, sadly.
#include <linux/fs.h>

#if defined(NOFORTIFY)
#define STDIO_TEST stdio_nofortify
#define STDIO_DEATHTEST stdio_nofortify_DeathTest
#else
#define STDIO_TEST stdio
#define STDIO_DEATHTEST stdio_DeathTest
#endif

using namespace std::string_literals;

using stdio_DeathTest = SilentDeathTest;
using stdio_nofortify_DeathTest = SilentDeathTest;

static void SetFileTo(const char* path, const char* content) {
  FILE* fp;
  ASSERT_NE(nullptr, fp = fopen(path, "w"));
  ASSERT_NE(EOF, fputs(content, fp));
  ASSERT_EQ(0, fclose(fp));
}

static void AssertFileIs(const char* path, const char* expected) {
  FILE* fp;
  ASSERT_NE(nullptr, fp = fopen(path, "r"));
  char* line = nullptr;
  size_t length;
  ASSERT_NE(EOF, getline(&line, &length, fp));
  ASSERT_EQ(0, fclose(fp));
  ASSERT_STREQ(expected, line);
  free(line);
}

static void AssertFileIs(FILE* fp, const char* expected, bool is_fmemopen = false) {
  rewind(fp);

  char line[1024];
  memset(line, 0xff, sizeof(line));
  ASSERT_EQ(line, fgets(line, sizeof(line), fp));
  ASSERT_STREQ(expected, line);

  if (is_fmemopen) {
    // fmemopen appends a trailing NUL byte, which probably shouldn't show up as an
    // extra empty line, but does on every C library I tested...
    ASSERT_EQ(line, fgets(line, sizeof(line), fp));
    ASSERT_STREQ("", line);
  }

  // Make sure there isn't anything else in the file.
  ASSERT_EQ(nullptr, fgets(line, sizeof(line), fp)) << "junk at end of file: " << line;
}

#define EXPECT_SNPRINTF_N(expected, n, fmt, ...)                        \
  {                                                                     \
    char buf[BUFSIZ];                                                   \
    int w = snprintf(buf, sizeof(buf), fmt __VA_OPT__(, ) __VA_ARGS__); \
    EXPECT_EQ(n, w);                                                    \
    EXPECT_STREQ(expected, buf);                                        \
  }

#define EXPECT_SNPRINTF(expected, fmt, ...) \
  EXPECT_SNPRINTF_N(expected, static_cast<int>(strlen(expected)), fmt __VA_OPT__(, ) __VA_ARGS__)

#define EXPECT_SWPRINTF_N(expected, n, fmt, ...)                        \
  {                                                                     \
    wchar_t buf[BUFSIZ];                                                \
    int w = swprintf(buf, sizeof(buf), fmt __VA_OPT__(, ) __VA_ARGS__); \
    EXPECT_EQ(n, w);                                                    \
    EXPECT_EQ(std::wstring(expected), std::wstring(buf, w));            \
  }
#define EXPECT_SWPRINTF(expected, fmt, ...) \
  EXPECT_SWPRINTF_N(expected, static_cast<int>(wcslen(expected)), fmt __VA_OPT__(, ) __VA_ARGS__)

TEST(STDIO_TEST, flockfile_18208568_stderr) {
  // Check that we have a _recursive_ mutex for flockfile.
  flockfile(stderr);
  feof(stderr); // We don't care about the result, but this needs to take the lock.
  funlockfile(stderr);
}

TEST(STDIO_TEST, flockfile_18208568_regular) {
  // We never had a bug for streams other than stdin/stdout/stderr, but test anyway.
  FILE* fp = fopen("/dev/null", "w");
  ASSERT_TRUE(fp != nullptr);
  flockfile(fp);
  feof(fp);
  funlockfile(fp);
  fclose(fp);
}

TEST(STDIO_TEST, tmpfile_fileno_fprintf_rewind_fgets) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

  int fd = fileno(fp);
  ASSERT_NE(fd, -1);

  struct stat sb;
  int rc = fstat(fd, &sb);
  ASSERT_NE(rc, -1);
  ASSERT_EQ(sb.st_mode & 0777, 0600U);

  rc = fprintf(fp, "hello\n");
  ASSERT_EQ(rc, 6);

  AssertFileIs(fp, "hello\n");
  fclose(fp);
}

TEST(STDIO_TEST, tmpfile64) {
  FILE* fp = tmpfile64();
  ASSERT_TRUE(fp != nullptr);
  fclose(fp);
}

TEST(STDIO_TEST, tmpfile_TMPDIR) {
  TemporaryDir td;
  setenv("TMPDIR", td.path, 1);

  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

  std::string fd_path = android::base::StringPrintf("/proc/self/fd/%d", fileno(fp));
  char path[PATH_MAX];
  ASSERT_GT(readlink(fd_path.c_str(), path, sizeof(path)), 0);
  // $TMPDIR influenced where our temporary file ended up?
  ASSERT_TRUE(android::base::StartsWith(path, td.path)) << path;
  // And we used O_TMPFILE, right?
  ASSERT_TRUE(android::base::EndsWith(path, " (deleted)")) << path;
}

TEST(STDIO_TEST, dprintf) {
  TemporaryFile tf;

  int rc = dprintf(tf.fd, "hello\n");
  ASSERT_EQ(rc, 6);

  lseek(tf.fd, 0, SEEK_SET);
  FILE* tfile = fdopen(tf.fd, "r");
  ASSERT_TRUE(tfile != nullptr);

  AssertFileIs(tfile, "hello\n");
  fclose(tfile);
}

TEST(STDIO_TEST, getdelim) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

  const char* line_written = "This  is a test";
  int rc = fprintf(fp, "%s", line_written);
  ASSERT_EQ(rc, static_cast<int>(strlen(line_written)));

  rewind(fp);

  char* word_read = nullptr;
  size_t allocated_length = 0;

  const char* expected[] = { "This ", " ", "is ", "a ", "test" };
  for (size_t i = 0; i < 5; ++i) {
    ASSERT_FALSE(feof(fp));
    ASSERT_EQ(getdelim(&word_read, &allocated_length, ' ', fp), static_cast<int>(strlen(expected[i])));
    ASSERT_GE(allocated_length, strlen(expected[i]));
    ASSERT_STREQ(expected[i], word_read);
  }
  // The last read should have set the end-of-file indicator for the stream.
  ASSERT_TRUE(feof(fp));
  clearerr(fp);

  // getdelim returns -1 but doesn't set errno if we're already at EOF.
  // It should set the end-of-file indicator for the stream, though.
  errno = 0;
  ASSERT_EQ(getdelim(&word_read, &allocated_length, ' ', fp), -1);
  ASSERT_ERRNO(0);
  ASSERT_TRUE(feof(fp));

  free(word_read);
  fclose(fp);
}

TEST(STDIO_TEST, getdelim_invalid) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

  char* buffer = nullptr;
  size_t buffer_length = 0;

  // The first argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getdelim(nullptr, &buffer_length, ' ', fp), -1);
  ASSERT_ERRNO(EINVAL);

  // The second argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getdelim(&buffer, nullptr, ' ', fp), -1);
  ASSERT_ERRNO(EINVAL);
  fclose(fp);
#pragma clang diagnostic pop
}

TEST(STDIO_TEST, getdelim_directory) {
  FILE* fp = fopen("/proc", "r");
  ASSERT_TRUE(fp != nullptr);
  char* word_read;
  size_t allocated_length;
  ASSERT_EQ(-1, getdelim(&word_read, &allocated_length, ' ', fp));
  fclose(fp);
}

TEST(STDIO_TEST, getline) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

  const char* line_written = "This is a test for getline\n";
  const size_t line_count = 5;

  for (size_t i = 0; i < line_count; ++i) {
    int rc = fprintf(fp, "%s", line_written);
    ASSERT_EQ(rc, static_cast<int>(strlen(line_written)));
  }

  rewind(fp);

  char* line_read = nullptr;
  size_t allocated_length = 0;

  size_t read_line_count = 0;
  ssize_t read_char_count;
  while ((read_char_count = getline(&line_read, &allocated_length, fp)) != -1) {
    ASSERT_EQ(read_char_count, static_cast<int>(strlen(line_written)));
    ASSERT_GE(allocated_length, strlen(line_written));
    ASSERT_STREQ(line_written, line_read);
    ++read_line_count;
  }
  ASSERT_EQ(read_line_count, line_count);

  // The last read should have set the end-of-file indicator for the stream.
  ASSERT_TRUE(feof(fp));
  clearerr(fp);

  // getline returns -1 but doesn't set errno if we're already at EOF.
  // It should set the end-of-file indicator for the stream, though.
  errno = 0;
  ASSERT_EQ(getline(&line_read, &allocated_length, fp), -1);
  ASSERT_ERRNO(0);
  ASSERT_TRUE(feof(fp));

  free(line_read);
  fclose(fp);
}

TEST(STDIO_TEST, getline_invalid) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

  char* buffer = nullptr;
  size_t buffer_length = 0;

  // The first argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getline(nullptr, &buffer_length, fp), -1);
  ASSERT_ERRNO(EINVAL);

  // The second argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getline(&buffer, nullptr, fp), -1);
  ASSERT_ERRNO(EINVAL);
  fclose(fp);
#pragma clang diagnostic pop
}

TEST(STDIO_TEST, printf_ssize_t) {
  // http://b/8253769
  ASSERT_EQ(sizeof(ssize_t), sizeof(long int));
  ASSERT_EQ(sizeof(ssize_t), sizeof(size_t));
  // For our 32-bit ABI, we had a ssize_t definition that confuses GCC into saying:
  // error: format '%zd' expects argument of type 'signed size_t',
  //     but argument 4 has type 'ssize_t {aka long int}' [-Werror=format]
  ssize_t v = 1;
  EXPECT_SNPRINTF("1", "%zd", v);
  EXPECT_SWPRINTF(L"1", L"%zd", v);
}

// https://code.google.com/p/android/issues/detail?id=64886
TEST(STDIO_TEST, snprintf_a) {
  EXPECT_SNPRINTF("<0x1.3831e147ae148p+13>", "<%a>", 9990.235);
}

// https://code.google.com/p/android/issues/detail?id=64886
TEST(STDIO_TEST, swprintf_a) {
  EXPECT_SWPRINTF(L"<0x1.3831e147ae148p+13>", L"<%a>", 9990.235);
}

// http://b/152588929
TEST(STDIO_TEST, snprintf_La) {
#if defined(__LP64__)
  union {
    uint64_t a[2];
    long double v;
  } u;

  u.a[0] = UINT64_C(0x9b9b9b9b9b9b9b9b);
  u.a[1] = UINT64_C(0xdfdfdfdfdfdfdfdf);
  EXPECT_SNPRINTF("<-0x1.dfdfdfdfdfdf9b9b9b9b9b9b9b9bp+8160>", "<%La>", u.v);

  u.a[0] = UINT64_C(0xffffffffffffffff);
  u.a[1] = UINT64_C(0x7ffeffffffffffff);
  EXPECT_SNPRINTF("<0x1.ffffffffffffffffffffffffffffp+16383>", "<%La>", u.v);

  u.a[0] = UINT64_C(0x0000000000000000);
  u.a[1] = UINT64_C(0x0000000000000000);
  EXPECT_SNPRINTF("<0x0p+0>", "<%La>", u.v);
#else
  GTEST_SKIP() << "no ld128";
#endif
}

// http://b/152588929
TEST(STDIO_TEST, swprintf_La) {
#if defined(__LP64__)
  union {
    uint64_t a[2];
    long double v;
  } u;

  u.a[0] = UINT64_C(0x9b9b9b9b9b9b9b9b);
  u.a[1] = UINT64_C(0xdfdfdfdfdfdfdfdf);
  EXPECT_SWPRINTF(L"<-0x1.dfdfdfdfdfdf9b9b9b9b9b9b9b9bp+8160>", L"<%La>", u.v);

  u.a[0] = UINT64_C(0xffffffffffffffff);
  u.a[1] = UINT64_C(0x7ffeffffffffffff);
  EXPECT_SWPRINTF(L"<0x1.ffffffffffffffffffffffffffffp+16383>", L"<%La>", u.v);

  u.a[0] = UINT64_C(0x0000000000000000);
  u.a[1] = UINT64_C(0x0000000000000000);
  EXPECT_SWPRINTF(L"<0x0p+0>", L"<%La>", u.v);
#else
  GTEST_SKIP() << "no ld128";
#endif
}

TEST(STDIO_TEST, snprintf_lc) {
  wint_t wc = L'a';
  EXPECT_SNPRINTF("<a>", "<%lc>", wc);
}

TEST(STDIO_TEST, swprintf_lc) {
  wint_t wc = L'a';
  EXPECT_SWPRINTF(L"<a>", L"<%lc>", wc);
}

TEST(STDIO_TEST, snprintf_C) {  // Synonym for %lc.
  wchar_t wc = L'a';
  EXPECT_SNPRINTF("<a>", "<%C>", wc);
}

TEST(STDIO_TEST, swprintf_C) {  // Synonym for %lc.
  wchar_t wc = L'a';
  EXPECT_SWPRINTF(L"<a>", L"<%C>", wc);
}

TEST(STDIO_TEST, snprintf_ls_null) {
  EXPECT_SNPRINTF("<(null)>", "<%ls>", static_cast<wchar_t*>(nullptr));
}

TEST(STDIO_TEST, swprintf_ls_null) {
  EXPECT_SWPRINTF(L"<(null)>", L"<%ls>", static_cast<wchar_t*>(nullptr));
}

TEST(STDIO_TEST, snprintf_ls) {
  static const wchar_t chars[] = L"Hello\u0666 World";
  EXPECT_SNPRINTF("<Hello\xd9\xa6 World>", "<%ls>", chars);
}

TEST(STDIO_TEST, swprintf_ls) {
  static const wchar_t chars[] = L"Hello\u0666 World";
  EXPECT_SWPRINTF(L"<Hello\u0666 World>", L"<%ls>", chars);
}

TEST(STDIO_TEST, snprintf_S_nullptr) {  // Synonym for %ls.
  EXPECT_SNPRINTF("<(null)>", "<%S>", static_cast<wchar_t*>(nullptr));
}

TEST(STDIO_TEST, swprintf_S_nullptr) {  // Synonym for %ls.
  EXPECT_SWPRINTF(L"<(null)>", L"<%S>", static_cast<wchar_t*>(nullptr));
}

TEST(STDIO_TEST, snprintf_S) { // Synonym for %ls.
  static const wchar_t chars[] = L"Hello\u0666 World";
  EXPECT_SNPRINTF("<Hello\xd9\xa6 World>", "<%S>", chars);
}

TEST(STDIO_TEST, swprintf_S) {  // Synonym for %ls.
  static const wchar_t chars[] = L"Hello\u0666 World";
  EXPECT_SWPRINTF(L"<Hello\u0666 World>", L"<%S>", chars);
}

TEST_F(STDIO_DEATHTEST, snprintf_n) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
  // http://b/14492135 and http://b/31832608.
  char buf[32];
  int i = 1234;
  EXPECT_DEATH(snprintf(buf, sizeof(buf), "a %n b", &i), "%n not allowed on Android");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "glibc does allow %n";
#endif
}

TEST_F(STDIO_DEATHTEST, swprintf_n) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
  // http://b/14492135 and http://b/31832608.
  wchar_t buf[32];
  int i = 1234;
  EXPECT_DEATH(swprintf(buf, sizeof(buf), L"a %n b", &i), "%n not allowed on Android");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "glibc does allow %n";
#endif
}

TEST(STDIO_TEST, snprintf_measure) {
  char buf[1] = {'x'};
  ASSERT_EQ(11, snprintf(buf, 0, "Hello %s", "world"));
  ASSERT_EQ('x', buf[0]);
}

// Unlike snprintf(), you *can't* use swprintf() to measure.
TEST(STDIO_TEST, swprintf_measure) {
  wchar_t buf[1] = {L'x'};
  ASSERT_EQ(-1, swprintf(buf, 0, L"Hello %S", L"world"));
  ASSERT_EQ(L'x', buf[0]);
}

TEST(STDIO_TEST, snprintf_smoke) {
  EXPECT_SNPRINTF("a", "a");
  EXPECT_SNPRINTF("%", "%%");
  EXPECT_SNPRINTF("01234", "01234");
  EXPECT_SNPRINTF("a01234b", "a%sb", "01234");

  EXPECT_SNPRINTF("a(null)b", "a%sb", static_cast<char*>(nullptr));
  EXPECT_SNPRINTF("aabbcc", "aa%scc", "bb");
  EXPECT_SNPRINTF("abc", "a%cc", 'b');
  EXPECT_SNPRINTF("a1234b", "a%db", 1234);
  EXPECT_SNPRINTF("a-8123b", "a%db", -8123);
  EXPECT_SNPRINTF("a16b", "a%hdb", static_cast<short>(0x7fff0010));
  EXPECT_SNPRINTF("a16b", "a%hhdb", static_cast<char>(0x7fffff10));
  EXPECT_SNPRINTF("a68719476736b", "a%lldb", 0x1000000000LL);
  EXPECT_SNPRINTF("a70000b", "a%ldb", 70000L);
  EXPECT_SNPRINTF("a0xb0001234b", "a%pb", reinterpret_cast<void*>(0xb0001234));
  EXPECT_SNPRINTF("a12abz", "a%xz", 0x12ab);
  EXPECT_SNPRINTF("a12ABz", "a%Xz", 0x12ab);
  EXPECT_SNPRINTF("a00123456z", "a%08xz", 0x123456);
  EXPECT_SNPRINTF("a 1234z", "a%5dz", 1234);
  EXPECT_SNPRINTF("a01234z", "a%05dz", 1234);
  EXPECT_SNPRINTF("a    1234z", "a%8dz", 1234);
  EXPECT_SNPRINTF("a1234    z", "a%-8dz", 1234);
  EXPECT_SNPRINTF("Aabcdef     Z", "A%-11sZ", "abcdef");
  EXPECT_SNPRINTF("Ahello:1234Z", "A%s:%dZ", "hello", 1234);
  EXPECT_SNPRINTF("a005:5:05z", "a%03d:%d:%02dz", 5, 5, 5);

#if defined(__BIONIC__)
  EXPECT_SNPRINTF("a5,0x0z", "a%d,%pz", 5, static_cast<void*>(nullptr));
#else // __BIONIC__
  EXPECT_SNPRINTF("a5,(nil)z", "a%d,%pz", 5, static_cast<void*>(nullptr));
#endif // __BIONIC__

  EXPECT_SNPRINTF("a68719476736,6,7,8z", "a%lld,%d,%d,%dz", 0x1000000000LL, 6, 7, 8);

  EXPECT_SNPRINTF("a_1.230000_b", "a_%f_b", 1.23f);
  EXPECT_SNPRINTF("a_3.14_b", "a_%g_b", 3.14);
  EXPECT_SNPRINTF("print_me_twice print_me_twice", "%1$s %1$s", "print_me_twice");
}

TEST(STDIO_TEST, swprintf_smoke) {
  EXPECT_SWPRINTF(L"a", L"a");
  EXPECT_SWPRINTF(L"%", L"%%");
  EXPECT_SWPRINTF(L"01234", L"01234");
  EXPECT_SWPRINTF(L"a01234b", L"a%sb", "01234");

  EXPECT_SWPRINTF(L"a(null)b", L"a%sb", static_cast<char*>(nullptr));
  EXPECT_SWPRINTF(L"aabbcc", L"aa%scc", "bb");
  EXPECT_SWPRINTF(L"abc", L"a%cc", 'b');
  EXPECT_SWPRINTF(L"a1234b", L"a%db", 1234);
  EXPECT_SWPRINTF(L"a-8123b", L"a%db", -8123);
  EXPECT_SWPRINTF(L"a16b", L"a%hdb", static_cast<short>(0x7fff0010));
  EXPECT_SWPRINTF(L"a16b", L"a%hhdb", static_cast<char>(0x7fffff10));
  EXPECT_SWPRINTF(L"a68719476736b", L"a%lldb", 0x1000000000LL);
  EXPECT_SWPRINTF(L"a70000b", L"a%ldb", 70000L);
  EXPECT_SWPRINTF(L"a0xb0001234b", L"a%pb", reinterpret_cast<void*>(0xb0001234));
  EXPECT_SWPRINTF(L"a12abz", L"a%xz", 0x12ab);
  EXPECT_SWPRINTF(L"a12ABz", L"a%Xz", 0x12ab);
  EXPECT_SWPRINTF(L"a00123456z", L"a%08xz", 0x123456);
  EXPECT_SWPRINTF(L"a 1234z", L"a%5dz", 1234);
  EXPECT_SWPRINTF(L"a01234z", L"a%05dz", 1234);
  EXPECT_SWPRINTF(L"a    1234z", L"a%8dz", 1234);
  EXPECT_SWPRINTF(L"a1234    z", L"a%-8dz", 1234);
  EXPECT_SWPRINTF(L"Aabcdef     Z", L"A%-11sZ", "abcdef");
  EXPECT_SWPRINTF(L"Ahello:1234Z", L"A%s:%dZ", "hello", 1234);
  EXPECT_SWPRINTF(L"a005:5:05z", L"a%03d:%d:%02dz", 5, 5, 5);

#if defined(__BIONIC__)
  EXPECT_SWPRINTF(L"a5,0x0z", L"a%d,%pz", 5, static_cast<void*>(nullptr));
#else   // __BIONIC__
  EXPECT_SWPRINTF(L"a5,(nil)z", L"a%d,%pz", 5, static_cast<void*>(nullptr));
#endif  // __BIONIC__

  EXPECT_SWPRINTF(L"a68719476736,6,7,8z", L"a%lld,%d,%d,%dz", 0x1000000000LL, 6, 7, 8);

  EXPECT_SWPRINTF(L"a_1.230000_b", L"a_%f_b", 1.23f);
  EXPECT_SWPRINTF(L"a_3.14_b", L"a_%g_b", 3.14);
  EXPECT_SWPRINTF(L"print_me_twice print_me_twice", L"%1$s %1$s", "print_me_twice");
}

template <typename T>
static void CheckInfNan(int snprintf_fn(T*, size_t, const T*, ...),
                        int sscanf_fn(const T*, const T*, ...),
                        const T* fmt_string, const T* fmt, const T* fmt_plus,
                        const T* minus_inf, const T* inf_, const T* plus_inf,
                        const T* minus_nan, const T* nan_, const T* plus_nan) {
  T buf[BUFSIZ];
  float f;

  // NaN.

  snprintf_fn(buf, sizeof(buf), fmt, nan(""));
  EXPECT_STREQ(nan_, buf) << fmt;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_TRUE(isnan(f));

  snprintf_fn(buf, sizeof(buf), fmt, -nan(""));
  EXPECT_STREQ(minus_nan, buf) << fmt;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_TRUE(isnan(f));

  snprintf_fn(buf, sizeof(buf), fmt_plus, nan(""));
  EXPECT_STREQ(plus_nan, buf) << fmt_plus;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_TRUE(isnan(f));

  snprintf_fn(buf, sizeof(buf), fmt_plus, -nan(""));
  EXPECT_STREQ(minus_nan, buf) << fmt_plus;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_TRUE(isnan(f));

  // Inf.

  snprintf_fn(buf, sizeof(buf), fmt, HUGE_VALF);
  EXPECT_STREQ(inf_, buf) << fmt;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_EQ(HUGE_VALF, f);

  snprintf_fn(buf, sizeof(buf), fmt, -HUGE_VALF);
  EXPECT_STREQ(minus_inf, buf) << fmt;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_EQ(-HUGE_VALF, f);

  snprintf_fn(buf, sizeof(buf), fmt_plus, HUGE_VALF);
  EXPECT_STREQ(plus_inf, buf) << fmt_plus;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_EQ(HUGE_VALF, f);

  snprintf_fn(buf, sizeof(buf), fmt_plus, -HUGE_VALF);
  EXPECT_STREQ(minus_inf, buf) << fmt_plus;
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f));
  EXPECT_EQ(-HUGE_VALF, f);

  // Check case-insensitivity.
  snprintf_fn(buf, sizeof(buf), fmt_string, "[InFiNiTy]");
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f)) << buf;
  EXPECT_EQ(HUGE_VALF, f);
  snprintf_fn(buf, sizeof(buf), fmt_string, "[NaN]");
  EXPECT_EQ(1, sscanf_fn(buf, fmt, &f)) << buf;
  EXPECT_TRUE(isnan(f));
}

TEST(STDIO_TEST, snprintf_sscanf_inf_nan) {
  CheckInfNan(snprintf, sscanf, "%s",
              "[%a]", "[%+a]",
              "[-inf]", "[inf]", "[+inf]",
              "[-nan]", "[nan]", "[+nan]");
  CheckInfNan(snprintf, sscanf, "%s",
              "[%A]", "[%+A]",
              "[-INF]", "[INF]", "[+INF]",
              "[-NAN]", "[NAN]", "[+NAN]");
  CheckInfNan(snprintf, sscanf, "%s",
              "[%e]", "[%+e]",
              "[-inf]", "[inf]", "[+inf]",
              "[-nan]", "[nan]", "[+nan]");
  CheckInfNan(snprintf, sscanf, "%s",
              "[%E]", "[%+E]",
              "[-INF]", "[INF]", "[+INF]",
              "[-NAN]", "[NAN]", "[+NAN]");
  CheckInfNan(snprintf, sscanf, "%s",
              "[%f]", "[%+f]",
              "[-inf]", "[inf]", "[+inf]",
              "[-nan]", "[nan]", "[+nan]");
  CheckInfNan(snprintf, sscanf, "%s",
              "[%F]", "[%+F]",
              "[-INF]", "[INF]", "[+INF]",
              "[-NAN]", "[NAN]", "[+NAN]");
  CheckInfNan(snprintf, sscanf, "%s",
              "[%g]", "[%+g]",
              "[-inf]", "[inf]", "[+inf]",
              "[-nan]", "[nan]", "[+nan]");
  CheckInfNan(snprintf, sscanf, "%s",
              "[%G]", "[%+G]",
              "[-INF]", "[INF]", "[+INF]",
              "[-NAN]", "[NAN]", "[+NAN]");
}

TEST(STDIO_TEST, swprintf_swscanf_inf_nan) {
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%a]", L"[%+a]",
              L"[-inf]", L"[inf]", L"[+inf]",
              L"[-nan]", L"[nan]", L"[+nan]");
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%A]", L"[%+A]",
              L"[-INF]", L"[INF]", L"[+INF]",
              L"[-NAN]", L"[NAN]", L"[+NAN]");
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%e]", L"[%+e]",
              L"[-inf]", L"[inf]", L"[+inf]",
              L"[-nan]", L"[nan]", L"[+nan]");
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%E]", L"[%+E]",
              L"[-INF]", L"[INF]", L"[+INF]",
              L"[-NAN]", L"[NAN]", L"[+NAN]");
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%f]", L"[%+f]",
              L"[-inf]", L"[inf]", L"[+inf]",
              L"[-nan]", L"[nan]", L"[+nan]");
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%F]", L"[%+F]",
              L"[-INF]", L"[INF]", L"[+INF]",
              L"[-NAN]", L"[NAN]", L"[+NAN]");
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%g]", L"[%+g]",
              L"[-inf]", L"[inf]", L"[+inf]",
              L"[-nan]", L"[nan]", L"[+nan]");
  CheckInfNan(swprintf, swscanf, L"%s",
              L"[%G]", L"[%+G]",
              L"[-INF]", L"[INF]", L"[+INF]",
              L"[-NAN]", L"[NAN]", L"[+NAN]");
}

TEST(STDIO_TEST, snprintf_jd_INTMAX_MAX) {
  EXPECT_SNPRINTF("9223372036854775807", "%jd", INTMAX_MAX);
}

TEST(STDIO_TEST, swprintf_jd_INTMAX_MAX) {
  EXPECT_SWPRINTF(L"9223372036854775807", L"%jd", INTMAX_MAX);
}

TEST(STDIO_TEST, snprintf_jd_INTMAX_MIN) {
  EXPECT_SNPRINTF("-9223372036854775808", "%jd", INTMAX_MIN);
}

TEST(STDIO_TEST, swprintf_jd_INTMAX_MIN) {
  EXPECT_SWPRINTF(L"-9223372036854775808", L"%jd", INTMAX_MIN);
}

TEST(STDIO_TEST, snprintf_ju_UINTMAX_MAX) {
  EXPECT_SNPRINTF("18446744073709551615", "%ju", UINTMAX_MAX);
}

TEST(STDIO_TEST, swprintf_ju_UINTMAX_MAX) {
  EXPECT_SWPRINTF(L"18446744073709551615", L"%ju", UINTMAX_MAX);
}

TEST(STDIO_TEST, snprintf_1$ju_UINTMAX_MAX) {
  EXPECT_SNPRINTF("18446744073709551615", "%1$ju", UINTMAX_MAX);
}

TEST(STDIO_TEST, swprintf_1$ju_UINTMAX_MAX) {
  EXPECT_SWPRINTF(L"18446744073709551615", L"%1$ju", UINTMAX_MAX);
}

TEST(STDIO_TEST, snprintf_d_INT_MAX) {
  EXPECT_SNPRINTF("2147483647", "%d", INT_MAX);
}

TEST(STDIO_TEST, swprintf_d_INT_MAX) {
  EXPECT_SWPRINTF(L"2147483647", L"%d", INT_MAX);
}

TEST(STDIO_TEST, snprintf_d_INT_MIN) {
  EXPECT_SNPRINTF("-2147483648", "%d", INT_MIN);
}

TEST(STDIO_TEST, swprintf_d_INT_MIN) {
  EXPECT_SWPRINTF(L"-2147483648", L"%d", INT_MIN);
}

TEST(STDIO_TEST, snprintf_ld_LONG_MAX) {
#if defined(__LP64__)
  EXPECT_SNPRINTF("9223372036854775807", "%ld", LONG_MAX);
#else
  EXPECT_SNPRINTF("2147483647", "%ld", LONG_MAX);
#endif
}

TEST(STDIO_TEST, swprintf_ld_LONG_MAX) {
#if defined(__LP64__)
  EXPECT_SWPRINTF(L"9223372036854775807", L"%ld", LONG_MAX);
#else
  EXPECT_SWPRINTF(L"2147483647", L"%ld", LONG_MAX);
#endif
}

TEST(STDIO_TEST, snprintf_ld_LONG_MIN) {
#if defined(__LP64__)
  EXPECT_SNPRINTF("-9223372036854775808", "%ld", LONG_MIN);
#else
  EXPECT_SNPRINTF("-2147483648", "%ld", LONG_MIN);
#endif
}

TEST(STDIO_TEST, swprintf_ld_LONG_MIN) {
#if defined(__LP64__)
  EXPECT_SWPRINTF(L"-9223372036854775808", L"%ld", LONG_MIN);
#else
  EXPECT_SWPRINTF(L"-2147483648", L"%ld", LONG_MIN);
#endif
}

TEST(STDIO_TEST, snprintf_lld_LLONG_MAX) {
  EXPECT_SNPRINTF("9223372036854775807", "%lld", LLONG_MAX);
}

TEST(STDIO_TEST, swprintf_lld_LLONG_MAX) {
  EXPECT_SWPRINTF(L"9223372036854775807", L"%lld", LLONG_MAX);
}

TEST(STDIO_TEST, snprintf_lld_LLONG_MIN) {
  EXPECT_SNPRINTF("-9223372036854775808", "%lld", LLONG_MIN);
}

TEST(STDIO_TEST, swprintf_lld_LLONG_MIN) {
  EXPECT_SWPRINTF(L"-9223372036854775808", L"%lld", LLONG_MIN);
}

TEST(STDIO_TEST, snprintf_o_UINT_MAX) {
  EXPECT_SNPRINTF("37777777777", "%o", UINT_MAX);
}

TEST(STDIO_TEST, swprintf_o_UINT_MAX) {
  EXPECT_SWPRINTF(L"37777777777", L"%o", UINT_MAX);
}

TEST(STDIO_TEST, snprintf_u_UINT_MAX) {
  EXPECT_SNPRINTF("4294967295", "%u", UINT_MAX);
}

TEST(STDIO_TEST, swprintf_u_UINT_MAX) {
  EXPECT_SWPRINTF(L"4294967295", L"%u", UINT_MAX);
}

TEST(STDIO_TEST, snprintf_x_UINT_MAX) {
  EXPECT_SNPRINTF("ffffffff", "%x", UINT_MAX);
}

TEST(STDIO_TEST, swprintf_x_UINT_MAX) {
  EXPECT_SWPRINTF(L"ffffffff", L"%x", UINT_MAX);
}

TEST(STDIO_TEST, snprintf_X_UINT_MAX) {
  EXPECT_SNPRINTF("FFFFFFFF", "%X", UINT_MAX);
}

TEST(STDIO_TEST, swprintf_X_UINT_MAX) {
  EXPECT_SWPRINTF(L"FFFFFFFF", L"%X", UINT_MAX);
}

TEST(STDIO_TEST, snprintf_e) {
  EXPECT_SNPRINTF("1.500000e+00", "%e", 1.5);
  EXPECT_SNPRINTF("1.500000e+00", "%Le", 1.5L);
}

TEST(STDIO_TEST, swprintf_e) {
  EXPECT_SWPRINTF(L"1.500000e+00", L"%e", 1.5);
  EXPECT_SWPRINTF(L"1.500000e+00", L"%Le", 1.5L);
}

TEST(STDIO_TEST, snprintf_negative_zero_5084292) {
  EXPECT_SNPRINTF("-0.000000e+00", "%e", -0.0);
  EXPECT_SNPRINTF("-0.000000E+00", "%E", -0.0);
  EXPECT_SNPRINTF("-0.000000", "%f", -0.0);
  EXPECT_SNPRINTF("-0.000000", "%F", -0.0);
  EXPECT_SNPRINTF("-0", "%g", -0.0);
  EXPECT_SNPRINTF("-0", "%G", -0.0);
  EXPECT_SNPRINTF("-0x0p+0", "%a", -0.0);
  EXPECT_SNPRINTF("-0X0P+0", "%A", -0.0);
}

TEST(STDIO_TEST, swprintf_negative_zero_5084292) {
  EXPECT_SWPRINTF(L"-0.000000e+00", L"%e", -0.0);
  EXPECT_SWPRINTF(L"-0.000000E+00", L"%E", -0.0);
  EXPECT_SWPRINTF(L"-0.000000", L"%f", -0.0);
  EXPECT_SWPRINTF(L"-0.000000", L"%F", -0.0);
  EXPECT_SWPRINTF(L"-0", L"%g", -0.0);
  EXPECT_SWPRINTF(L"-0", L"%G", -0.0);
  EXPECT_SWPRINTF(L"-0x0p+0", L"%a", -0.0);
  EXPECT_SWPRINTF(L"-0X0P+0", L"%A", -0.0);
}

TEST(STDIO_TEST, snprintf_utf8_15439554) {
  locale_t cloc = newlocale(LC_ALL, "C.UTF-8", nullptr);
  locale_t old_locale = uselocale(cloc);

  // http://b/15439554
  char buf[BUFSIZ];

  // 1-byte character.
  snprintf(buf, sizeof(buf), "%dx%d", 1, 2);
  EXPECT_STREQ("1x2", buf);
  // 2-byte character.
  snprintf(buf, sizeof(buf), "%d\xc2\xa2%d", 1, 2);
  EXPECT_STREQ("1¢2", buf);
  // 3-byte character.
  snprintf(buf, sizeof(buf), "%d\xe2\x82\xac%d", 1, 2);
  EXPECT_STREQ("1€2", buf);
  // 4-byte character.
  snprintf(buf, sizeof(buf), "%d\xf0\xa4\xad\xa2%d", 1, 2);
  EXPECT_STREQ("1𤭢2", buf);

  uselocale(old_locale);
  freelocale(cloc);
}

static void* snprintf_small_stack_fn(void*) {
  // Make life (realistically) hard for ourselves by allocating our own buffer for the result.
  char buf[PATH_MAX];
  snprintf(buf, sizeof(buf), "/proc/%d", getpid());
  return nullptr;
}

TEST(STDIO_TEST, snprintf_small_stack) {
  // Is it safe to call snprintf on a thread with a small stack?
  // (The snprintf implementation puts some pretty large buffers on the stack.)
  pthread_attr_t a;
  ASSERT_EQ(0, pthread_attr_init(&a));
  ASSERT_EQ(0, pthread_attr_setstacksize(&a, PTHREAD_STACK_MIN));

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &a, snprintf_small_stack_fn, nullptr));
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

TEST(STDIO_TEST, snprintf_asterisk_overflow) {
  char buf[128];
  ASSERT_EQ(5, snprintf(buf, sizeof(buf), "%.*s%c", 4, "hello world", '!'));
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%.*s%c", INT_MAX/2, "hello world", '!'));
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%.*s%c", INT_MAX-1, "hello world", '!'));
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%.*s%c", INT_MAX, "hello world", '!'));
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%.*s%c", -1, "hello world", '!'));

  // INT_MAX-1, INT_MAX, INT_MAX+1.
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%.2147483646s%c", "hello world", '!'));
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%.2147483647s%c", "hello world", '!'));
  ASSERT_EQ(-1, snprintf(buf, sizeof(buf), "%.2147483648s%c", "hello world", '!'));
  ASSERT_ERRNO(ENOMEM);
}

TEST(STDIO_TEST, swprintf_asterisk_overflow) {
  wchar_t buf[128];
  ASSERT_EQ(5, swprintf(buf, sizeof(buf), L"%.*s%c", 4, "hello world", '!'));
  ASSERT_EQ(12, swprintf(buf, sizeof(buf), L"%.*s%c", INT_MAX / 2, "hello world", '!'));
  ASSERT_EQ(12, swprintf(buf, sizeof(buf), L"%.*s%c", INT_MAX - 1, "hello world", '!'));
  ASSERT_EQ(12, swprintf(buf, sizeof(buf), L"%.*s%c", INT_MAX, "hello world", '!'));
  ASSERT_EQ(12, swprintf(buf, sizeof(buf), L"%.*s%c", -1, "hello world", '!'));

  // INT_MAX-1, INT_MAX, INT_MAX+1.
  ASSERT_EQ(12, swprintf(buf, sizeof(buf), L"%.2147483646s%c", "hello world", '!'));
  ASSERT_EQ(12, swprintf(buf, sizeof(buf), L"%.2147483647s%c", "hello world", '!'));
  ASSERT_EQ(-1, swprintf(buf, sizeof(buf), L"%.2147483648s%c", "hello world", '!'));
  ASSERT_ERRNO(ENOMEM);
}

// Inspired by https://github.com/landley/toybox/issues/163.
TEST(STDIO_TEST, printf_NULL) {
  char* null = nullptr;
  EXPECT_SNPRINTF("<(n>", "<%*.*s>", 2, 2, null);
  EXPECT_SNPRINTF("<(null)>", "<%*.*s>", 2, 8, null);
  EXPECT_SNPRINTF("<      (n>", "<%*.*s>", 8, 2, null);
  EXPECT_SNPRINTF("<  (null)>", "<%*.*s>", 8, 8, null);
}

TEST(STDIO_TEST, wprintf_NULL) {
  char* null = nullptr;
  EXPECT_SWPRINTF(L"<(n>", L"<%*.*s>", 2, 2, null);
  EXPECT_SWPRINTF(L"<(null)>", L"<%*.*s>", 2, 8, null);
  EXPECT_SWPRINTF(L"<      (n>", L"<%*.*s>", 8, 2, null);
  EXPECT_SWPRINTF(L"<  (null)>", L"<%*.*s>", 8, 8, null);
}

TEST(STDIO_TEST, fprintf) {
  TemporaryFile tf;

  FILE* tfile = fdopen(tf.fd, "r+");
  ASSERT_TRUE(tfile != nullptr);

  ASSERT_EQ(7, fprintf(tfile, "%d %s", 123, "abc"));
  AssertFileIs(tfile, "123 abc");
  fclose(tfile);
}

TEST(STDIO_TEST, fprintf_failures_7229520) {
  // http://b/7229520
  FILE* fp;
  int fd_rdonly = open("/dev/null", O_RDONLY);
  ASSERT_NE(-1, fd_rdonly);

  // Unbuffered case where the fprintf(3) itself fails.
  ASSERT_NE(nullptr, fp = tmpfile());
  setbuf(fp, nullptr);
  ASSERT_EQ(4, fprintf(fp, "epic"));
  ASSERT_NE(-1, dup2(fd_rdonly, fileno(fp)));
  ASSERT_EQ(-1, fprintf(fp, "fail"));
  ASSERT_EQ(0, fclose(fp));

  // Buffered case where we won't notice until the fclose(3).
  // It's likely this is what was actually seen in http://b/7229520,
  // and that expecting fprintf to fail is setting yourself up for
  // disappointment. Remember to check fclose(3)'s return value, kids!
  ASSERT_NE(nullptr, fp = tmpfile());
  ASSERT_EQ(4, fprintf(fp, "epic"));
  ASSERT_NE(-1, dup2(fd_rdonly, fileno(fp)));
  ASSERT_EQ(4, fprintf(fp, "fail"));
  ASSERT_EQ(-1, fclose(fp));
}

TEST(STDIO_TEST, popen_r) {
  FILE* fp = popen("cat /proc/version", "r");
  ASSERT_TRUE(fp != nullptr);

  char buf[16];
  char* s = fgets(buf, sizeof(buf), fp);
  buf[13] = '\0';
  ASSERT_STREQ("Linux version", s);

  ASSERT_EQ(0, pclose(fp));
}

TEST(STDIO_TEST, popen_socketpair) {
  FILE* fp = popen("cat", "r+");
  ASSERT_TRUE(fp != nullptr);

  fputs("hello\nworld\n", fp);
  fflush(fp);

  char buf[16];
  ASSERT_NE(nullptr, fgets(buf, sizeof(buf), fp));
  EXPECT_STREQ("hello\n", buf);
  ASSERT_NE(nullptr, fgets(buf, sizeof(buf), fp));
  EXPECT_STREQ("world\n", buf);

  ASSERT_EQ(0, pclose(fp));
}

TEST(STDIO_TEST, popen_socketpair_shutdown) {
  FILE* fp = popen("uniq -c", "r+");
  ASSERT_TRUE(fp != nullptr);

  fputs("a\na\na\na\nb\n", fp);
  fflush(fp);
  ASSERT_EQ(0, shutdown(fileno(fp), SHUT_WR));

  char buf[16];
  ASSERT_NE(nullptr, fgets(buf, sizeof(buf), fp));
  EXPECT_STREQ("      4 a\n", buf);
  ASSERT_NE(nullptr, fgets(buf, sizeof(buf), fp));
  EXPECT_STREQ("      1 b\n", buf);

  ASSERT_EQ(0, pclose(fp));
}

TEST(STDIO_TEST, popen_return_value_0) {
  FILE* fp = popen("true", "r");
  ASSERT_TRUE(fp != nullptr);
  int status = pclose(fp);
  EXPECT_TRUE(WIFEXITED(status));
  EXPECT_EQ(0, WEXITSTATUS(status));
}

TEST(STDIO_TEST, popen_return_value_1) {
  FILE* fp = popen("false", "r");
  ASSERT_TRUE(fp != nullptr);
  int status = pclose(fp);
  EXPECT_TRUE(WIFEXITED(status));
  EXPECT_EQ(1, WEXITSTATUS(status));
}

TEST(STDIO_TEST, popen_return_value_signal) {
  // Use a realtime signal to avoid creating a tombstone when running.
  std::string cmd = android::base::StringPrintf("kill -%d $$", SIGRTMIN);
  FILE* fp = popen(cmd.c_str(), "r");
  ASSERT_TRUE(fp != nullptr);
  int status = pclose(fp);
  EXPECT_TRUE(WIFSIGNALED(status));
  EXPECT_EQ(SIGRTMIN, WTERMSIG(status));
}

TEST(STDIO_TEST, getc) {
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_TRUE(fp != nullptr);
  ASSERT_EQ('L', getc(fp));
  ASSERT_EQ('i', getc(fp));
  ASSERT_EQ('n', getc(fp));
  ASSERT_EQ('u', getc(fp));
  ASSERT_EQ('x', getc(fp));
  fclose(fp);
}

TEST(STDIO_TEST, putc) {
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_TRUE(fp != nullptr);
  ASSERT_EQ(EOF, putc('x', fp));
  fclose(fp);
}

TEST(STDIO_TEST, sscanf_swscanf) {
  struct stuff {
    char s1[123];
    int i1, i2;
    char cs1[3];
    char s2[3];
    char c1;
    double d1;
    float f1;
    char s3[123];

    void Check() {
      EXPECT_STREQ("hello", s1);
      EXPECT_EQ(123, i1);
      EXPECT_EQ(456, i2);
      EXPECT_EQ('a', cs1[0]);
      EXPECT_EQ('b', cs1[1]);
      EXPECT_EQ('x', cs1[2]); // No terminating NUL.
      EXPECT_STREQ("AB", s2); // Terminating NUL.
      EXPECT_EQ('!', c1);
      EXPECT_DOUBLE_EQ(1.23, d1);
      EXPECT_FLOAT_EQ(9.0f, f1);
      EXPECT_STREQ("world", s3);
    }
  } s;

  memset(&s, 'x', sizeof(s));
  ASSERT_EQ(9, sscanf("  hello 123 456abAB! 1.23 0x1.2p3 world",
                      "%s %i%i%2c%[A-Z]%c %lf %f %s",
                      s.s1, &s.i1, &s.i2, s.cs1, s.s2, &s.c1, &s.d1, &s.f1, s.s3));
  s.Check();

  memset(&s, 'x', sizeof(s));
  ASSERT_EQ(9, swscanf(L"  hello 123 456abAB! 1.23 0x1.2p3 world",
                       L"%s %i%i%2c%[A-Z]%c %lf %f %s",
                       s.s1, &s.i1, &s.i2, s.cs1, s.s2, &s.c1, &s.d1, &s.f1, s.s3));
  s.Check();
}

template <typename T>
static void CheckScanf(int sscanf_fn(const T*, const T*, ...),
                       const T* input, const T* fmt,
                       int expected_count, const char* expected_string) {
  char buf[256] = {};
  ASSERT_EQ(expected_count, sscanf_fn(input, fmt, &buf)) << fmt;
  ASSERT_STREQ(expected_string, buf) << fmt;
}

TEST(STDIO_TEST, sscanf_ccl) {
  // `abc` is just those characters.
  CheckScanf(sscanf, "abcd", "%[abc]", 1, "abc");
  // `a-c` is the range 'a' .. 'c'.
  CheckScanf(sscanf, "abcd", "%[a-c]", 1, "abc");
  CheckScanf(sscanf, "-d", "%[a-c]", 0, "");
  CheckScanf(sscanf, "ac-bAd", "%[a--c]", 1, "ac-bA");
  // `a-c-e` is equivalent to `a-e`.
  CheckScanf(sscanf, "abcdefg", "%[a-c-e]", 1, "abcde");
  // `e-a` is equivalent to `ae-` (because 'e' > 'a').
  CheckScanf(sscanf, "-a-e-b", "%[e-a]", 1, "-a-e-");
  // An initial '^' negates the set.
  CheckScanf(sscanf, "abcde", "%[^d]", 1, "abc");
  CheckScanf(sscanf, "abcdefgh", "%[^c-d]", 1, "ab");
  CheckScanf(sscanf, "hgfedcba", "%[^c-d]", 1, "hgfe");
  // The first character may be ']' or '-' without being special.
  CheckScanf(sscanf, "[[]]x", "%[][]", 1, "[[]]");
  CheckScanf(sscanf, "-a-x", "%[-a]", 1, "-a-");
  // The last character may be '-' without being special.
  CheckScanf(sscanf, "-a-x", "%[a-]", 1, "-a-");
  // X--Y is [X--] + Y, not [X--] + [--Y] (a bug in my initial implementation).
  CheckScanf(sscanf, "+,-/.", "%[+--/]", 1, "+,-/");
}

TEST(STDIO_TEST, swscanf_ccl) {
  // `abc` is just those characters.
  CheckScanf(swscanf, L"abcd", L"%[abc]", 1, "abc");
  // `a-c` is the range 'a' .. 'c'.
  CheckScanf(swscanf, L"abcd", L"%[a-c]", 1, "abc");
  CheckScanf(swscanf, L"-d", L"%[a-c]", 0, "");
  CheckScanf(swscanf, L"ac-bAd", L"%[a--c]", 1, "ac-bA");
  // `a-c-e` is equivalent to `a-e`.
  CheckScanf(swscanf, L"abcdefg", L"%[a-c-e]", 1, "abcde");
  // `e-a` is equivalent to `ae-` (because 'e' > 'a').
  CheckScanf(swscanf, L"-a-e-b", L"%[e-a]", 1, "-a-e-");
  // An initial '^' negates the set.
  CheckScanf(swscanf, L"abcde", L"%[^d]", 1, "abc");
  CheckScanf(swscanf, L"abcdefgh", L"%[^c-d]", 1, "ab");
  CheckScanf(swscanf, L"hgfedcba", L"%[^c-d]", 1, "hgfe");
  // The first character may be ']' or '-' without being special.
  CheckScanf(swscanf, L"[[]]x", L"%[][]", 1, "[[]]");
  CheckScanf(swscanf, L"-a-x", L"%[-a]", 1, "-a-");
  // The last character may be '-' without being special.
  CheckScanf(swscanf, L"-a-x", L"%[a-]", 1, "-a-");
  // X--Y is [X--] + Y, not [X--] + [--Y] (a bug in my initial implementation).
  CheckScanf(swscanf, L"+,-/.", L"%[+--/]", 1, "+,-/");
}

template <typename T1, typename T2>
static void CheckScanfM(int sscanf_fn(const T1*, const T1*, ...),
                        const T1* input, const T1* fmt,
                        int expected_count, const T2* expected_string) {
  T2* result = nullptr;
  ASSERT_EQ(expected_count, sscanf_fn(input, fmt, &result)) << fmt;
  if (expected_string == nullptr) {
    ASSERT_EQ(nullptr, result);
  } else {
    ASSERT_STREQ(expected_string, result) << fmt;
  }
  free(result);
}

TEST(STDIO_TEST, sscanf_mc) {
  char* p1 = nullptr;
  char* p2 = nullptr;
  ASSERT_EQ(2, sscanf("hello", "%mc%mc", &p1, &p2));
  ASSERT_EQ('h', *p1);
  ASSERT_EQ('e', *p2);
  free(p1);
  free(p2);

  p1 = nullptr;
  ASSERT_EQ(1, sscanf("hello", "%4mc", &p1));
  ASSERT_EQ('h', p1[0]);
  ASSERT_EQ('e', p1[1]);
  ASSERT_EQ('l', p1[2]);
  ASSERT_EQ('l', p1[3]);
  free(p1);

  p1 = nullptr;
  ASSERT_EQ(1, sscanf("hello world", "%30mc", &p1));
  ASSERT_EQ('h', p1[0]);
  ASSERT_EQ('e', p1[1]);
  ASSERT_EQ('l', p1[2]);
  ASSERT_EQ('l', p1[3]);
  ASSERT_EQ('o', p1[4]);
  free(p1);
}

TEST(STDIO_TEST, sscanf_mlc) {
  // This is so useless that clang doesn't even believe it exists...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wformat-extra-args"

  wchar_t* p1 = nullptr;
  wchar_t* p2 = nullptr;
  ASSERT_EQ(2, sscanf("hello", "%mlc%mlc", &p1, &p2));
  ASSERT_EQ(L'h', *p1);
  ASSERT_EQ(L'e', *p2);
  free(p1);
  free(p2);

  p1 = nullptr;
  ASSERT_EQ(1, sscanf("hello", "%4mlc", &p1));
  ASSERT_EQ(L'h', p1[0]);
  ASSERT_EQ(L'e', p1[1]);
  ASSERT_EQ(L'l', p1[2]);
  ASSERT_EQ(L'l', p1[3]);
  free(p1);

  p1 = nullptr;
  ASSERT_EQ(1, sscanf("hello world", "%30mlc", &p1));
  ASSERT_EQ(L'h', p1[0]);
  ASSERT_EQ(L'e', p1[1]);
  ASSERT_EQ(L'l', p1[2]);
  ASSERT_EQ(L'l', p1[3]);
  ASSERT_EQ(L'o', p1[4]);
  free(p1);
#pragma clang diagnostic pop
}

TEST(STDIO_TEST, sscanf_ms) {
  CheckScanfM(sscanf, "hello", "%ms", 1, "hello");
  CheckScanfM(sscanf, "hello", "%4ms", 1, "hell");
  CheckScanfM(sscanf, "hello world", "%30ms", 1, "hello");
}

TEST(STDIO_TEST, sscanf_mls) {
  CheckScanfM(sscanf, "hello", "%mls", 1, L"hello");
  CheckScanfM(sscanf, "hello", "%4mls", 1, L"hell");
  CheckScanfM(sscanf, "hello world", "%30mls", 1, L"hello");
}

TEST(STDIO_TEST, sscanf_m_ccl) {
  CheckScanfM(sscanf, "hello", "%m[a-z]", 1, "hello");
  CheckScanfM(sscanf, "hello", "%4m[a-z]", 1, "hell");
  CheckScanfM(sscanf, "hello world", "%30m[a-z]", 1, "hello");
}

TEST(STDIO_TEST, sscanf_ml_ccl) {
  CheckScanfM(sscanf, "hello", "%ml[a-z]", 1, L"hello");
  CheckScanfM(sscanf, "hello", "%4ml[a-z]", 1, L"hell");
  CheckScanfM(sscanf, "hello world", "%30ml[a-z]", 1, L"hello");
}

TEST(STDIO_TEST, sscanf_ls) {
  wchar_t w[32] = {};
  ASSERT_EQ(1, sscanf("hello world", "%ls", w));
  ASSERT_EQ(L"hello", std::wstring(w));
}

TEST(STDIO_TEST, sscanf_ls_suppress) {
  ASSERT_EQ(0, sscanf("hello world", "%*ls %*ls"));
}

TEST(STDIO_TEST, sscanf_ls_n) {
  setlocale(LC_ALL, "C.UTF-8");
  wchar_t w[32] = {};
  int pos = 0;
  ASSERT_EQ(1, sscanf("\xc4\x80", "%ls%n", w, &pos));
  ASSERT_EQ(static_cast<wchar_t>(256), w[0]);
  ASSERT_EQ(2, pos);
}

TEST(STDIO_TEST, sscanf_ls_realloc) {
  // This is so useless that clang doesn't even believe it exists...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wformat-extra-args"
  wchar_t* p1 = nullptr;
  wchar_t* p2 = nullptr;
  ASSERT_EQ(2, sscanf("1234567890123456789012345678901234567890 world", "%mls %mls", &p1, &p2));
  ASSERT_EQ(L"1234567890123456789012345678901234567890", std::wstring(p1));
  ASSERT_EQ(L"world", std::wstring(p2));
#pragma clang diagnostic pop
}

// https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=202240
TEST(STDIO_TEST, scanf_wscanf_EOF) {
  EXPECT_EQ(0, sscanf("b", "ab"));
  EXPECT_EQ(EOF, sscanf("", "a"));
  EXPECT_EQ(0, swscanf(L"b", L"ab"));
  EXPECT_EQ(EOF, swscanf(L"", L"a"));
}

TEST(STDIO_TEST, scanf_invalid_UTF8) {
#if 0 // TODO: more tests invented during code review; no regressions, so fix later.
  char buf[BUFSIZ];
  wchar_t wbuf[BUFSIZ];

  memset(buf, 0, sizeof(buf));
  memset(wbuf, 0, sizeof(wbuf));
  EXPECT_EQ(0, sscanf("\xc0" " foo", "%ls %s", wbuf, buf));
#endif
}

TEST(STDIO_TEST, scanf_no_match_no_termination) {
  char buf[4] = "x";
  EXPECT_EQ(0, sscanf("d", "%[abc]", buf));
  EXPECT_EQ('x', buf[0]);
  EXPECT_EQ(0, swscanf(L"d", L"%[abc]", buf));
  EXPECT_EQ('x', buf[0]);

  wchar_t wbuf[4] = L"x";
  EXPECT_EQ(0, swscanf(L"d", L"%l[abc]", wbuf));
  EXPECT_EQ(L'x', wbuf[0]);

  EXPECT_EQ(EOF, sscanf("", "%s", buf));
  EXPECT_EQ('x', buf[0]);

  EXPECT_EQ(EOF, swscanf(L"", L"%ls", wbuf));
  EXPECT_EQ(L'x', wbuf[0]);
}

TEST(STDIO_TEST, scanf_wscanf_wide_character_class) {
#if 0 // TODO: more tests invented during code review; no regressions, so fix later.
  wchar_t buf[BUFSIZ];

  // A wide character shouldn't match an ASCII-only class for scanf or wscanf.
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, sscanf("xĀyz", "%l[xy]", buf));
  EXPECT_EQ(L"x"s, std::wstring(buf));
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, swscanf(L"xĀyz", L"%l[xy]", buf));
  EXPECT_EQ(L"x"s, std::wstring(buf));

  // Even if scanf has wide characters in a class, they won't match...
  // TODO: is that a bug?
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, sscanf("xĀyz", "%l[xĀy]", buf));
  EXPECT_EQ(L"x"s, std::wstring(buf));
  // ...unless you use wscanf.
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, swscanf(L"xĀyz", L"%l[xĀy]", buf));
  EXPECT_EQ(L"xĀy"s, std::wstring(buf));

  // Negation only covers ASCII for scanf...
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, sscanf("xĀyz", "%l[^ab]", buf));
  EXPECT_EQ(L"x"s, std::wstring(buf));
  // ...but covers wide characters for wscanf.
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, swscanf(L"xĀyz", L"%l[^ab]", buf));
  EXPECT_EQ(L"xĀyz"s, std::wstring(buf));

  // We already determined that non-ASCII characters are ignored in scanf classes.
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, sscanf("x"
                      "\xc4\x80" // Matches a byte from each wide char in the class.
                      "\xc6\x82" // Neither byte is in the class.
                      "yz",
                      "%l[xy" "\xc5\x80" "\xc4\x81" "]", buf));
  EXPECT_EQ(L"x", std::wstring(buf));
  // bionic and glibc both behave badly for wscanf, so let's call it right for now...
  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(1, swscanf(L"x"
                       L"\xc4\x80"
                       L"\xc6\x82"
                       L"yz",
                       L"%l[xy" L"\xc5\x80" L"\xc4\x81" L"]", buf));
  // Note that this isn't L"xĀ" --- although the *bytes* matched, they're
  // not put back together as a wide character.
  EXPECT_EQ(L"x" L"\xc4" L"\x80", std::wstring(buf));
#endif
}

TEST(STDIO_TEST, cantwrite_EBADF) {
  // If we open a file read-only...
  FILE* fp = fopen("/proc/version", "r");

  // ...all attempts to write to that file should return failure.

  // They should also set errno to EBADF. This isn't POSIX, but it's traditional.
  // glibc gets the wide-character functions wrong.

  errno = 0;
  EXPECT_EQ(EOF, putc('x', fp));
  EXPECT_ERRNO(EBADF);

  errno = 0;
  EXPECT_EQ(EOF, fprintf(fp, "hello"));
  EXPECT_ERRNO(EBADF);

  errno = 0;
  EXPECT_EQ(EOF, fwprintf(fp, L"hello"));
#if defined(__BIONIC__)
  EXPECT_ERRNO(EBADF);
#endif

  errno = 0;
  EXPECT_EQ(0U, fwrite("hello", 1, 2, fp));
  EXPECT_ERRNO(EBADF);

  errno = 0;
  EXPECT_EQ(EOF, fputs("hello", fp));
  EXPECT_ERRNO(EBADF);

  errno = 0;
  EXPECT_EQ(WEOF, fputwc(L'x', fp));
#if defined(__BIONIC__)
  EXPECT_ERRNO(EBADF);
#endif
}

// Tests that we can only have a consistent and correct fpos_t when using
// f*pos functions (i.e. fpos doesn't get inside a multi byte character).
TEST(STDIO_TEST, consistent_fpos_t) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

  wchar_t mb_one_bytes = L'h';
  wchar_t mb_two_bytes = 0x00a2;
  wchar_t mb_three_bytes = 0x20ac;
  wchar_t mb_four_bytes = 0x24b62;

  // Write to file.
  ASSERT_EQ(mb_one_bytes, static_cast<wchar_t>(fputwc(mb_one_bytes, fp)));
  ASSERT_EQ(mb_two_bytes, static_cast<wchar_t>(fputwc(mb_two_bytes, fp)));
  ASSERT_EQ(mb_three_bytes, static_cast<wchar_t>(fputwc(mb_three_bytes, fp)));
  ASSERT_EQ(mb_four_bytes, static_cast<wchar_t>(fputwc(mb_four_bytes, fp)));

  rewind(fp);

  // Record each character position.
  fpos_t pos1;
  fpos_t pos2;
  fpos_t pos3;
  fpos_t pos4;
  fpos_t pos5;
  EXPECT_EQ(0, fgetpos(fp, &pos1));
  ASSERT_EQ(mb_one_bytes, static_cast<wchar_t>(fgetwc(fp)));
  EXPECT_EQ(0, fgetpos(fp, &pos2));
  ASSERT_EQ(mb_two_bytes, static_cast<wchar_t>(fgetwc(fp)));
  EXPECT_EQ(0, fgetpos(fp, &pos3));
  ASSERT_EQ(mb_three_bytes, static_cast<wchar_t>(fgetwc(fp)));
  EXPECT_EQ(0, fgetpos(fp, &pos4));
  ASSERT_EQ(mb_four_bytes, static_cast<wchar_t>(fgetwc(fp)));
  EXPECT_EQ(0, fgetpos(fp, &pos5));

#if defined(__BIONIC__)
  // Bionic's fpos_t is just an alias for off_t. This is inherited from OpenBSD
  // upstream. Glibc differs by storing the mbstate_t inside its fpos_t. In
  // Bionic (and upstream OpenBSD) the mbstate_t is stored inside the FILE
  // structure.
  ASSERT_EQ(0, static_cast<off_t>(pos1));
  ASSERT_EQ(1, static_cast<off_t>(pos2));
  ASSERT_EQ(3, static_cast<off_t>(pos3));
  ASSERT_EQ(6, static_cast<off_t>(pos4));
  ASSERT_EQ(10, static_cast<off_t>(pos5));
#endif

  // Exercise back and forth movements of the position.
  ASSERT_EQ(0, fsetpos(fp, &pos2));
  ASSERT_EQ(mb_two_bytes, static_cast<wchar_t>(fgetwc(fp)));
  ASSERT_EQ(0, fsetpos(fp, &pos1));
  ASSERT_EQ(mb_one_bytes, static_cast<wchar_t>(fgetwc(fp)));
  ASSERT_EQ(0, fsetpos(fp, &pos4));
  ASSERT_EQ(mb_four_bytes, static_cast<wchar_t>(fgetwc(fp)));
  ASSERT_EQ(0, fsetpos(fp, &pos3));
  ASSERT_EQ(mb_three_bytes, static_cast<wchar_t>(fgetwc(fp)));
  ASSERT_EQ(0, fsetpos(fp, &pos5));
  ASSERT_EQ(WEOF, fgetwc(fp));

  fclose(fp);
}

// Exercise the interaction between fpos and seek.
TEST(STDIO_TEST, fpos_t_and_seek) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // In glibc-2.16 fseek doesn't work properly in wide mode
  // (https://sourceware.org/bugzilla/show_bug.cgi?id=14543). One workaround is
  // to close and re-open the file. We do it in order to make the test pass
  // with all glibcs.

  TemporaryFile tf;
  FILE* fp = fdopen(tf.fd, "w+");
  ASSERT_TRUE(fp != nullptr);

  wchar_t mb_two_bytes = 0x00a2;
  wchar_t mb_three_bytes = 0x20ac;
  wchar_t mb_four_bytes = 0x24b62;

  // Write to file.
  ASSERT_EQ(mb_two_bytes, static_cast<wchar_t>(fputwc(mb_two_bytes, fp)));
  ASSERT_EQ(mb_three_bytes, static_cast<wchar_t>(fputwc(mb_three_bytes, fp)));
  ASSERT_EQ(mb_four_bytes, static_cast<wchar_t>(fputwc(mb_four_bytes, fp)));

  fflush(fp);
  fclose(fp);

  fp = fopen(tf.path, "r");
  ASSERT_TRUE(fp != nullptr);

  // Store a valid position.
  fpos_t mb_two_bytes_pos;
  ASSERT_EQ(0, fgetpos(fp, &mb_two_bytes_pos));

  // Move inside mb_four_bytes with fseek.
  long offset_inside_mb = 6;
  ASSERT_EQ(0, fseek(fp, offset_inside_mb, SEEK_SET));

  // Store the "inside multi byte" position.
  fpos_t pos_inside_mb;
  ASSERT_EQ(0, fgetpos(fp, &pos_inside_mb));
#if defined(__BIONIC__)
  ASSERT_EQ(offset_inside_mb, static_cast<off_t>(pos_inside_mb));
#endif

  // Reading from within a byte should produce an error.
  ASSERT_EQ(WEOF, fgetwc(fp));
  ASSERT_ERRNO(EILSEQ);

  // Reverting to a valid position should work.
  ASSERT_EQ(0, fsetpos(fp, &mb_two_bytes_pos));
  ASSERT_EQ(mb_two_bytes, static_cast<wchar_t>(fgetwc(fp)));

  // Moving withing a multi byte with fsetpos should work but reading should
  // produce an error.
  ASSERT_EQ(0, fsetpos(fp, &pos_inside_mb));
  ASSERT_EQ(WEOF, fgetwc(fp));
  ASSERT_ERRNO(EILSEQ);

  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen) {
  char buf[16];
  memset(buf, 0, sizeof(buf));
  FILE* fp = fmemopen(buf, sizeof(buf), "r+");
  ASSERT_EQ('<', fputc('<', fp));
  ASSERT_NE(EOF, fputs("abc>\n", fp));
  fflush(fp);

  // We wrote to the buffer...
  ASSERT_STREQ("<abc>\n", buf);

  // And can read back from the file.
  AssertFileIs(fp, "<abc>\n", true);
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_nullptr) {
  FILE* fp = fmemopen(nullptr, 128, "r+");
  ASSERT_NE(EOF, fputs("xyz\n", fp));

  AssertFileIs(fp, "xyz\n", true);
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_trailing_NUL_byte) {
  FILE* fp;
  char buf[8];

  // POSIX: "When a stream open for writing is flushed or closed, a null byte
  // shall be written at the current position or at the end of the buffer,
  // depending on the size of the contents."
  memset(buf, 'x', sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "w"));
  // Even with nothing written (and not in truncate mode), we'll flush a NUL...
  ASSERT_EQ(0, fflush(fp));
  EXPECT_EQ("\0xxxxxxx"s, std::string(buf, buf + sizeof(buf)));
  // Now write and check that the NUL moves along with our writes...
  ASSERT_NE(EOF, fputs("hello", fp));
  ASSERT_EQ(0, fflush(fp));
  EXPECT_EQ("hello\0xx"s, std::string(buf, buf + sizeof(buf)));
  ASSERT_NE(EOF, fputs("wo", fp));
  ASSERT_EQ(0, fflush(fp));
  EXPECT_EQ("hellowo\0"s, std::string(buf, buf + sizeof(buf)));
  ASSERT_EQ(0, fclose(fp));

  // "If a stream open for update is flushed or closed and the last write has
  // advanced the current buffer size, a null byte shall be written at the end
  // of the buffer if it fits."
  memset(buf, 'x', sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "r+"));
  // Nothing written yet, so no advance...
  ASSERT_EQ(0, fflush(fp));
  EXPECT_EQ("xxxxxxxx"s, std::string(buf, buf + sizeof(buf)));
  ASSERT_NE(EOF, fputs("hello", fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_size) {
  FILE* fp;
  char buf[16];
  memset(buf, 'x', sizeof(buf));

  // POSIX: "The stream shall also maintain the size of the current buffer
  // contents; use of fseek() or fseeko() on the stream with SEEK_END shall
  // seek relative to this size."

  // "For modes r and r+ the size shall be set to the value given by the size
  // argument."
  ASSERT_NE(nullptr, fp = fmemopen(buf, 16, "r"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fclose(fp));
  ASSERT_NE(nullptr, fp = fmemopen(buf, 16, "r+"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fclose(fp));

  // "For modes w and w+ the initial size shall be zero..."
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 16, "w"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fclose(fp));
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 16, "w+"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fclose(fp));

  // "...and for modes a and a+ the initial size shall be:
  // 1. Zero, if buf is a null pointer
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 16, "a"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fclose(fp));
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 16, "a+"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  EXPECT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fclose(fp));

  // 2. The position of the first null byte in the buffer, if one is found
  memset(buf, 'x', sizeof(buf));
  buf[3] = '\0';
  ASSERT_NE(nullptr, fp = fmemopen(buf, 16, "a"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(3, ftell(fp));
  EXPECT_EQ(3, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(3, ftell(fp));
  EXPECT_EQ(3, ftello(fp));
  ASSERT_EQ(0, fclose(fp));
  memset(buf, 'x', sizeof(buf));
  buf[3] = '\0';
  ASSERT_NE(nullptr, fp = fmemopen(buf, 16, "a+"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(3, ftell(fp));
  EXPECT_EQ(3, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(3, ftell(fp));
  EXPECT_EQ(3, ftello(fp));
  ASSERT_EQ(0, fclose(fp));

  // 3. The value of the size argument, if buf is not a null pointer and no
  // null byte is found.
  memset(buf, 'x', sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, 16, "a"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fclose(fp));
  memset(buf, 'x', sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, 16, "a+"));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fseeko(fp, 0, SEEK_END));
  EXPECT_EQ(16, ftell(fp));
  EXPECT_EQ(16, ftello(fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_SEEK_END) {
  // fseek SEEK_END is relative to the current string length, not the buffer size.
  FILE* fp;
  char buf[8];
  memset(buf, 'x', sizeof(buf));
  strcpy(buf, "str");
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "w+"));
  ASSERT_NE(EOF, fputs("string", fp));
  EXPECT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(static_cast<long>(strlen("string")), ftell(fp));
  EXPECT_EQ(static_cast<off_t>(strlen("string")), ftello(fp));
  EXPECT_EQ(0, fclose(fp));

  // glibc < 2.22 interpreted SEEK_END the wrong way round (subtracting rather
  // than adding).
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "w+"));
  ASSERT_NE(EOF, fputs("54321", fp));
  EXPECT_EQ(0, fseek(fp, -2, SEEK_END));
  EXPECT_EQ('2', fgetc(fp));
  EXPECT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_seek_invalid) {
  char buf[8];
  memset(buf, 'x', sizeof(buf));
  FILE* fp = fmemopen(buf, sizeof(buf), "w");
  ASSERT_TRUE(fp != nullptr);

  // POSIX: "An attempt to seek ... to a negative position or to a position
  // larger than the buffer size given in the size argument shall fail."
  // (There's no mention of what errno should be set to, and glibc doesn't
  // set errno in any of these cases.)
  EXPECT_EQ(-1, fseek(fp, -2, SEEK_SET));
  EXPECT_EQ(-1, fseeko(fp, -2, SEEK_SET));
  EXPECT_EQ(-1, fseek(fp, sizeof(buf) + 1, SEEK_SET));
  EXPECT_EQ(-1, fseeko(fp, sizeof(buf) + 1, SEEK_SET));
}

TEST(STDIO_TEST, fmemopen_read_EOF) {
  // POSIX: "A read operation on the stream shall not advance the current
  // buffer position beyond the current buffer size."
  char buf[8];
  memset(buf, 'x', sizeof(buf));
  FILE* fp = fmemopen(buf, sizeof(buf), "r");
  ASSERT_TRUE(fp != nullptr);
  char buf2[BUFSIZ];
  ASSERT_EQ(8U, fread(buf2, 1, sizeof(buf2), fp));
  // POSIX: "Reaching the buffer size in a read operation shall count as
  // end-of-file.
  ASSERT_TRUE(feof(fp));
  ASSERT_EQ(EOF, fgetc(fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_read_null_bytes) {
  // POSIX: "Null bytes in the buffer shall have no special meaning for reads."
  char buf[] = "h\0e\0l\0l\0o";
  FILE* fp = fmemopen(buf, sizeof(buf), "r");
  ASSERT_TRUE(fp != nullptr);
  ASSERT_EQ('h', fgetc(fp));
  ASSERT_EQ(0, fgetc(fp));
  ASSERT_EQ('e', fgetc(fp));
  ASSERT_EQ(0, fgetc(fp));
  ASSERT_EQ('l', fgetc(fp));
  ASSERT_EQ(0, fgetc(fp));
  // POSIX: "The read operation shall start at the current buffer position of
  // the stream."
  char buf2[8];
  memset(buf2, 'x', sizeof(buf2));
  ASSERT_EQ(4U, fread(buf2, 1, sizeof(buf2), fp));
  ASSERT_EQ('l', buf2[0]);
  ASSERT_EQ(0, buf2[1]);
  ASSERT_EQ('o', buf2[2]);
  ASSERT_EQ(0, buf2[3]);
  for (size_t i = 4; i < sizeof(buf2); ++i) ASSERT_EQ('x', buf2[i]) << i;
  ASSERT_TRUE(feof(fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_write) {
  FILE* fp;
  char buf[8];

  // POSIX: "A write operation shall start either at the current position of
  // the stream (if mode has not specified 'a' as the first character)..."
  memset(buf, 'x', sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "r+"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(0, fseek(fp, 2, SEEK_SET));
  ASSERT_EQ(' ', fputc(' ', fp));
  EXPECT_EQ("xx xxxxx", std::string(buf, buf + sizeof(buf)));
  ASSERT_EQ(0, fclose(fp));

  // "...or at the current size of the stream (if mode had 'a' as the first
  // character)." (See the fmemopen_size test for what "size" means, but for
  // mode "a", it's the first NUL byte.)
  memset(buf, 'x', sizeof(buf));
  buf[3] = '\0';
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "a+"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(' ', fputc(' ', fp));
  EXPECT_EQ("xxx \0xxx"s, std::string(buf, buf + sizeof(buf)));
  ASSERT_EQ(0, fclose(fp));

  // "If the current position at the end of the write is larger than the
  // current buffer size, the current buffer size shall be set to the current
  // position." (See the fmemopen_size test for what "size" means, but to
  // query it we SEEK_END with offset 0, and then ftell.)
  memset(buf, 'x', sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "w+"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(0, ftell(fp));
  ASSERT_EQ(' ', fputc(' ', fp));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(1, ftell(fp));
  ASSERT_NE(EOF, fputs("123", fp));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(4, ftell(fp));
  EXPECT_EQ(" 123\0xxx"s, std::string(buf, buf + sizeof(buf)));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_write_EOF) {
  // POSIX: "A write operation on the stream shall not advance the current
  // buffer size beyond the size given in the size argument."
  FILE* fp;

  // Scalar writes...
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 4, "w"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ('x', fputc('x', fp));
  ASSERT_EQ('x', fputc('x', fp));
  ASSERT_EQ('x', fputc('x', fp));
  ASSERT_EQ(EOF, fputc('x', fp)); // Only 3 fit because of the implicit NUL.
  ASSERT_EQ(0, fclose(fp));

  // Vector writes...
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 4, "w"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(3U, fwrite("xxxx", 1, 4, fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_initial_position) {
  // POSIX: "The ... current position in the buffer ... shall be initially
  // set to either the beginning of the buffer (for r and w modes) ..."
  char buf[] = "hello\0world";
  FILE* fp;
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "r"));
  EXPECT_EQ(0L, ftell(fp));
  EXPECT_EQ(0, fclose(fp));
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "w"));
  EXPECT_EQ(0L, ftell(fp));
  EXPECT_EQ(0, fclose(fp));
  buf[0] = 'h'; // (Undo the effects of the above.)

  // POSIX: "...or to the first null byte in the buffer (for a modes)."
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "a"));
  EXPECT_EQ(5L, ftell(fp));
  EXPECT_EQ(0, fclose(fp));

  // POSIX: "If no null byte is found in append mode, the initial position
  // shall be set to one byte after the end of the buffer."
  memset(buf, 'x', sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "a"));
  EXPECT_EQ(static_cast<long>(sizeof(buf)), ftell(fp));
  EXPECT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_initial_position_allocated) {
  // POSIX: "If buf is a null pointer, the initial position shall always be
  // set to the beginning of the buffer."
  FILE* fp = fmemopen(nullptr, 128, "a+");
  ASSERT_TRUE(fp != nullptr);
  EXPECT_EQ(0L, ftell(fp));
  EXPECT_EQ(0L, fseek(fp, 0, SEEK_SET));
  EXPECT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_zero_length) {
  // POSIX says it's up to the implementation whether or not you can have a
  // zero-length buffer (but "A future version of this standard may require
  // support of zero-length buffer streams explicitly"). BSD and glibc < 2.22
  // agreed that you couldn't, but glibc >= 2.22 allows it for consistency.
  FILE* fp;
  char buf[16];
  ASSERT_NE(nullptr, fp = fmemopen(buf, 0, "r+"));
  ASSERT_EQ(EOF, fgetc(fp));
  ASSERT_TRUE(feof(fp));
  ASSERT_EQ(0, fclose(fp));
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 0, "r+"));
  ASSERT_EQ(EOF, fgetc(fp));
  ASSERT_TRUE(feof(fp));
  ASSERT_EQ(0, fclose(fp));

  ASSERT_NE(nullptr, fp = fmemopen(buf, 0, "w+"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(EOF, fputc('x', fp));
  ASSERT_EQ(0, fclose(fp));
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 0, "w+"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(EOF, fputc('x', fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_zero_length_buffer_overrun) {
  char buf[2] = "x";
  ASSERT_EQ('x', buf[0]);
  FILE* fp = fmemopen(buf, 0, "w");
  ASSERT_EQ('x', buf[0]);
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_write_only_allocated) {
  // POSIX says fmemopen "may fail if the mode argument does not include a '+'".
  // BSD fails, glibc doesn't. We side with the more lenient.
  FILE* fp;
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 16, "r"));
  ASSERT_EQ(0, fclose(fp));
  ASSERT_NE(nullptr, fp = fmemopen(nullptr, 16, "w"));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_fileno) {
  // There's no fd backing an fmemopen FILE*.
  FILE* fp = fmemopen(nullptr, 16, "r");
  ASSERT_TRUE(fp != nullptr);
  errno = 0;
  ASSERT_EQ(-1, fileno(fp));
  ASSERT_ERRNO(EBADF);
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fmemopen_append_after_seek) {
  // In BSD and glibc < 2.22, append mode didn't force writes to append if
  // there had been an intervening seek.

  FILE* fp;
  char buf[] = "hello\0world";
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "a"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(0, fseek(fp, 0, SEEK_SET));
  ASSERT_NE(EOF, fputc('!', fp));
  EXPECT_EQ("hello!\0orld\0"s, std::string(buf, buf + sizeof(buf)));
  ASSERT_EQ(0, fclose(fp));

  memcpy(buf, "hello\0world", sizeof(buf));
  ASSERT_NE(nullptr, fp = fmemopen(buf, sizeof(buf), "a+"));
  setbuf(fp, nullptr); // Turn off buffering so we can see what's happening as it happens.
  ASSERT_EQ(0, fseek(fp, 0, SEEK_SET));
  ASSERT_NE(EOF, fputc('!', fp));
  EXPECT_EQ("hello!\0orld\0"s, std::string(buf, buf + sizeof(buf)));
  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, open_memstream) {
  char* p = nullptr;
  size_t size = 0;
  FILE* fp = open_memstream(&p, &size);
  ASSERT_NE(EOF, fputs("hello, world!", fp));
  fclose(fp);

  ASSERT_STREQ("hello, world!", p);
  ASSERT_EQ(strlen("hello, world!"), size);
  free(p);
}

TEST(STDIO_TEST, open_memstream_EINVAL) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  char* p;
  size_t size;

  // Invalid buffer.
  errno = 0;
  ASSERT_EQ(nullptr, open_memstream(nullptr, &size));
  ASSERT_ERRNO(EINVAL);

  // Invalid size.
  errno = 0;
  ASSERT_EQ(nullptr, open_memstream(&p, nullptr));
  ASSERT_ERRNO(EINVAL);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "glibc is broken";
#endif
}

TEST(STDIO_TEST, fdopen_add_CLOEXEC) {
  // This fd doesn't have O_CLOEXEC...
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_FALSE(CloseOnExec(fd));
  // ...but the new one does.
  FILE* fp = fdopen(fd, "re");
  ASSERT_TRUE(CloseOnExec(fileno(fp)));
  fclose(fp);
}

TEST(STDIO_TEST, fdopen_remove_CLOEXEC) {
  // This fd has O_CLOEXEC...
  int fd = open("/proc/version", O_RDONLY | O_CLOEXEC);
  ASSERT_TRUE(CloseOnExec(fd));
  // ...but the new one doesn't.
  FILE* fp = fdopen(fd, "r");
  ASSERT_TRUE(CloseOnExec(fileno(fp)));
  fclose(fp);
}

TEST(STDIO_TEST, freopen_add_CLOEXEC) {
  // This FILE* doesn't have O_CLOEXEC...
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_FALSE(CloseOnExec(fileno(fp)));
  // ...but the new one does.
  fp = freopen("/proc/version", "re", fp);
  ASSERT_TRUE(CloseOnExec(fileno(fp)));

  fclose(fp);
}

TEST(STDIO_TEST, freopen_remove_CLOEXEC) {
  // This FILE* has O_CLOEXEC...
  FILE* fp = fopen("/proc/version", "re");
  ASSERT_TRUE(CloseOnExec(fileno(fp)));
  // ...but the new one doesn't.
  fp = freopen("/proc/version", "r", fp);
  ASSERT_FALSE(CloseOnExec(fileno(fp)));
  fclose(fp);
}

TEST(STDIO_TEST, freopen_null_filename_add_CLOEXEC) {
  // This FILE* doesn't have O_CLOEXEC...
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_FALSE(CloseOnExec(fileno(fp)));
  // ...but the new one does.
  fp = freopen(nullptr, "re", fp);
  ASSERT_TRUE(CloseOnExec(fileno(fp)));
  fclose(fp);
}

TEST(STDIO_TEST, freopen_null_filename_remove_CLOEXEC) {
  // This FILE* has O_CLOEXEC...
  FILE* fp = fopen("/proc/version", "re");
  ASSERT_TRUE(CloseOnExec(fileno(fp)));
  // ...but the new one doesn't.
  fp = freopen(nullptr, "r", fp);
  ASSERT_FALSE(CloseOnExec(fileno(fp)));
  fclose(fp);
}

TEST(STDIO_TEST, fopen64_freopen64) {
  FILE* fp = fopen64("/proc/version", "r");
  ASSERT_TRUE(fp != nullptr);
  fp = freopen64("/proc/version", "re", fp);
  ASSERT_TRUE(fp != nullptr);
  fclose(fp);
}

// https://code.google.com/p/android/issues/detail?id=81155
// http://b/18556607
TEST(STDIO_TEST, fread_unbuffered_pathological_performance) {
  FILE* fp = fopen("/dev/zero", "r");
  ASSERT_TRUE(fp != nullptr);

  // Make this stream unbuffered.
  setvbuf(fp, nullptr, _IONBF, 0);

  char buf[65*1024];
  memset(buf, 0xff, sizeof(buf));

  time_t t0 = time(nullptr);
  for (size_t i = 0; i < 1024; ++i) {
    ASSERT_EQ(1U, fread(buf, 64*1024, 1, fp));
  }
  time_t t1 = time(nullptr);

  fclose(fp);

  // 1024 64KiB reads should have been very quick.
  ASSERT_LE(t1 - t0, 1);

  for (size_t i = 0; i < 64*1024; ++i) {
    ASSERT_EQ('\0', buf[i]);
  }
  for (size_t i = 64*1024; i < 65*1024; ++i) {
    ASSERT_EQ('\xff', buf[i]);
  }
}

TEST(STDIO_TEST, fread_EOF) {
  std::string digits("0123456789");
  FILE* fp = fmemopen(&digits[0], digits.size(), "r");

  // Try to read too much, but little enough that it still fits in the FILE's internal buffer.
  char buf1[4 * 4];
  memset(buf1, 0, sizeof(buf1));
  ASSERT_EQ(2U, fread(buf1, 4, 4, fp));
  ASSERT_STREQ("0123456789", buf1);
  ASSERT_TRUE(feof(fp));

  rewind(fp);

  // Try to read way too much so stdio tries to read more direct from the stream.
  char buf2[4 * 4096];
  memset(buf2, 0, sizeof(buf2));
  ASSERT_EQ(2U, fread(buf2, 4, 4096, fp));
  ASSERT_STREQ("0123456789", buf2);
  ASSERT_TRUE(feof(fp));

  fclose(fp);
}

static void test_fread_from_write_only_stream(size_t n) {
  FILE* fp = fopen("/dev/null", "w");
  std::vector<char> buf(n, 0);
  errno = 0;
  ASSERT_EQ(0U, fread(&buf[0], n, 1, fp));
  ASSERT_ERRNO(EBADF);
  ASSERT_TRUE(ferror(fp));
  ASSERT_FALSE(feof(fp));
  fclose(fp);
}

TEST(STDIO_TEST, fread_from_write_only_stream_slow_path) {
  test_fread_from_write_only_stream(1);
}

TEST(STDIO_TEST, fread_from_write_only_stream_fast_path) {
  test_fread_from_write_only_stream(64*1024);
}

TEST(STDIO_TEST, fwrite_to_read_only_stream) {
  FILE* fp = fopen("/proc/version", "re");
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(0U, fwrite("hello", 1, 5, fp));
  ASSERT_TRUE(ferror(fp));
}

TEST(STDIO_TEST, fputc_to_read_only_stream) {
  FILE* fp = fopen("/proc/version", "re");
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(EOF, fputc('x', fp));
  ASSERT_TRUE(ferror(fp));
}

TEST(STDIO_TEST, fprintf_to_read_only_stream) {
  FILE* fp = fopen("/proc/version", "re");
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(-1, fprintf(fp, "%s%d", "hello", 123));
  ASSERT_TRUE(ferror(fp));
}

static void test_fwrite_after_fread(size_t n) {
  TemporaryFile tf;

  FILE* fp = fdopen(tf.fd, "w+");
  ASSERT_EQ(1U, fwrite("1", 1, 1, fp));
  fflush(fp);

  // We've flushed but not rewound, so there's nothing to read.
  std::vector<char> buf(n, 0);
  ASSERT_EQ(0U, fread(&buf[0], 1, buf.size(), fp));
  ASSERT_TRUE(feof(fp));

  // But hitting EOF doesn't prevent us from writing...
  errno = 0;
  ASSERT_EQ(1U, fwrite("2", 1, 1, fp)) << strerror(errno);

  // And if we rewind, everything's there.
  rewind(fp);
  ASSERT_EQ(2U, fread(&buf[0], 1, buf.size(), fp));
  ASSERT_EQ('1', buf[0]);
  ASSERT_EQ('2', buf[1]);

  fclose(fp);
}

TEST(STDIO_TEST, fwrite_after_fread_slow_path) {
  test_fwrite_after_fread(16);
}

TEST(STDIO_TEST, fwrite_after_fread_fast_path) {
  test_fwrite_after_fread(64*1024);
}

// http://b/19172514
TEST(STDIO_TEST, fread_after_fseek) {
  TemporaryFile tf;

  FILE* fp = fopen(tf.path, "w+");
  ASSERT_TRUE(fp != nullptr);

  char file_data[12288];
  for (size_t i = 0; i < 12288; i++) {
    file_data[i] = i;
  }
  ASSERT_EQ(12288U, fwrite(file_data, 1, 12288, fp));
  fclose(fp);

  fp = fopen(tf.path, "r");
  ASSERT_TRUE(fp != nullptr);

  char buffer[8192];
  size_t cur_location = 0;
  // Small read to populate internal buffer.
  ASSERT_EQ(100U, fread(buffer, 1, 100, fp));
  ASSERT_EQ(memcmp(file_data, buffer, 100), 0);

  cur_location = static_cast<size_t>(ftell(fp));
  // Large read to force reading into the user supplied buffer and bypassing
  // the internal buffer.
  ASSERT_EQ(8192U, fread(buffer, 1, 8192, fp));
  ASSERT_EQ(memcmp(file_data+cur_location, buffer, 8192), 0);

  // Small backwards seek to verify fseek does not reuse the internal buffer.
  ASSERT_EQ(0, fseek(fp, -22, SEEK_CUR)) << strerror(errno);
  cur_location = static_cast<size_t>(ftell(fp));
  ASSERT_EQ(22U, fread(buffer, 1, 22, fp));
  ASSERT_EQ(memcmp(file_data+cur_location, buffer, 22), 0);

  fclose(fp);
}

// https://code.google.com/p/android/issues/detail?id=184847
TEST(STDIO_TEST, fread_EOF_184847) {
  TemporaryFile tf;
  char buf[6] = {0};

  FILE* fw = fopen(tf.path, "w");
  ASSERT_TRUE(fw != nullptr);

  FILE* fr = fopen(tf.path, "r");
  ASSERT_TRUE(fr != nullptr);

  fwrite("a", 1, 1, fw);
  fflush(fw);
  ASSERT_EQ(1U, fread(buf, 1, 1, fr));
  ASSERT_STREQ("a", buf);

  // 'fr' is now at EOF.
  ASSERT_EQ(0U, fread(buf, 1, 1, fr));
  ASSERT_TRUE(feof(fr));

  // Write some more...
  fwrite("z", 1, 1, fw);
  fflush(fw);

  // ...and check that we can read it back.
  // (BSD thinks that once a stream has hit EOF, it must always return EOF. SysV disagrees.)
  ASSERT_EQ(1U, fread(buf, 1, 1, fr));
  ASSERT_STREQ("z", buf);

  // But now we're done.
  ASSERT_EQ(0U, fread(buf, 1, 1, fr));

  fclose(fr);
  fclose(fw);
}

TEST(STDIO_TEST, fclose_invalidates_fd) {
  // The typical error we're trying to help people catch involves accessing
  // memory after it's been freed. But we know that stdin/stdout/stderr are
  // special and don't get deallocated, so this test uses stdin.
  ASSERT_EQ(0, fclose(stdin));

  // Even though using a FILE* after close is undefined behavior, I've closed
  // this bug as "WAI" too many times. We shouldn't hand out stale fds,
  // especially because they might actually correspond to a real stream.
  errno = 0;
  ASSERT_EQ(-1, fileno(stdin));
  ASSERT_ERRNO(EBADF);
}

TEST(STDIO_TEST, fseek_ftell_unseekable) {
#if defined(__BIONIC__) // glibc has fopencookie instead.
  auto read_fn = [](void*, char*, int) { return -1; };
  FILE* fp = funopen(nullptr, read_fn, nullptr, nullptr, nullptr);
  ASSERT_TRUE(fp != nullptr);

  // Check that ftell balks on an unseekable FILE*.
  errno = 0;
  ASSERT_EQ(-1, ftell(fp));
  ASSERT_ERRNO(ESPIPE);

  // SEEK_CUR is rewritten as SEEK_SET internally...
  errno = 0;
  ASSERT_EQ(-1, fseek(fp, 0, SEEK_CUR));
  ASSERT_ERRNO(ESPIPE);

  // ...so it's worth testing the direct seek path too.
  errno = 0;
  ASSERT_EQ(-1, fseek(fp, 0, SEEK_SET));
  ASSERT_ERRNO(ESPIPE);

  fclose(fp);
#else
  GTEST_SKIP() << "glibc uses fopencookie instead";
#endif
}

TEST(STDIO_TEST, funopen_EINVAL) {
#if defined(__BIONIC__)
  errno = 0;
  ASSERT_EQ(nullptr, funopen(nullptr, nullptr, nullptr, nullptr, nullptr));
  ASSERT_ERRNO(EINVAL);
#else
  GTEST_SKIP() << "glibc uses fopencookie instead";
#endif
}

TEST(STDIO_TEST, funopen_seek) {
#if defined(__BIONIC__)
  auto read_fn = [](void*, char*, int) { return -1; };

  auto seek_fn = [](void*, fpos_t, int) -> fpos_t { return 0xfedcba12; };
  auto seek64_fn = [](void*, fpos64_t, int) -> fpos64_t { return 0xfedcba12345678; };

  FILE* fp = funopen(nullptr, read_fn, nullptr, seek_fn, nullptr);
  ASSERT_TRUE(fp != nullptr);
  fpos_t pos;
#if defined(__LP64__)
  EXPECT_EQ(0, fgetpos(fp, &pos)) << strerror(errno);
  EXPECT_EQ(0xfedcba12LL, pos);
#else
  EXPECT_EQ(-1, fgetpos(fp, &pos)) << strerror(errno);
  EXPECT_ERRNO(EOVERFLOW);
#endif

  FILE* fp64 = funopen64(nullptr, read_fn, nullptr, seek64_fn, nullptr);
  ASSERT_TRUE(fp64 != nullptr);
  fpos64_t pos64;
  EXPECT_EQ(0, fgetpos64(fp64, &pos64)) << strerror(errno);
  EXPECT_EQ(0xfedcba12345678, pos64);
#else
  GTEST_SKIP() << "glibc uses fopencookie instead";
#endif
}

TEST(STDIO_TEST, lots_of_concurrent_files) {
  std::vector<TemporaryFile*> tfs;
  std::vector<FILE*> fps;

  for (size_t i = 0; i < 256; ++i) {
    TemporaryFile* tf = new TemporaryFile;
    tfs.push_back(tf);
    FILE* fp = fopen(tf->path, "w+");
    fps.push_back(fp);
    fprintf(fp, "hello %zu!\n", i);
    fflush(fp);
  }

  for (size_t i = 0; i < 256; ++i) {
    char expected[BUFSIZ];
    snprintf(expected, sizeof(expected), "hello %zu!\n", i);

    AssertFileIs(fps[i], expected);
    fclose(fps[i]);
    delete tfs[i];
  }
}

static void AssertFileOffsetAt(FILE* fp, off64_t offset) {
  EXPECT_EQ(offset, ftell(fp));
  EXPECT_EQ(offset, ftello(fp));
  EXPECT_EQ(offset, ftello64(fp));
  fpos_t pos;
  fpos64_t pos64;
  EXPECT_EQ(0, fgetpos(fp, &pos));
  EXPECT_EQ(0, fgetpos64(fp, &pos64));
#if defined(__BIONIC__)
  EXPECT_EQ(offset, static_cast<off64_t>(pos));
  EXPECT_EQ(offset, static_cast<off64_t>(pos64));
#else
  GTEST_SKIP() << "glibc's fpos_t is opaque";
#endif
}

TEST(STDIO_TEST, seek_tell_family_smoke) {
  TemporaryFile tf;
  FILE* fp = fdopen(tf.fd, "w+");

  // Initially we should be at 0.
  AssertFileOffsetAt(fp, 0);

  // Seek to offset 8192.
  ASSERT_EQ(0, fseek(fp, 8192, SEEK_SET));
  AssertFileOffsetAt(fp, 8192);
  fpos_t eight_k_pos;
  ASSERT_EQ(0, fgetpos(fp, &eight_k_pos));

  // Seek forward another 8192...
  ASSERT_EQ(0, fseek(fp, 8192, SEEK_CUR));
  AssertFileOffsetAt(fp, 8192 + 8192);
  fpos64_t sixteen_k_pos64;
  ASSERT_EQ(0, fgetpos64(fp, &sixteen_k_pos64));

  // Seek back 8192...
  ASSERT_EQ(0, fseek(fp, -8192, SEEK_CUR));
  AssertFileOffsetAt(fp, 8192);

  // Since we haven't written anything, the end is also at 0.
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  AssertFileOffsetAt(fp, 0);

  // Check that our fpos64_t from 16KiB works...
  ASSERT_EQ(0, fsetpos64(fp, &sixteen_k_pos64));
  AssertFileOffsetAt(fp, 8192 + 8192);
  // ...as does our fpos_t from 8192.
  ASSERT_EQ(0, fsetpos(fp, &eight_k_pos));
  AssertFileOffsetAt(fp, 8192);

  // Do fseeko and fseeko64 work too?
  ASSERT_EQ(0, fseeko(fp, 1234, SEEK_SET));
  AssertFileOffsetAt(fp, 1234);
  ASSERT_EQ(0, fseeko64(fp, 5678, SEEK_SET));
  AssertFileOffsetAt(fp, 5678);

  fclose(fp);
}

TEST(STDIO_TEST, fseek_fseeko_EINVAL) {
  TemporaryFile tf;
  FILE* fp = fdopen(tf.fd, "w+");

  // Bad whence.
  errno = 0;
  ASSERT_EQ(-1, fseek(fp, 0, 123));
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  ASSERT_EQ(-1, fseeko(fp, 0, 123));
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  ASSERT_EQ(-1, fseeko64(fp, 0, 123));
  ASSERT_ERRNO(EINVAL);

  // Bad offset.
  errno = 0;
  ASSERT_EQ(-1, fseek(fp, -1, SEEK_SET));
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  ASSERT_EQ(-1, fseeko(fp, -1, SEEK_SET));
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  ASSERT_EQ(-1, fseeko64(fp, -1, SEEK_SET));
  ASSERT_ERRNO(EINVAL);

  fclose(fp);
}

TEST(STDIO_TEST, ctermid) {
  ASSERT_STREQ("/dev/tty", ctermid(nullptr));

  char buf[L_ctermid] = {};
  ASSERT_EQ(buf, ctermid(buf));
  ASSERT_STREQ("/dev/tty", buf);
}

TEST(STDIO_TEST, remove) {
  struct stat sb;

  TemporaryFile tf;
  ASSERT_EQ(0, remove(tf.path));
  ASSERT_EQ(-1, lstat(tf.path, &sb));
  ASSERT_ERRNO(ENOENT);

  TemporaryDir td;
  ASSERT_EQ(0, remove(td.path));
  ASSERT_EQ(-1, lstat(td.path, &sb));
  ASSERT_ERRNO(ENOENT);

  errno = 0;
  ASSERT_EQ(-1, remove(tf.path));
  ASSERT_ERRNO(ENOENT);

  errno = 0;
  ASSERT_EQ(-1, remove(td.path));
  ASSERT_ERRNO(ENOENT);
}

TEST_F(STDIO_DEATHTEST, snprintf_30445072_known_buffer_size) {
  char buf[16];
  ASSERT_EXIT(snprintf(buf, atol("-1"), "hello"),
              testing::KilledBySignal(SIGABRT),
#if defined(NOFORTIFY)
              "FORTIFY: vsnprintf: size .* > SSIZE_MAX"
#else
              "FORTIFY: vsnprintf: prevented .*-byte write into 16-byte buffer"
#endif
              );
}

TEST_F(STDIO_DEATHTEST, snprintf_30445072_unknown_buffer_size) {
  std::string buf = "hello";  // So the compiler doesn't know the buffer size.
  ASSERT_EXIT(snprintf(&buf[0], atol("-1"), "hello"),
              testing::KilledBySignal(SIGABRT),
              "FORTIFY: vsnprintf: size .* > SSIZE_MAX");
}

TEST(STDIO_TEST, sprintf_30445072) {
  std::string buf = "world";
  sprintf(&buf[0], "hello");
  ASSERT_EQ(buf, "hello");
}

TEST(STDIO_TEST, printf_m) {
  errno = 0;
  EXPECT_SNPRINTF("<Success>", "<%m>");
  errno = -1;
  EXPECT_SNPRINTF("<Unknown error -1>", "<%m>");
  errno = EINVAL;
  EXPECT_SNPRINTF("<Invalid argument>", "<%m>");
}

TEST(STDIO_TEST, wprintf_m) {
  errno = 0;
  EXPECT_SWPRINTF(L"<Success>", L"<%m>");
  errno = -1;
  EXPECT_SWPRINTF(L"<Unknown error -1>", L"<%m>");
  errno = EINVAL;
  EXPECT_SWPRINTF(L"<Invalid argument>", L"<%m>");
}

TEST(STDIO_TEST, printf_hash_m) {
  errno = 0;
  EXPECT_SNPRINTF("<0>", "<%#m>");
  errno = -1;
  EXPECT_SNPRINTF("<-1>", "<%#m>");
  errno = EINVAL;
  EXPECT_SNPRINTF("<EINVAL>", "<%#m>");
}

TEST(STDIO_TEST, wprintf_hash_m) {
  errno = 0;
  EXPECT_SWPRINTF(L"<0>", L"<%#m>");
  errno = -1;
  EXPECT_SWPRINTF(L"<-1>", L"<%#m>");
  errno = EINVAL;
  EXPECT_SWPRINTF(L"<EINVAL>", L"<%#m>");
}

TEST(STDIO_TEST, printf_m_does_not_clobber_strerror) {
  const char* m = strerror(-1);
  ASSERT_STREQ("Unknown error -1", m);
  errno = -2;
  EXPECT_SNPRINTF("<Unknown error -2>", "<%m>");
  ASSERT_STREQ("Unknown error -1", m);
}

TEST(STDIO_TEST, wprintf_m_does_not_clobber_strerror) {
  const char* m = strerror(-1);
  ASSERT_STREQ("Unknown error -1", m);
  errno = -2;
  EXPECT_SWPRINTF(L"<Unknown error -2>", L"<%m>");
  ASSERT_STREQ("Unknown error -1", m);
}

TEST(STDIO_TEST, fopen_append_mode_and_ftell) {
  TemporaryFile tf;
  SetFileTo(tf.path, "0123456789");
  FILE* fp = fopen(tf.path, "a");
  EXPECT_EQ(10, ftell(fp));
  ASSERT_EQ(0, fseek(fp, 2, SEEK_SET));
  EXPECT_EQ(2, ftell(fp));
  ASSERT_NE(EOF, fputs("xxx", fp));
  ASSERT_EQ(0, fflush(fp));
  EXPECT_EQ(13, ftell(fp));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(13, ftell(fp));
  ASSERT_EQ(0, fclose(fp));
  AssertFileIs(tf.path, "0123456789xxx");
}

TEST(STDIO_TEST, fdopen_append_mode_and_ftell) {
  TemporaryFile tf;
  SetFileTo(tf.path, "0123456789");
  int fd = open(tf.path, O_RDWR);
  ASSERT_NE(-1, fd);
  // POSIX: "The file position indicator associated with the new stream is set to the position
  // indicated by the file offset associated with the file descriptor."
  ASSERT_EQ(4, lseek(fd, 4, SEEK_SET));
  FILE* fp = fdopen(fd, "a");
  EXPECT_EQ(4, ftell(fp));
  ASSERT_EQ(0, fseek(fp, 2, SEEK_SET));
  EXPECT_EQ(2, ftell(fp));
  ASSERT_NE(EOF, fputs("xxx", fp));
  ASSERT_EQ(0, fflush(fp));
  EXPECT_EQ(13, ftell(fp));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(13, ftell(fp));
  ASSERT_EQ(0, fclose(fp));
  AssertFileIs(tf.path, "0123456789xxx");
}

TEST(STDIO_TEST, freopen_append_mode_and_ftell) {
  TemporaryFile tf;
  SetFileTo(tf.path, "0123456789");
  FILE* other_fp = fopen("/proc/version", "r");
  FILE* fp = freopen(tf.path, "a", other_fp);
  EXPECT_EQ(10, ftell(fp));
  ASSERT_EQ(0, fseek(fp, 2, SEEK_SET));
  EXPECT_EQ(2, ftell(fp));
  ASSERT_NE(EOF, fputs("xxx", fp));
  ASSERT_EQ(0, fflush(fp));
  EXPECT_EQ(13, ftell(fp));
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  EXPECT_EQ(13, ftell(fp));
  ASSERT_EQ(0, fclose(fp));
  AssertFileIs(tf.path, "0123456789xxx");
}

TEST(STDIO_TEST, constants) {
  ASSERT_LE(FILENAME_MAX, PATH_MAX);
  ASSERT_EQ(L_tmpnam, PATH_MAX);
}

TEST(STDIO_TEST, perror) {
  ExecTestHelper eth;
  eth.Run([&]() { errno = EINVAL; perror("a b c"); exit(0); }, 0, "a b c: Invalid argument\n");
  eth.Run([&]() { errno = EINVAL; perror(nullptr); exit(0); }, 0, "Invalid argument\n");
  eth.Run([&]() { errno = EINVAL; perror(""); exit(0); }, 0, "Invalid argument\n");
}

TEST(STDIO_TEST, puts) {
  ExecTestHelper eth;
  eth.Run([&]() { exit(puts("a b c")); }, 0, "a b c\n");
}

TEST(STDIO_TEST, putchar) {
  ExecTestHelper eth;
  eth.Run([&]() { exit(putchar('A')); }, 65, "A");
}

TEST(STDIO_TEST, putchar_unlocked) {
  ExecTestHelper eth;
  eth.Run([&]() { exit(putchar('B')); }, 66, "B");
}

TEST(STDIO_TEST, unlocked) {
  TemporaryFile tf;

  FILE* fp = fopen(tf.path, "w+");
  ASSERT_TRUE(fp != nullptr);

  clearerr_unlocked(fp);
  ASSERT_FALSE(feof_unlocked(fp));
  ASSERT_FALSE(ferror_unlocked(fp));

  ASSERT_EQ(fileno(fp), fileno_unlocked(fp));

  ASSERT_NE(EOF, putc_unlocked('a', fp));
  ASSERT_NE(EOF, putc('b', fp));
  ASSERT_NE(EOF, fputc_unlocked('c', fp));
  ASSERT_NE(EOF, fputc('d', fp));

  rewind(fp);
  ASSERT_EQ('a', getc_unlocked(fp));
  ASSERT_EQ('b', getc(fp));
  ASSERT_EQ('c', fgetc_unlocked(fp));
  ASSERT_EQ('d', fgetc(fp));

  rewind(fp);
  ASSERT_EQ(2U, fwrite_unlocked("AB", 1, 2, fp));
  ASSERT_EQ(2U, fwrite("CD", 1, 2, fp));
  ASSERT_EQ(0, fflush_unlocked(fp));

  rewind(fp);
  char buf[BUFSIZ] = {};
  ASSERT_EQ(2U, fread_unlocked(&buf[0], 1, 2, fp));
  ASSERT_EQ(2U, fread(&buf[2], 1, 2, fp));
  ASSERT_STREQ("ABCD", buf);

  rewind(fp);
  ASSERT_NE(EOF, fputs("hello ", fp));
  ASSERT_NE(EOF, fputs_unlocked("world", fp));
  ASSERT_NE(EOF, fputc('\n', fp));

  rewind(fp);
  ASSERT_TRUE(fgets_unlocked(buf, sizeof(buf), fp) != nullptr);
  ASSERT_STREQ("hello world\n", buf);

  ASSERT_EQ(0, fclose(fp));
}

TEST(STDIO_TEST, fseek_64bit) {
  TemporaryFile tf;
  FILE* fp = fopen64(tf.path, "w+");
  ASSERT_TRUE(fp != nullptr);
  ASSERT_EQ(0, fseeko64(fp, 0x2'0000'0000, SEEK_SET));
  ASSERT_EQ(0x2'0000'0000, ftello64(fp));
  ASSERT_EQ(0, fseeko64(fp, 0x1'0000'0000, SEEK_CUR));
  ASSERT_EQ(0x3'0000'0000, ftello64(fp));
  ASSERT_EQ(0, fclose(fp));
}

// POSIX requires that fseek/fseeko fail with EOVERFLOW if the new file offset
// isn't representable in long/off_t.
TEST(STDIO_TEST, fseek_overflow_32bit) {
  TemporaryFile tf;
  FILE* fp = fopen64(tf.path, "w+");
  ASSERT_EQ(0, ftruncate64(fileno(fp), 0x2'0000'0000));

  // Bionic implements overflow checking for SEEK_CUR, but glibc doesn't.
#if defined(__BIONIC__) && !defined(__LP64__)
  ASSERT_EQ(0, fseek(fp, 0x7fff'ffff, SEEK_SET));
  ASSERT_EQ(-1, fseek(fp, 1, SEEK_CUR));
  ASSERT_ERRNO(EOVERFLOW);
#endif

  // Neither Bionic nor glibc implement the overflow checking for SEEK_END.
  // (Aside: FreeBSD's libc is an example of a libc that checks both SEEK_CUR
  // and SEEK_END -- many C libraries check neither.)
  ASSERT_EQ(0, fseek(fp, 0, SEEK_END));
  ASSERT_EQ(0x2'0000'0000, ftello64(fp));

  fclose(fp);
}

TEST(STDIO_TEST, dev_std_files) {
  // POSIX only mentions /dev/stdout, but we should have all three (http://b/31824379).
  char path[PATH_MAX];
  ssize_t length = readlink("/dev/stdin", path, sizeof(path));
  ASSERT_LT(0, length);
  ASSERT_EQ("/proc/self/fd/0", std::string(path, length));

  length = readlink("/dev/stdout", path, sizeof(path));
  ASSERT_LT(0, length);
  ASSERT_EQ("/proc/self/fd/1", std::string(path, length));

  length = readlink("/dev/stderr", path, sizeof(path));
  ASSERT_LT(0, length);
  ASSERT_EQ("/proc/self/fd/2", std::string(path, length));
}

TEST(STDIO_TEST, fread_with_locked_file) {
  // Reading an unbuffered/line-buffered file from one thread shouldn't block on
  // files locked on other threads, even if it flushes some line-buffered files.
  FILE* fp1 = fopen("/dev/zero", "r");
  ASSERT_TRUE(fp1 != nullptr);
  flockfile(fp1);

  std::thread([] {
    for (int mode : { _IONBF, _IOLBF }) {
      FILE* fp2 = fopen("/dev/zero", "r");
      ASSERT_TRUE(fp2 != nullptr);
      setvbuf(fp2, nullptr, mode, 0);
      ASSERT_EQ('\0', fgetc(fp2));
      fclose(fp2);
    }
  }).join();

  funlockfile(fp1);
  fclose(fp1);
}

TEST(STDIO_TEST, SEEK_macros) {
  ASSERT_EQ(0, SEEK_SET);
  ASSERT_EQ(1, SEEK_CUR);
  ASSERT_EQ(2, SEEK_END);
  ASSERT_EQ(3, SEEK_DATA);
  ASSERT_EQ(4, SEEK_HOLE);
  // So we'll notice if Linux grows another constant in <linux/fs.h>...
  ASSERT_EQ(SEEK_MAX, SEEK_HOLE);
}

TEST(STDIO_TEST, rename) {
  TemporaryDir td;
  std::string old_path = td.path + "/old"s;
  std::string new_path = td.path + "/new"s;

  // Create the file, check it exists.
  ASSERT_EQ(0, close(creat(old_path.c_str(), 0666)));
  struct stat sb;
  ASSERT_EQ(0, stat(old_path.c_str(), &sb));
  ASSERT_EQ(-1, stat(new_path.c_str(), &sb));

  // Rename and check it moved.
  ASSERT_EQ(0, rename(old_path.c_str(), new_path.c_str()));
  ASSERT_EQ(-1, stat(old_path.c_str(), &sb));
  ASSERT_EQ(0, stat(new_path.c_str(), &sb));
}

TEST(STDIO_TEST, renameat) {
  TemporaryDir td;
  android::base::unique_fd dirfd{open(td.path, O_PATH)};
  std::string old_path = td.path + "/old"s;
  std::string new_path = td.path + "/new"s;

  // Create the file, check it exists.
  ASSERT_EQ(0, close(creat(old_path.c_str(), 0666)));
  struct stat sb;
  ASSERT_EQ(0, stat(old_path.c_str(), &sb));
  ASSERT_EQ(-1, stat(new_path.c_str(), &sb));

  // Rename and check it moved.
  ASSERT_EQ(0, renameat(dirfd, "old", dirfd, "new"));
  ASSERT_EQ(-1, stat(old_path.c_str(), &sb));
  ASSERT_EQ(0, stat(new_path.c_str(), &sb));
}

TEST(STDIO_TEST, renameat2) {
#if defined(__GLIBC__) || defined(ANDROID_HOST_MUSL)
  GTEST_SKIP() << "glibc doesn't have renameat2 until 2.28 and musl doesn't have renameat2";
#else
  TemporaryDir td;
  android::base::unique_fd dirfd{open(td.path, O_PATH)};
  std::string old_path = td.path + "/old"s;
  std::string new_path = td.path + "/new"s;

  // Create the file, check it exists.
  ASSERT_EQ(0, close(creat(old_path.c_str(), 0666)));
  struct stat sb;
  ASSERT_EQ(0, stat(old_path.c_str(), &sb));
  ASSERT_EQ(-1, stat(new_path.c_str(), &sb));

  // Rename and check it moved.
  ASSERT_EQ(0, renameat2(dirfd, "old", dirfd, "new", 0));
  ASSERT_EQ(-1, stat(old_path.c_str(), &sb));
  ASSERT_EQ(0, stat(new_path.c_str(), &sb));

  // After this, both "old" and "new" exist.
  ASSERT_EQ(0, close(creat(old_path.c_str(), 0666)));

  // Rename and check it moved.
  ASSERT_EQ(-1, renameat2(dirfd, "old", dirfd, "new", RENAME_NOREPLACE));
  ASSERT_ERRNO(EEXIST);
#endif
}

TEST(STDIO_TEST, renameat2_flags) {
#if defined(__GLIBC__)
  GTEST_SKIP() << "glibc doesn't have renameat2 until 2.28";
#else
 ASSERT_NE(0, RENAME_EXCHANGE);
 ASSERT_NE(0, RENAME_NOREPLACE);
 ASSERT_NE(0, RENAME_WHITEOUT);
#endif
}

TEST(STDIO_TEST, fdopen_failures) {
  FILE* fp;
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);

  // Nonsense mode.
  errno = 0;
  fp = fdopen(fd, "nonsense");
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(EINVAL);

  // Mode that isn't a subset of the fd's actual mode.
  errno = 0;
  fp = fdopen(fd, "w");
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(EINVAL);

  // Can't set append on the underlying fd.
  errno = 0;
  fp = fdopen(fd, "a");
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(EINVAL);

  // Bad fd.
  errno = 0;
  fp = fdopen(-1, "re");
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(EBADF);

  close(fd);
}

TEST(STDIO_TEST, fmemopen_invalid_mode) {
  errno = 0;
  FILE* fp = fmemopen(nullptr, 16, "nonsense");
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(EINVAL);
}

TEST(STDIO_TEST, fopen_invalid_mode) {
  errno = 0;
  FILE* fp = fopen("/proc/version", "nonsense");
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(EINVAL);
}

TEST(STDIO_TEST, freopen_invalid_mode) {
  FILE* fp = fopen("/proc/version", "re");
  ASSERT_TRUE(fp != nullptr);

  errno = 0;
  fp = freopen("/proc/version", "nonsense", fp);
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(EINVAL);
}

TEST(STDIO_TEST, asprintf_smoke) {
  char* p = nullptr;
  ASSERT_EQ(11, asprintf(&p, "hello %s", "world"));
  ASSERT_STREQ("hello world", p);
  free(p);
}

TEST(STDIO_TEST, fopen_ENOENT) {
  errno = 0;
  FILE* fp = fopen("/proc/does-not-exist", "re");
  ASSERT_TRUE(fp == nullptr);
  ASSERT_ERRNO(ENOENT);
}

static void tempnam_test(bool has_TMPDIR, const char* dir, const char* prefix, const char* re) {
  if (has_TMPDIR) {
    setenv("TMPDIR", "/my/tmp/dir", 1);
  } else {
    unsetenv("TMPDIR");
  }
  char* s1 = tempnam(dir, prefix);
  char* s2 = tempnam(dir, prefix);
  ASSERT_MATCH(s1, re);
  ASSERT_MATCH(s2, re);
  ASSERT_STRNE(s1, s2);
  free(s1);
  free(s2);
}

TEST(STDIO_TEST, tempnam__system_directory_system_prefix_with_TMPDIR) {
  tempnam_test(true, nullptr, nullptr, "^/my/tmp/dir/.*");
}

TEST(STDIO_TEST, tempnam__system_directory_system_prefix_without_TMPDIR) {
  tempnam_test(false, nullptr, nullptr, "^/data/local/tmp/.*");
}

TEST(STDIO_TEST, tempnam__system_directory_user_prefix_with_TMPDIR) {
  tempnam_test(true, nullptr, "prefix", "^/my/tmp/dir/prefix.*");
}

TEST(STDIO_TEST, tempnam__system_directory_user_prefix_without_TMPDIR) {
  tempnam_test(false, nullptr, "prefix", "^/data/local/tmp/prefix.*");
}

TEST(STDIO_TEST, tempnam__user_directory_system_prefix_with_TMPDIR) {
  tempnam_test(true, "/a/b/c", nullptr, "^/my/tmp/dir/.*");
}

TEST(STDIO_TEST, tempnam__user_directory_system_prefix_without_TMPDIR) {
  tempnam_test(false, "/a/b/c", nullptr, "^/a/b/c/.*");
}

TEST(STDIO_TEST, tempnam__user_directory_user_prefix_with_TMPDIR) {
  tempnam_test(true, "/a/b/c", "prefix", "^/my/tmp/dir/prefix.*");
}

TEST(STDIO_TEST, tempnam__user_directory_user_prefix_without_TMPDIR) {
  tempnam_test(false, "/a/b/c", "prefix", "^/a/b/c/prefix.*");
}

static void tmpnam_test(char* s) {
  char s1[L_tmpnam], s2[L_tmpnam];

  strcpy(s1, tmpnam(s));
  strcpy(s2, tmpnam(s));
  ASSERT_MATCH(s1, "/tmp/.*");
  ASSERT_MATCH(s2, "/tmp/.*");
  ASSERT_STRNE(s1, s2);
}

TEST(STDIO_TEST, tmpnam) {
  tmpnam_test(nullptr);
}

TEST(STDIO_TEST, tmpnam_buf) {
  char buf[L_tmpnam];
  tmpnam_test(buf);
}

TEST(STDIO_TEST, freopen_null_filename_mode) {
  TemporaryFile tf;
  FILE* fp = fopen(tf.path, "r");
  ASSERT_TRUE(fp != nullptr);

  // "r" = O_RDONLY
  char buf[1];
  ASSERT_EQ(0, read(fileno(fp), buf, 1));
  ASSERT_EQ(-1, write(fileno(fp), "hello", 1));
  // "r+" = O_RDWR
  fp = freopen(nullptr, "r+", fp);
  ASSERT_EQ(0, read(fileno(fp), buf, 1));
  ASSERT_EQ(1, write(fileno(fp), "hello", 1));
  // "w" = O_WRONLY
  fp = freopen(nullptr, "w", fp);
  ASSERT_EQ(-1, read(fileno(fp), buf, 1));
  ASSERT_EQ(1, write(fileno(fp), "hello", 1));

  fclose(fp);
}

#if defined(__LP64__)
static int64_t GetTotalRamGiB() {
  struct sysinfo si;
  sysinfo(&si);
  return (static_cast<int64_t>(si.totalram) * si.mem_unit) / 1024 / 1024 / 1024;
}
#endif

TEST(STDIO_TEST, fread_int_overflow) {
#if defined(__LP64__)
  if (GetTotalRamGiB() <= 4) GTEST_SKIP() << "not enough memory";

  const size_t too_big_for_an_int = 0x80000000ULL;
  std::vector<char> buf(too_big_for_an_int);
  std::unique_ptr<FILE, decltype(&fclose)> fp{fopen("/dev/zero", "re"), fclose};
  ASSERT_EQ(too_big_for_an_int, fread(&buf[0], 1, too_big_for_an_int, fp.get()));
#else
  GTEST_SKIP() << "32-bit can't allocate 2GiB";
#endif
}

TEST(STDIO_TEST, fwrite_int_overflow) {
#if defined(__LP64__)
  if (GetTotalRamGiB() <= 4) GTEST_SKIP() << "not enough memory";

  const size_t too_big_for_an_int = 0x80000000ULL;
  std::vector<char> buf(too_big_for_an_int);
  std::unique_ptr<FILE, decltype(&fclose)> fp{fopen("/dev/null", "we"), fclose};
  ASSERT_EQ(too_big_for_an_int, fwrite(&buf[0], 1, too_big_for_an_int, fp.get()));
#else
  GTEST_SKIP() << "32-bit can't allocate 2GiB";
#endif
}

TEST(STDIO_TEST, snprintf_b_B) {
#if defined(__BIONIC__)
  uint8_t b = 5;
  EXPECT_SNPRINTF("<101>", "<%" PRIb8 ">", b);
  EXPECT_SNPRINTF("<101>", "<%" PRIB8 ">", b);
  EXPECT_SNPRINTF("<00000101>", "<%08" PRIb8 ">", b);
  EXPECT_SNPRINTF("<00000101>", "<%08" PRIB8 ">", b);

  uint16_t s = 0xaaaa;
  EXPECT_SNPRINTF("<1010101010101010>", "<%" PRIb16 ">", s);
  EXPECT_SNPRINTF("<1010101010101010>", "<%" PRIB16 ">", s);
  EXPECT_SNPRINTF("<0b1010101010101010>", "<%#" PRIb16 ">", s);
  EXPECT_SNPRINTF("<0B1010101010101010>", "<%#" PRIB16 ">", s);

  EXPECT_SNPRINTF("<10101010101010101010101010101010>", "<%" PRIb32 ">", 0xaaaaaaaa);
  EXPECT_SNPRINTF("<10101010101010101010101010101010>", "<%" PRIB32 ">", 0xaaaaaaaa);
  EXPECT_SNPRINTF("<0b10101010101010101010101010101010>", "<%#" PRIb32 ">", 0xaaaaaaaa);
  EXPECT_SNPRINTF("<0B10101010101010101010101010101010>", "<%#" PRIB32 ">", 0xaaaaaaaa);

  // clang doesn't like "%lb" (https://github.com/llvm/llvm-project/issues/62247)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
  EXPECT_SNPRINTF("<1010101010101010101010101010101010101010101010101010101010101010>",
                  "<%" PRIb64 ">", 0xaaaaaaaa'aaaaaaaa);
  EXPECT_SNPRINTF("<1010101010101010101010101010101010101010101010101010101010101010>",
                  "<%" PRIB64 ">", 0xaaaaaaaa'aaaaaaaa);
  EXPECT_SNPRINTF("<0b1010101010101010101010101010101010101010101010101010101010101010>",
                  "<%#" PRIb64 ">", 0xaaaaaaaa'aaaaaaaa);
  EXPECT_SNPRINTF("<0B1010101010101010101010101010101010101010101010101010101010101010>",
                  "<%#" PRIB64 ">", 0xaaaaaaaa'aaaaaaaa);
#pragma clang diagnostic pop

  EXPECT_SNPRINTF("<0>", "<%#b>", 0);
  EXPECT_SNPRINTF("<0>", "<%#B>", 0);
#else
  GTEST_SKIP() << "no %b in glibc";
#endif
}

TEST(STDIO_TEST, swprintf_b_B) {
#if defined(__BIONIC__)
  uint8_t b = 5;
  EXPECT_SWPRINTF(L"<101>", L"<%" PRIb8 ">", b);
  EXPECT_SWPRINTF(L"<101>", L"<%" PRIB8 ">", b);
  EXPECT_SWPRINTF(L"<0b101>", L"<%#" PRIb8 ">", b);
  EXPECT_SWPRINTF(L"<0B101>", L"<%#" PRIB8 ">", b);
  EXPECT_SWPRINTF(L"<00000101>", L"<%08" PRIb8 ">", b);
  EXPECT_SWPRINTF(L"<00000101>", L"<%08" PRIB8 ">", b);

  uint16_t s = 0xaaaa;
  EXPECT_SWPRINTF(L"<1010101010101010>", L"<%" PRIb16 ">", s);
  EXPECT_SWPRINTF(L"<1010101010101010>", L"<%" PRIB16 ">", s);
  EXPECT_SWPRINTF(L"<0b1010101010101010>", L"<%#" PRIb16 ">", s);
  EXPECT_SWPRINTF(L"<0B1010101010101010>", L"<%#" PRIB16 ">", s);

  EXPECT_SWPRINTF(L"<10101010101010101010101010101010>", L"<%" PRIb32 ">", 0xaaaaaaaa);
  EXPECT_SWPRINTF(L"<10101010101010101010101010101010>", L"<%" PRIB32 ">", 0xaaaaaaaa);
  EXPECT_SWPRINTF(L"<0b10101010101010101010101010101010>", L"<%#" PRIb32 ">", 0xaaaaaaaa);
  EXPECT_SWPRINTF(L"<0B10101010101010101010101010101010>", L"<%#" PRIB32 ">", 0xaaaaaaaa);

  // clang doesn't like "%lb" (https://github.com/llvm/llvm-project/issues/62247)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
  EXPECT_SWPRINTF(L"<1010101010101010101010101010101010101010101010101010101010101010>",
                  L"<%" PRIb64 ">", 0xaaaaaaaa'aaaaaaaa);
  EXPECT_SWPRINTF(L"<1010101010101010101010101010101010101010101010101010101010101010>",
                  L"<%" PRIB64 ">", 0xaaaaaaaa'aaaaaaaa);
  EXPECT_SWPRINTF(L"<0b1010101010101010101010101010101010101010101010101010101010101010>",
                  L"<%#" PRIb64 ">", 0xaaaaaaaa'aaaaaaaa);
  EXPECT_SWPRINTF(L"<0B1010101010101010101010101010101010101010101010101010101010101010>",
                  L"<%#" PRIB64 ">", 0xaaaaaaaa'aaaaaaaa);
#pragma clang diagnostic pop

  EXPECT_SWPRINTF(L"<0>", L"<%#b>", 0);
  EXPECT_SWPRINTF(L"<0>", L"<%#B>", 0);
#else
  GTEST_SKIP() << "no %b in glibc";
#endif
}

TEST(STDIO_TEST, scanf_i_decimal) {
  int i;
  EXPECT_EQ(1, sscanf("<123789>", "<%i>", &i));
  EXPECT_EQ(123789, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, sscanf("1234567890abcdefg", "%lli%c", &lli, &ch));
  EXPECT_EQ(1234567890, lli);
  EXPECT_EQ('a', ch);
}

TEST(STDIO_TEST, scanf_i_hex) {
  int i;
  EXPECT_EQ(1, sscanf("<0x123abf>", "<%i>", &i));
  EXPECT_EQ(0x123abf, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, sscanf("0x1234567890abcdefg", "%lli%c", &lli, &ch));
  EXPECT_EQ(0x1234567890abcdefLL, lli);
  EXPECT_EQ('g', ch);
}

TEST(STDIO_TEST, scanf_i_octal) {
  int i;
  EXPECT_EQ(1, sscanf("<01234567>", "<%i>", &i));
  EXPECT_EQ(01234567, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, sscanf("010234567890abcdefg", "%lli%c", &lli, &ch));
  EXPECT_EQ(010234567, lli);
  EXPECT_EQ('8', ch);
}

TEST(STDIO_TEST, scanf_i_binary) {
  int i;
  EXPECT_EQ(1, sscanf("<0b101>", "<%i>", &i));
  EXPECT_EQ(0b101, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, sscanf("0b10234567890abcdefg", "%lli%c", &lli, &ch));
  EXPECT_EQ(0b10, lli);
  EXPECT_EQ('2', ch);
}

TEST(STDIO_TEST, wscanf_i_decimal) {
  int i;
  EXPECT_EQ(1, swscanf(L"<123789>", L"<%i>", &i));
  EXPECT_EQ(123789, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, swscanf(L"1234567890abcdefg", L"%lli%c", &lli, &ch));
  EXPECT_EQ(1234567890, lli);
  EXPECT_EQ('a', ch);
}

TEST(STDIO_TEST, wscanf_i_hex) {
  int i;
  EXPECT_EQ(1, swscanf(L"<0x123abf>", L"<%i>", &i));
  EXPECT_EQ(0x123abf, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, swscanf(L"0x1234567890abcdefg", L"%lli%c", &lli, &ch));
  EXPECT_EQ(0x1234567890abcdefLL, lli);
  EXPECT_EQ('g', ch);
}

TEST(STDIO_TEST, wscanf_i_octal) {
  int i;
  EXPECT_EQ(1, swscanf(L"<01234567>", L"<%i>", &i));
  EXPECT_EQ(01234567, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, swscanf(L"010234567890abcdefg", L"%lli%c", &lli, &ch));
  EXPECT_EQ(010234567, lli);
  EXPECT_EQ('8', ch);
}

TEST(STDIO_TEST, wscanf_i_binary) {
  int i;
  EXPECT_EQ(1, swscanf(L"<0b101>", L"<%i>", &i));
  EXPECT_EQ(0b101, i);

  long long int lli;
  char ch;
  EXPECT_EQ(2, swscanf(L"0b10234567890abcdefg", L"%lli%c", &lli, &ch));
  EXPECT_EQ(0b10, lli);
  EXPECT_EQ('2', ch);
}

TEST(STDIO_TEST, scanf_b) {
  int i;
  char ch;
  EXPECT_EQ(2, sscanf("<1012>", "<%b%c>", &i, &ch));
  EXPECT_EQ(0b101, i);
  EXPECT_EQ('2', ch);
  EXPECT_EQ(1, sscanf("<00000101>", "<%08b>", &i));
  EXPECT_EQ(0b00000101, i);
  EXPECT_EQ(1, sscanf("<0b1010>", "<%b>", &i));
  EXPECT_EQ(0b1010, i);
  EXPECT_EQ(2, sscanf("-0b", "%i%c", &i, &ch));
  EXPECT_EQ(0, i);
  EXPECT_EQ('b', ch);
}

TEST(STDIO_TEST, swscanf_b) {
  int i;
  char ch;
  EXPECT_EQ(2, swscanf(L"<1012>", L"<%b%c>", &i, &ch));
  EXPECT_EQ(0b101, i);
  EXPECT_EQ('2', ch);
  EXPECT_EQ(1, swscanf(L"<00000101>", L"<%08b>", &i));
  EXPECT_EQ(0b00000101, i);
  EXPECT_EQ(1, swscanf(L"<0b1010>", L"<%b>", &i));
  EXPECT_EQ(0b1010, i);
  EXPECT_EQ(2, swscanf(L"-0b", L"%i%c", &i, &ch));
  EXPECT_EQ(0, i);
  EXPECT_EQ('b', ch);
}

TEST(STDIO_TEST, snprintf_w_base) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wconstant-conversion"
  int8_t a = 0b101;
  EXPECT_SNPRINTF("<101>", "<%w8b>", a);
  int8_t b1 = 0xFF;
  EXPECT_SNPRINTF("<-1>", "<%w8d>", b1);
  int8_t b2 = 0x1FF;
  EXPECT_SNPRINTF("<-1>", "<%w8d>", b2);
  int16_t c = 0xFFFF;
  EXPECT_SNPRINTF("<-1>", "<%w16i>", c);
  int32_t d = 021;
  EXPECT_SNPRINTF("<21>", "<%w32o>", d);
  uint32_t e = -1;
  EXPECT_SNPRINTF("<4294967295>", "<%w32u>", e);
  int64_t f = 0x3b;
  EXPECT_SNPRINTF("<3b>", "<%w64x>", f);
  EXPECT_SNPRINTF("<3B>", "<%w64X>", f);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, swprintf_w_base) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wconstant-conversion"
  int8_t a = 0b101;
  EXPECT_SWPRINTF(L"<101>", L"<%w8b>", a);
  int8_t b1 = 0xFF;
  EXPECT_SWPRINTF(L"<-1>", L"<%w8d>", b1);
  int8_t b2 = 0x1FF;
  EXPECT_SWPRINTF(L"<-1>", L"<%w8d>", b2);
  int16_t c = 0xFFFF;
  EXPECT_SWPRINTF(L"<-1>", L"<%w16i>", c);
  int32_t d = 021;
  EXPECT_SWPRINTF(L"<21>", L"<%w32o>", d);
  uint32_t e = -1;
  EXPECT_SWPRINTF(L"<4294967295>", L"<%w32u>", e);
  int64_t f = 0x3b;
  EXPECT_SWPRINTF(L"<3b>", L"<%w64x>", f);
  EXPECT_SWPRINTF(L"<3B>", L"<%w64X>", f);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, snprintf_w_arguments_reordering) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wformat-extra-args"
  int32_t a = 0xaaaaaaaa;
  int64_t b = 0x11111111'22222222;
  int64_t c = 0x33333333'44444444;
  int64_t d = 0xaaaaaaaa'aaaaaaaa;
  EXPECT_SNPRINTF("<10101010101010101010101010101010 --- 3333333344444444>",
                  "<%2$w32b --- %1$w64x>", c, a);
  EXPECT_SNPRINTF(
      "<1010101010101010101010101010101010101010101010101010101010101010 --- 1111111122222222 --- "
      "3333333344444444>",
      "<%3$w64b --- %1$w64x --- %2$w64x>", b, c, d);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, swprintf_w_arguments_reordering) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wformat-extra-args"
  int32_t a = 0xaaaaaaaa;
  int64_t b = 0x11111111'22222222;
  int64_t c = 0x33333333'44444444;
  int64_t d = 0xaaaaaaaa'aaaaaaaa;
  EXPECT_SWPRINTF(L"<10101010101010101010101010101010 --- 3333333344444444>",
                  L"<%2$w32b --- %1$w64x>", c, a);
  EXPECT_SWPRINTF(
      L"<1010101010101010101010101010101010101010101010101010101010101010 --- 1111111122222222 --- "
      L"3333333344444444>",
      L"<%3$w64b --- %1$w64x --- %2$w64x>", b, c, d);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST_F(STDIO_DEATHTEST, snprintf_invalid_w_width) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  char buf[BUFSIZ];
  int32_t a = 100;
  EXPECT_DEATH(snprintf(buf, sizeof(buf), "%w20d", &a), "%w20 is unsupported");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST_F(STDIO_DEATHTEST, swprintf_invalid_w_width) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  wchar_t buf[BUFSIZ];
  int32_t a = 100;
  EXPECT_DEATH(swprintf(buf, sizeof(buf), L"%w20d", &a), "%w20 is unsupported");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, snprintf_wf_base) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconstant-conversion"
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int_fast8_t a = 0b101;
  EXPECT_SNPRINTF("<101>", "<%wf8b>", a);
  int_fast8_t b = 0x12341234'12341234;
  EXPECT_SNPRINTF("<34>", "<%wf8x>", b);
  uint_fast16_t c = 0x11111111'22222222;
#if defined(__LP64__)
  EXPECT_SNPRINTF("<1111111122222222>", "<%wf16x>", c);
#else
  EXPECT_SNPRINTF("<22222222>", "<%wf16x>", c);
#endif
  int_fast32_t d = 0x33333333'44444444;
#if defined(__LP64__)
  EXPECT_SNPRINTF("<3333333344444444>", "<%wf32x>", d);
#else
  EXPECT_SNPRINTF("<44444444>", "<%wf32x>", d);
#endif
  int_fast64_t e = 0xaaaaaaaa'aaaaaaaa;
  EXPECT_SNPRINTF("<aaaaaaaaaaaaaaaa>", "<%wf64x>", e);
  EXPECT_SNPRINTF("<AAAAAAAAAAAAAAAA>", "<%wf64X>", e);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %wf in glibc";
#endif
}
TEST(STDIO_TEST, swprintf_wf_base) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconstant-conversion"
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int_fast8_t a = 0b101;
  EXPECT_SWPRINTF(L"<101>", L"<%wf8b>", a);
  int_fast8_t b = 0x12341234'12341234;
  EXPECT_SWPRINTF(L"<34>", L"<%wf8x>", b);
  uint_fast16_t c = 0x11111111'22222222;
#if defined(__LP64__)
  EXPECT_SWPRINTF(L"<1111111122222222>", L"<%wf16x>", c);
#else
  EXPECT_SWPRINTF(L"<22222222>", L"<%wf16x>", c);
#endif
  int_fast32_t d = 0x33333333'44444444;
#if defined(__LP64__)
  EXPECT_SWPRINTF(L"<3333333344444444>", L"<%wf32x>", d);
#else
  EXPECT_SWPRINTF(L"<44444444>", L"<%wf32x>", d);
#endif
  int_fast64_t e = 0xaaaaaaaa'aaaaaaaa;
  EXPECT_SWPRINTF(L"<aaaaaaaaaaaaaaaa>", L"<%wf64x>", e);
  EXPECT_SWPRINTF(L"<AAAAAAAAAAAAAAAA>", L"<%wf64X>", e);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %wf in glibc";
#endif
}

TEST(STDIO_TEST, snprintf_wf_arguments_reordering) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconstant-conversion"
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-extra-args"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int_fast16_t a = 0x11111111'22222222;
  int_fast32_t b = 0x33333333'44444444;
  int_fast32_t c = 0xaaaaaaaa'aaaaaaaa;
#if defined(__LP64__)
  EXPECT_SNPRINTF(
      "<3333333344444444 --- 1010101010101010101010101010101010101010101010101010101010101010>",
      "<%2$wf32x --- %1$wf32b>", c, b);

  EXPECT_SNPRINTF(
      "<1010101010101010101010101010101010101010101010101010101010101010 --- 1111111122222222 --- "
      "3333333344444444>",
      "<%3$wf32b --- %1$wf16x --- %2$wf32x>", a, b, c);
#else
  EXPECT_SNPRINTF("<44444444 --- 10101010101010101010101010101010>", "<%2$wf32x --- %1$wf32b>", c,
                  b);
  EXPECT_SNPRINTF("<10101010101010101010101010101010 --- 22222222 --- 44444444>",
                  "<%3$wf32b --- %1$wf16x --- %2$wf32x>", a, b, c);
#endif
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, swprintf_wf_arguments_reordering) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconstant-conversion"
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-extra-args"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int_fast16_t a = 0x11111111'22222222;
  int_fast32_t b = 0x33333333'44444444;
  int_fast32_t c = 0xaaaaaaaa'aaaaaaaa;
#if defined(__LP64__)
  EXPECT_SWPRINTF(
      L"<3333333344444444 --- 1010101010101010101010101010101010101010101010101010101010101010>",
      L"<%2$wf32x --- %1$wf32b>", c, b);

  EXPECT_SWPRINTF(
      L"<1010101010101010101010101010101010101010101010101010101010101010 --- 1111111122222222 --- "
      L"3333333344444444>",
      L"<%3$wf32b --- %1$wf16x --- %2$wf32x>", a, b, c);
#else
  EXPECT_SWPRINTF(L"<44444444 --- 10101010101010101010101010101010>", L"<%2$wf32x --- %1$wf32b>", c,
                  b);
  EXPECT_SWPRINTF(L"<10101010101010101010101010101010 --- 22222222 --- 44444444>",
                  L"<%3$wf32b --- %1$wf16x --- %2$wf32x>", a, b, c);
#endif
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST_F(STDIO_DEATHTEST, snprintf_invalid_wf_width) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  char buf[BUFSIZ];
  int_fast32_t a = 100;
  EXPECT_DEATH(snprintf(buf, sizeof(buf), "%wf20d", &a), "%wf20 is unsupported");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST_F(STDIO_DEATHTEST, swprintf_invalid_wf_width) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  wchar_t buf[BUFSIZ];
  int_fast32_t a = 100;
  EXPECT_DEATH(swprintf(buf, sizeof(buf), L"%wf20d", &a), "%wf20 is unsupported");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, sscanf_w_or_wf_base) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int8_t a;
  EXPECT_EQ(1, sscanf("<0b101>", "<%w8b>", &a));
  EXPECT_EQ(0b101, a);
  int_fast8_t fast_a;
  EXPECT_EQ(1, sscanf("<0b101>", "<%wf8b>", &fast_a));
  EXPECT_EQ(0b101, fast_a);
  int8_t b1;
  EXPECT_EQ(1, sscanf("<0xFF>", "<%w8i>", &b1));
  EXPECT_EQ(-1, b1);
  int8_t b2;
  EXPECT_EQ(1, sscanf("<0x1FF>", "<%w8i>", &b2));
  EXPECT_EQ(-1, b2);
  int_fast8_t fast_b;
  EXPECT_EQ(1, sscanf("<0x1234123412341234>", "<%wf8x>", &fast_b));
  EXPECT_EQ(0x34, fast_b);
  int16_t c1;
  EXPECT_EQ(1, sscanf("<0xFFFF>", "<%w16i>", &c1));
  EXPECT_EQ(-1, c1);
  uint16_t c2;
  EXPECT_EQ(1, sscanf("<64>", "<%w16d>", &c2));
  EXPECT_EQ(64, c2);
  int_fast16_t fast_c;
#if defined(__LP64__)
  EXPECT_EQ(1, sscanf("<0x1111111122222222>", "<%wf16x>", &fast_c));
  EXPECT_EQ(0x1111111122222222, fast_c);
#else
  EXPECT_EQ(1, sscanf("<0x1111111122222222>", "<%wf16x>", &fast_c));
  EXPECT_EQ(0x22222222, fast_c);
#endif
  int32_t d;
  EXPECT_EQ(1, sscanf("<021>", "<%w32o>", &d));
  EXPECT_EQ(021, d);
  int_fast32_t fast_d;
#if defined(__LP64__)
  EXPECT_EQ(1, sscanf("<0x3333333344444444>", "<%wf32x>", &fast_d));
  EXPECT_EQ(0x3333333344444444, fast_d);
#else
  EXPECT_EQ(1, sscanf("<0x3333333344444444>", "<%wf32x>", &fast_d));
  EXPECT_EQ(0x44444444, fast_d);
#endif
  uint32_t e;
  EXPECT_EQ(1, sscanf("<-1>", "<%w32u>", &e));
  EXPECT_EQ(4294967295, e);
  int64_t f;
  EXPECT_EQ(1, sscanf("<0x3b>", "<%w64x>", &f));
  EXPECT_EQ(0x3b, f);
  EXPECT_EQ(1, sscanf("<0x3b>", "<%w64X>", &f));
  EXPECT_EQ(0x3B, f);
  uint_fast64_t fast_f;
  EXPECT_EQ(1, sscanf("<0xaaaaaaaa>", "<%wf64x>", &fast_f));
  EXPECT_EQ(0xaaaaaaaa, fast_f);
  EXPECT_EQ(1, sscanf("<0xaaaaaaaa>", "<%wf64X>", &fast_f));
  EXPECT_EQ(0xAAAAAAAA, fast_f);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, sscanf_w_combination) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wformat-extra-args"
  uint32_t a;
  int64_t b;
  char c;

  EXPECT_EQ(3, sscanf("<0b10101010101010101010101010101010 0x3333333344444444 1>",
                      "<%w32b %w64x %c>", &a, &b, &c));
  EXPECT_EQ(0xaaaaaaaa, a);
  EXPECT_EQ(0x3333333344444444, b);
  EXPECT_EQ('1', c);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST_F(STDIO_DEATHTEST, sscanf_invalid_w_or_wf_width) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int32_t a;
  EXPECT_DEATH(sscanf("<100>", "<%w20d>", &a), "%w20 is unsupported");
  int_fast32_t fast_a;
  EXPECT_DEATH(sscanf("<100>", "<%wf20d>", &fast_a), "%wf20 is unsupported");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, swscanf_w_or_wf_base) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int8_t a;
  EXPECT_EQ(1, swscanf(L"<0b101>", L"<%w8b>", &a));
  EXPECT_EQ(0b101, a);
  int_fast8_t fast_a;
  EXPECT_EQ(1, swscanf(L"<0b101>", L"<%wf8b>", &fast_a));
  EXPECT_EQ(0b101, fast_a);
  int8_t b1;
  EXPECT_EQ(1, swscanf(L"<0xFF>", L"<%w8i>", &b1));
  EXPECT_EQ(-1, b1);
  int8_t b2;
  EXPECT_EQ(1, swscanf(L"<0x1FF>", L"<%w8i>", &b2));
  EXPECT_EQ(-1, b2);
  int_fast8_t fast_b;
  EXPECT_EQ(1, swscanf(L"<0x1234123412341234>", L"<%wf8i>", &fast_b));
  EXPECT_EQ(0x34, fast_b);
  int16_t c1;
  EXPECT_EQ(1, swscanf(L"<0xFFFF>", L"<%w16i>", &c1));
  EXPECT_EQ(-1, c1);
  uint16_t c2;
  EXPECT_EQ(1, swscanf(L"<64>", L"<%w16d>", &c2));
  EXPECT_EQ(64, c2);
  int_fast16_t fast_c;
#if defined(__LP64__)
  EXPECT_EQ(1, swscanf(L"<0x1111111122222222>", L"<%wf16x>", &fast_c));
  EXPECT_EQ(0x1111111122222222, fast_c);
#else
  EXPECT_EQ(1, swscanf(L"<0x1111111122222222>", L"<%wf16x>", &fast_c));
  EXPECT_EQ(0x22222222, fast_c);
#endif
  int32_t d;
  EXPECT_EQ(1, swscanf(L"<021>", L"<%w32o>", &d));
  EXPECT_EQ(021, d);
  int_fast32_t fast_d;
#if defined(__LP64__)
  EXPECT_EQ(1, swscanf(L"<0x3333333344444444>", L"<%wf32x>", &fast_d));
  EXPECT_EQ(0x3333333344444444, fast_d);
#else
  EXPECT_EQ(1, swscanf(L"<0x3333333344444444>", L"<%wf32x>", &fast_d));
  EXPECT_EQ(0x44444444, fast_d);
#endif
  uint32_t e;
  EXPECT_EQ(1, swscanf(L"<-1>", L"<%w32u>", &e));
  EXPECT_EQ(4294967295, e);
  int64_t f;
  EXPECT_EQ(1, swscanf(L"<0x3b>", L"<%w64x>", &f));
  EXPECT_EQ(0x3b, f);
  EXPECT_EQ(1, swscanf(L"<0x3b>", L"<%w64X>", &f));
  EXPECT_EQ(0x3B, f);
  uint_fast64_t fast_f;
  EXPECT_EQ(1, swscanf(L"<0xaaaaaaaa>", L"<%wf64x>", &fast_f));
  EXPECT_EQ(0xaaaaaaaa, fast_f);
  EXPECT_EQ(1, swscanf(L"<0xaaaaaaaa>", L"<%wf64X>", &fast_f));
  EXPECT_EQ(0xAAAAAAAA, fast_f);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, swscanf_w_combination) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
#pragma clang diagnostic ignored "-Wformat-extra-args"
  uint32_t a;
  int64_t b;
  char c;

  EXPECT_EQ(3, swscanf(L"<0b10101010101010101010101010101010 0x3333333344444444 1>",
                       L"<%w32b %w64x %c>", &a, &b, &c));
  EXPECT_EQ(0xaaaaaaaa, a);
  EXPECT_EQ(0x3333333344444444, b);
  EXPECT_EQ('1', c);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST_F(STDIO_DEATHTEST, swscanf_invalid_w_or_wf_width) {
#if defined(__BIONIC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-invalid-specifier"
  int32_t a;
  EXPECT_DEATH(swscanf(L"<100>", L"<%w20d>", &a), "%w20 is unsupported");
  int_fast32_t fast_a;
  EXPECT_DEATH(swscanf(L"<100>", L"<%wf20d>", &fast_a), "%wf20 is unsupported");
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "no %w in glibc";
#endif
}

TEST(STDIO_TEST, printf_lc_0) {
  // https://austingroupbugs.net/view.php?id=1647
  char buf[BUFSIZ];
  EXPECT_EQ(3, snprintf(buf, sizeof(buf), "<%lc>", L'\0'));
  EXPECT_TRUE(!memcmp(buf, "<\0>", 3));
}
