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

#include <gtest/gtest.h>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <wchar.h>

#include "utils.h"

#define NUM_WCHARS(num_bytes) ((num_bytes)/sizeof(wchar_t))

#ifdef __GLIBC__
// glibc immediately dereferences the locale passed to all wcsto*_l functions,
// even if it won't be used, and even if it's LC_GLOBAL_LOCALE, which isn't a
// pointer to valid memory.
static locale_t SAFE_LC_GLOBAL_LOCALE = duplocale(LC_GLOBAL_LOCALE);
#else
static locale_t SAFE_LC_GLOBAL_LOCALE = LC_GLOBAL_LOCALE;
#endif

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

#if defined(__GLIBC__)
constexpr bool kLibcSupportsParsingBinaryLiterals = __GLIBC_PREREQ(2, 38);
#else
constexpr bool kLibcSupportsParsingBinaryLiterals = true;
#endif

TEST(wchar, sizeof_wchar_t) {
  EXPECT_EQ(4U, sizeof(wchar_t));
  EXPECT_EQ(4U, sizeof(wint_t));
}

TEST(wchar, mbrlen) {
  char bytes[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
  EXPECT_EQ(static_cast<size_t>(-2), mbrlen(&bytes[0], 0, nullptr));
  EXPECT_EQ(1U, mbrlen(&bytes[0], 1, nullptr));

  EXPECT_EQ(1U, mbrlen(&bytes[4], 1, nullptr));
  EXPECT_EQ(0U, mbrlen(&bytes[5], 1, nullptr));
}

TEST(wchar, wctomb_wcrtomb) {
  // wctomb and wcrtomb behave differently when s == NULL.
  EXPECT_EQ(0, wctomb(nullptr, L'h'));
  EXPECT_EQ(0, wctomb(nullptr, L'\0'));
  EXPECT_EQ(1U, wcrtomb(nullptr, L'\0', nullptr));
  EXPECT_EQ(1U, wcrtomb(nullptr, L'h', nullptr));

  char bytes[MB_LEN_MAX];

  // wctomb and wcrtomb behave similarly for the null wide character.
  EXPECT_EQ(1, wctomb(bytes, L'\0'));
  EXPECT_EQ(1U, wcrtomb(bytes, L'\0', nullptr));

  // ...and for regular characters.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1, wctomb(bytes, L'h'));
  EXPECT_EQ('h', bytes[0]);
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, wcrtomb(bytes, L'h', nullptr));
  EXPECT_EQ('h', bytes[0]);

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, wcrtomb(bytes, L'h', nullptr));
  EXPECT_EQ('h', bytes[0]);
  // 2-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(2U, wcrtomb(bytes, 0x00a2, nullptr));
  EXPECT_EQ('\xc2', bytes[0]);
  EXPECT_EQ('\xa2', bytes[1]);
  // 3-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(3U, wcrtomb(bytes, 0x20ac, nullptr));
  EXPECT_EQ('\xe2', bytes[0]);
  EXPECT_EQ('\x82', bytes[1]);
  EXPECT_EQ('\xac', bytes[2]);
  // 4-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(4U, wcrtomb(bytes, 0x24b62, nullptr));
  EXPECT_EQ('\xf0', bytes[0]);
  EXPECT_EQ('\xa4', bytes[1]);
  EXPECT_EQ('\xad', bytes[2]);
  EXPECT_EQ('\xa2', bytes[3]);
  // Invalid code point.
  EXPECT_EQ(static_cast<size_t>(-1), wcrtomb(bytes, 0xffffffff, nullptr));
  EXPECT_ERRNO(EILSEQ);
}

TEST(wchar, wcrtomb_start_state) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  char out[MB_LEN_MAX];
  mbstate_t ps;

  // Any non-initial state is invalid when calling wcrtomb.
  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(nullptr, "\xc2", 1, &ps));
  EXPECT_EQ(static_cast<size_t>(-1), wcrtomb(out, 0x00a2, &ps));
  EXPECT_ERRNO(EILSEQ);

  // If the first argument to wcrtomb is NULL or the second is L'\0' the shift
  // state should be reset.
  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(nullptr, "\xc2", 1, &ps));
  EXPECT_EQ(1U, wcrtomb(nullptr, 0x00a2, &ps));
  EXPECT_TRUE(mbsinit(&ps));

  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(nullptr, "\xf0\xa4", 1, &ps));
  EXPECT_EQ(1U, wcrtomb(out, L'\0', &ps));
  EXPECT_TRUE(mbsinit(&ps));
}

