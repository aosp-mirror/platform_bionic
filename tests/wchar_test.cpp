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
#include <limits.h>
#include <locale.h>
#include <stdint.h>
#include <wchar.h>

TEST(wchar, sizeof_wchar_t) {
  EXPECT_EQ(4U, sizeof(wchar_t));
  EXPECT_EQ(4U, sizeof(wint_t));
}

TEST(wchar, mbrlen) {
  char bytes[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
  EXPECT_EQ(0U, mbrlen(&bytes[0], 0, NULL));
  EXPECT_EQ(1U, mbrlen(&bytes[0], 1, NULL));

  EXPECT_EQ(1U, mbrlen(&bytes[4], 1, NULL));
  EXPECT_EQ(0U, mbrlen(&bytes[5], 1, NULL));
}

TEST(wchar, wctomb_wcrtomb) {
  // wctomb and wcrtomb behave differently when s == NULL.
  EXPECT_EQ(0, wctomb(NULL, L'h'));
  EXPECT_EQ(0, wctomb(NULL, L'\0'));
  EXPECT_EQ(1U, wcrtomb(NULL, L'\0', NULL));
  EXPECT_EQ(1U, wcrtomb(NULL, L'h', NULL));

  char bytes[MB_LEN_MAX];

  // wctomb and wcrtomb behave similarly for the null wide character.
  EXPECT_EQ(1, wctomb(bytes, L'\0'));
  EXPECT_EQ(1U, wcrtomb(bytes, L'\0', NULL));

  // ...and for regular characters.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1, wctomb(bytes, L'h'));
  EXPECT_EQ('h', bytes[0]);
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, wcrtomb(bytes, L'h', NULL));
  EXPECT_EQ('h', bytes[0]);

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(1U, wcrtomb(bytes, L'h', NULL));
  EXPECT_EQ('h', bytes[0]);
  // 2-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(2U, wcrtomb(bytes, 0x00a2, NULL));
  EXPECT_EQ('\xc2', bytes[0]);
  EXPECT_EQ('\xa2', bytes[1]);
  // 3-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(3U, wcrtomb(bytes, 0x20ac, NULL));
  EXPECT_EQ('\xe2', bytes[0]);
  EXPECT_EQ('\x82', bytes[1]);
  EXPECT_EQ('\xac', bytes[2]);
  // 4-byte UTF-8.
  memset(bytes, 0, sizeof(bytes));
  EXPECT_EQ(4U, wcrtomb(bytes, 0x24b62, NULL));
  EXPECT_EQ('\xf0', bytes[0]);
  EXPECT_EQ('\xa4', bytes[1]);
  EXPECT_EQ('\xad', bytes[2]);
  EXPECT_EQ('\xa2', bytes[3]);
  // Invalid code point.
  EXPECT_EQ(static_cast<size_t>(-1), wcrtomb(bytes, 0xffffffff, NULL));
  EXPECT_EQ(EILSEQ, errno);
}

TEST(wchar, wcrtomb_start_state) {
  char out[MB_LEN_MAX];
  mbstate_t ps;

  // Any non-initial state is invalid when calling wcrtomb.
  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(NULL, "\xc2", 1, &ps));
  EXPECT_EQ(static_cast<size_t>(-1), wcrtomb(out, 0x00a2, &ps));
  EXPECT_EQ(EILSEQ, errno);

  // If the first argument to wcrtomb is NULL or the second is L'\0' the shift
  // state should be reset.
  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(NULL, "\xc2", 1, &ps));
  EXPECT_EQ(1U, wcrtomb(NULL, 0x00a2, &ps));
  EXPECT_TRUE(mbsinit(&ps));

  memset(&ps, 0, sizeof(ps));
  EXPECT_EQ(static_cast<size_t>(-2), mbrtowc(NULL, "\xf0\xa4", 1, &ps));
  EXPECT_EQ(1U, wcrtomb(out, L'\0', &ps));
  EXPECT_TRUE(mbsinit(&ps));
}

