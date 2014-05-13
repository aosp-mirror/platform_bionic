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

TEST(locale, setlocale) {
  EXPECT_STREQ("C", setlocale(LC_ALL, NULL));
  EXPECT_STREQ("C", setlocale(LC_CTYPE, NULL));

  errno = 0;
  EXPECT_EQ(NULL, setlocale(-1, NULL));
  EXPECT_EQ(EINVAL, errno);
  errno = 0;
  EXPECT_EQ(NULL, setlocale(13, NULL));
  EXPECT_EQ(EINVAL, errno);

#if defined(__BIONIC__)
  // The "" locale is implementation-defined. For bionic, it's the C locale.
  // glibc will give us something like "en_US.UTF-8", depending on the user's configuration.
  EXPECT_STREQ("C", setlocale(LC_ALL, ""));
#endif
  EXPECT_STREQ("C", setlocale(LC_ALL, "C"));
  EXPECT_STREQ("C", setlocale(LC_ALL, "POSIX"));

  errno = 0;
  EXPECT_EQ(NULL, setlocale(LC_ALL, "this-is-not-a-locale"));
  EXPECT_EQ(ENOENT, errno); // POSIX specified, not an implementation detail!
}

TEST(locale, newlocale) {
  errno = 0;
  EXPECT_EQ(0, newlocale(1 << 20, "C", 0));
  EXPECT_EQ(EINVAL, errno);

  locale_t l = newlocale(LC_ALL, "C", 0);
  ASSERT_TRUE(l != NULL);
  freelocale(l);

  errno = 0;
  EXPECT_EQ(0, newlocale(LC_ALL, "this-is-not-a-locale", 0));
  EXPECT_EQ(ENOENT, errno); // POSIX specified, not an implementation detail!
}

TEST(locale, duplocale) {
  locale_t cloned_global = duplocale(LC_GLOBAL_LOCALE);
  ASSERT_TRUE(cloned_global != NULL);
  freelocale(cloned_global);
}

TEST(locale, uselocale) {
  locale_t original = uselocale(NULL);
  EXPECT_FALSE(original == 0);
  EXPECT_EQ(LC_GLOBAL_LOCALE, original);

  locale_t n = newlocale(LC_ALL, "C", 0);
  EXPECT_FALSE(n == 0);
  EXPECT_FALSE(n == original);

  locale_t old = uselocale(n);
  EXPECT_TRUE(old == original);

  EXPECT_EQ(n, uselocale(NULL));
}