TEST(wchar, wcstombs_wcrtombs) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  const wchar_t chars[] = { L'h', L'e', L'l', L'l', L'o', 0 };
  const wchar_t bad_chars[] = { L'h', L'i', static_cast<wchar_t>(0xffffffff), 0 };
  const wchar_t* src;
  char bytes[BUFSIZ];

  // Given a NULL destination, these functions count valid characters.
  EXPECT_EQ(5U, wcstombs(nullptr, chars, 0));
  EXPECT_EQ(5U, wcstombs(nullptr, chars, 4));
  EXPECT_EQ(5U, wcstombs(nullptr, chars, 256));
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(nullptr, &src, 0, nullptr));
  EXPECT_EQ(&chars[0], src);
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(nullptr, &src, 4, nullptr));
  EXPECT_EQ(&chars[0], src);
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(nullptr, &src, 256, nullptr));
  EXPECT_EQ(&chars[0], src);

  // An unrepresentable char just returns an error from wcstombs...
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), wcstombs(nullptr, bad_chars, 0));
  EXPECT_ERRNO(EILSEQ);
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), wcstombs(nullptr, bad_chars, 256));
  EXPECT_ERRNO(EILSEQ);

  // And wcsrtombs doesn't tell us where it got stuck because we didn't ask it
  // to actually convert anything...
  errno = 0;
  src = bad_chars;
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(nullptr, &src, 0, nullptr));
  EXPECT_EQ(&bad_chars[0], src);
  EXPECT_ERRNO(EILSEQ);
  errno = 0;
  src = bad_chars;
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(nullptr, &src, 256, nullptr));
  EXPECT_EQ(&bad_chars[0], src);
  EXPECT_ERRNO(EILSEQ);

  // Okay, now let's test actually converting something...
  memset(bytes, 'x', sizeof(bytes));
  EXPECT_EQ(0U, wcstombs(bytes, chars, 0));
  memset(bytes, 'x', sizeof(bytes));
  EXPECT_EQ(4U, wcstombs(bytes, chars, 4));
  bytes[5] = 0;
  EXPECT_STREQ("hellx", bytes);
  memset(bytes, 'x', sizeof(bytes));
  EXPECT_EQ(5U, wcstombs(bytes, chars, 256));
  EXPECT_STREQ("hello", bytes);
  memset(bytes, 'x', sizeof(bytes));
  EXPECT_EQ(5U, wcstombs(bytes, chars, 6));
  EXPECT_STREQ("hello", bytes);
  errno = 0;
  memset(bytes, 'x', sizeof(bytes));
  EXPECT_EQ(static_cast<size_t>(-1), wcstombs(bytes, bad_chars, 256));
  EXPECT_ERRNO(EILSEQ);
  bytes[3] = 0;
  EXPECT_STREQ("hix", bytes);

  // wcsrtombs is a bit more informative...
  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(0U, wcsrtombs(bytes, &src, 0, nullptr));
  EXPECT_EQ(&chars[0], src); // No input consumed.
  EXPECT_ERRNO(EILSEQ);

  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(4U, wcsrtombs(bytes, &src, 4, nullptr));
  EXPECT_EQ(&chars[4], src); // Some input consumed.
  EXPECT_ERRNO(EILSEQ);
  bytes[5] = 0;
  EXPECT_STREQ("hellx", bytes);

  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(bytes, &src, 256, nullptr));
  EXPECT_EQ(nullptr, src); // All input consumed!
  EXPECT_ERRNO(EILSEQ);
  EXPECT_STREQ("hello", bytes);

  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(bytes, &src, 6, nullptr));
  EXPECT_EQ(nullptr, src); // All input consumed.
  EXPECT_ERRNO(EILSEQ);
  EXPECT_STREQ("hello", bytes);

  memset(bytes, 'x', sizeof(bytes));
  src = bad_chars;
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(bytes, &src, 256, nullptr));
  EXPECT_EQ(&bad_chars[2], src);
  EXPECT_ERRNO(EILSEQ);
  bytes[3] = 0;
  EXPECT_STREQ("hix", bytes);

  // Any non-initial state is invalid when calling wcsrtombs.
  mbstate_t ps;
  src = chars;
  memset(&ps, 0, sizeof(ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(nullptr, "\xc2", 1, &ps));
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(nullptr, &src, 0, &ps));
  EXPECT_ERRNO(EILSEQ);
}

TEST(wchar, limits) {
  ASSERT_LT(WCHAR_MIN, WCHAR_MAX);
}

TEST(wchar, wcsstr) {
  const wchar_t* haystack = L"big daddy/giant haystacks!";
  const wchar_t* empty_haystack = L"";

  // The empty needle is a special case.
  ASSERT_EQ(haystack, wcsstr(haystack, L""));
  ASSERT_EQ(empty_haystack, wcsstr(empty_haystack, L""));

  ASSERT_EQ(haystack, wcsstr(haystack, L"b"));
  ASSERT_EQ(haystack, wcsstr(haystack, L"big"));
  ASSERT_EQ(haystack + 9, wcsstr(haystack, L"/"));
  ASSERT_EQ(haystack + 9, wcsstr(haystack, L"/giant"));
  ASSERT_EQ(haystack + 25, wcsstr(haystack, L"!"));
  ASSERT_EQ(haystack + 19, wcsstr(haystack, L"stacks!"));

  ASSERT_EQ(nullptr, wcsstr(haystack, L"monkey"));
  ASSERT_EQ(nullptr, wcsstr(empty_haystack, L"monkey"));
}

TEST(wchar, wcsstr_80199) {
  // https://code.google.com/p/android/issues/detail?id=80199
  ASSERT_TRUE(wcsstr(L"romrom", L"rom") != nullptr);
}

TEST(wchar, mbtowc) {
  wchar_t out[8];

  // mbtowc and all the mbrto* APIs behave slightly differently when n is 0:
  //
  // mbrtowc returns 0 "if the next n or fewer bytes complete the multibyte
  // character that corresponds to the null wide character"
  //
  // mbrtoc (C23 7.24.7.2.4) says:
  //
  //     If s is not a null pointer, the mbtowc function either returns 0 (if s
  //     points to the null character), or returns the number of bytes that are
  //     contained in the converted multibyte character (if the next n or fewer
  //     bytes form a valid multibyte character), or returns -1 (if they do not
  //     form a valid multibyte character).
  //
  // glibc's interpretation differs from all the BSDs (including macOS) and
  // bionic (by way of openbsd). glibc returns 0 since s does point to the null
  // character, whereas the BSDs return -1 because the next 0 bytes do not form
  // a valid multibyte chatacter. glibc's interpretation is probably more
  // correct from a strict interpretation of the spec, but considering the other
  // APIs behave more like the BSD interpretation that may be a bug in the spec.
#ifdef __GLIBC__
  int expected_result_for_zero_length_empty_string = 0;
#else
  int expected_result_for_zero_length_empty_string = -1;
#endif

  out[0] = 'x';
  EXPECT_EQ(-1, mbtowc(out, "hello", 0));
  EXPECT_EQ('x', out[0]);

  EXPECT_EQ(-1, mbtowc(out, "hello", 0));
  EXPECT_EQ(expected_result_for_zero_length_empty_string, mbtowc(out, "", 0));
  EXPECT_EQ(1, mbtowc(out, "hello", 1));
  EXPECT_EQ(L'h', out[0]);

  EXPECT_EQ(-1, mbtowc(nullptr, "hello", 0));
  EXPECT_EQ(expected_result_for_zero_length_empty_string, mbtowc(nullptr, "", 0));
  EXPECT_EQ(1, mbtowc(nullptr, "hello", 1));

  EXPECT_EQ(0, mbtowc(nullptr, nullptr, 0));
}

TEST(wchar, mbrtowc) {
  wchar_t out[8];

  out[0] = 'x';
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(out, "hello", 0, nullptr));
  EXPECT_EQ('x', out[0]);

  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(out, "hello", 0, nullptr));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(out, "", 0, nullptr));
  EXPECT_EQ(1U, mbrtowc(out, "hello", 1, nullptr));
  EXPECT_EQ(L'h', out[0]);

  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(nullptr, "hello", 0, nullptr));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(nullptr, "", 0, nullptr));
  EXPECT_EQ(1U, mbrtowc(nullptr, "hello", 1, nullptr));

  EXPECT_EQ(0U, mbrtowc(nullptr, nullptr, 0, nullptr));

  EXPECT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  EXPECT_EQ(1U, mbrtowc(out, "abcdef", 6, nullptr));
  EXPECT_EQ(L'a', out[0]);
  // 2-byte UTF-8.
  EXPECT_EQ(2U, mbrtowc(out,
                        "\xc2\xa2"
                        "cdef",
                        6, nullptr));
  EXPECT_EQ(static_cast<wchar_t>(0x00a2), out[0]);
  // 3-byte UTF-8.
  EXPECT_EQ(3U, mbrtowc(out,
                        "\xe2\x82\xac"
                        "def",
                        6, nullptr));
  EXPECT_EQ(static_cast<wchar_t>(0x20ac), out[0]);
  // 4-byte UTF-8.
  EXPECT_EQ(4U, mbrtowc(out,
                        "\xf0\xa4\xad\xa2"
                        "ef",
                        6, nullptr));
  EXPECT_EQ(static_cast<wchar_t>(0x24b62), out[0]);