TEST(wchar, wcstombs_wcrtombs) {
  const wchar_t chars[] = { L'h', L'e', L'l', L'l', L'o', 0 };
  const wchar_t bad_chars[] = { L'h', L'i', static_cast<wchar_t>(0xffffffff), 0 };
  const wchar_t* src;
  char bytes[BUFSIZ];

  // Given a NULL destination, these functions count valid characters.
  EXPECT_EQ(5U, wcstombs(NULL, chars, 0));
  EXPECT_EQ(5U, wcstombs(NULL, chars, 4));
  EXPECT_EQ(5U, wcstombs(NULL, chars, 256));
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(NULL, &src, 0, NULL));
  EXPECT_EQ(&chars[0], src);
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(NULL, &src, 4, NULL));
  EXPECT_EQ(&chars[0], src);
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(NULL, &src, 256, NULL));
  EXPECT_EQ(&chars[0], src);

  // An unrepresentable char just returns an error from wcstombs...
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), wcstombs(NULL, bad_chars, 0));
  EXPECT_EQ(EILSEQ, errno);
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), wcstombs(NULL, bad_chars, 256));
  EXPECT_EQ(EILSEQ, errno);

  // And wcsrtombs doesn't tell us where it got stuck because we didn't ask it
  // to actually convert anything...
  errno = 0;
  src = bad_chars;
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(NULL, &src, 0, NULL));
  EXPECT_EQ(&bad_chars[0], src);
  EXPECT_EQ(EILSEQ, errno);
  errno = 0;
  src = bad_chars;
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(NULL, &src, 256, NULL));
  EXPECT_EQ(&bad_chars[0], src);
  EXPECT_EQ(EILSEQ, errno);

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
  EXPECT_EQ(EILSEQ, errno);
  bytes[3] = 0;
  EXPECT_STREQ("hix", bytes);

  // wcsrtombs is a bit more informative...
  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(0U, wcsrtombs(bytes, &src, 0, NULL));
  EXPECT_EQ(&chars[0], src); // No input consumed.
  EXPECT_EQ(EILSEQ, errno);

  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(4U, wcsrtombs(bytes, &src, 4, NULL));
  EXPECT_EQ(&chars[4], src); // Some input consumed.
  EXPECT_EQ(EILSEQ, errno);
  bytes[5] = 0;
  EXPECT_STREQ("hellx", bytes);

  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(bytes, &src, 256, NULL));
  EXPECT_EQ(NULL, src); // All input consumed!
  EXPECT_EQ(EILSEQ, errno);
  EXPECT_STREQ("hello", bytes);

  memset(bytes, 'x', sizeof(bytes));
  src = chars;
  EXPECT_EQ(5U, wcsrtombs(bytes, &src, 6, NULL));
  EXPECT_EQ(NULL, src); // All input consumed.
  EXPECT_EQ(EILSEQ, errno);
  EXPECT_STREQ("hello", bytes);

  memset(bytes, 'x', sizeof(bytes));
  src = bad_chars;
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(bytes, &src, 256, NULL));
  EXPECT_EQ(&bad_chars[2], src);
  EXPECT_EQ(EILSEQ, errno);
  bytes[3] = 0;
  EXPECT_STREQ("hix", bytes);

  // Any non-initial state is invalid when calling wcsrtombs.
  mbstate_t ps;
  src = chars;
  memset(&ps, 0, sizeof(ps));
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(NULL, "\xc2", 1, &ps));
  EXPECT_EQ(static_cast<size_t>(-1), wcsrtombs(NULL, &src, 0, &ps));
  EXPECT_EQ(EILSEQ, errno);
}

TEST(wchar, limits) {
  ASSERT_LT(WCHAR_MIN, WCHAR_MAX);
}

TEST(wchar, wcsstr) {
  const wchar_t* haystack = L"matches hello world, not the second hello world";
  const wchar_t* empty_needle = L"";
  const wchar_t* good_needle = L"ll";
  const wchar_t* bad_needle = L"wort";

  ASSERT_EQ(haystack, wcsstr(haystack, empty_needle));
  ASSERT_EQ(&haystack[10], wcsstr(haystack, good_needle));
  ASSERT_EQ(NULL, wcsstr(haystack, bad_needle));
}

TEST(wchar, mbtowc) {
  wchar_t out[8];

  out[0] = 'x';
  ASSERT_EQ(0, mbtowc(out, "hello", 0));
  ASSERT_EQ('x', out[0]);

  ASSERT_EQ(0, mbtowc(out, "hello", 0));
  ASSERT_EQ(0, mbtowc(out, "", 0));
  ASSERT_EQ(1, mbtowc(out, "hello", 1));
  ASSERT_EQ(L'h', out[0]);

  ASSERT_EQ(0, mbtowc(NULL, "hello", 0));
  ASSERT_EQ(0, mbtowc(NULL, "", 0));
  ASSERT_EQ(1, mbtowc(NULL, "hello", 1));

  ASSERT_EQ(0, mbtowc(NULL, NULL, 0));
}

