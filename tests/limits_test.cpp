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

#include <limits.h>

TEST(limits, macros) {
  ASSERT_EQ(8, CHAR_BIT);
  ASSERT_EQ(8 * static_cast<int>(sizeof(int)), WORD_BIT);
  ASSERT_EQ(20, NZERO);
#if !defined(MB_LEN_MAX)
#error MB_LEN_MAX
#endif
#if !defined(CHAR_MIN)
#error CHAR_MIN
#endif
#if !defined(CHAR_MAX)
#error CHAR_MAX
#endif
#if !defined(SCHAR_MIN)
#error SCHAR_MIN
#endif
#if !defined(SCHAR_MAX)
#error SCHAR_MAX
#endif
#if !defined(SHRT_MIN)
#error SHRT_MIN
#endif
#if !defined(SHRT_MAX)
#error SHRT_MAX
#endif
#if !defined(INT_MIN)
#error INT_MIN
#endif
#if !defined(INT_MAX)
#error INT_MAX
#endif
#if !defined(LONG_MIN)
#error LONG_MIN
#endif
#if !defined(LONG_MAX)
#error LONG_MAX
#endif
#if !defined(LLONG_MIN)
#error LLONG_MIN
#endif
#if !defined(LLONG_MAX)
#error LLONG_MAX
#endif
#if !defined(UCHAR_MAX)
#error UCHAR_MAX
#endif
#if !defined(USHRT_MAX)
#error USHRT_MAX
#endif
#if !defined(UINT_MAX)
#error UINT_MAX
#endif
#if !defined(ULONG_MAX)
#error ULONG_MAX
#endif
#if !defined(ULLONG_MAX)
#error ULLONG_MAX
#endif
}
