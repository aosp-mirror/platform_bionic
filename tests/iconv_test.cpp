/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <iconv.h>

#include "utils.h"

#define INVALID_ICONV_T reinterpret_cast<iconv_t>(-1)

TEST(iconv, iconv_open_EINVAL) {
  errno = 0;
  ASSERT_EQ(INVALID_ICONV_T, iconv_open("silly", "silly"));
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  ASSERT_EQ(INVALID_ICONV_T, iconv_open("silly", "UTF-8"));
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  ASSERT_EQ(INVALID_ICONV_T, iconv_open("UTF-8", "silly"));
  ASSERT_ERRNO(EINVAL);
}

TEST(iconv, iconv_open_comparator) {
  // Examples from http://www.unicode.org/reports/tr22/#Charset_Alias_Matching:
  // "For example, the following names should match: "UTF-8", "utf8", "u.t.f-008", ..."
  iconv_t c;
  ASSERT_NE(INVALID_ICONV_T, c = iconv_open("UTF-8", "utf8"));
  ASSERT_EQ(0, iconv_close(c));
  ASSERT_NE(INVALID_ICONV_T, c = iconv_open("UTF-8", "u.t.f-008"));
  ASSERT_EQ(0, iconv_close(c));

  // "...but not "utf-80" or "ut8"."
  errno = 0;
  ASSERT_EQ(INVALID_ICONV_T, iconv_open("UTF-8", "utf-80"));
  ASSERT_ERRNO(EINVAL);
  errno = 0;
  ASSERT_EQ(INVALID_ICONV_T, iconv_open("UTF-8", "ut80"));
  ASSERT_ERRNO(EINVAL);
}

