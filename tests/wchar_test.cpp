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

#include <limits.h>
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
  bytes[0] = 'x';
  EXPECT_EQ(1, wctomb(bytes, L'h'));
  EXPECT_EQ('h', bytes[0]);

  bytes[0] = 'x';
  EXPECT_EQ(1U, wcrtomb(bytes, L'h', NULL));
  EXPECT_EQ('h', bytes[0]);
}
