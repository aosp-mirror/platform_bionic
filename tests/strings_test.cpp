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
#include <strings.h>

TEST(strings, ffs) {
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
