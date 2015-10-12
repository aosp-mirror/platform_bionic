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
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>

#include <vector>

#include "TemporaryFile.h"

TEST(stdio, flockfile_18208568_stderr) {
  // Check that we have a _recursive_ mutex for flockfile.
  flockfile(stderr);
  feof(stderr); // We don't care about the result, but this needs to take the lock.
  funlockfile(stderr);
}

TEST(stdio, flockfile_18208568_regular) {
  // We never had a bug for streams other than stdin/stdout/stderr, but test anyway.
  FILE* fp = fopen("/dev/null", "w");
  ASSERT_TRUE(fp != NULL);
  flockfile(fp);
  feof(fp);
  funlockfile(fp);
  fclose(fp);
}

TEST(stdio, tmpfile_fileno_fprintf_rewind_fgets) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

  int fd = fileno(fp);
  ASSERT_NE(fd, -1);

  struct stat sb;
  int rc = fstat(fd, &sb);
  ASSERT_NE(rc, -1);
  ASSERT_EQ(sb.st_mode & 0777, 0600U);

  rc = fprintf(fp, "hello\n");
  ASSERT_EQ(rc, 6);

  rewind(fp);

  char buf[16];
  char* s = fgets(buf, sizeof(buf), fp);
  ASSERT_TRUE(s != NULL);
  ASSERT_STREQ("hello\n", s);

  fclose(fp);
}

TEST(stdio, dprintf) {
  TemporaryFile tf;

  int rc = dprintf(tf.fd, "hello\n");
  ASSERT_EQ(rc, 6);

  lseek(tf.fd, 0, SEEK_SET);
  FILE* tfile = fdopen(tf.fd, "r");
  ASSERT_TRUE(tfile != NULL);

  char buf[7];
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), tfile));
  ASSERT_STREQ("hello\n", buf);
  // Make sure there isn't anything else in the file.
  ASSERT_EQ(NULL, fgets(buf, sizeof(buf), tfile));
  fclose(tfile);
}

TEST(stdio, getdelim) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

  const char* line_written = "This  is a test";
  int rc = fprintf(fp, "%s", line_written);
  ASSERT_EQ(rc, static_cast<int>(strlen(line_written)));

  rewind(fp);

  char* word_read = NULL;
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
  ASSERT_EQ(0, errno);
  ASSERT_TRUE(feof(fp));

  free(word_read);
  fclose(fp);
}

TEST(stdio, getdelim_invalid) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

  char* buffer = NULL;
  size_t buffer_length = 0;

  // The first argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getdelim(NULL, &buffer_length, ' ', fp), -1);
  ASSERT_EQ(EINVAL, errno);

  // The second argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getdelim(&buffer, NULL, ' ', fp), -1);
  ASSERT_EQ(EINVAL, errno);

  // The underlying fd can't be closed.
  ASSERT_EQ(0, close(fileno(fp)));
  errno = 0;
  ASSERT_EQ(getdelim(&buffer, &buffer_length, ' ', fp), -1);
  ASSERT_EQ(EBADF, errno);
  fclose(fp);
}

TEST(stdio, getdelim_directory) {
  FILE* fp = fopen("/proc", "r");
  ASSERT_TRUE(fp != NULL);
  char* word_read;
  size_t allocated_length;
  ASSERT_EQ(-1, getdelim(&word_read, &allocated_length, ' ', fp));
  fclose(fp);
}

TEST(stdio, getline) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

  const char* line_written = "This is a test for getline\n";
  const size_t line_count = 5;

  for (size_t i = 0; i < line_count; ++i) {
    int rc = fprintf(fp, "%s", line_written);
    ASSERT_EQ(rc, static_cast<int>(strlen(line_written)));
  }

  rewind(fp);

  char* line_read = NULL;
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
  ASSERT_EQ(0, errno);
  ASSERT_TRUE(feof(fp));

  free(line_read);
  fclose(fp);
}