#if defined(__BIONIC__) // glibc allows this.
  // Illegal 5-byte UTF-8.
  EXPECT_EQ(static_cast<size_t>(-1), mbrtowc(out,
                                             "\xf8\xa1\xa2\xa3\xa4"
                                             "f",
                                             6, nullptr));
  EXPECT_ERRNO(EILSEQ);
#endif
  // Illegal over-long sequence.
  EXPECT_EQ(static_cast<size_t>(-1), mbrtowc(out,
                                             "\xf0\x82\x82\xac"
                                             "ef",
                                             6, nullptr));
  EXPECT_ERRNO(EILSEQ);
}

TEST(wchar, mbrtowc_valid_non_characters) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  wchar_t out[8] = {};

  ASSERT_EQ(3U, mbrtowc(out, "\xef\xbf\xbe", 3, nullptr));
  ASSERT_EQ(static_cast<wchar_t>(0xfffe), out[0]);
  ASSERT_EQ(3U, mbrtowc(out, "\xef\xbf\xbf", 3, nullptr));
  ASSERT_EQ(static_cast<wchar_t>(0xffff), out[0]);
}

TEST(wchar, mbrtowc_out_of_range) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  wchar_t out[8] = {};
  errno = 0;
  auto result = mbrtowc(out, "\xf5\x80\x80\x80", 4, nullptr);
  if (kLibcRejectsOverLongUtf8Sequences) {
    ASSERT_EQ(static_cast<size_t>(-1), result);
    ASSERT_ERRNO(EILSEQ);
  } else {
    ASSERT_EQ(4U, result);
    ASSERT_ERRNO(0);
  }
}

static void test_mbrtowc_incomplete(mbstate_t* ps) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  wchar_t out;
  // 2-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\xc2", 1, ps));
  ASSERT_EQ(1U, mbrtowc(&out, "\xa2" "cdef", 5, ps));
  ASSERT_EQ(static_cast<wchar_t>(0x00a2), out);
  ASSERT_TRUE(mbsinit(ps));
  // 3-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\xe2", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\x82", 1, ps));
  ASSERT_EQ(1U, mbrtowc(&out, "\xac" "def", 4, ps));
  ASSERT_EQ(static_cast<wchar_t>(0x20ac), out);
  ASSERT_TRUE(mbsinit(ps));
  // 4-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\xf0", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\xa4\xad", 2, ps));
  ASSERT_EQ(1U, mbrtowc(&out, "\xa2" "ef", 3, ps));
  ASSERT_EQ(static_cast<wchar_t>(0x24b62), out);
  ASSERT_TRUE(mbsinit(ps));

  // Invalid 2-byte
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\xc2", 1, ps));
  ASSERT_EQ(static_cast<size_t>(-1), mbrtowc(&out, "\x20" "cdef", 5, ps));
  ASSERT_ERRNO(EILSEQ);
}

TEST(wchar, mbrtowc_incomplete) {
  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));

  test_mbrtowc_incomplete(&ps);
  test_mbrtowc_incomplete(nullptr);
}

static void test_mbsrtowcs(mbstate_t* ps) {
  constexpr const char* VALID = "A" "\xc2\xa2" "\xe2\x82\xac" "\xf0\xa4\xad\xa2" "ef";
  constexpr const char* INVALID = "A" "\xc2\x20" "ef";
  constexpr const char* INCOMPLETE = "A" "\xc2";
  wchar_t out[4];

  const char* valid = VALID;
  ASSERT_EQ(4U, mbsrtowcs(out, &valid, 4, ps));
  ASSERT_EQ(L'A', out[0]);
  ASSERT_EQ(static_cast<wchar_t>(0x00a2), out[1]);
  ASSERT_EQ(static_cast<wchar_t>(0x20ac), out[2]);
  ASSERT_EQ(static_cast<wchar_t>(0x24b62), out[3]);
  // Check that valid has advanced to the next unread character.
  ASSERT_EQ('e', *valid);

  wmemset(out, L'x', NUM_WCHARS(sizeof(out)));
  ASSERT_EQ(2U, mbsrtowcs(out, &valid, 4, ps));
  ASSERT_EQ(L'e', out[0]);
  ASSERT_EQ(L'f', out[1]);
  ASSERT_EQ(L'\0', out[2]);
  // Check that we didn't clobber the rest of out.
  ASSERT_EQ(L'x', out[3]);
  // Check that valid has advanced to the end of the string.
  ASSERT_EQ(nullptr, valid);

  const char* invalid = INVALID;
  ASSERT_EQ(static_cast<size_t>(-1), mbsrtowcs(out, &invalid, 4, ps));
  EXPECT_ERRNO(EILSEQ);
  ASSERT_EQ('\xc2', *invalid);

  const char* incomplete = INCOMPLETE;
  ASSERT_EQ(static_cast<size_t>(-1), mbsrtowcs(out, &incomplete, 2, ps));
  EXPECT_ERRNO(EILSEQ);
  ASSERT_EQ('\xc2', *incomplete);

  // If dst is null, *src shouldn't be updated.
  // https://code.google.com/p/android/issues/detail?id=166381
  const char* mbs = VALID;
  EXPECT_EQ(6U, mbsrtowcs(nullptr, &mbs, 0, ps));
  EXPECT_EQ(VALID, mbs);
  mbs = INVALID;
  EXPECT_EQ(static_cast<size_t>(-1), mbsrtowcs(nullptr, &mbs, 0, ps));
  EXPECT_EQ(INVALID, mbs);
  mbs = INCOMPLETE;
  EXPECT_EQ(static_cast<size_t>(-1), mbsrtowcs(nullptr, &mbs, 0, ps));
  EXPECT_EQ(INCOMPLETE, mbs);
}

TEST(wchar, mbsrtowcs) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));
  test_mbsrtowcs(&ps);
  test_mbsrtowcs(nullptr);

  // Invalid multi byte continuation.
  const char* invalid = "\x20";
  wchar_t out;
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\xc2", 1, &ps));
  ASSERT_EQ(static_cast<size_t>(-1), mbsrtowcs(&out, &invalid, 1, &ps));
  EXPECT_ERRNO(EILSEQ);
  ASSERT_EQ('\x20', *invalid);
}

