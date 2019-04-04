/*
 * Copyright (C) 2019 The Android Open Source Project
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
#include <sys/param.h>

TEST(sys_param_test, powerof2_positives) {
  ASSERT_TRUE(powerof2(1));
  ASSERT_TRUE(powerof2(2));
  ASSERT_TRUE(powerof2(4));
  ASSERT_TRUE(powerof2(8));
  ASSERT_FALSE(powerof2(3));
  ASSERT_FALSE(powerof2(5));
  ASSERT_FALSE(powerof2(7));
  ASSERT_FALSE(powerof2(9));
  ASSERT_FALSE(powerof2(10));
}

TEST(sys_param_test, powerof2_zero) {
  // 0 isn't a power of 2, but for compatibility, we assume it is.
  ASSERT_TRUE(powerof2(0));
  uint32_t zero = 0;
  ASSERT_TRUE(powerof2(zero));
}

TEST(sys_param_test, powerof2_negatives) {
  // negative numbers can never be a power of 2, but for compatibility,
  // we assume they can be.
  int32_t min32 = INT32_MIN;
  int64_t min64 = INT64_MIN;
  ASSERT_TRUE(powerof2(min32));
  ASSERT_FALSE(powerof2(min32 + 1));
  ASSERT_TRUE(powerof2(min64));
  ASSERT_FALSE(powerof2(min64 + 1));
}