TEST(stdio, getline_invalid) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

  char* buffer = NULL;
  size_t buffer_length = 0;

  // The first argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getline(NULL, &buffer_length, fp), -1);
  ASSERT_EQ(EINVAL, errno);

  // The second argument can't be NULL.
  errno = 0;
  ASSERT_EQ(getline(&buffer, NULL, fp), -1);
  ASSERT_EQ(EINVAL, errno);

  // The underlying fd can't be closed.
  ASSERT_EQ(0, close(fileno(fp)));
  errno = 0;
  ASSERT_EQ(getline(&buffer, &buffer_length, fp), -1);
  ASSERT_EQ(EBADF, errno);
  fclose(fp);
}

TEST(stdio, printf_ssize_t) {
  // http://b/8253769
  ASSERT_EQ(sizeof(ssize_t), sizeof(long int));
  ASSERT_EQ(sizeof(ssize_t), sizeof(size_t));
  // For our 32-bit ABI, we had a ssize_t definition that confuses GCC into saying:
  // error: format '%zd' expects argument of type 'signed size_t',
  //     but argument 4 has type 'ssize_t {aka long int}' [-Werror=format]
  ssize_t v = 1;
  char buf[32];
  snprintf(buf, sizeof(buf), "%zd", v);
}

// https://code.google.com/p/android/issues/detail?id=64886
TEST(stdio, snprintf_a) {
  char buf[BUFSIZ];
  EXPECT_EQ(23, snprintf(buf, sizeof(buf), "<%a>", 9990.235));
  EXPECT_STREQ("<0x1.3831e147ae148p+13>", buf);
}

TEST(stdio, snprintf_lc) {
  char buf[BUFSIZ];
  wint_t wc = L'a';
  EXPECT_EQ(3, snprintf(buf, sizeof(buf), "<%lc>", wc));
  EXPECT_STREQ("<a>", buf);
}

TEST(stdio, snprintf_ls) {
  char buf[BUFSIZ];
  wchar_t* ws = NULL;
  EXPECT_EQ(8, snprintf(buf, sizeof(buf), "<%ls>", ws));
  EXPECT_STREQ("<(null)>", buf);

  wchar_t chars[] = { L'h', L'i', 0 };
  ws = chars;
  EXPECT_EQ(4, snprintf(buf, sizeof(buf), "<%ls>", ws));
  EXPECT_STREQ("<hi>", buf);
}

