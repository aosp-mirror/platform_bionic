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
#include <locale.h>

TEST(locale, localeconv) {
  EXPECT_STREQ(".", localeconv()->decimal_point);
  EXPECT_STREQ("", localeconv()->thousands_sep);
  EXPECT_STREQ("", localeconv()->grouping);
  EXPECT_STREQ("", localeconv()->int_curr_symbol);
  EXPECT_STREQ("", localeconv()->currency_symbol);
  EXPECT_STREQ("", localeconv()->mon_decimal_point);
  EXPECT_STREQ("", localeconv()->mon_thousands_sep);
  EXPECT_STREQ("", localeconv()->mon_grouping);
  EXPECT_STREQ("", localeconv()->positive_sign);
  EXPECT_STREQ("", localeconv()->negative_sign);
  EXPECT_EQ(CHAR_MAX, localeconv()->int_frac_digits);
  EXPECT_EQ(CHAR_MAX, localeconv()->frac_digits);
  EXPECT_EQ(CHAR_MAX, localeconv()->p_cs_precedes);
  EXPECT_EQ(CHAR_MAX, localeconv()->p_sep_by_space);
  EXPECT_EQ(CHAR_MAX, localeconv()->n_cs_precedes);
  EXPECT_EQ(CHAR_MAX, localeconv()->n_sep_by_space);
  EXPECT_EQ(CHAR_MAX, localeconv()->p_sign_posn);
  EXPECT_EQ(CHAR_MAX, localeconv()->n_sign_posn);
  EXPECT_EQ(CHAR_MAX, localeconv()->int_p_cs_precedes);
  EXPECT_EQ(CHAR_MAX, localeconv()->int_p_sep_by_space);
  EXPECT_EQ(CHAR_MAX, localeconv()->int_n_cs_precedes);
  EXPECT_EQ(CHAR_MAX, localeconv()->int_n_sep_by_space);
  EXPECT_EQ(CHAR_MAX, localeconv()->int_p_sign_posn);
  EXPECT_EQ(CHAR_MAX, localeconv()->int_n_sign_posn);
}