template <typename T>
using WcsToIntFn = T (*)(const wchar_t*, wchar_t**, int);

template <typename T>
void TestSingleWcsToInt(WcsToIntFn<T> fn, const wchar_t* str, int base,
                        T expected_value, ptrdiff_t expected_len) {
  wchar_t* p;
  EXPECT_EQ(expected_value, fn(str, &p, base)) << str << " " << base;
  EXPECT_EQ(expected_len, p - str) << str << " " << base;
}

template <typename T>
void TestWcsToInt(WcsToIntFn<T> fn) {
  TestSingleWcsToInt(fn, L"123", 10, static_cast<T>(123), 3);
  TestSingleWcsToInt(fn, L"123", 0, static_cast<T>(123), 3);
  TestSingleWcsToInt(fn, L"123#", 10, static_cast<T>(123), 3);
  TestSingleWcsToInt(fn, L"01000", 8, static_cast<T>(512), 5);
  TestSingleWcsToInt(fn, L"01000", 0, static_cast<T>(512), 5);
  TestSingleWcsToInt(fn, L"   123 45", 0, static_cast<T>(123), 6);
  TestSingleWcsToInt(fn, L"  -123", 0, static_cast<T>(-123), 6);
  TestSingleWcsToInt(fn, L"0x10000", 0, static_cast<T>(65536), 7);
  if (kLibcSupportsParsingBinaryLiterals) {
    TestSingleWcsToInt(fn, L"0b1011", 0, static_cast<T>(0b1011), 6);
  }
}

template <typename T>
void TestWcsToIntLimits(WcsToIntFn<T> fn, const wchar_t* min_str,
                        const wchar_t* max_str) {
  if (std::is_signed<T>::value) {
    ASSERT_EQ(std::numeric_limits<T>::min(), fn(min_str, nullptr, 0)) << min_str;
  } else {
    // If the subject sequence begins with a <hyphen-minus>, the value resulting
    // from the conversion shall be negated.
    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/strtoul.html
    ASSERT_EQ(std::numeric_limits<T>::max(), fn(min_str, nullptr, 0)) << min_str;
  }
  ASSERT_EQ(std::numeric_limits<T>::max(), fn(max_str, nullptr, 0)) << max_str;
}

TEST(wchar, wcstol) {
  TestWcsToInt(wcstol);
}

TEST(wchar, wcstol_limits) {
  if (sizeof(long) == 8) {
    TestWcsToIntLimits(wcstol, L"-9223372036854775809", L"9223372036854775808");
  } else {
    TestWcsToIntLimits(wcstol, L"-2147483649", L"2147483648");
  }
}

TEST(wchar, wcstoul) {
  TestWcsToInt(wcstoul);
}

TEST(wchar, wcstoul_limits) {
  if (sizeof(long) == 8) {
    TestWcsToIntLimits(wcstoul, L"-1", L"18446744073709551616");
  } else {
    TestWcsToIntLimits(wcstoul, L"-1", L"4294967296");
  }
}

TEST(wchar, wcstoll) {
  TestWcsToInt(wcstoll);
}

TEST(wchar, wcstoll_limits) {
  TestWcsToIntLimits(wcstoll, L"-9223372036854775809", L"9223372036854775808");
}

TEST(wchar, wcstoull) {
  TestWcsToInt(wcstoull);
}

TEST(wchar, wcstoull_limits) {
  TestWcsToIntLimits(wcstoull, L"-1", L"18446744073709551616");
}

TEST(wchar, wcstoimax) {
  TestWcsToInt(wcstoimax);
}

TEST(wchar, wcstoimax_limits) {
  TestWcsToIntLimits(wcstoimax, L"-9223372036854775809",
                     L"9223372036854775808");
}

TEST(wchar, wcstoumax) {
  TestWcsToInt(wcstoumax);
}

TEST(wchar, wcstoumax_limits) {
  TestWcsToIntLimits(wcstoumax, L"-1", L"18446744073709551616");
}

TEST(wchar, mbsnrtowcs) {
  wchar_t dst[128];
  const char* s = "hello, world!";
  const char* src;

  memset(dst, 0, sizeof(dst));
  src = s;
  ASSERT_EQ(0U, mbsnrtowcs(dst, &src, 0, 0, nullptr));

  memset(dst, 0, sizeof(dst));
  src = s;
  ASSERT_EQ(2U, mbsnrtowcs(dst, &src, 2, 123, nullptr)); // glibc chokes on SIZE_MAX here.
  ASSERT_EQ(L'h', dst[0]);
  ASSERT_EQ(L'e', dst[1]);
  ASSERT_EQ(&s[2], src);

  memset(dst, 0, sizeof(dst));
  src = s;
  ASSERT_EQ(3U, mbsnrtowcs(dst, &src, SIZE_MAX, 3, nullptr));
  ASSERT_EQ(L'h', dst[0]);
  ASSERT_EQ(L'e', dst[1]);
  ASSERT_EQ(L'l', dst[2]);
  ASSERT_EQ(&s[3], src);

  memset(dst, 0, sizeof(dst));
  const char* incomplete = "\xc2"; // Incomplete UTF-8 sequence.
  src = incomplete;
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(-1), mbsnrtowcs(dst, &src, SIZE_MAX, 3, nullptr));
  ASSERT_ERRNO(EILSEQ);

  src = incomplete;
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(-1), mbsnrtowcs(nullptr, &src, SIZE_MAX, 3, nullptr));
  ASSERT_ERRNO(EILSEQ);
}

TEST(wchar, wcsftime__wcsftime_l) {
  setenv("TZ", "UTC", 1);

  struct tm t;
  memset(&t, 0, sizeof(tm));
  t.tm_year = 200;
  t.tm_mon = 2;
  t.tm_mday = 10;

  wchar_t buf[64];

  EXPECT_EQ(24U, wcsftime(buf, sizeof(buf), L"%c", &t));
  EXPECT_STREQ(L"Sun Mar 10 00:00:00 2100", buf);
  EXPECT_EQ(24U, wcsftime_l(buf, sizeof(buf), L"%c", &t, SAFE_LC_GLOBAL_LOCALE));
  EXPECT_STREQ(L"Sun Mar 10 00:00:00 2100", buf);
}