TEST(stdio, snprintf_n) {
#if defined(__BIONIC__)
  // http://b/14492135
  char buf[32];
  int i = 1234;
  EXPECT_EQ(5, snprintf(buf, sizeof(buf), "a %n b", &i));
  EXPECT_EQ(1234, i);
  EXPECT_STREQ("a n b", buf);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

TEST(stdio, snprintf_smoke) {
  char buf[BUFSIZ];

  snprintf(buf, sizeof(buf), "a");
  EXPECT_STREQ("a", buf);

  snprintf(buf, sizeof(buf), "%%");
  EXPECT_STREQ("%", buf);

  snprintf(buf, sizeof(buf), "01234");
  EXPECT_STREQ("01234", buf);

  snprintf(buf, sizeof(buf), "a%sb", "01234");
  EXPECT_STREQ("a01234b", buf);

  char* s = NULL;
  snprintf(buf, sizeof(buf), "a%sb", s);
  EXPECT_STREQ("a(null)b", buf);

  snprintf(buf, sizeof(buf), "aa%scc", "bb");
  EXPECT_STREQ("aabbcc", buf);

  snprintf(buf, sizeof(buf), "a%cc", 'b');
  EXPECT_STREQ("abc", buf);

  snprintf(buf, sizeof(buf), "a%db", 1234);
  EXPECT_STREQ("a1234b", buf);

  snprintf(buf, sizeof(buf), "a%db", -8123);
  EXPECT_STREQ("a-8123b", buf);

  snprintf(buf, sizeof(buf), "a%hdb", static_cast<short>(0x7fff0010));
  EXPECT_STREQ("a16b", buf);

  snprintf(buf, sizeof(buf), "a%hhdb", static_cast<char>(0x7fffff10));
  EXPECT_STREQ("a16b", buf);

  snprintf(buf, sizeof(buf), "a%lldb", 0x1000000000LL);
  EXPECT_STREQ("a68719476736b", buf);

  snprintf(buf, sizeof(buf), "a%ldb", 70000L);
  EXPECT_STREQ("a70000b", buf);

  snprintf(buf, sizeof(buf), "a%pb", reinterpret_cast<void*>(0xb0001234));
  EXPECT_STREQ("a0xb0001234b", buf);

  snprintf(buf, sizeof(buf), "a%xz", 0x12ab);
  EXPECT_STREQ("a12abz", buf);

  snprintf(buf, sizeof(buf), "a%Xz", 0x12ab);
  EXPECT_STREQ("a12ABz", buf);

  snprintf(buf, sizeof(buf), "a%08xz", 0x123456);
  EXPECT_STREQ("a00123456z", buf);

  snprintf(buf, sizeof(buf), "a%5dz", 1234);
  EXPECT_STREQ("a 1234z", buf);

  snprintf(buf, sizeof(buf), "a%05dz", 1234);
  EXPECT_STREQ("a01234z", buf);

  snprintf(buf, sizeof(buf), "a%8dz", 1234);
  EXPECT_STREQ("a    1234z", buf);

  snprintf(buf, sizeof(buf), "a%-8dz", 1234);
  EXPECT_STREQ("a1234    z", buf);

  snprintf(buf, sizeof(buf), "A%-11sZ", "abcdef");
  EXPECT_STREQ("Aabcdef     Z", buf);

  snprintf(buf, sizeof(buf), "A%s:%dZ", "hello", 1234);
  EXPECT_STREQ("Ahello:1234Z", buf);

  snprintf(buf, sizeof(buf), "a%03d:%d:%02dz", 5, 5, 5);
  EXPECT_STREQ("a005:5:05z", buf);

  void* p = NULL;
  snprintf(buf, sizeof(buf), "a%d,%pz", 5, p);
#if defined(__BIONIC__)
  EXPECT_STREQ("a5,0x0z", buf);
#else // __BIONIC__
  EXPECT_STREQ("a5,(nil)z", buf);
#endif // __BIONIC__

  snprintf(buf, sizeof(buf), "a%lld,%d,%d,%dz", 0x1000000000LL, 6, 7, 8);
  EXPECT_STREQ("a68719476736,6,7,8z", buf);

  snprintf(buf, sizeof(buf), "a_%f_b", 1.23f);
  EXPECT_STREQ("a_1.230000_b", buf);

  snprintf(buf, sizeof(buf), "a_%g_b", 3.14);
  EXPECT_STREQ("a_3.14_b", buf);

  snprintf(buf, sizeof(buf), "%1$s %1$s", "print_me_twice");
  EXPECT_STREQ("print_me_twice print_me_twice", buf);
}

template <typename T>
void CheckInfNan(int snprintf_fn(T*, size_t, const T*, ...),
                 const T* fmt, const T* fmt_plus,
                 const T* minus_inf, const T* inf_, const T* plus_inf,
                 const T* minus_nan, const T* nan_, const T* plus_nan) {
  T buf[BUFSIZ];

  snprintf_fn(buf, sizeof(buf), fmt, nan(""));
  EXPECT_STREQ(nan_, buf) << fmt;
  snprintf_fn(buf, sizeof(buf), fmt, -nan(""));
  EXPECT_STREQ(minus_nan, buf) << fmt;
  snprintf_fn(buf, sizeof(buf), fmt_plus, nan(""));
  EXPECT_STREQ(plus_nan, buf) << fmt_plus;
  snprintf_fn(buf, sizeof(buf), fmt_plus, -nan(""));
  EXPECT_STREQ(minus_nan, buf) << fmt_plus;

  snprintf_fn(buf, sizeof(buf), fmt, HUGE_VAL);
  EXPECT_STREQ(inf_, buf) << fmt;
  snprintf_fn(buf, sizeof(buf), fmt, -HUGE_VAL);
  EXPECT_STREQ(minus_inf, buf) << fmt;
  snprintf_fn(buf, sizeof(buf), fmt_plus, HUGE_VAL);
  EXPECT_STREQ(plus_inf, buf) << fmt_plus;
  snprintf_fn(buf, sizeof(buf), fmt_plus, -HUGE_VAL);
  EXPECT_STREQ(minus_inf, buf) << fmt_plus;
}

TEST(stdio, snprintf_inf_nan) {
  CheckInfNan(snprintf, "%a", "%+a", "-inf", "inf", "+inf", "-nan", "nan", "+nan");
  CheckInfNan(snprintf, "%A", "%+A", "-INF", "INF", "+INF", "-NAN", "NAN", "+NAN");
  CheckInfNan(snprintf, "%e", "%+e", "-inf", "inf", "+inf", "-nan", "nan", "+nan");
  CheckInfNan(snprintf, "%E", "%+E", "-INF", "INF", "+INF", "-NAN", "NAN", "+NAN");
  CheckInfNan(snprintf, "%f", "%+f", "-inf", "inf", "+inf", "-nan", "nan", "+nan");
  CheckInfNan(snprintf, "%F", "%+F", "-INF", "INF", "+INF", "-NAN", "NAN", "+NAN");
  CheckInfNan(snprintf, "%g", "%+g", "-inf", "inf", "+inf", "-nan", "nan", "+nan");
  CheckInfNan(snprintf, "%G", "%+G", "-INF", "INF", "+INF", "-NAN", "NAN", "+NAN");
}

TEST(stdio, wsprintf_inf_nan) {
  CheckInfNan(swprintf, L"%a", L"%+a", L"-inf", L"inf", L"+inf", L"-nan", L"nan", L"+nan");
  CheckInfNan(swprintf, L"%A", L"%+A", L"-INF", L"INF", L"+INF", L"-NAN", L"NAN", L"+NAN");
  CheckInfNan(swprintf, L"%e", L"%+e", L"-inf", L"inf", L"+inf", L"-nan", L"nan", L"+nan");
  CheckInfNan(swprintf, L"%E", L"%+E", L"-INF", L"INF", L"+INF", L"-NAN", L"NAN", L"+NAN");
  CheckInfNan(swprintf, L"%f", L"%+f", L"-inf", L"inf", L"+inf", L"-nan", L"nan", L"+nan");
  CheckInfNan(swprintf, L"%F", L"%+F", L"-INF", L"INF", L"+INF", L"-NAN", L"NAN", L"+NAN");
  CheckInfNan(swprintf, L"%g", L"%+g", L"-inf", L"inf", L"+inf", L"-nan", L"nan", L"+nan");
  CheckInfNan(swprintf, L"%G", L"%+G", L"-INF", L"INF", L"+INF", L"-NAN", L"NAN", L"+NAN");
}

TEST(stdio, snprintf_d_INT_MAX) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%d", INT_MAX);
  EXPECT_STREQ("2147483647", buf);
}

