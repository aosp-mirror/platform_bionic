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

#include "TemporaryFile.h"

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
    ASSERT_STREQ(word_read, expected[i]);
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
    ASSERT_STREQ(line_read, line_written);
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

TEST(stdio, snprintf_f_special) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%f", nanf(""));
  EXPECT_STRCASEEQ("NaN", buf);

  snprintf(buf, sizeof(buf), "%f", HUGE_VALF);
  EXPECT_STRCASEEQ("Inf", buf);
}

TEST(stdio, snprintf_g_special) {
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), "%g", nan(""));
  EXPECT_STRCASEEQ("NaN", buf);

  snprintf(buf, sizeof(buf), "%g", HUGE_VAL);
  EXPECT_STRCASEEQ("Inf", buf);
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

  snprintf(buf, sizeof(buf), "%f", -0.0);
  EXPECT_STREQ("-0.000000", buf);
}

TEST(stdio, fprintf_failures_7229520) {
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
  EXPECT_EQ(EOF, putw(1234, fp));
  EXPECT_EQ(EBADF, errno);

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
