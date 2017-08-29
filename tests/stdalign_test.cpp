/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <stdalign.h>

TEST(stdalign, smoke) {
#if !defined(__alignas_is_defined) || __alignas_is_defined != 1
#error __alignas_is_defined
#endif
#if !defined(__alignof_is_defined) || __alignof_is_defined != 1
#error __alignof_is_defined
#endif
  ASSERT_EQ(1U, alignof(char));
  struct alignas(128) S128 {};
  ASSERT_EQ(128U, alignof(S128));
}
