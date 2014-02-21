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

#include <stdint.h>

TEST(stdint_types, type_sizes) {
  ASSERT_EQ(1U, sizeof(int_fast8_t));
  ASSERT_EQ(8U, sizeof(int_fast64_t));
  ASSERT_EQ(1U, sizeof(uint_fast8_t));
  ASSERT_EQ(8U, sizeof(uint_fast64_t));
#if defined(__LP64__)
  ASSERT_EQ(8U, sizeof(int_fast16_t));
  ASSERT_EQ(8U, sizeof(int_fast32_t));
  ASSERT_EQ(8U, sizeof(uint_fast16_t));
  ASSERT_EQ(8U, sizeof(uint_fast32_t));
#else
  ASSERT_EQ(4U, sizeof(int_fast16_t));
  ASSERT_EQ(4U, sizeof(int_fast32_t));
  ASSERT_EQ(4U, sizeof(uint_fast16_t));
  ASSERT_EQ(4U, sizeof(uint_fast32_t));
#endif
}