TEST(wchar, mbrtowc) {
  wchar_t out[8];

  out[0] = 'x';
  ASSERT_EQ(0U, mbrtowc(out, "hello", 0, NULL));
  ASSERT_EQ('x', out[0]);

  ASSERT_EQ(0U, mbrtowc(out, "hello", 0, NULL));
  ASSERT_EQ(0U, mbrtowc(out, "", 0, NULL));
  ASSERT_EQ(1U, mbrtowc(out, "hello", 1, NULL));
  ASSERT_EQ(L'h', out[0]);

  ASSERT_EQ(0U, mbrtowc(NULL, "hello", 0, NULL));
  ASSERT_EQ(0U, mbrtowc(NULL, "", 0, NULL));
  ASSERT_EQ(1U, mbrtowc(NULL, "hello", 1, NULL));

  ASSERT_EQ(0U, mbrtowc(NULL, NULL, 0, NULL));

  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  // 1-byte UTF-8.
  ASSERT_EQ(1U, mbrtowc(out, "abcdef", 6, NULL));
  ASSERT_EQ(L'a', out[0]);
  // 2-byte UTF-8.
  ASSERT_EQ(2U, mbrtowc(out, "\xc2\xa2" "cdef", 6, NULL));
  ASSERT_EQ(static_cast<wchar_t>(0x00a2), out[0]);
  // 3-byte UTF-8.
  ASSERT_EQ(3U, mbrtowc(out, "\xe2\x82\xac" "def", 6, NULL));
  ASSERT_EQ(static_cast<wchar_t>(0x20ac), out[0]);
  // 4-byte UTF-8.
  ASSERT_EQ(4U, mbrtowc(out, "\xf0\xa4\xad\xa2" "ef", 6, NULL));
  ASSERT_EQ(static_cast<wchar_t>(0x24b62), out[0]);
#if defined(__BIONIC__) // glibc allows this.
  // Illegal 5-byte UTF-8.
  ASSERT_EQ(static_cast<size_t>(-1), mbrtowc(out, "\xf8\xa1\xa2\xa3\xa4" "f", 6, NULL));
  ASSERT_EQ(EILSEQ, errno);
#endif
  // Illegal over-long sequence.
  ASSERT_EQ(static_cast<size_t>(-1), mbrtowc(out, "\xf0\x82\x82\xac" "ef", 6, NULL));
  ASSERT_EQ(EILSEQ, errno);
}

void test_mbrtowc_incomplete(mbstate_t* ps) {
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
  ASSERT_EQ(EILSEQ, errno);
}

TEST(wchar, mbrtowc_incomplete) {
  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));

  test_mbrtowc_incomplete(&ps);
  test_mbrtowc_incomplete(NULL);
}

void test_mbsrtowcs(mbstate_t* ps) {
  wchar_t out[4];

  const char* valid = "A" "\xc2\xa2" "\xe2\x82\xac" "\xf0\xa4\xad\xa2" "ef";
  ASSERT_EQ(4U, mbsrtowcs(out, &valid, 4, ps));
  ASSERT_EQ(L'A', out[0]);
  ASSERT_EQ(static_cast<wchar_t>(0x00a2), out[1]);
  ASSERT_EQ(static_cast<wchar_t>(0x20ac), out[2]);
  ASSERT_EQ(static_cast<wchar_t>(0x24b62), out[3]);
  // Check that valid has advanced to the next unread character.
  ASSERT_EQ('e', *valid);

  wmemset(out, L'x', sizeof(out) / sizeof(wchar_t));
  ASSERT_EQ(2U, mbsrtowcs(out, &valid, 4, ps));
  ASSERT_EQ(L'e', out[0]);
  ASSERT_EQ(L'f', out[1]);
  ASSERT_EQ(L'\0', out[2]);
  // Check that we didn't clobber the rest of out.
  ASSERT_EQ(L'x', out[3]);
  // Check that valid has advanced to the end of the string.
  ASSERT_EQ(nullptr, valid);

  const char* invalid = "A" "\xc2\x20" "ef";
  ASSERT_EQ(static_cast<size_t>(-1), mbsrtowcs(out, &invalid, 4, ps));
  EXPECT_EQ(EILSEQ, errno);
  ASSERT_EQ('\xc2', *invalid);

  const char* incomplete = "A" "\xc2";
  ASSERT_EQ(static_cast<size_t>(-1), mbsrtowcs(out, &incomplete, 2, ps));
  EXPECT_EQ(EILSEQ, errno);
  ASSERT_EQ('\xc2', *incomplete);
}

