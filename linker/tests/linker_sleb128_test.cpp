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

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <gtest/gtest.h>

#include "../linker_sleb128.h"

TEST(linker_sleb128, smoke) {
  std::vector<uint8_t> encoding;
  // 624485
  encoding.push_back(0xe5);
  encoding.push_back(0x8e);
  encoding.push_back(0x26);
  // 0
  encoding.push_back(0x00);
  // 1
  encoding.push_back(0x01);
  // 63
  encoding.push_back(0x3f);
  // 64
  encoding.push_back(0xc0);
  encoding.push_back(0x00);
  // -1
  encoding.push_back(0x7f);
  // -624485
  encoding.push_back(0x9b);
  encoding.push_back(0xf1);
  encoding.push_back(0x59);
  // 2147483647
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0x07);
  // -2147483648
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x78);
#if defined(__LP64__)
  // 9223372036854775807
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0xff);
  encoding.push_back(0x00);
  // -9223372036854775808
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x80);
  encoding.push_back(0x7f);
#endif
  sleb128_decoder decoder(&encoding[0], encoding.size());

  EXPECT_EQ(624485U, decoder.pop_front());

  EXPECT_EQ(0U, decoder.pop_front());
  EXPECT_EQ(1U, decoder.pop_front());
  EXPECT_EQ(63U, decoder.pop_front());
  EXPECT_EQ(64U, decoder.pop_front());
  EXPECT_EQ(static_cast<size_t>(-1), decoder.pop_front());
  EXPECT_EQ(static_cast<size_t>(-624485), decoder.pop_front());
  EXPECT_EQ(2147483647U, decoder.pop_front());
  EXPECT_EQ(static_cast<size_t>(-2147483648), decoder.pop_front());
#if defined(__LP64__)
  EXPECT_EQ(9223372036854775807ULL, decoder.pop_front());
  EXPECT_EQ(static_cast<uint64_t>(-9223372036854775807LL - 1), decoder.pop_front());
#endif
}
