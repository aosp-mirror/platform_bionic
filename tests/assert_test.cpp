/*
 * Copyright (C) 2016 The Android Open Source Project
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

#undef NDEBUG
#include <assert.h>

TEST(assert, assert_true) {
  assert(true);
}

TEST(assert, assert_false) {
  EXPECT_DEATH(assert(false),
               "bionic/tests/assert_test.cpp:.*: "
               "virtual void assert_assert_false_Test::TestBody\\(\\): "
               "assertion \"false\" failed");
}

// Re-include <assert.h> with assertions disabled.
#define NDEBUG
#include <assert.h>

TEST(assert, assert_true_NDEBUG) {
  assert(true);
}

TEST(assert, assert_false_NDEBUG) {
  assert(false);
}
