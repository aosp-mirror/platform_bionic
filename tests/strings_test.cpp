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

TEST(STRINGS_TEST, strcasecmp) {
  ASSERT_EQ(0, strcasecmp("hello", "HELLO"));
  ASSERT_LT(strcasecmp("hello1", "hello2"), 0);
  ASSERT_GT(strcasecmp("hello2", "hello1"), 0);
}

TEST(STRINGS_TEST, strcasecmp_l) {
  locale_t l = newlocale(LC_ALL, "C", 0);
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
  locale_t l = newlocale(LC_ALL, "C", 0);
  ASSERT_EQ(0, strncasecmp_l("hello", "HELLO", 3, l));
  ASSERT_EQ(0, strncasecmp_l("abcXX", "ABCYY", 3, l));
  ASSERT_LT(strncasecmp_l("hello1", "hello2", 6, l), 0);
  ASSERT_GT(strncasecmp_l("hello2", "hello1", 6, l), 0);
  freelocale(l);
}
