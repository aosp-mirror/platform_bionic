/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <sys/types.h>
#include <regex.h>

TEST(regex, smoke) {
  // A quick test of all the regex functions.
  regex_t re;
  ASSERT_EQ(0, regcomp(&re, "ab*c", 0));
  ASSERT_EQ(0, regexec(&re, "abbbc", 0, nullptr, 0));
  ASSERT_EQ(REG_NOMATCH, regexec(&re, "foo", 0, nullptr, 0));

  char buf[80];
  regerror(REG_NOMATCH, &re, buf, sizeof(buf));
#if defined(__BIONIC__)
  ASSERT_STREQ("regexec() failed to match", buf);
#else
  ASSERT_STREQ("No match", buf);
#endif

  regfree(&re);
}

TEST(regex, match_offsets) {
  regex_t re;
  regmatch_t matches[1];
  ASSERT_EQ(0, regcomp(&re, "b", 0));
  ASSERT_EQ(0, regexec(&re, "abc", 1, matches, 0));
  ASSERT_EQ(1, matches[0].rm_so);
  ASSERT_EQ(2, matches[0].rm_eo);
  regfree(&re);
}

TEST(regex, regerror_NULL_0) {
  regex_t re;
  int error = regcomp(&re, "*", REG_EXTENDED);
  ASSERT_NE(0, error);

  // Passing a null pointer and a size of 0 is a legitimate way to ask
  // how large a buffer we would need for the error message.
  int error_length = regerror(error, &re, nullptr, 0);
  ASSERT_GT(error_length, 0);
}
