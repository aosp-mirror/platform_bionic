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


#include <uchar.h>

#include <gtest/gtest.h>

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdint.h>

#include "utils.h"

// Modern versions of UTF-8 (https://datatracker.ietf.org/doc/html/rfc3629 and
// newer) explicitly disallow code points beyond U+10FFFF, which exclude all 5-
// and 6-byte sequences. Earlier versions of UTF-8 allowed the wider range:
// https://datatracker.ietf.org/doc/html/rfc2279.
//
// Bionic's unicode implementation was written after the high values were
// excluded, so it has never supported them. Other implementations (at least
// as of glibc 2.36), do support those sequences.
#if defined(__ANDROID__) || defined(ANDROID_HOST_MUSL)
constexpr bool kLibcRejectsOverLongUtf8Sequences = true;
#elif defined(__GLIBC__)
constexpr bool kLibcRejectsOverLongUtf8Sequences = false;
#else
#error kLibcRejectsOverLongUtf8Sequences must be configured for this platform
#endif

TEST(uchar, sizeof_uchar_t) {
  EXPECT_EQ(2U, sizeof(char16_t));
  EXPECT_EQ(4U, sizeof(char32_t));
}

TEST(uchar, start_state) {
  // C23 does not appear to specify the behavior of the conversion functions if
  // a state is reused before the character is completed. In the wchar.h section
  // (7.31.6.3) it says:
  //
  //     If an mbstate_t object has been altered by any of the functions
  //     described in this subclause, and is then used with a different
  //     multibyte character sequence, or in the other conversion direction, or
  //     with a different LC_CTYPE category setting than on earlier function
  //     calls, the behavior is undefined.
  //
  // But "described in this subclause" refers to the wchar.h functions, not the
  // uchar.h ones.
  //
  // Since C has no opinion, we need to make a choice. While no caller should
  // ever do this (what does it mean to begin decoding a UTF-32 character while
  // still in the middle of a UTF-8 sequence?), considering that a decoding
  // error seems the least surprising. Bionic and glibc both have that behavior.
  // musl ignores the state (it also doesn't make much sense to read the state
  // when the entire conversion completes in a single call) and decodes the
  // UTF-32 character.
#if !defined(ANDROID_HOST_MUSL)
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char out[MB_LEN_MAX];
  mbstate_t ps;

  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(nullptr, "\xc2", 1, &ps));
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), c32rtomb(out, 0x00a2, &ps));
  EXPECT_ERRNO(EILSEQ);

  // Similarly (but not in compliance with the standard afaict), musl seems to
  // ignore the state entirely for the UTF-32 functions rather than reset it.

  // If the first argument to c32rtomb is nullptr or the second is L'\0' the shift
  // state should be reset.
  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(nullptr, "\xc2", 1, &ps));
  EXPECT_EQ(1U, c32rtomb(nullptr, 0x00a2, &ps));
  EXPECT_TRUE(mbsinit(&ps));

  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(nullptr, "\xf0\xa4", 1, &ps));
  EXPECT_EQ(1U, c32rtomb(out, L'\0', &ps));
  EXPECT_TRUE(mbsinit(&ps));
#endif
}

TEST(uchar, c16rtomb_null_out) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  EXPECT_EQ(1U, c16rtomb(nullptr, L'\0', nullptr));
  EXPECT_EQ(1U, c16rtomb(nullptr, L'h', nullptr));
}

TEST(uchar, c16rtomb_null_char) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char bytes[MB_LEN_MAX];
  EXPECT_EQ(1U, c16rtomb(bytes, L'\0', nullptr));
}

TEST(uchar, c16rtomb) {
  char bytes[MB_LEN_MAX];

  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c16rtomb(bytes, L'h', nullptr));
  EXPECT_EQ('h', bytes[0]);

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c16rtomb(bytes, L'h', nullptr));
  EXPECT_EQ('h', bytes[0]);
  // 2-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(2U, c16rtomb(bytes, 0x00a2, nullptr));
  EXPECT_EQ('\xc2', bytes[0]);
  EXPECT_EQ('\xa2', bytes[1]);
  // 3-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(3U, c16rtomb(bytes, 0x20ac, nullptr));
  EXPECT_EQ('\xe2', bytes[0]);
  EXPECT_EQ('\x82', bytes[1]);
  EXPECT_EQ('\xac', bytes[2]);
  // 4-byte UTF-8 from a surrogate pair...
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(0U, c16rtomb(bytes, 0xdbea, nullptr));
  EXPECT_EQ(4U, c16rtomb(bytes, 0xdfcd, nullptr));
  EXPECT_EQ('\xf4', bytes[0]);
  EXPECT_EQ('\x8a', bytes[1]);
  EXPECT_EQ('\xaf', bytes[2]);
  EXPECT_EQ('\x8d', bytes[3]);
}

