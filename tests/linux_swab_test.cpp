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

#include <linux/swab.h>

// This test makes sure that references to all of the kernel swab
// macros/inline functions that are exported work properly.
// Verifies that any kernel header updates do not break these macros.
TEST(linux_swab, smoke) {
  EXPECT_EQ(0x3412U, __swab16(0x1234));
  EXPECT_EQ(0x78563412U, __swab32(0x12345678U));
  EXPECT_EQ(0xbaefcdab78563412ULL, __swab64(0x12345678abcdefbaULL));

  __u16 bval16 = 0x1234;
  EXPECT_EQ(0x3412U, __swab16p(&bval16));
  __u32 bval32 = 0x12345678U;
  EXPECT_EQ(0x78563412U, __swab32p(&bval32));
  __u64 bval64 = 0x12345678abcdefbaULL;
  EXPECT_EQ(0xbaefcdab78563412ULL, __swab64p(&bval64));

  __u16 sval16 = 0x1234;
  __swab16s(&sval16);
  EXPECT_EQ(0x3412U, sval16);
  __u32 sval32 = 0x12345678U;
  __swab32s(&sval32);
  EXPECT_EQ(0x78563412U, sval32);
  __u64 sval64 = 0x12345678abcdefbaULL;
  __swab64s(&sval64);
  EXPECT_EQ(0xbaefcdab78563412ULL, sval64);

  EXPECT_EQ(0x56781234U, __swahw32(0x12345678U));
  EXPECT_EQ(0x34127856U, __swahb32(0x12345678U));

  __u32 hval32 = 0x12345678U;
  EXPECT_EQ(0x56781234U, __swahw32p(&hval32));
  hval32 = 0x12345678U;
  EXPECT_EQ(0x34127856U, __swahb32p(&hval32));

  __u32 hsval32 = 0x12345678U;
  __swahw32s(&hsval32);
  EXPECT_EQ(0x56781234U, hsval32);
  hsval32 = 0x12345678U;
  __swahb32s(&hsval32);
  EXPECT_EQ(0x34127856U, hsval32);
}