TEST(wchar, wmemmove_smoke) {
  const wchar_t const_wstr[] = L"This is a test of something or other.....";
  wchar_t wstr[NUM_WCHARS(sizeof(const_wstr))];

  EXPECT_EQ(wstr, wmemmove(wstr, const_wstr, NUM_WCHARS(sizeof(const_wstr))));
  EXPECT_STREQ(const_wstr, wstr);

  EXPECT_EQ(wstr+5, wmemmove(wstr+5, wstr, NUM_WCHARS(sizeof(const_wstr)) - 6));
  EXPECT_STREQ(L"This This is a test of something or other", wstr);
}

TEST(wchar, wmemcpy_smoke) {
  const wchar_t src[] = L"Source string";
  wchar_t dst[NUM_WCHARS(sizeof(src))];

  EXPECT_EQ(dst, wmemcpy(dst, src, NUM_WCHARS(sizeof(src))));
  EXPECT_STREQ(dst, src);
}

TEST(wchar, wcpcpy_smoke) {
  const wchar_t src[] = L"Source string";
  wchar_t dst[NUM_WCHARS(sizeof(src))];

  EXPECT_EQ(dst + NUM_WCHARS(sizeof(src)) - 1, wcpcpy(dst, src));
  EXPECT_STREQ(dst, src);
}

TEST(wchar, wcpncpy_smoke) {
  const wchar_t src[] = L"Source string";
  wchar_t dst[NUM_WCHARS(sizeof(src)) + 5];

  size_t src_len = NUM_WCHARS(sizeof(src)) - 1;
  EXPECT_EQ(dst + src_len, wcpncpy(dst, src, src_len + 1));
  EXPECT_STREQ(dst, src);

  EXPECT_EQ(dst + 6, wcpncpy(dst, src, 6));
  dst[6] = L'\0';
  EXPECT_STREQ(dst, L"Source");

  wmemset(dst, L'x', NUM_WCHARS(sizeof(dst)));
  EXPECT_EQ(dst + src_len, wcpncpy(dst, src, src_len + 4));
  EXPECT_STREQ(dst, src);
  EXPECT_EQ(dst[src_len], L'\0');
  EXPECT_EQ(dst[src_len+1], L'\0');
  EXPECT_EQ(dst[src_len+2], L'\0');
  EXPECT_EQ(dst[src_len+3], L'\0');
  EXPECT_EQ(dst[src_len+4], L'x');
}

TEST(wchar, wcscpy_smoke) {
  const wchar_t src[] = L"Source string";
  wchar_t dst[NUM_WCHARS(sizeof(src))];

  EXPECT_EQ(dst, wcscpy(dst, src));
  EXPECT_STREQ(src, dst);
}

TEST(wchar, wcsncpy_smoke) {
  const wchar_t src[] = L"Source string";
  wchar_t dst[NUM_WCHARS(sizeof(src)) + 5];

  size_t src_len = NUM_WCHARS(sizeof(src)) - 1;
  EXPECT_EQ(dst, wcsncpy(dst, src, src_len + 1));
  EXPECT_STREQ(dst, src);

  EXPECT_EQ(dst, wcsncpy(dst, src, 6));
  dst[6] = L'\0';
  EXPECT_STREQ(dst, L"Source");
  EXPECT_EQ(dst, wcsncpy(dst, L"clobber", 0));
  EXPECT_STREQ(dst, L"Source");

  wmemset(dst, L'x', NUM_WCHARS(sizeof(dst)));
  EXPECT_EQ(dst, wcsncpy(dst, src, src_len + 4));
  EXPECT_STREQ(dst, src);
  EXPECT_EQ(dst[src_len], L'\0');
  EXPECT_EQ(dst[src_len+1], L'\0');
  EXPECT_EQ(dst[src_len+2], L'\0');
  EXPECT_EQ(dst[src_len+3], L'\0');
  EXPECT_EQ(dst[src_len+4], L'x');
}

TEST(wchar, mbrtowc_15439554) {
  // http://b/15439554
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  ASSERT_GE(static_cast<size_t>(MB_LEN_MAX), MB_CUR_MAX);
  ASSERT_GE(MB_CUR_MAX, 4U);

  wchar_t wc;
  size_t n;

  // 1-byte character.
  n = mbrtowc(&wc, "x", MB_CUR_MAX, nullptr);
  EXPECT_EQ(1U, n);
  EXPECT_EQ(L'x', wc);
  // 2-byte character.
  n = mbrtowc(&wc, "\xc2\xa2", MB_CUR_MAX, nullptr);
  EXPECT_EQ(2U, n);
  EXPECT_EQ(L'¢', wc);
  // 3-byte character.
  n = mbrtowc(&wc, "\xe2\x82\xac", MB_CUR_MAX, nullptr);
  EXPECT_EQ(3U, n);
  EXPECT_EQ(L'€', wc);
  // 4-byte character.
  n = mbrtowc(&wc, "\xf0\xa4\xad\xa2", MB_CUR_MAX, nullptr);
  EXPECT_EQ(4U, n);
  EXPECT_EQ(L'𤭢', wc);
}

TEST(wchar, open_wmemstream) {
  wchar_t* p = nullptr;
  size_t size = 0;
  FILE* fp = open_wmemstream(&p, &size);
  ASSERT_NE(EOF, fputws(L"hello, world!", fp));
  fclose(fp);

  ASSERT_STREQ(L"hello, world!", p);
  ASSERT_EQ(wcslen(L"hello, world!"), size);
  free(p);
}

TEST(stdio, open_wmemstream_EINVAL) {
#if defined(__BIONIC__)
  wchar_t* p;
  size_t size;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  // Invalid buffer.
  errno = 0;
  ASSERT_EQ(nullptr, open_wmemstream(nullptr, &size));
  ASSERT_ERRNO(EINVAL);

  // Invalid size.
  errno = 0;
  ASSERT_EQ(nullptr, open_wmemstream(&p, nullptr));
  ASSERT_ERRNO(EINVAL);
#pragma clang diagnostic pop
#else
  GTEST_SKIP() << "This test is bionic-specific";
#endif
}

