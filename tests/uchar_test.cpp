/*
 * Copyright (C) 2014 The Android Open Source Project
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
#if defined(__BIONIC__)
#define HAVE_UCHAR 1
#elif defined(__GLIBC__)
#include <features.h>
#define HAVE_UCHAR __GLIBC_PREREQ(2, 16)
#endif

#include <gtest/gtest.h>

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdint.h>

#if HAVE_UCHAR
#include <uchar.h>
#endif

TEST(uchar, sizeof_uchar_t) {
#if HAVE_UCHAR
  EXPECT_EQ(2U, sizeof(char16_t));
  EXPECT_EQ(4U, sizeof(char32_t));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, start_state) {
#if HAVE_UCHAR
  char out[MB_LEN_MAX];
  mbstate_t ps;

  // Any non-initial state is invalid when calling c32rtomb.
  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(NULL, "\xc2", 1, &ps));
  EXPECT_EQ(static_cast<size_t>(-1), c32rtomb(out, 0x00a2, &ps));
  EXPECT_EQ(EILSEQ, errno);

  // If the first argument to c32rtomb is NULL or the second is L'\0' the shift
  // state should be reset.
  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(NULL, "\xc2", 1, &ps));
  EXPECT_EQ(1U, c32rtomb(NULL, 0x00a2, &ps));
  EXPECT_TRUE(mbsinit(&ps));

  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(NULL, "\xf0\xa4", 1, &ps));
  EXPECT_EQ(1U, c32rtomb(out, L'\0', &ps));
  EXPECT_TRUE(mbsinit(&ps));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, c16rtomb_null_out) {
#if HAVE_UCHAR
  EXPECT_EQ(1U, c16rtomb(NULL, L'\0', NULL));
  EXPECT_EQ(1U, c16rtomb(NULL, L'h', NULL));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, c16rtomb_null_char) {
#if HAVE_UCHAR
  char bytes[MB_LEN_MAX];
  EXPECT_EQ(1U, c16rtomb(bytes, L'\0', NULL));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, c16rtomb) {
#if HAVE_UCHAR
  char bytes[MB_LEN_MAX];

  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c16rtomb(bytes, L'h', NULL));
  EXPECT_EQ('h', bytes[0]);

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c16rtomb(bytes, L'h', NULL));
  EXPECT_EQ('h', bytes[0]);
  // 2-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(2U, c16rtomb(bytes, 0x00a2, NULL));
  EXPECT_EQ('\xc2', bytes[0]);
  EXPECT_EQ('\xa2', bytes[1]);
  // 3-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(3U, c16rtomb(bytes, 0x20ac, NULL));
  EXPECT_EQ('\xe2', bytes[0]);
  EXPECT_EQ('\x82', bytes[1]);
  EXPECT_EQ('\xac', bytes[2]);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, c16rtomb_surrogate) {
#if HAVE_UCHAR
  char bytes[MB_LEN_MAX];

  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(0U, c16rtomb(bytes, 0xdbea, NULL));
  EXPECT_EQ(4U, c16rtomb(bytes, 0xdfcd, NULL));
  EXPECT_EQ('\xf4', bytes[0]);
  EXPECT_EQ('\x8a', bytes[1]);
  EXPECT_EQ('\xaf', bytes[2]);
  EXPECT_EQ('\x8d', bytes[3]);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, c16rtomb_invalid) {
#if HAVE_UCHAR
  char bytes[MB_LEN_MAX];

  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(static_cast<size_t>(-1), c16rtomb(bytes, 0xdfcd, NULL));

  EXPECT_EQ(0U, c16rtomb(bytes, 0xdbea, NULL));
  EXPECT_EQ(static_cast<size_t>(-1), c16rtomb(bytes, 0xdbea, NULL));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, mbrtoc16_null) {
#if HAVE_UCHAR
  ASSERT_EQ(0U, mbrtoc16(NULL, NULL, 0, NULL));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, mbrtoc16_zero_len) {
#if HAVE_UCHAR
  char16_t out;

  out = L'x';
  ASSERT_EQ(0U, mbrtoc16(&out, "hello", 0, NULL));
  ASSERT_EQ(L'x', out);

  ASSERT_EQ(0U, mbrtoc16(&out, "hello", 0, NULL));
  ASSERT_EQ(0U, mbrtoc16(&out, "", 0, NULL));
  ASSERT_EQ(1U, mbrtoc16(&out, "hello", 1, NULL));
  ASSERT_EQ(L'h', out);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, mbrtoc16) {
#if HAVE_UCHAR
  char16_t out;

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  ASSERT_EQ(1U, mbrtoc16(&out, "abcdef", 6, NULL));
  ASSERT_EQ(L'a', out);
  // 2-byte UTF-8.
  ASSERT_EQ(2U, mbrtoc16(&out, "\xc2\xa2" "cdef", 6, NULL));
  ASSERT_EQ(static_cast<char16_t>(0x00a2), out);
  // 3-byte UTF-8.
  ASSERT_EQ(3U, mbrtoc16(&out, "\xe2\x82\xac" "def", 6, NULL));
  ASSERT_EQ(static_cast<char16_t>(0x20ac), out);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, mbrtoc16_surrogate) {
#if HAVE_UCHAR
  char16_t out;

  ASSERT_EQ(static_cast<size_t>(-3),
            mbrtoc16(&out, "\xf4\x8a\xaf\x8d", 6, NULL));
  ASSERT_EQ(static_cast<char16_t>(0xdbea), out);
  ASSERT_EQ(4U, mbrtoc16(&out, "\xf4\x8a\xaf\x8d" "ef", 6, NULL));
  ASSERT_EQ(static_cast<char16_t>(0xdfcd), out);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, mbrtoc16_reserved_range) {
#if HAVE_UCHAR
  char16_t out;
  ASSERT_EQ(static_cast<size_t>(-1),
            mbrtoc16(&out, "\xf0\x80\xbf\xbf", 6, NULL));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, mbrtoc16_beyond_range) {
#if HAVE_UCHAR
  char16_t out;
  ASSERT_EQ(static_cast<size_t>(-1),
            mbrtoc16(&out, "\xf5\x80\x80\x80", 6, NULL));
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

#if HAVE_UCHAR
void test_mbrtoc16_incomplete(mbstate_t* ps) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char16_t out;
  // 2-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "\xc2", 1, ps));
  ASSERT_EQ(1U, mbrtoc16(&out, "\xa2" "cdef", 5, ps));
  ASSERT_EQ(static_cast<char16_t>(0x00a2), out);
  ASSERT_TRUE(mbsinit(ps));
  // 3-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "\xe2", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "\x82", 1, ps));
  ASSERT_EQ(1U, mbrtoc16(&out, "\xac" "def", 4, ps));
  ASSERT_EQ(static_cast<char16_t>(0x20ac), out);
  ASSERT_TRUE(mbsinit(ps));
  // 4-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "\xf4", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "\x8a\xaf", 2, ps));
  ASSERT_EQ(static_cast<size_t>(-3), mbrtoc16(&out, "\x8d" "ef", 3, ps));
  ASSERT_EQ(static_cast<char16_t>(0xdbea), out);
  ASSERT_EQ(1U, mbrtoc16(&out, "\x80" "ef", 3, ps));
  ASSERT_EQ(static_cast<char16_t>(0xdfcd), out);
  ASSERT_TRUE(mbsinit(ps));

  // Invalid 2-byte
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "\xc2", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-1), mbrtoc16(&out, "\x20" "cdef", 5, ps));
  ASSERT_EQ(EILSEQ, errno);
}
#endif

TEST(uchar, mbrtoc16_incomplete) {
#if HAVE_UCHAR
  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));

  test_mbrtoc16_incomplete(&ps);
  test_mbrtoc16_incomplete(NULL);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, c32rtomb) {
#if HAVE_UCHAR
  EXPECT_EQ(1U, c32rtomb(NULL, L'\0', NULL));
  EXPECT_EQ(1U, c32rtomb(NULL, L'h', NULL));

  char bytes[MB_LEN_MAX];

  EXPECT_EQ(1U, c32rtomb(bytes, L'\0', NULL));

  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c32rtomb(bytes, L'h', NULL));
  EXPECT_EQ('h', bytes[0]);

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c32rtomb(bytes, L'h', NULL));
  EXPECT_EQ('h', bytes[0]);
  // 2-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(2U, c32rtomb(bytes, 0x00a2, NULL));
  EXPECT_EQ('\xc2', bytes[0]);
  EXPECT_EQ('\xa2', bytes[1]);
  // 3-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(3U, c32rtomb(bytes, 0x20ac, NULL));
  EXPECT_EQ('\xe2', bytes[0]);
  EXPECT_EQ('\x82', bytes[1]);
  EXPECT_EQ('\xac', bytes[2]);
  // 4-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(4U, c32rtomb(bytes, 0x24b62, NULL));
  EXPECT_EQ('\xf0', bytes[0]);
  EXPECT_EQ('\xa4', bytes[1]);
  EXPECT_EQ('\xad', bytes[2]);
  EXPECT_EQ('\xa2', bytes[3]);
  // Invalid code point.
  EXPECT_EQ(static_cast<size_t>(-1), c32rtomb(bytes, 0xffffffff, NULL));
  EXPECT_EQ(EILSEQ, errno);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

TEST(uchar, mbrtoc32) {
#if HAVE_UCHAR
  char32_t out[8];

  out[0] = L'x';
  ASSERT_EQ(0U, mbrtoc32(out, "hello", 0, NULL));
  ASSERT_EQ(static_cast<char32_t>(L'x'), out[0]);

  ASSERT_EQ(0U, mbrtoc32(out, "hello", 0, NULL));
  ASSERT_EQ(0U, mbrtoc32(out, "", 0, NULL));
  ASSERT_EQ(1U, mbrtoc32(out, "hello", 1, NULL));
  ASSERT_EQ(static_cast<char32_t>(L'h'), out[0]);

  ASSERT_EQ(0U, mbrtoc32(NULL, "hello", 0, NULL));
  ASSERT_EQ(0U, mbrtoc32(NULL, "", 0, NULL));
  ASSERT_EQ(1U, mbrtoc32(NULL, "hello", 1, NULL));

  ASSERT_EQ(0U, mbrtoc32(NULL, NULL, 0, NULL));

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  ASSERT_EQ(1U, mbrtoc32(out, "abcdef", 6, NULL));
  ASSERT_EQ(static_cast<char32_t>(L'a'), out[0]);
  // 2-byte UTF-8.
  ASSERT_EQ(2U, mbrtoc32(out, "\xc2\xa2" "cdef", 6, NULL));
  ASSERT_EQ(static_cast<char32_t>(0x00a2), out[0]);
  // 3-byte UTF-8.
  ASSERT_EQ(3U, mbrtoc32(out, "\xe2\x82\xac" "def", 6, NULL));
  ASSERT_EQ(static_cast<char32_t>(0x20ac), out[0]);
  // 4-byte UTF-8.
  ASSERT_EQ(4U, mbrtoc32(out, "\xf0\xa4\xad\xa2" "ef", 6, NULL));
  ASSERT_EQ(static_cast<char32_t>(0x24b62), out[0]);
#if defined(__BIONIC__) // glibc allows this.
  // Illegal 5-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-1), mbrtoc32(out, "\xf8\xa1\xa2\xa3\xa4" "f", 6, NULL));
  ASSERT_EQ(EILSEQ, errno);
#endif
  // Illegal over-long sequence.
  ASSERT_EQ(static_cast<size_t>(-1), mbrtoc32(out, "\xf0\x82\x82\xac" "ef", 6, NULL));
  ASSERT_EQ(EILSEQ, errno);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

#if HAVE_UCHAR
void test_mbrtoc32_incomplete(mbstate_t* ps) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char32_t out;
  // 2-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc32(&out, "\xc2", 1, ps));
  ASSERT_EQ(1U, mbrtoc32(&out, "\xa2" "cdef", 5, ps));
  ASSERT_EQ(static_cast<char32_t>(0x00a2), out);
  ASSERT_TRUE(mbsinit(ps));
  // 3-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc32(&out, "\xe2", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc32(&out, "\x82", 1, ps));
  ASSERT_EQ(1U, mbrtoc32(&out, "\xac" "def", 4, ps));
  ASSERT_EQ(static_cast<char32_t>(0x20ac), out);
  ASSERT_TRUE(mbsinit(ps));
  // 4-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc32(&out, "\xf0", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc32(&out, "\xa4\xad", 2, ps));
  ASSERT_EQ(1U, mbrtoc32(&out, "\xa2" "ef", 3, ps));
  ASSERT_EQ(static_cast<char32_t>(0x24b62), out);
  ASSERT_TRUE(mbsinit(ps));

  // Invalid 2-byte
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc32(&out, "\xc2", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-1), mbrtoc32(&out, "\x20" "cdef", 5, ps));
  ASSERT_EQ(EILSEQ, errno);
}
#endif

TEST(uchar, mbrtoc32_incomplete) {
#if HAVE_UCHAR
  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));

  test_mbrtoc32_incomplete(&ps);
  test_mbrtoc32_incomplete(NULL);
#else
  GTEST_LOG_(INFO) << "uchar.h is unavailable.\n";
#endif
}

