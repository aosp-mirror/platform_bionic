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
#include <util.h>

TEST(benchmark, memory_align) {
  std::vector<char> buf(100);
  for (size_t alignment = 1; alignment <= 32; alignment *= 2) {
    for (size_t or_mask = 0; or_mask < alignment; ++or_mask) {
      uintptr_t aligned_ptr = reinterpret_cast<uintptr_t>(GetAlignedMemory(buf.data(), alignment,
                                                                           or_mask));
      ASSERT_EQ(aligned_ptr % alignment, or_mask);
      ASSERT_EQ(aligned_ptr & alignment, alignment);
    }
  }
}

TEST(benchmark, ptr_align) {
  std::vector<char> buf;
  for (size_t alignment = 1; alignment <= 2048; alignment *= 2) {
    uintptr_t aligned_ptr = reinterpret_cast<uintptr_t>(GetAlignedPtr(&buf, alignment, 100));
    ASSERT_EQ(aligned_ptr & alignment, alignment);
    ASSERT_EQ(aligned_ptr & (alignment - 1), 0u);
  }
}