TEST(uchar, c16rtomb_invalid) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char bytes[MB_LEN_MAX];

  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(static_cast<size_t>(-1), c16rtomb(bytes, 0xdfcd, nullptr));

  EXPECT_EQ(0U, c16rtomb(bytes, 0xdbea, nullptr));
  EXPECT_EQ(static_cast<size_t>(-1), c16rtomb(bytes, 0xdbea, nullptr));
}

TEST(uchar, mbrtoc16_null) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  ASSERT_EQ(0U, mbrtoc16(nullptr, nullptr, 0, nullptr));
}

TEST(uchar, mbrtoc16_zero_len) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char16_t out;

  out = L'x';
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "hello", 0, nullptr));
  EXPECT_EQ(L'x', out);

  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "hello", 0, nullptr));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "", 0, nullptr));
  EXPECT_EQ(1U, mbrtoc16(&out, "hello", 1, nullptr));
  EXPECT_EQ(L'h', out);
}

TEST(uchar, mbrtoc16) {
  char16_t out;

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  ASSERT_EQ(1U, mbrtoc16(&out, "abcdef", 6, nullptr));
  ASSERT_EQ(L'a', out);
  // 2-byte UTF-8.
  ASSERT_EQ(2U, mbrtoc16(&out, "\xc2\xa2" "cdef", 6, nullptr));
  ASSERT_EQ(static_cast<char16_t>(0x00a2), out);
  // 3-byte UTF-8.
  ASSERT_EQ(3U, mbrtoc16(&out, "\xe2\x82\xac" "def", 6, nullptr));
  ASSERT_EQ(static_cast<char16_t>(0x20ac), out);
  // 4-byte UTF-8 will be returned as a surrogate pair...
  ASSERT_EQ(4U, mbrtoc16(&out, "\xf4\x8a\xaf\x8d", 6, nullptr));
  ASSERT_EQ(static_cast<char16_t>(0xdbea), out);
  ASSERT_EQ(static_cast<size_t>(-3), mbrtoc16(&out,
                                              "\xf4\x8a\xaf\x8d"
                                              "ef",
                                              6, nullptr));
  ASSERT_EQ(static_cast<char16_t>(0xdfcd), out);
}

TEST(uchar, mbrtoc16_long_sequences) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char16_t out = u'\0';
  errno = 0;
  auto result = mbrtoc16(&out, "\xf8\xa1\xa2\xa3\xa4", 5, nullptr);
  if (kLibcRejectsOverLongUtf8Sequences) {
    EXPECT_EQ(static_cast<size_t>(-1), result);
    EXPECT_ERRNO(EILSEQ);
    EXPECT_EQ(u'\0', out);
  } else {
    EXPECT_EQ(5U, result);
    EXPECT_ERRNO(0);
    EXPECT_EQ(u'\uf94a', out);
  }
}

TEST(uchar, mbrtoc16_reserved_range) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  errno = 0;
  char16_t out = u'\0';
  EXPECT_EQ(static_cast<size_t>(-1), mbrtoc16(&out, "\xf0\x80\xbf\xbf", 6, nullptr));
  EXPECT_EQ(u'\0', out);
  EXPECT_ERRNO(EILSEQ);
}

TEST(uchar, mbrtoc16_beyond_range) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  errno = 0;
  char16_t out = u'\0';
  auto result = mbrtoc16(&out, "\xf5\x80\x80\x80", 6, nullptr);
  if (kLibcRejectsOverLongUtf8Sequences) {
    EXPECT_EQ(static_cast<size_t>(-1), result);
    EXPECT_EQ(u'\0', out);
    EXPECT_ERRNO(EILSEQ);
  } else {
    EXPECT_EQ(4U, result);
    EXPECT_EQ(u'\xdcc0', out);
    EXPECT_ERRNO(0);
  }
}

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
  ASSERT_EQ(1U, mbrtoc16(&out,
                         "\x8d"
                         "ef",
                         3, ps));
  ASSERT_EQ(static_cast<char16_t>(0xdbea), out);
  ASSERT_EQ(static_cast<size_t>(-3), mbrtoc16(&out,
                                              "\x80"
                                              "ef",
                                              3, ps));
  ASSERT_EQ(static_cast<char16_t>(0xdfcd), out);
  ASSERT_TRUE(mbsinit(ps));

  // Invalid 2-byte
  ASSERT_EQ(static_cast<size_t>(-2), mbrtoc16(&out, "\xc2", 1, ps));
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(-1), mbrtoc16(&out, "\x20" "cdef", 5, ps));
  ASSERT_ERRNO(EILSEQ);
}

TEST(uchar, mbrtoc16_incomplete) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));

  test_mbrtoc16_incomplete(&ps);
  test_mbrtoc16_incomplete(nullptr);
}