TEST(wchar, wcstol_EINVAL) {
  errno = 0;
  wcstol(L"123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstol(L"123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstol(L"123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(wchar, wcstoll_EINVAL) {
  errno = 0;
  wcstoll(L"123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoll(L"123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoll(L"123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(wchar, wcstoul_EINVAL) {
  errno = 0;
  wcstoul(L"123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoul(L"123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoul(L"123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(wchar, wcstoull_EINVAL) {
  errno = 0;
  wcstoull(L"123", nullptr, -1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoull(L"123", nullptr, 1);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoull(L"123", nullptr, 37);
  ASSERT_ERRNO(EINVAL);
}

TEST(wchar, wcstoll_l_EINVAL) {
  errno = 0;
  wcstoll_l(L"123", nullptr, -1, SAFE_LC_GLOBAL_LOCALE);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoll_l(L"123", nullptr, 1, SAFE_LC_GLOBAL_LOCALE);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoll_l(L"123", nullptr, 37, SAFE_LC_GLOBAL_LOCALE);
  ASSERT_ERRNO(EINVAL);
}

TEST(wchar, wcstoull_l_EINVAL) {
  errno = 0;
  wcstoull_l(L"123", nullptr, -1, SAFE_LC_GLOBAL_LOCALE);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoull_l(L"123", nullptr, 1, SAFE_LC_GLOBAL_LOCALE);
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  wcstoull_l(L"123", nullptr, 37, SAFE_LC_GLOBAL_LOCALE);
  ASSERT_ERRNO(EINVAL);
}

TEST(wchar, wmempcpy) {
#if !defined(ANDROID_HOST_MUSL)
  wchar_t dst[6];
  ASSERT_EQ(&dst[4], wmempcpy(dst, L"hello", 4));
#else
  GTEST_SKIP() << "musl doesn't have wmempcpy";
#endif
}

template <typename T>
using WcsToFloatFn = T (*)(const wchar_t*, wchar_t**);

template <typename T>
void TestSingleWcsToFloat(WcsToFloatFn<T> fn, const wchar_t* str,
                          T expected_value, ptrdiff_t expected_len) {
  wchar_t* p;
  ASSERT_EQ(expected_value, fn(str, &p));
  ASSERT_EQ(expected_len, p - str);
}

template <typename T>
void TestWcsToFloat(WcsToFloatFn<T> fn) {
  TestSingleWcsToFloat(fn, L"123", static_cast<T>(123.0L), 3);
  TestSingleWcsToFloat(fn, L"123#", static_cast<T>(123.0L), 3);
  TestSingleWcsToFloat(fn, L"   123 45", static_cast<T>(123.0L), 6);
  TestSingleWcsToFloat(fn, L"9.0", static_cast<T>(9.0L), 3);
  TestSingleWcsToFloat(fn, L"-9.0", static_cast<T>(-9.0L), 4);
  TestSingleWcsToFloat(fn, L" \t\v\f\r\n9.0", static_cast<T>(9.0L), 9);
}

template <typename T>
void TestWcsToFloatHexFloats(WcsToFloatFn<T> fn) {
  TestSingleWcsToFloat(fn, L"0.9e1", static_cast<T>(9.0L), 5);
  TestSingleWcsToFloat(fn, L"0x1.2p3", static_cast<T>(9.0L), 7);
  TestSingleWcsToFloat(fn, L"+1e+100", static_cast<T>(1e100L), 7);
  TestSingleWcsToFloat(fn, L"0x10000.80", static_cast<T>(65536.50L), 10);
}

template <typename T>
void TestWcsToFloatInfNan(WcsToFloatFn<T> fn) {
  ASSERT_TRUE(isnan(fn(L"+nan", nullptr)));
  ASSERT_TRUE(isnan(fn(L"nan", nullptr)));
  ASSERT_TRUE(isnan(fn(L"-nan", nullptr)));

  ASSERT_TRUE(isnan(fn(L"+nan(0xff)", nullptr)));
  ASSERT_TRUE(isnan(fn(L"nan(0xff)", nullptr)));
  ASSERT_TRUE(isnan(fn(L"-nan(0xff)", nullptr)));

  wchar_t* p;
  ASSERT_TRUE(isnan(fn(L"+nanny", &p)));
  ASSERT_STREQ(L"ny", p);
  ASSERT_TRUE(isnan(fn(L"nanny", &p)));
  ASSERT_STREQ(L"ny", p);
  ASSERT_TRUE(isnan(fn(L"-nanny", &p)));
  ASSERT_STREQ(L"ny", p);

  ASSERT_EQ(0, fn(L"muppet", &p));
  ASSERT_STREQ(L"muppet", p);
  ASSERT_EQ(0, fn(L"  muppet", &p));
  ASSERT_STREQ(L"  muppet", p);

  ASSERT_EQ(std::numeric_limits<T>::infinity(), fn(L"+inf", nullptr));
  ASSERT_EQ(std::numeric_limits<T>::infinity(), fn(L"inf", nullptr));
  ASSERT_EQ(-std::numeric_limits<T>::infinity(), fn(L"-inf", nullptr));

  ASSERT_EQ(std::numeric_limits<T>::infinity(), fn(L"+infinity", nullptr));
  ASSERT_EQ(std::numeric_limits<T>::infinity(), fn(L"infinity", nullptr));
  ASSERT_EQ(-std::numeric_limits<T>::infinity(), fn(L"-infinity", nullptr));

  ASSERT_EQ(std::numeric_limits<T>::infinity(), fn(L"+infinitude", &p));
  ASSERT_STREQ(L"initude", p);
  ASSERT_EQ(std::numeric_limits<T>::infinity(), fn(L"infinitude", &p));
  ASSERT_STREQ(L"initude", p);
  ASSERT_EQ(-std::numeric_limits<T>::infinity(), fn(L"-infinitude", &p));
  ASSERT_STREQ(L"initude", p);

  // Check case-insensitivity.
  ASSERT_EQ(std::numeric_limits<T>::infinity(), fn(L"InFiNiTy", nullptr));
  ASSERT_TRUE(isnan(fn(L"NaN", nullptr)));
}

TEST(wchar, wcstof) {
  TestWcsToFloat(wcstof);
}

TEST(wchar, wcstof_hex_floats) {
  TestWcsToFloatHexFloats(wcstof);
}

TEST(wchar, wcstof_hex_inf_nan) {
  TestWcsToFloatInfNan(wcstof);
}

TEST(wchar, wcstod) {
  TestWcsToFloat(wcstod);
}

TEST(wchar, wcstod_hex_floats) {
  TestWcsToFloatHexFloats(wcstod);
}

TEST(wchar, wcstod_hex_inf_nan) {
  TestWcsToFloatInfNan(wcstod);
}

TEST(wchar, wcstold) {
  TestWcsToFloat(wcstold);
}

TEST(wchar, wcstold_hex_floats) {
  TestWcsToFloatHexFloats(wcstold);
}

TEST(wchar, wcstold_hex_inf_nan) {
  TestWcsToFloatInfNan(wcstold);
}

TEST(wchar, wcstod_l) {
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(1.23, wcstod_l(L"1.23", nullptr, SAFE_LC_GLOBAL_LOCALE));
#else
  GTEST_SKIP() << "musl doesn't have wcstod_l";
#endif
}

TEST(wchar, wcstof_l) {
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(1.23f, wcstof_l(L"1.23", nullptr, SAFE_LC_GLOBAL_LOCALE));
#else
  GTEST_SKIP() << "musl doesn't have wcstof_l";
#endif
}

TEST(wchar, wcstol_l) {
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(123L, wcstol_l(L"123", nullptr, 10, SAFE_LC_GLOBAL_LOCALE));
#else
  GTEST_SKIP() << "musl doesn't have wcstol_l";
#endif
}

TEST(wchar, wcstold_l) {
  EXPECT_EQ(1.23L, wcstold_l(L"1.23", nullptr, SAFE_LC_GLOBAL_LOCALE));
}

TEST(wchar, wcstoll_l) {
  EXPECT_EQ(123LL, wcstoll_l(L"123", nullptr, 10, SAFE_LC_GLOBAL_LOCALE));
}

TEST(wchar, wcstoul_l) {
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(123UL, wcstoul_l(L"123", nullptr, 10, SAFE_LC_GLOBAL_LOCALE));
#else
  GTEST_SKIP() << "musl doesn't have wcstoul_l";
#endif
}

TEST(wchar, wcstoull_l) {
  EXPECT_EQ(123ULL, wcstoull_l(L"123", nullptr, 10, SAFE_LC_GLOBAL_LOCALE));
}

static void AssertWcwidthRange(wchar_t begin, wchar_t end, int expected) {
  for (wchar_t i = begin; i < end; ++i) {
    EXPECT_EQ(expected, wcwidth(i)) << static_cast<int>(i);
  }
}

TEST(wchar, wcwidth_NUL) {
  // NUL is defined to return 0 rather than -1, despite being a C0 control.
  EXPECT_EQ(0, wcwidth(0));
}

TEST(wchar, wcwidth_ascii) {
  AssertWcwidthRange(0x20, 0x7f, 1); // Non-C0 non-DEL ASCII.
}

TEST(wchar, wcwidth_controls) {
  AssertWcwidthRange(0x01, 0x20, -1); // C0 controls.
  EXPECT_EQ(-1, wcwidth(0x7f)); // DEL.
  AssertWcwidthRange(0x80, 0xa0, -1); // C1 controls.
}

TEST(wchar, wcwidth_non_spacing_and_enclosing_marks_and_format) {
  if (!have_dl()) return;

  EXPECT_EQ(0, wcwidth(0x0300)); // Combining grave.
  EXPECT_EQ(0, wcwidth(0x20dd)); // Combining enclosing circle.
  EXPECT_EQ(0, wcwidth(0x00ad)); // Soft hyphen (SHY).
  EXPECT_EQ(0, wcwidth(0x200b)); // Zero width space.
}

TEST(wchar, wcwidth_cjk) {
  if (!have_dl()) return;

  EXPECT_EQ(2, wcwidth(0x4e00)); // Start of CJK unified block.
  EXPECT_EQ(2, wcwidth(0x9fff)); // End of CJK unified block.
  EXPECT_EQ(2, wcwidth(0x3400)); // Start of CJK extension A block.
  EXPECT_EQ(2, wcwidth(0x4dbf)); // End of CJK extension A block.
  EXPECT_EQ(2, wcwidth(0x20000)); // Start of CJK extension B block.
  EXPECT_EQ(2, wcwidth(0x2a6df)); // End of CJK extension B block.
}

TEST(wchar, wcwidth_korean_combining_jamo) {
  if (!have_dl()) return;

  AssertWcwidthRange(0x1160, 0x1200, 0); // Original range.
  EXPECT_EQ(0, wcwidth(0xd7b0)); // Newer.
  EXPECT_EQ(0, wcwidth(0xd7cb));
}

TEST(wchar, wcwidth_korean_jeongeul_syllables) {
  if (!have_dl()) return;

  EXPECT_EQ(2, wcwidth(0xac00)); // Start of block.
  EXPECT_EQ(2, wcwidth(0xd7a3)); // End of defined code points in Unicode 7.
  // Undefined characters at the end of the block have width 1.
}

TEST(wchar, wcwidth_kana) {
  if (!have_dl()) return;

  // Hiragana (most, not undefined).
  AssertWcwidthRange(0x3041, 0x3097, 2);
  // Katakana.
  AssertWcwidthRange(0x30a0, 0x3100, 2);
}

TEST(wchar, wcwidth_circled_two_digit_cjk) {
  if (!have_dl()) return;

  // Circled two-digit CJK "speed sign" numbers are wide,
  // though EastAsianWidth is ambiguous.
  AssertWcwidthRange(0x3248, 0x3250, 2);
}

TEST(wchar, wcwidth_hexagrams) {
  if (!have_dl()) return;

  // Hexagrams are wide, though EastAsianWidth is neutral.
  AssertWcwidthRange(0x4dc0, 0x4e00, 2);
}

TEST(wchar, wcwidth_default_ignorables) {
  if (!have_dl()) return;

  AssertWcwidthRange(0xfff0, 0xfff8, 0); // Unassigned by default ignorable.
  EXPECT_EQ(0, wcwidth(0xe0000)); // ...through 0xe0fff.
}

TEST(wchar, wcwidth_korean_common_non_syllables) {
  if (!have_dl()) return;

  EXPECT_EQ(2, wcwidth(L'ㅜ')); // Korean "crying" emoticon.
  EXPECT_EQ(2, wcwidth(L'ㅋ')); // Korean "laughing" emoticon.
}

TEST(wchar, wcswidth) {
  EXPECT_EQ(2, wcswidth(L"abc", 2));
  EXPECT_EQ(2, wcswidth(L"ab\t", 2));
  EXPECT_EQ(-1, wcswidth(L"a\tb", 2));
}

TEST(wchar, wcslcpy) {
#if defined(__BIONIC__)
  wchar_t dst[32];
  ASSERT_EQ(11U, wcslcpy(dst, L"hello world", 3));
  ASSERT_STREQ(L"he", dst);
  ASSERT_EQ(11U, wcslcpy(dst, L"hello world", 32));
  ASSERT_STREQ(L"hello world", dst);
#else
  GTEST_SKIP() << "no wcslcpy in glibc";
#endif
}

TEST(wchar, wcscat) {
  wchar_t dst[32];
  ASSERT_EQ(dst, wcscat(dst, L"hello"));
  ASSERT_STREQ(dst, L"hello");
  ASSERT_EQ(dst, wcscat(dst, L" world"));
  ASSERT_STREQ(dst, L"hello world");
}

TEST(wchar, wcscpy) {
  wchar_t dst[32];
  ASSERT_EQ(dst, wcscpy(dst, L"hello"));
  ASSERT_STREQ(dst, L"hello");
  ASSERT_EQ(dst, wcscpy(dst, L"world"));
  ASSERT_STREQ(dst, L"world");
}

TEST(wchar, wcscasecmp) {
  ASSERT_EQ(0, wcscasecmp(L"hello", L"HELLO"));
  ASSERT_TRUE(wcscasecmp(L"hello1", L"HELLO2") < 0);
  ASSERT_TRUE(wcscasecmp(L"hello2", L"HELLO1") > 0);
  ASSERT_TRUE(wcscasecmp(L"hello", L"HELL") > 0);
  ASSERT_TRUE(wcscasecmp(L"hell", L"HELLO") < 0);
}

TEST(wchar, wcscspn) {
  ASSERT_EQ(0U, wcscspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz"));
  ASSERT_EQ(5U, wcscspn(L"hello world", L" "));
  ASSERT_EQ(11U, wcscspn(L"hello world", L"!"));
}

TEST(wchar, wcsspn) {
  ASSERT_EQ(0U, wcsspn(L"hello world", L"!"));
  ASSERT_EQ(5U, wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz"));
  ASSERT_EQ(11U, wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz "));
}

TEST(wchar, wcsdup) {
  wchar_t* s = wcsdup(L"hello");
  ASSERT_STREQ(s, L"hello");
  free(s);
}

TEST(wchar, wcslcat) {
#if defined(__BIONIC__)
  wchar_t dst[4] = {};
  ASSERT_EQ(1U, wcslcat(dst, L"a", 4));
  ASSERT_EQ(7U, wcslcat(dst, L"bcdefg", 4));
  ASSERT_STREQ(dst, L"abc");
#else
  GTEST_SKIP() << "no wcslcpy in glibc";
#endif
}

TEST(wchar, wcsncasecmp) {
  ASSERT_EQ(0, wcsncasecmp(L"foo", L"bar", 0));

  ASSERT_EQ(0, wcsncasecmp(L"hello1", L"HELLO2", 5));
  ASSERT_TRUE(wcsncasecmp(L"hello1", L"HELLO2", 6) < 0);
  ASSERT_TRUE(wcsncasecmp(L"hello2", L"HELLO1", 6) > 0);
  ASSERT_TRUE(wcsncasecmp(L"hello", L"HELL", 5) > 0);
  ASSERT_TRUE(wcsncasecmp(L"hell", L"HELLO", 5) < 0);
}

TEST(wchar, wcsncat) {
  wchar_t dst[32];
  ASSERT_EQ(dst, wcsncat(dst, L"hello, world!", 5));
  ASSERT_STREQ(dst, L"hello");
  ASSERT_EQ(dst, wcsncat(dst, L"hello, world!", 0));
  ASSERT_STREQ(dst, L"hello");
  ASSERT_EQ(dst, wcsncat(dst, L", world!", 8));
  ASSERT_STREQ(dst, L"hello, world!");
}

TEST(wchar, wcsncmp) {
  ASSERT_EQ(0, wcsncmp(L"foo", L"bar", 0));
  ASSERT_EQ(0, wcsncmp(L"aaaa", L"aaab", 3));
  ASSERT_TRUE(wcsncmp(L"aaaa", L"aaab", 4) < 0);
  ASSERT_TRUE(wcsncmp(L"aaab", L"aaaa", 4) > 0);
}

TEST(wchar, wcsnlen) {
  ASSERT_EQ(2U, wcsnlen(L"hello", 2));
  ASSERT_EQ(5U, wcsnlen(L"hello", 5));
  ASSERT_EQ(5U, wcsnlen(L"hello", 666));
}

TEST(wchar, wcspbrk) {
  const wchar_t* s = L"hello, world!";
  ASSERT_EQ(nullptr, wcspbrk(s, L"-"));
  ASSERT_EQ(s, wcspbrk(s, L"abch"));
  ASSERT_EQ(s + 2, wcspbrk(s, L"l"));
  ASSERT_EQ(s + 5, wcspbrk(s, L",. !"));
}

TEST(wchar, wcstok) {
  wchar_t s[] = L"this is\ta\nstring";
  wchar_t* p;
  ASSERT_EQ(s, wcstok(s, L"\t\n ", &p));
  ASSERT_STREQ(s, L"this");
  ASSERT_STREQ(p, L"is\ta\nstring");
  ASSERT_EQ(s + 5, wcstok(nullptr, L"\t\n ", &p));
  ASSERT_STREQ(s + 5, L"is");
  ASSERT_STREQ(p, L"a\nstring");
  ASSERT_EQ(s + 8, wcstok(nullptr, L"\t\n ", &p));
  ASSERT_STREQ(s + 8, L"a");
  ASSERT_STREQ(p, L"string");
  ASSERT_EQ(s + 10, wcstok(nullptr, L"\t\n ", &p));
  ASSERT_STREQ(s + 10, L"string");
  ASSERT_EQ(nullptr, p);
}

TEST(wchar, wmemchr) {
  const wchar_t* s = L"hello, world!";
  ASSERT_EQ(s, wmemchr(s, L'h', 13));
  ASSERT_EQ(s + 5, wmemchr(s, L',', 13));
  ASSERT_EQ(s + 12, wmemchr(s, L'!', 13));
  ASSERT_EQ(nullptr, wmemchr(s, L'a', 13));
}

TEST(wchar, wmemcmp) {
  ASSERT_EQ(0, wmemcmp(L"aaaa", L"aaab", 3));
  ASSERT_TRUE(wmemcmp(L"aaaa", L"aaab", 4) < 0);
  ASSERT_TRUE(wmemcmp(L"aaab", L"aaaa", 4) > 0);
}

TEST(wchar, wmemcpy) {
  wchar_t dst[32] = {};
  ASSERT_EQ(dst, wmemcpy(dst, L"hello", 5));
  ASSERT_STREQ(dst, L"hello");
}

TEST(wchar, wmemmove) {
  wchar_t dst[32] = {};
  ASSERT_EQ(dst, wmemmove(dst, L"hello", 5));
  ASSERT_STREQ(dst, L"hello");
}

TEST(wchar, wmemset) {
  wchar_t dst[4] = {};
  ASSERT_EQ(dst, wmemset(dst, 0x12345678, 3));
  ASSERT_EQ(dst[0], wchar_t(0x12345678));
  ASSERT_EQ(dst[1], wchar_t(0x12345678));
  ASSERT_EQ(dst[2], wchar_t(0x12345678));
  ASSERT_EQ(dst[3], wchar_t(0));
  ASSERT_EQ(dst, wmemset(dst, L'y', 0));
  ASSERT_EQ(dst[0], wchar_t(0x12345678));
}
