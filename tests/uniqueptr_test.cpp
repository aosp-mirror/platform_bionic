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

#include <private/UniquePtr.h>

static int cCount = 0;
struct C {
  C() { ++cCount; }
  ~C() { --cCount; }
};

static bool freed = false;
struct Freer {
  void operator() (int* p) {
    ASSERT_EQ(123, *p);
    free(p);
    freed = true;
  }
};

TEST(UniquePtr, smoke) {
  //
  // UniquePtr<T> tests...
  //

  // Can we free a single object?
  {
    UniquePtr<C> c(new C);
    ASSERT_TRUE(cCount == 1);
  }
  ASSERT_TRUE(cCount == 0);
  // Does release work?
  C* rawC;
  {
      UniquePtr<C> c(new C);
      ASSERT_TRUE(cCount == 1);
      rawC = c.release();
  }
  ASSERT_TRUE(cCount == 1);
  delete rawC;
  // Does reset work?
  {
      UniquePtr<C> c(new C);
      ASSERT_TRUE(cCount == 1);
      c.reset(new C);
      ASSERT_TRUE(cCount == 1);
  }
  ASSERT_TRUE(cCount == 0);

  //
  // UniquePtr<T[]> tests...
  //

  // Can we free an array?
  {
      UniquePtr<C[]> cs(new C[4]);
      ASSERT_TRUE(cCount == 4);
  }
  ASSERT_TRUE(cCount == 0);
  // Does release work?
  {
      UniquePtr<C[]> c(new C[4]);
      ASSERT_TRUE(cCount == 4);
      rawC = c.release();
  }
  ASSERT_TRUE(cCount == 4);
  delete[] rawC;
  // Does reset work?
  {
      UniquePtr<C[]> c(new C[4]);
      ASSERT_TRUE(cCount == 4);
      c.reset(new C[2]);
      ASSERT_TRUE(cCount == 2);
  }
  ASSERT_TRUE(cCount == 0);

  //
  // Custom deleter tests...
  //
  ASSERT_TRUE(!freed);
  {
      UniquePtr<int, Freer> i(reinterpret_cast<int*>(malloc(sizeof(int))));
      *i = 123;
  }
  ASSERT_TRUE(freed);
}