TEST(stdio, snprintf_d_INT_MIN) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%d", INT_MIN);
  EXPECT_STREQ("-2147483648", buf);
}

TEST(stdio, snprintf_ld_LONG_MAX) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%ld", LONG_MAX);
#if __LP64__
  EXPECT_STREQ("9223372036854775807", buf);
#else
  EXPECT_STREQ("2147483647", buf);
#endif
}

TEST(stdio, snprintf_ld_LONG_MIN) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%ld", LONG_MIN);
#if __LP64__
  EXPECT_STREQ("-9223372036854775808", buf);
#else
  EXPECT_STREQ("-2147483648", buf);
#endif
}

TEST(stdio, snprintf_lld_LLONG_MAX) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%lld", LLONG_MAX);
  EXPECT_STREQ("9223372036854775807", buf);
}

TEST(stdio, snprintf_lld_LLONG_MIN) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%lld", LLONG_MIN);
  EXPECT_STREQ("-9223372036854775808", buf);
}

TEST(stdio, snprintf_e) {
  char buf[BUFSIZ];

  snprintf(buf, sizeof(buf), "%e", 1.5);
  EXPECT_STREQ("1.500000e+00", buf);

  snprintf(buf, sizeof(buf), "%Le", 1.5l);
  EXPECT_STREQ("1.500000e+00", buf);
}