TEST(iconv, iconv_smoke) {
  const char* utf8 = "a٦ᄀ"; // U+0666 ٦ 0xd9 0xa6 // U+1100 ᄀ 0xe1 0x84 0x80
  char buf[BUFSIZ] = {};

  iconv_t c = iconv_open("UTF-32LE", "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c);

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(in);

  char* out = buf;
  size_t out_bytes = sizeof(buf);

  EXPECT_EQ(0U, iconv(c, &in, &in_bytes, &out, &out_bytes));

  wchar_t* utf16 = reinterpret_cast<wchar_t*>(buf);
  EXPECT_EQ(L'a', utf16[0]);
  EXPECT_EQ(L'٦', utf16[1]);
  EXPECT_EQ(L'ᄀ', utf16[2]);
  EXPECT_EQ(L'\0', utf16[3]);
  EXPECT_EQ(0U, in_bytes);
  EXPECT_EQ(sizeof(buf) - (3 /* chars */ * 4 /* bytes each */), out_bytes);

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_lossy_TRANSLIT) {
  const char* utf8 = "a٦ᄀz"; // U+0666 ٦ 0xd9 0xa6 // U+1100 ᄀ 0xe1 0x84 0x80
  char buf[BUFSIZ] = {};

  iconv_t c = iconv_open("ASCII//TRANSLIT", "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c);

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(in);

  char* out = buf;
  size_t out_bytes = sizeof(buf);

  // Two of the input characters (5 input bytes) aren't representable as ASCII.
  // With "//TRANSLIT", we use a replacement character, and report the number
  // of replacements.
  EXPECT_EQ(2U, iconv(c, &in, &in_bytes, &out, &out_bytes));

  EXPECT_EQ('a', buf[0]);
  EXPECT_EQ('?', buf[1]);
  EXPECT_EQ('?', buf[2]);
  EXPECT_EQ('z', buf[3]);
  EXPECT_EQ(0, buf[4]);
  EXPECT_EQ(0U, in_bytes);
  EXPECT_EQ(sizeof(buf) - 4, out_bytes);

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_lossy_IGNORE) {
  const char* utf8 = "a٦ᄀz"; // U+0666 ٦ 0xd9 0xa6 // U+1100 ᄀ 0xe1 0x84 0x80
  char buf[BUFSIZ] = {};

  iconv_t c = iconv_open("ASCII//IGNORE", "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c);

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(in);

  char* out = buf;
  size_t out_bytes = sizeof(buf);

  // Two of the input characters (5 input bytes) aren't representable as ASCII.
  // With "//IGNORE", we just skip them (but return failure).
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(EILSEQ);

  EXPECT_EQ('a', buf[0]);
  EXPECT_EQ('z', buf[1]);
  EXPECT_EQ(0, buf[2]);
  EXPECT_EQ(0U, in_bytes);
  EXPECT_EQ(sizeof(buf) - 2, out_bytes);

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_lossy) {
  const char* utf8 = "a٦ᄀz"; // U+0666 ٦ 0xd9 0xa6 // U+1100 ᄀ 0xe1 0x84 0x80
  char buf[BUFSIZ] = {};

  iconv_t c = iconv_open("ASCII", "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c);

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(in);

  char* out = buf;
  size_t out_bytes = sizeof(buf);

  // The second input character isn't representable as ASCII, so we stop there.
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(EILSEQ);

  EXPECT_EQ('a', buf[0]);
  EXPECT_EQ(0, buf[1]);
  EXPECT_EQ(6U, in_bytes); // Two bytes for ٦, three bytes for ᄀ, and one byte for z.
  EXPECT_EQ(sizeof(buf) - 1, out_bytes);

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_malformed_sequence_EILSEQ) {
  const char* utf8 = "a\xd9z"; // 0xd9 is the first byte of the two-byte U+0666 ٦.
  char buf[BUFSIZ] = {};

  iconv_t c = iconv_open("UTF-8", "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c);

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(in);

  char* out = buf;
  size_t out_bytes = sizeof(buf);

  // The second input byte is a malformed character, so we stop there.
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(EILSEQ);
  EXPECT_EQ('\xd9', *in); // *in is left pointing to the start of the invalid sequence.
  ++in;
  --in_bytes;
  errno = 0;
  EXPECT_EQ(0U, iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(0);

  EXPECT_EQ('a', buf[0]);
  EXPECT_EQ('z', buf[1]);
  EXPECT_EQ(0, buf[2]);
  EXPECT_EQ(0U, in_bytes);
  EXPECT_EQ(sizeof(buf) - 2, out_bytes);

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_incomplete_sequence_EINVAL) {
  const char* utf8 = "a\xd9"; // 0xd9 is the first byte of the two-byte U+0666 ٦.
  char buf[BUFSIZ] = {};

  iconv_t c = iconv_open("UTF-8", "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c);

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(in);

  char* out = buf;
  size_t out_bytes = sizeof(buf);

  // The second input byte is just the start of a character, and we don't have any more bytes.
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(EINVAL);
  EXPECT_EQ('\xd9', *in); // *in is left pointing to the start of the incomplete sequence.

  EXPECT_EQ('a', buf[0]);
  EXPECT_EQ(0, buf[1]);
  EXPECT_EQ(1U, in_bytes);
  EXPECT_EQ(sizeof(buf) - 1, out_bytes);

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_E2BIG) {
  const char* utf8 = "abc";
  char buf[BUFSIZ] = {};

  iconv_t c = iconv_open("UTF-8", "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c);

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(in);

  char* out = buf;
  size_t out_bytes = 1;

  // We need three bytes, so one isn't enough (but we will make progress).
  out_bytes = 1;
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(E2BIG);
  EXPECT_EQ(2U, in_bytes);
  EXPECT_EQ(0U, out_bytes);

  // Two bytes left, so zero isn't enough (and we can't even make progress).
  out_bytes = 0;
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(E2BIG);
  EXPECT_EQ(2U, in_bytes);
  EXPECT_EQ(0U, out_bytes);

  // Two bytes left, so one isn't enough (but we will make progress).
  out_bytes = 1;
  errno = 0;
  EXPECT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(E2BIG);
  EXPECT_EQ(1U, in_bytes);
  EXPECT_EQ(0U, out_bytes);

  // One byte left, so one byte is now enough.
  out_bytes = 1;
  errno = 0;
  EXPECT_EQ(0U, iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(0);
  EXPECT_EQ(0U, in_bytes);
  EXPECT_EQ(0U, out_bytes);

  EXPECT_EQ('a', buf[0]);
  EXPECT_EQ('b', buf[1]);
  EXPECT_EQ('c', buf[2]);
  EXPECT_EQ(0, buf[3]);

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_invalid_converter_EBADF) {
  char* in = nullptr;
  char* out = nullptr;
  size_t in_bytes = 0;
  size_t out_bytes = 0;
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(-1), iconv(INVALID_ICONV_T, &in, &in_bytes, &out, &out_bytes));
  ASSERT_ERRNO(EBADF);
}

TEST(iconv, iconv_close_invalid_converter_EBADF) {
  errno = 0;
  ASSERT_EQ(-1, iconv_close(INVALID_ICONV_T));
  ASSERT_ERRNO(EBADF);
}

static void RoundTrip(const char* dst_enc, const char* expected_bytes, size_t n) {
  // Examples from https://en.wikipedia.org/wiki/UTF-16.
  const char* utf8 = "$€𐐷"; // U+0024, U+20AC, U+10437.

  iconv_t c = iconv_open(dst_enc, "UTF-8");
  ASSERT_NE(INVALID_ICONV_T, c) << dst_enc;

  char* in = const_cast<char*>(utf8);
  size_t in_bytes = strlen(utf8);
  char buf[BUFSIZ] = {};
  char* out = buf;
  size_t out_bytes = sizeof(buf);
  size_t replacement_count = iconv(c, &in, &in_bytes, &out, &out_bytes);

  // Check we got the bytes we were expecting.
  for (size_t i = 0; i < n; ++i) {
    EXPECT_EQ(expected_bytes[i], buf[i]) << i << ' '<< dst_enc;
  }

  ASSERT_EQ(0, iconv_close(c));

  // We can't round-trip if there were replacements.
  if (strstr(dst_enc, "ascii")) {
    GTEST_LOG_(INFO) << "can't round-trip " << dst_enc << "\n";
    return;
  }
  ASSERT_EQ(0U, replacement_count);

  c = iconv_open("UTF-8", dst_enc);
  ASSERT_NE(INVALID_ICONV_T, c) << dst_enc;

  in = buf;
  in_bytes = n;
  char buf2[BUFSIZ] = {};
  out = buf2;
  out_bytes = sizeof(buf2);
  iconv(c, &in, &in_bytes, &out, &out_bytes);

  ASSERT_STREQ(utf8, buf2) << dst_enc;

  ASSERT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_round_trip_ascii) {
  RoundTrip("ascii//TRANSLIT", "$??", 3);
}

TEST(iconv, iconv_round_trip_utf8) {
  RoundTrip("utf8", "\x24\xe2\x82\xac\xf0\x90\x90\xb7", 8);
}

TEST(iconv, iconv_round_trip_utf16be) {
  RoundTrip("utf16be", "\x00\x24" "\x20\xac" "\xd8\x01\xdc\x37", 8);
}

TEST(iconv, iconv_round_trip_utf16le) {
  RoundTrip("utf16le", "\x24\x00" "\xac\x20" "\x01\xd8\x37\xdc", 8);
}

TEST(iconv, iconv_round_trip_utf32be) {
  RoundTrip("utf32be", "\x00\x00\x00\x24" "\x00\x00\x20\xac" "\x00\x01\x04\x37", 12);
}

TEST(iconv, iconv_round_trip_utf32le) {
  RoundTrip("utf32le", "\x24\x00\x00\x00" "\xac\x20\x00\x00" "\x37\x04\x01\x00", 12);
}

TEST(iconv, iconv_round_trip_wchar_t) {
  RoundTrip("wchar_t", "\x24\x00\x00\x00" "\xac\x20\x00\x00" "\x37\x04\x01\x00", 12);
}

static void Check(int expected_errno, const char* src_enc, const char* src, size_t n) {
  iconv_t c = iconv_open("wchar_t", src_enc);
  char* in = const_cast<char*>(src);
  size_t in_bytes = n;
  wchar_t out_buf[16];
  size_t out_bytes = sizeof(out_buf);
  char* out = reinterpret_cast<char*>(out_buf);
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(-1), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(expected_errno);
  EXPECT_EQ(0, iconv_close(c));
}

TEST(iconv, iconv_EILSEQ_ascii) {
  Check(EILSEQ, "ASCII", "\xac", 1); // > 0x7f, so not ASCII.
}

TEST(iconv, iconv_EILSEQ_utf8_initial) {
  Check(EILSEQ, "utf8", "\x82", 1); // Invalid initial byte.
}

TEST(iconv, iconv_EILSEQ_utf8_non_initial) {
  Check(EILSEQ, "utf8", "\xe2\xe2\x82", 3); // Invalid second byte.
}

TEST(iconv, iconv_EILSEQ_utf16be_low_surrogate_first) {
  Check(EILSEQ, "utf16be", "\xdc\x37" "\xd8\x01", 4);
}

TEST(iconv, iconv_EILSEQ_utf16le_low_surrogate_first) {
  Check(EILSEQ, "utf16le", "\x37\xdc" "\x01\xd8", 4);
}

TEST(iconv, iconv_EINVAL_utf8_short) {
  Check(EINVAL, "utf8", "\xe2\x82", 2); // Missing final byte of 3-byte sequence.
}

TEST(iconv, iconv_EINVAL_utf16be_short) {
  Check(EINVAL, "utf16be", "\x00", 1); // Missing second byte.
}

TEST(iconv, iconv_EINVAL_utf16be_missing_low_surrogate) {
  Check(EINVAL, "utf16be", "\xd8\x01", 2);
}

TEST(iconv, iconv_EINVAL_utf16be_half_low_surrogate) {
  Check(EINVAL, "utf16be", "\xd8\x01\xdc", 3);
}

TEST(iconv, iconv_EINVAL_utf16le_short) {
  Check(EINVAL, "utf16le", "\x24", 1); // Missing second byte.
}

TEST(iconv, iconv_EINVAL_utf16le_missing_low_surrogate) {
  Check(EINVAL, "utf16le", "\x01\xd8", 2);
}

TEST(iconv, iconv_EINVAL_utf16le_half_low_surrogate) {
  Check(EINVAL, "utf16le", "\x01\xd8\x37", 3);
}

TEST(iconv, iconv_EINVAL_utf32be_short) {
  Check(EINVAL, "utf32be", "\x00\x00\x00", 3); // Missing final byte.
}

TEST(iconv, iconv_EINVAL_utf32le_short) {
  Check(EINVAL, "utf32le", "\x24\x00\x00", 3); // Missing final byte.
}

TEST(iconv, iconv_initial_shift_state) {
  // POSIX: "For state-dependent encodings, the conversion descriptor
  // cd is placed into its initial shift state by a call for which inbuf
  // is a null pointer, or for which inbuf points to a null pointer."
  iconv_t c = iconv_open("utf8", "utf8");
  char* in = nullptr;
  size_t in_bytes = 0;
  wchar_t out_buf[16];
  size_t out_bytes = sizeof(out_buf);
  char* out = reinterpret_cast<char*>(out_buf);

  // Points to a null pointer...
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(0), iconv(c, &in, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(0);
  EXPECT_EQ(sizeof(out_buf), out_bytes);

  // Is a null pointer...
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(0), iconv(c, nullptr, &in_bytes, &out, &out_bytes));
  EXPECT_ERRNO(0);
  EXPECT_EQ(sizeof(out_buf), out_bytes);

  // Is a null pointer and so is in_bytes. This isn't specified by POSIX, but
  // glibc and macOS both allow that, where Android historically didn't.
  // https://issuetracker.google.com/180598400
  errno = 0;
  ASSERT_EQ(static_cast<size_t>(0), iconv(c, nullptr, nullptr, &out, &out_bytes));
  EXPECT_ERRNO(0);
  EXPECT_EQ(sizeof(out_buf), out_bytes);

  EXPECT_EQ(0, iconv_close(c));
}
