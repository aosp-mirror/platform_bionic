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

#include <ctype.h>

TEST(ctype, isalnum) {
  EXPECT_TRUE(isalnum('1'));
  EXPECT_TRUE(isalnum('a'));
  EXPECT_TRUE(isalnum('A'));
  EXPECT_FALSE(isalnum('!'));
  EXPECT_FALSE(isalnum(' '));
}

TEST(ctype, isalnum_l) {
  EXPECT_TRUE(isalnum_l('1', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isalnum_l('a', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isalnum_l('A', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isalnum_l('!', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isalnum_l(' ', LC_GLOBAL_LOCALE));
}

TEST(ctype, isalpha) {
  EXPECT_FALSE(isalpha('1'));
  EXPECT_TRUE(isalpha('a'));
  EXPECT_TRUE(isalpha('A'));
  EXPECT_FALSE(isalpha('!'));
  EXPECT_FALSE(isalpha(' '));
}

TEST(ctype, isalpha_l) {
  EXPECT_FALSE(isalpha_l('1', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isalpha_l('a', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isalpha_l('A', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isalpha_l('!', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isalpha_l(' ', LC_GLOBAL_LOCALE));
}

TEST(ctype, isascii) {
  EXPECT_TRUE(isascii('\x7f'));
  EXPECT_FALSE(isascii('\x80'));
}

TEST(ctype, isblank) {
  EXPECT_FALSE(isblank('1'));
  EXPECT_TRUE(isblank(' '));
  EXPECT_TRUE(isblank('\t'));
}

TEST(ctype, isblank_l) {
  EXPECT_FALSE(isblank_l('1', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isblank_l(' ', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isblank_l('\t', LC_GLOBAL_LOCALE));
}

TEST(ctype, iscntrl) {
  EXPECT_FALSE(iscntrl('1'));
  EXPECT_TRUE(iscntrl('\b'));
}

TEST(ctype, iscntrl_l) {
  EXPECT_FALSE(iscntrl_l('1', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(iscntrl_l('\b', LC_GLOBAL_LOCALE));
}

TEST(ctype, isdigit) {
  EXPECT_TRUE(isdigit('1'));
  EXPECT_FALSE(isdigit('a'));
  EXPECT_FALSE(isdigit('x'));
}

TEST(ctype, isdigit_l) {
  EXPECT_TRUE(isdigit_l('1', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isdigit_l('a', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isdigit_l('x', LC_GLOBAL_LOCALE));
}

TEST(ctype, isgraph) {
  EXPECT_TRUE(isgraph('a'));
  EXPECT_TRUE(isgraph('A'));
  EXPECT_TRUE(isgraph('1'));
  EXPECT_TRUE(isgraph('!'));
  EXPECT_FALSE(isgraph(' '));
}

TEST(ctype, isgraph_l) {
  EXPECT_TRUE(isgraph_l('a', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isgraph_l('A', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isgraph_l('1', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isgraph_l('!', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isgraph_l(' ', LC_GLOBAL_LOCALE));
}

TEST(ctype, islower) {
  EXPECT_TRUE(islower('a'));
  EXPECT_FALSE(islower('A'));
  EXPECT_FALSE(islower('!'));
}

TEST(ctype, islower_l) {
  EXPECT_TRUE(islower_l('a', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(islower_l('A', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(islower_l('!', LC_GLOBAL_LOCALE));
}

TEST(ctype, isprint) {
  EXPECT_TRUE(isprint('a'));
  EXPECT_TRUE(isprint(' '));
  EXPECT_FALSE(isprint('\b'));
}

TEST(ctype, isprint_l) {
  EXPECT_TRUE(isprint_l('a', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isprint_l(' ', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isprint_l('\b', LC_GLOBAL_LOCALE));
}

TEST(ctype, ispunct) {
  EXPECT_TRUE(ispunct('!'));
  EXPECT_FALSE(ispunct('a'));
  EXPECT_FALSE(ispunct(' '));
  EXPECT_FALSE(ispunct('\b'));
}

TEST(ctype, ispunct_l) {
  EXPECT_TRUE(ispunct_l('!', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(ispunct_l('a', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(ispunct_l(' ', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(ispunct_l('\b', LC_GLOBAL_LOCALE));
}

TEST(ctype, isspace) {
  EXPECT_TRUE(isspace(' '));
  EXPECT_TRUE(isspace('\f'));
  EXPECT_TRUE(isspace('\n'));
  EXPECT_TRUE(isspace('\r'));
  EXPECT_TRUE(isspace('\t'));
  EXPECT_TRUE(isspace('\v'));
  EXPECT_FALSE(isspace('a'));
  EXPECT_FALSE(isspace('!'));
}

TEST(ctype, isspace_l) {
  EXPECT_TRUE(isspace_l(' ', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isspace_l('\f', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isspace_l('\n', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isspace_l('\r', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isspace_l('\t', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isspace_l('\v', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isspace_l('a', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isspace_l('!', LC_GLOBAL_LOCALE));
}

TEST(ctype, isupper) {
  EXPECT_TRUE(isupper('A'));
  EXPECT_FALSE(isupper('a'));
  EXPECT_FALSE(isupper('!'));
}

TEST(ctype, isupper_l) {
  EXPECT_TRUE(isupper_l('A', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isupper_l('a', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isupper_l('!', LC_GLOBAL_LOCALE));
}

TEST(ctype, isxdigit) {
  EXPECT_TRUE(isxdigit('0'));
  EXPECT_FALSE(isxdigit('x'));
  EXPECT_TRUE(isxdigit('1'));
  EXPECT_TRUE(isxdigit('a'));
  EXPECT_TRUE(isxdigit('A'));
  EXPECT_FALSE(isxdigit('g'));
  EXPECT_FALSE(isxdigit(' '));
}

TEST(ctype, isxdigit_l) {
  EXPECT_TRUE(isxdigit_l('0', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isxdigit_l('x', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isxdigit_l('1', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isxdigit_l('a', LC_GLOBAL_LOCALE));
  EXPECT_TRUE(isxdigit_l('A', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isxdigit_l('g', LC_GLOBAL_LOCALE));
  EXPECT_FALSE(isxdigit_l(' ', LC_GLOBAL_LOCALE));
}

TEST(ctype, toascii) {
  EXPECT_EQ('a', toascii('a'));
  EXPECT_EQ('a', toascii(0x80 | 'a'));
}

TEST(ctype, tolower) {
  EXPECT_EQ('!', tolower('!'));
  EXPECT_EQ('a', tolower('a'));
  EXPECT_EQ('a', tolower('A'));
}

TEST(ctype, tolower_l) {
  EXPECT_EQ('!', tolower_l('!', LC_GLOBAL_LOCALE));
  EXPECT_EQ('a', tolower_l('a', LC_GLOBAL_LOCALE));
  EXPECT_EQ('a', tolower_l('A', LC_GLOBAL_LOCALE));
}

TEST(ctype, _tolower) {
  // _tolower may mangle characters for which isupper is false.
  EXPECT_EQ('a', _tolower('A'));
}

TEST(ctype, toupper) {
  EXPECT_EQ('!', toupper('!'));
  EXPECT_EQ('A', toupper('a'));
  EXPECT_EQ('A', toupper('A'));
}

TEST(ctype, toupper_l) {
  EXPECT_EQ('!', toupper_l('!', LC_GLOBAL_LOCALE));
  EXPECT_EQ('A', toupper_l('a', LC_GLOBAL_LOCALE));
  EXPECT_EQ('A', toupper_l('A', LC_GLOBAL_LOCALE));
}

TEST(ctype, _toupper) {
  // _toupper may mangle characters for which islower is false.
  EXPECT_EQ('A', _toupper('a'));
}