TEST(stdio, snprintf_negative_zero_5084292) {
  char buf[BUFSIZ];

  snprintf(buf, sizeof(buf), "%e", -0.0);
  EXPECT_STREQ("-0.000000e+00", buf);
  snprintf(buf, sizeof(buf), "%E", -0.0);
  EXPECT_STREQ("-0.000000E+00", buf);
  snprintf(buf, sizeof(buf), "%f", -0.0);
  EXPECT_STREQ("-0.000000", buf);
  snprintf(buf, sizeof(buf), "%F", -0.0);
  EXPECT_STREQ("-0.000000", buf);
  snprintf(buf, sizeof(buf), "%g", -0.0);
  EXPECT_STREQ("-0", buf);
  snprintf(buf, sizeof(buf), "%G", -0.0);
  EXPECT_STREQ("-0", buf);
  snprintf(buf, sizeof(buf), "%a", -0.0);
  EXPECT_STREQ("-0x0p+0", buf);
  snprintf(buf, sizeof(buf), "%A", -0.0);
  EXPECT_STREQ("-0X0P+0", buf);
}

TEST(stdio, snprintf_utf8_15439554) {
  locale_t cloc = newlocale(LC_ALL, "C.UTF-8", 0);
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

TEST(stdio, fprintf_failures_7229520) {
  // http://b/7229520
  FILE* fp;

  // Unbuffered case where the fprintf(3) itself fails.
  ASSERT_NE(nullptr, fp = tmpfile());
  setbuf(fp, NULL);
  ASSERT_EQ(4, fprintf(fp, "epic"));
  ASSERT_EQ(0, close(fileno(fp)));
  ASSERT_EQ(-1, fprintf(fp, "fail"));
  ASSERT_EQ(-1, fclose(fp));

  // Buffered case where we won't notice until the fclose(3).
  // It's likely this is what was actually seen in http://b/7229520,
  // and that expecting fprintf to fail is setting yourself up for
  // disappointment. Remember to check fclose(3)'s return value, kids!
  ASSERT_NE(nullptr, fp = tmpfile());
  ASSERT_EQ(4, fprintf(fp, "epic"));
  ASSERT_EQ(0, close(fileno(fp)));
  ASSERT_EQ(4, fprintf(fp, "fail"));
  ASSERT_EQ(-1, fclose(fp));
}

TEST(stdio, popen) {
  FILE* fp = popen("cat /proc/version", "r");
  ASSERT_TRUE(fp != NULL);

  char buf[16];
  char* s = fgets(buf, sizeof(buf), fp);
  buf[13] = '\0';
  ASSERT_STREQ("Linux version", s);

  ASSERT_EQ(0, pclose(fp));
}

TEST(stdio, getc) {
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_TRUE(fp != NULL);
  ASSERT_EQ('L', getc(fp));
  ASSERT_EQ('i', getc(fp));
  ASSERT_EQ('n', getc(fp));
  ASSERT_EQ('u', getc(fp));
  ASSERT_EQ('x', getc(fp));
  fclose(fp);
}

TEST(stdio, putc) {
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_TRUE(fp != NULL);
  ASSERT_EQ(EOF, putc('x', fp));
  fclose(fp);
}

TEST(stdio, sscanf) {
  char s1[123];
  int i1;
  double d1;
  char s2[123];
  ASSERT_EQ(3, sscanf("  hello 123 1.23 ", "%s %i %lf %s", s1, &i1, &d1, s2));
  ASSERT_STREQ("hello", s1);
  ASSERT_EQ(123, i1);
  ASSERT_DOUBLE_EQ(1.23, d1);
}

TEST(stdio, cantwrite_EBADF) {
  // If we open a file read-only...
  FILE* fp = fopen("/proc/version", "r");

  // ...all attempts to write to that file should return failure.

  // They should also set errno to EBADF. This isn't POSIX, but it's traditional.
  // glibc gets the wide-character functions wrong.

  errno = 0;
  EXPECT_EQ(EOF, putc('x', fp));
  EXPECT_EQ(EBADF, errno);

  errno = 0;
  EXPECT_EQ(EOF, fprintf(fp, "hello"));
  EXPECT_EQ(EBADF, errno);

  errno = 0;
  EXPECT_EQ(EOF, fwprintf(fp, L"hello"));
#if defined(__BIONIC__)
  EXPECT_EQ(EBADF, errno);
#endif

  errno = 0;
  EXPECT_EQ(0U, fwrite("hello", 1, 2, fp));
  EXPECT_EQ(EBADF, errno);

  errno = 0;
  EXPECT_EQ(EOF, fputs("hello", fp));
  EXPECT_EQ(EBADF, errno);

  errno = 0;
  EXPECT_EQ(WEOF, fputwc(L'x', fp));
#if defined(__BIONIC__)
  EXPECT_EQ(EBADF, errno);
#endif
}

// Tests that we can only have a consistent and correct fpos_t when using
// f*pos functions (i.e. fpos doesn't get inside a multi byte character).
TEST(stdio, consistent_fpos_t) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

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
TEST(stdio, fpos_t_and_seek) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // In glibc-2.16 fseek doesn't work properly in wide mode
  // (https://sourceware.org/bugzilla/show_bug.cgi?id=14543). One workaround is
  // to close and re-open the file. We do it in order to make the test pass
  // with all glibcs.

  TemporaryFile tf;
  FILE* fp = fdopen(tf.fd, "w+");
  ASSERT_TRUE(fp != NULL);

  wchar_t mb_two_bytes = 0x00a2;
  wchar_t mb_three_bytes = 0x20ac;
  wchar_t mb_four_bytes = 0x24b62;

  // Write to file.
  ASSERT_EQ(mb_two_bytes, static_cast<wchar_t>(fputwc(mb_two_bytes, fp)));
  ASSERT_EQ(mb_three_bytes, static_cast<wchar_t>(fputwc(mb_three_bytes, fp)));
  ASSERT_EQ(mb_four_bytes, static_cast<wchar_t>(fputwc(mb_four_bytes, fp)));

  fflush(fp);
  fclose(fp);

  fp = fopen(tf.filename, "r");
  ASSERT_TRUE(fp != NULL);

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
  ASSERT_EQ(EILSEQ, errno);

  // Reverting to a valid position should work.
  ASSERT_EQ(0, fsetpos(fp, &mb_two_bytes_pos));
  ASSERT_EQ(mb_two_bytes, static_cast<wchar_t>(fgetwc(fp)));

  // Moving withing a multi byte with fsetpos should work but reading should
  // produce an error.
  ASSERT_EQ(0, fsetpos(fp, &pos_inside_mb));
  ASSERT_EQ(WEOF, fgetwc(fp));
  ASSERT_EQ(EILSEQ, errno);

  fclose(fp);
}

