/*
 * Copyright (C) 2019 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <gtest/gtest.h>

#include "linker_gnu_hash.h"

TEST(linker_gnu_hash, compare_neon_to_simple) {
#if USE_GNU_HASH_NEON
  auto check_input = [&](const char* name) {
    auto expected = calculate_gnu_hash_simple(name);
    auto actual = calculate_gnu_hash_neon(name);
    EXPECT_EQ(expected.first, actual.first) << name;
    EXPECT_EQ(expected.second, actual.second) << name;
  };

  __attribute__((aligned(8))) const char test1[] = "abcdefghijklmnop\0qrstuvwxyz";
  for (size_t i = 0; i < sizeof(test1) - 1; ++i) {
    check_input(&test1[i]);
  }

  __attribute__((aligned(8))) const char test2[] = "abcdefghijklmnopqrs\0tuvwxyz";
  for (size_t i = 0; i < sizeof(test2) - 1; ++i) {
    check_input(&test2[i]);
  }

  __attribute__((aligned(8))) const char test3[] = "abcdefghijklmnopqrstuv\0wxyz";
  for (size_t i = 0; i < sizeof(test3) - 1; ++i) {
    check_input(&test3[i]);
  }
#else
  GTEST_SKIP() << "This test is only implemented on arm/arm64";
#endif
}
