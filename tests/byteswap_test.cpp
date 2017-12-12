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

#include <byteswap.h>

#include <gtest/gtest.h>

static constexpr uint16_t le16 = 0x1234;
static constexpr uint32_t le32 = 0x12345678;
static constexpr uint64_t le64 = 0x123456789abcdef0;

static constexpr uint16_t be16 = 0x3412;
static constexpr uint32_t be32 = 0x78563412;
static constexpr uint64_t be64 = 0xf0debc9a78563412;

TEST(byteswap, bswap_16) {
  EXPECT_EQ(le16, bswap_16(be16));
  EXPECT_EQ(be16, bswap_16(le16));
}

TEST(byteswap, bswap_32) {
  EXPECT_EQ(le32, bswap_32(be32));
  EXPECT_EQ(be32, bswap_32(le32));
}

TEST(byteswap, bswap_64) {
  EXPECT_EQ(le64, bswap_64(be64));
  EXPECT_EQ(be64, bswap_64(le64));
}

