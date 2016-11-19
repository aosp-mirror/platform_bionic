/*
 * Copyright (C) 2016 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <gtest/gtest.h>

#include <langinfo.h>

TEST(langinfo, category_CTYPE) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  EXPECT_STREQ("UTF-8", nl_langinfo(CODESET));
}

TEST(langinfo, category_TIME) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

#if defined(__BIONIC__)
  // bionic's C locale is ISO rather than en_US.
  EXPECT_STREQ("%F %T %z", nl_langinfo(D_T_FMT));
  EXPECT_STREQ("%F", nl_langinfo(D_FMT));
#else
  EXPECT_STREQ("%a %d %b %Y %r %Z", nl_langinfo(D_T_FMT));
  EXPECT_STREQ("%m/%d/%Y", nl_langinfo(D_FMT));
#endif
  EXPECT_STREQ("%T", nl_langinfo(T_FMT));
  EXPECT_STREQ("%I:%M:%S %p", nl_langinfo(T_FMT_AMPM));
  EXPECT_STREQ("AM", nl_langinfo(AM_STR));
  EXPECT_STREQ("PM", nl_langinfo(PM_STR));
  EXPECT_STREQ("Sunday", nl_langinfo(DAY_1));
  EXPECT_STREQ("Monday", nl_langinfo(DAY_2));
  EXPECT_STREQ("Tuesday", nl_langinfo(DAY_3));
  EXPECT_STREQ("Wednesday", nl_langinfo(DAY_4));
  EXPECT_STREQ("Thursday", nl_langinfo(DAY_5));
  EXPECT_STREQ("Friday", nl_langinfo(DAY_6));
  EXPECT_STREQ("Saturday", nl_langinfo(DAY_7));
  EXPECT_STREQ("Sun", nl_langinfo(ABDAY_1));
  EXPECT_STREQ("Mon", nl_langinfo(ABDAY_2));
  EXPECT_STREQ("Tue", nl_langinfo(ABDAY_3));
  EXPECT_STREQ("Wed", nl_langinfo(ABDAY_4));
  EXPECT_STREQ("Thu", nl_langinfo(ABDAY_5));
  EXPECT_STREQ("Fri", nl_langinfo(ABDAY_6));
  EXPECT_STREQ("Sat", nl_langinfo(ABDAY_7));
  EXPECT_STREQ("January", nl_langinfo(MON_1));
  EXPECT_STREQ("February", nl_langinfo(MON_2));
  EXPECT_STREQ("March", nl_langinfo(MON_3));
  EXPECT_STREQ("April", nl_langinfo(MON_4));
  EXPECT_STREQ("May", nl_langinfo(MON_5));
  EXPECT_STREQ("June", nl_langinfo(MON_6));
  EXPECT_STREQ("July", nl_langinfo(MON_7));
  EXPECT_STREQ("August", nl_langinfo(MON_8));
  EXPECT_STREQ("September", nl_langinfo(MON_9));
  EXPECT_STREQ("October", nl_langinfo(MON_10));
  EXPECT_STREQ("November", nl_langinfo(MON_11));
  EXPECT_STREQ("December", nl_langinfo(MON_12));
  EXPECT_STREQ("Jan", nl_langinfo(ABMON_1));
  EXPECT_STREQ("Feb", nl_langinfo(ABMON_2));
  EXPECT_STREQ("Mar", nl_langinfo(ABMON_3));
  EXPECT_STREQ("Apr", nl_langinfo(ABMON_4));
  EXPECT_STREQ("May", nl_langinfo(ABMON_5));
  EXPECT_STREQ("Jun", nl_langinfo(ABMON_6));
  EXPECT_STREQ("Jul", nl_langinfo(ABMON_7));
  EXPECT_STREQ("Aug", nl_langinfo(ABMON_8));
  EXPECT_STREQ("Sep", nl_langinfo(ABMON_9));
  EXPECT_STREQ("Oct", nl_langinfo(ABMON_10));
  EXPECT_STREQ("Nov", nl_langinfo(ABMON_11));
  EXPECT_STREQ("Dec", nl_langinfo(ABMON_12));
  EXPECT_STREQ("", nl_langinfo(ERA));
  EXPECT_STREQ("", nl_langinfo(ERA_D_FMT));
  EXPECT_STREQ("", nl_langinfo(ERA_D_T_FMT));
  EXPECT_STREQ("", nl_langinfo(ERA_T_FMT));
  EXPECT_STREQ("", nl_langinfo(ALT_DIGITS));
}

TEST(langinfo, category_NUMERIC) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  EXPECT_STREQ(".", nl_langinfo(RADIXCHAR));
  EXPECT_STREQ("", nl_langinfo(THOUSEP));
}

TEST(langinfo, category_MESSAGES) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  EXPECT_STREQ("^[yY]", nl_langinfo(YESEXPR));
  EXPECT_STREQ("^[nN]", nl_langinfo(NOEXPR));
}

TEST(langinfo, category_MONETARY) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  // POSIX says that if the currency symbol is the empty string (as it is for
  // the C locale), an implementation can return the empty string and not
  // include the leading [+-.] that signifies where the currency symbol should
  // appear. For consistency with localeconv (which POSIX says to prefer for
  // RADIXCHAR, THOUSEP, and CRNCYSTR) we return the empty string. glibc
  // disagrees.
#if defined(__BIONIC__)
  EXPECT_STREQ("", nl_langinfo(CRNCYSTR));
#else
  EXPECT_STREQ("-", nl_langinfo(CRNCYSTR));
#endif
}

TEST(langinfo, invalid) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  EXPECT_STREQ("", nl_langinfo(-1));
  EXPECT_STREQ("", nl_langinfo(0));
  EXPECT_STREQ("", nl_langinfo(666));
}

TEST(langinfo, matches_localeconv) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  EXPECT_STREQ(localeconv()->decimal_point, nl_langinfo(RADIXCHAR));
  EXPECT_STREQ(localeconv()->thousands_sep, nl_langinfo(THOUSEP));
#if defined(__BIONIC__)
  // (See comment in category_MONETARY test.)
  EXPECT_STREQ(localeconv()->currency_symbol, nl_langinfo(CRNCYSTR));
#endif
}
