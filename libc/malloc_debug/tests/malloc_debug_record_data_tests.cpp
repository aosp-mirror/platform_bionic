/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include "Config.h"
#include "RecordData.h"

#include "log_fake.h"

class MallocDebugRecordDataTest : public ::testing::Test {
 protected:
  void SetUp() override {
    page_size_ = getpagesize();
    Config config;
    ASSERT_TRUE(config.Init("record_allocs"));
    ASSERT_TRUE(record_.Initialize(config));
  }

  uint8_t* AllocPageAligned(size_t alloc_pages) {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(memalign(page_size_, alloc_pages * page_size_));
    if (ptr == nullptr) {
      return nullptr;
    }
    // Release all the pages so the test can make them present.
    EXPECT_EQ(0, madvise(ptr, page_size_ * alloc_pages, MADV_DONTNEED));
    return ptr;
  }

  size_t page_size_;
  RecordData record_;
};

TEST_F(MallocDebugRecordDataTest, get_present_bytes_error) {
  EXPECT_EQ(-1, record_.GetPresentBytes(nullptr, 1000));
  EXPECT_EQ(-1, record_.GetPresentBytes(reinterpret_cast<void*>(1000), 0));
}

TEST_F(MallocDebugRecordDataTest, get_present_bytes_edge_cases) {
  // Need two pages to check allocations crossing over the page.
  size_t alloc_pages = 2;
  uint8_t* ptr = AllocPageAligned(alloc_pages);
  ASSERT_TRUE(ptr != nullptr);
  memset(ptr, 1, alloc_pages * page_size_);

  EXPECT_EQ(20, record_.GetPresentBytes(ptr, 20));
  EXPECT_EQ(page_size_ + 20, record_.GetPresentBytes(ptr, page_size_ + 20));
  EXPECT_EQ(17, record_.GetPresentBytes(&ptr[page_size_ - 20], 17));
  EXPECT_EQ(32, record_.GetPresentBytes(&ptr[page_size_ - 16], 32));
  EXPECT_EQ(page_size_, record_.GetPresentBytes(ptr, page_size_));
  EXPECT_EQ(page_size_ * 2, record_.GetPresentBytes(ptr, page_size_ * 2));
}

TEST_F(MallocDebugRecordDataTest, get_present_bytes_first_page_not_present) {
  uint8_t* ptr = AllocPageAligned(2);
  ASSERT_TRUE(ptr != nullptr);
  ptr[page_size_] = 1;

  EXPECT_EQ(0, record_.GetPresentBytes(ptr, page_size_));
  EXPECT_EQ(3996, record_.GetPresentBytes(&ptr[100], page_size_ * 2 - 200));
}

TEST_F(MallocDebugRecordDataTest, get_present_bytes_last_page_not_present) {
  uint8_t* ptr = AllocPageAligned(2);
  ASSERT_TRUE(ptr != nullptr);
  ptr[0] = 1;

  EXPECT_EQ(3596, record_.GetPresentBytes(&ptr[500], page_size_ * 2 - 600));
}

TEST_F(MallocDebugRecordDataTest, get_present_bytes_large) {
  // Needs to match the kMaxReadPages from GetPresentBytes
  constexpr size_t kMaxReadPages = 1024;
  // Allocate large enough that it requires at least two preads.
  size_t alloc_pages = 2 * kMaxReadPages;
  uint8_t* ptr = AllocPageAligned(alloc_pages);
  ASSERT_TRUE(ptr != nullptr);
  // Make sure that there are different number of pages present in the first
  // read than in the second read.
  ptr[0] = 1;
  ptr[page_size_] = 1;
  ptr[page_size_ * 4] = 1;
  // Should be in the second read.
  size_t start = kMaxReadPages * page_size_;
  ptr[start + page_size_ * 2] = 1;
  ptr[start + page_size_ * 4] = 1;
  ptr[start + page_size_ * 8] = 1;
  ptr[start + page_size_ * 9] = 1;

  EXPECT_EQ(page_size_ * 7, record_.GetPresentBytes(ptr, alloc_pages * page_size_));

  // Make the entire allocation resident for the next few tests.
  for (size_t i = 0; i < alloc_pages; i++) {
    ptr[i * page_size_] = 1;
  }

  EXPECT_EQ(page_size_ * kMaxReadPages, record_.GetPresentBytes(ptr, page_size_ * kMaxReadPages));
  EXPECT_EQ(page_size_ * (kMaxReadPages + 1),
            record_.GetPresentBytes(ptr, page_size_ * (kMaxReadPages + 1)));
  EXPECT_EQ(page_size_ * kMaxReadPages - 50,
            record_.GetPresentBytes(ptr, page_size_ * kMaxReadPages - 50));
  EXPECT_EQ(page_size_ * (kMaxReadPages + 1) - 50,
            record_.GetPresentBytes(ptr, page_size_ * (kMaxReadPages + 1) - 50));
}