TEST(wchar, mbsrtowcs) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_CTYPE, "C.UTF-8"));
  uselocale(LC_GLOBAL_LOCALE);

  mbstate_t ps;
  memset(&ps, 0, sizeof(ps));
  test_mbsrtowcs(&ps);
  test_mbsrtowcs(NULL);

  // Invalid multi byte continuation.
  const char* invalid = "\x20";
  wchar_t out;
  ASSERT_EQ(static_cast<size_t>(-2), mbrtowc(&out, "\xc2", 1, &ps));
  ASSERT_EQ(static_cast<size_t>(-1), mbsrtowcs(&out, &invalid, 1, &ps));
  EXPECT_EQ(EILSEQ, errno);
  ASSERT_EQ('\x20', *invalid);
}

TEST(wchar, wcstod) {
  ASSERT_DOUBLE_EQ(1.23, wcstod(L"1.23", NULL));
}

TEST(wchar, wcstof) {
  ASSERT_FLOAT_EQ(1.23f, wcstof(L"1.23", NULL));
}

TEST(wchar, wcstol) {
  ASSERT_EQ(123L, wcstol(L"123", NULL, 0));
}

TEST(wchar, wcstoll) {
  ASSERT_EQ(123LL, wcstol(L"123", NULL, 0));
}

TEST(wchar, wcstold) {
  ASSERT_DOUBLE_EQ(1.23L, wcstold(L"1.23", NULL));
}

TEST(wchar, wcstoul) {
  ASSERT_EQ(123UL, wcstoul(L"123", NULL, 0));
}

TEST(wchar, wcstoull) {
  ASSERT_EQ(123ULL, wcstoul(L"123", NULL, 0));
}

TEST(wchar, mbsnrtowcs) {
  wchar_t dst[128];
  const char* s = "hello, world!";
  const char* src;

  memset(dst, 0, sizeof(dst));
  src = s;
  ASSERT_EQ(0U, mbsnrtowcs(dst, &src, 0, 0, NULL));

  memset(dst, 0, sizeof(dst));
  src = s;
  ASSERT_EQ(2U, mbsnrtowcs(dst, &src, 2, 123, NULL)); // glibc chokes on SIZE_MAX here.
  ASSERT_EQ(L'h', dst[0]);
  ASSERT_EQ(L'e', dst[1]);
  ASSERT_EQ(&s[2], src);

  memset(dst, 0, sizeof(dst));
  src = s;
  ASSERT_EQ(3U, mbsnrtowcs(dst, &src, SIZE_MAX, 3, NULL));
  ASSERT_EQ(L'h', dst[0]);
  ASSERT_EQ(L'e', dst[1]);
  ASSERT_EQ(L'l', dst[2]);
  ASSERT_EQ(&s[3], src);
}

TEST(wchar, wcsftime) {
  setenv("TZ", "UTC", 1);

  struct tm t;
  memset(&t, 0, sizeof(tm));
  t.tm_year = 200;
  t.tm_mon = 2;
  t.tm_mday = 10;

  wchar_t buf[64];

  EXPECT_EQ(24U, wcsftime(buf, sizeof(buf), L"%c", &t));
  EXPECT_STREQ(L"Sun Mar 10 00:00:00 2100", buf);
}

TEST(wchar, wmemmove) {
  const wchar_t const_wstr[] = L"This is a test of something or other.....";
  wchar_t* wstr = new wchar_t[sizeof(const_wstr)];

  wmemmove(wstr, const_wstr, sizeof(const_wstr)/sizeof(wchar_t));
  EXPECT_STREQ(const_wstr, wstr);

  wmemmove(wstr+5, wstr, sizeof(const_wstr)/sizeof(wchar_t) - 6);
  EXPECT_STREQ(L"This This is a test of something or other", wstr);
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
  n = mbrtowc(&wc, "x", MB_CUR_MAX, NULL);
  EXPECT_EQ(1U, n);
  EXPECT_EQ(L'x', wc);
  // 2-byte character.
  n = mbrtowc(&wc, "\xc2\xa2", MB_CUR_MAX, NULL);
  EXPECT_EQ(2U, n);
  EXPECT_EQ(L'¢', wc);
  // 3-byte character.
  n = mbrtowc(&wc, "\xe2\x82\xac", MB_CUR_MAX, NULL);
  EXPECT_EQ(3U, n);
  EXPECT_EQ(L'€', wc);
  // 4-byte character.
  n = mbrtowc(&wc, "\xf0\xa4\xad\xa2", MB_CUR_MAX, NULL);
  EXPECT_EQ(4U, n);
  EXPECT_EQ(L'𤭢', wc);
}