TEST(stdio, fmemopen) {
  char buf[16];
  memset(buf, 0, sizeof(buf));
  FILE* fp = fmemopen(buf, sizeof(buf), "r+");
  ASSERT_EQ('<', fputc('<', fp));
  ASSERT_NE(EOF, fputs("abc>\n", fp));
  fflush(fp);

  ASSERT_STREQ("<abc>\n", buf);

  rewind(fp);

  char line[16];
  char* s = fgets(line, sizeof(line), fp);
  ASSERT_TRUE(s != NULL);
  ASSERT_STREQ("<abc>\n", s);

  fclose(fp);
}

TEST(stdio, fmemopen_NULL) {
  FILE* fp = fmemopen(nullptr, 128, "r+");
  ASSERT_NE(EOF, fputs("xyz\n", fp));

  rewind(fp);

  char line[16];
  char* s = fgets(line, sizeof(line), fp);
  ASSERT_TRUE(s != NULL);
  ASSERT_STREQ("xyz\n", s);

  fclose(fp);
}

TEST(stdio, fmemopen_EINVAL) {
  char buf[16];

  // Invalid size.
  errno = 0;
  ASSERT_EQ(nullptr, fmemopen(buf, 0, "r+"));
  ASSERT_EQ(EINVAL, errno);

  // No '+' with NULL buffer.
  errno = 0;
  ASSERT_EQ(nullptr, fmemopen(nullptr, 0, "r"));
  ASSERT_EQ(EINVAL, errno);
}

TEST(stdio, open_memstream) {
  char* p = nullptr;
  size_t size = 0;
  FILE* fp = open_memstream(&p, &size);
  ASSERT_NE(EOF, fputs("hello, world!", fp));
  fclose(fp);

  ASSERT_STREQ("hello, world!", p);
  ASSERT_EQ(strlen("hello, world!"), size);
  free(p);
}

