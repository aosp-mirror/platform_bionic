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

#include <sstream>

// TODO: move this test to libcxx.

TEST(sstream, __get_integer_overflow) {
  std::stringstream ss("9223372036854775808");
  int64_t result;
  ss >> result;
  EXPECT_TRUE(ss.eof());
  EXPECT_TRUE(ss.fail());
  EXPECT_FALSE(ss.bad());
}