TEST(uchar, c32rtomb) {
  EXPECT_EQ(1U, c32rtomb(nullptr, L'\0', nullptr));
  EXPECT_EQ(1U, c32rtomb(nullptr, L'h', nullptr));

  char bytes[MB_LEN_MAX];

  memset(bytes, 1, sizeof(bytes));
  EXPECT_EQ(1U, c32rtomb(bytes, L'\0', nullptr));
  EXPECT_EQ('\0', bytes[0]);
  EXPECT_EQ('\x01', bytes[1]);

  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c32rtomb(bytes, L'h', nullptr));
  EXPECT_EQ('h', bytes[0]);

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, c32rtomb(bytes, L'h', nullptr));
  EXPECT_EQ('h', bytes[0]);
  // 2-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(2U, c32rtomb(bytes, 0x00a2, nullptr));
  EXPECT_EQ('\xc2', bytes[0]);
  EXPECT_EQ('\xa2', bytes[1]);
  // 3-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(3U, c32rtomb(bytes, 0x20ac, nullptr));
  EXPECT_EQ('\xe2', bytes[0]);
  EXPECT_EQ('\x82', bytes[1]);
  EXPECT_EQ('\xac', bytes[2]);
  // 4-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(4U, c32rtomb(bytes, 0x24b62, nullptr));
  EXPECT_EQ('\xf0', bytes[0]);
  EXPECT_EQ('\xa4', bytes[1]);
  EXPECT_EQ('\xad', bytes[2]);
  EXPECT_EQ('\xa2', bytes[3]);
  // Invalid code point.
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), c32rtomb(bytes, 0xffffffff, nullptr));
  EXPECT_ERRNO(EILSEQ);
}

TEST(uchar, mbrtoc32_valid_non_characters) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char32_t out[8] = {};
  ASSERT_EQ(3U, mbrtoc32(out, "\xef\xbf\xbe", 3, nullptr));
  ASSERT_EQ(0xfffeU, out[0]);
  ASSERT_EQ(3U, mbrtoc32(out, "\xef\xbf\xbf", 3, nullptr));
  ASSERT_EQ(0xffffU, out[0]);
}

TEST(uchar, mbrtoc32_out_of_range) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char32_t out = U'\0';
  errno = 0;
  auto result = mbrtoc32(&out, "\xf5\x80\x80\x80", 4, nullptr);
  if (kLibcRejectsOverLongUtf8Sequences) {
    EXPECT_EQ(static_cast<size_t>(-1), result);
    EXPECT_ERRNO(EILSEQ);
    EXPECT_EQ(U'\0', out);
  } else {
    EXPECT_EQ(4U, result);
    EXPECT_ERRNO(0);
    EXPECT_EQ(U'\x140000', out);
  }
}

TEST(uchar, mbrtoc32) {
  char32_t out[8];

  out[0] = L'x';
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(out, "hello", 0, nullptr));
  EXPECT_EQ(static_cast<char32_t>(L'x'), out[0]);

  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(out, "hello", 0, nullptr));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(out, "", 0, nullptr));
  EXPECT_EQ(1U, mbrtoc32(out, "hello", 1, nullptr));
  EXPECT_EQ(static_cast<char32_t>(L'h'), out[0]);

  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(nullptr, "hello", 0, nullptr));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtoc32(nullptr, "", 0, nullptr));
  EXPECT_EQ(1U, mbrtoc32(nullptr, "hello", 1, nullptr));

  EXPECT_EQ(0U, mbrtoc32(nullptr, nullptr, 0, nullptr));

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  EXPECT_EQ(1U, mbrtoc32(out, "abcdef", 6, nullptr));
  EXPECT_EQ(static_cast<char32_t>(L'a'), out[0]);
  // 2-byte UTF-8.
  EXPECT_EQ(2U, mbrtoc32(out,
                         "\xc2\xa2"
                         "cdef",
                         6, nullptr));
  EXPECT_EQ(static_cast<char32_t>(0x00a2), out[0]);
  // 3-byte UTF-8.
  EXPECT_EQ(3U, mbrtoc32(out,
                         "\xe2\x82\xac"
                         "def",
                         6, nullptr));
  EXPECT_EQ(static_cast<char32_t>(0x20ac), out[0]);
  // 4-byte UTF-8.
  EXPECT_EQ(4U, mbrtoc32(out,
                         "\xf0\xa4\xad\xa2"
                         "ef",
                         6, nullptr));
  EXPECT_EQ(static_cast<char32_t>(0x24b62), out[0]);
#if defined(__BIONIC__) // glibc allows this.
  // Illegal 5-byte UTF-8.
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), mbrtoc32(out,
                                              "\xf8\xa1\xa2\xa3\xa4"
                                              "f",
                                              6, nullptr));
  EXPECT_ERRNO(EILSEQ);
#endif
  // Illegal over-long sequence.
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), mbrtoc32(out,
                                              "\xf0\x82\x82\xac"
                                              "ef",
                                              6, nullptr));
  EXPECT_ERRNO(EILSEQ);
}

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
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(-1), mbrtoc32(&out, "\x20" "cdef", 5, ps));
  ASSERT_ERRNO(EILSEQ);
}

TEST(uchar, mbrtoc32_incomplete) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));

  test_mbrtoc32_incomplete(&ps);
  test_mbrtoc32_incomplete(nullptr);
}
