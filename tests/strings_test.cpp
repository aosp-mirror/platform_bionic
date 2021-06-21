/*
 * Copyright (C) 2013 The Android Open Source Project
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
#include <locale.h>
#include <strings.h>

#if defined(NOFORTIFY)
#define STRINGS_TEST strings_nofortify
#else
#define STRINGS_TEST strings
#endif

TEST(STRINGS_TEST, ffs) {
  ASSERT_EQ( 0, ffs(0x00000000));
  ASSERT_EQ( 1, ffs(0x00000001));
  ASSERT_EQ( 6, ffs(0x00000020));
  ASSERT_EQ(11, ffs(0x00000400));
  ASSERT_EQ(16, ffs(0x00008000));
  ASSERT_EQ(17, ffs(0x00010000));
  ASSERT_EQ(22, ffs(0x00200000));
  ASSERT_EQ(27, ffs(0x04000000));
  ASSERT_EQ(32, ffs(0x80000000));
}

TEST(STRINGS_TEST, ffsl) {
  ASSERT_EQ( 0, ffsl(0x00000000L));
  ASSERT_EQ( 1, ffsl(0x00000001L));
  ASSERT_EQ( 6, ffsl(0x00000020L));
  ASSERT_EQ(11, ffsl(0x00000400L));
  ASSERT_EQ(16, ffsl(0x00008000L));
  ASSERT_EQ(17, ffsl(0x00010000L));
  ASSERT_EQ(22, ffsl(0x00200000L));
  ASSERT_EQ(27, ffsl(0x04000000L));
  ASSERT_EQ(32, ffsl(0x80000000L));
#if defined(__LP64__)
  ASSERT_EQ(33, ffsl(0x0000000100000000L));
  ASSERT_EQ(38, ffsl(0x0000002000000000L));
  ASSERT_EQ(43, ffsl(0x0000040000000000L));
  ASSERT_EQ(48, ffsl(0x0000800000000000L));
  ASSERT_EQ(49, ffsl(0x0001000000000000L));
  ASSERT_EQ(54, ffsl(0x0020000000000000L));
  ASSERT_EQ(59, ffsl(0x0400000000000000L));
  ASSERT_EQ(64, ffsl(0x8000000000000000L));
#endif
}

TEST(STRINGS_TEST, ffsll) {
  ASSERT_EQ( 0, ffsll(0x0000000000000000LL));
  ASSERT_EQ( 1, ffsll(0x0000000000000001LL));
  ASSERT_EQ( 6, ffsll(0x0000000000000020LL));
  ASSERT_EQ(11, ffsll(0x0000000000000400LL));
  ASSERT_EQ(16, ffsll(0x0000000000008000LL));
  ASSERT_EQ(17, ffsll(0x0000000000010000LL));
  ASSERT_EQ(22, ffsll(0x0000000000200000LL));
  ASSERT_EQ(27, ffsll(0x0000000004000000LL));
  ASSERT_EQ(32, ffsll(0x0000000080000000LL));
  ASSERT_EQ(33, ffsll(0x0000000100000000LL));
  ASSERT_EQ(38, ffsll(0x0000002000000000LL));
  ASSERT_EQ(43, ffsll(0x0000040000000000LL));
  ASSERT_EQ(48, ffsll(0x0000800000000000LL));
  ASSERT_EQ(49, ffsll(0x0001000000000000LL));
  ASSERT_EQ(54, ffsll(0x0020000000000000LL));
  ASSERT_EQ(59, ffsll(0x0400000000000000LL));
  ASSERT_EQ(64, ffsll(0x8000000000000000LL));
}

TEST(STRINGS_TEST, strcasecmp) {
  ASSERT_EQ(0, strcasecmp("hello", "HELLO"));
  ASSERT_LT(strcasecmp("hello1", "hello2"), 0);
  ASSERT_GT(strcasecmp("hello2", "hello1"), 0);
}

TEST(STRINGS_TEST, strcasecmp_l) {
  locale_t l = newlocale(LC_ALL, "C", nullptr);
  ASSERT_EQ(0, strcasecmp_l("hello", "HELLO", l));
  ASSERT_LT(strcasecmp_l("hello1", "hello2", l), 0);
  ASSERT_GT(strcasecmp_l("hello2", "hello1", l), 0);
  freelocale(l);
}

TEST(STRINGS_TEST, strncasecmp) {
  ASSERT_EQ(0, strncasecmp("hello", "HELLO", 3));
  ASSERT_EQ(0, strncasecmp("abcXX", "ABCYY", 3));
  ASSERT_LT(strncasecmp("hello1", "hello2", 6), 0);
  ASSERT_GT(strncasecmp("hello2", "hello1", 6), 0);
}

TEST(STRINGS_TEST, strncasecmp_l) {
  locale_t l = newlocale(LC_ALL, "C", nullptr);
  ASSERT_EQ(0, strncasecmp_l("hello", "HELLO", 3, l));
  ASSERT_EQ(0, strncasecmp_l("abcXX", "ABCYY", 3, l));
  ASSERT_LT(strncasecmp_l("hello1", "hello2", 6, l), 0);
  ASSERT_GT(strncasecmp_l("hello2", "hello1", 6, l), 0);
  freelocale(l);
}