TEST(stdio, open_memstream_EINVAL) {
#if defined(__BIONIC__)
  char* p;
  size_t size;

  // Invalid buffer.
  errno = 0;
  ASSERT_EQ(nullptr, open_memstream(nullptr, &size));
  ASSERT_EQ(EINVAL, errno);

  // Invalid size.
  errno = 0;
  ASSERT_EQ(nullptr, open_memstream(&p, nullptr));
  ASSERT_EQ(EINVAL, errno);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

TEST(stdio, fdopen_CLOEXEC) {
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);

  // This fd doesn't have O_CLOEXEC...
  int flags = fcntl(fd, F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(0, flags & FD_CLOEXEC);

  FILE* fp = fdopen(fd, "re");
  ASSERT_TRUE(fp != NULL);

  // ...but the new one does.
  flags = fcntl(fileno(fp), F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(FD_CLOEXEC, flags & FD_CLOEXEC);

  fclose(fp);
  close(fd);
}

TEST(stdio, freopen_CLOEXEC) {
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_TRUE(fp != NULL);

  // This FILE* doesn't have O_CLOEXEC...
  int flags = fcntl(fileno(fp), F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(0, flags & FD_CLOEXEC);

  fp = freopen("/proc/version", "re", fp);

  // ...but the new one does.
  flags = fcntl(fileno(fp), F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(FD_CLOEXEC, flags & FD_CLOEXEC);

  fclose(fp);
}

// https://code.google.com/p/android/issues/detail?id=81155
// http://b/18556607
TEST(stdio, fread_unbuffered_pathological_performance) {
  FILE* fp = fopen("/dev/zero", "r");
  ASSERT_TRUE(fp != NULL);

  // Make this stream unbuffered.
  setvbuf(fp, 0, _IONBF, 0);

  char buf[65*1024];
  memset(buf, 0xff, sizeof(buf));

  time_t t0 = time(NULL);
  for (size_t i = 0; i < 1024; ++i) {
    ASSERT_EQ(1U, fread(buf, 64*1024, 1, fp));
  }
  time_t t1 = time(NULL);

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

TEST(stdio, fread_EOF) {
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
  ASSERT_EQ(EBADF, errno);
  ASSERT_TRUE(ferror(fp));
  ASSERT_FALSE(feof(fp));
  fclose(fp);
}

TEST(stdio, fread_from_write_only_stream_slow_path) {
  test_fread_from_write_only_stream(1);
}

TEST(stdio, fread_from_write_only_stream_fast_path) {
  test_fread_from_write_only_stream(64*1024);
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
  ASSERT_EQ(1U, fwrite("2", 1, 1, fp)) << errno;

  // And if we rewind, everything's there.
  rewind(fp);
  ASSERT_EQ(2U, fread(&buf[0], 1, buf.size(), fp));
  ASSERT_EQ('1', buf[0]);
  ASSERT_EQ('2', buf[1]);

  fclose(fp);
}

TEST(stdio, fwrite_after_fread_slow_path) {
  test_fwrite_after_fread(16);
}

TEST(stdio, fwrite_after_fread_fast_path) {
  test_fwrite_after_fread(64*1024);
}

// http://b/19172514
TEST(stdio, fread_after_fseek) {
  TemporaryFile tf;

  FILE* fp = fopen(tf.filename, "w+");
  ASSERT_TRUE(fp != nullptr);

  char file_data[12288];
  for (size_t i = 0; i < 12288; i++) {
    file_data[i] = i;
  }
  ASSERT_EQ(12288U, fwrite(file_data, 1, 12288, fp));
  fclose(fp);

  fp = fopen(tf.filename, "r");
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
  ASSERT_EQ(0, fseek(fp, -22, SEEK_CUR));
  cur_location = static_cast<size_t>(ftell(fp));
  ASSERT_EQ(22U, fread(buffer, 1, 22, fp));
  ASSERT_EQ(memcmp(file_data+cur_location, buffer, 22), 0);

  fclose(fp);
}

// https://code.google.com/p/android/issues/detail?id=184847
TEST(stdio, fread_EOF_184847) {
  TemporaryFile tf;
  char buf[6] = {0};

  FILE* fw = fopen(tf.filename, "w");
  ASSERT_TRUE(fw != nullptr);

  FILE* fr = fopen(tf.filename, "r");
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
