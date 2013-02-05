/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

TEST(stdlib, drand48) {
  srand48(0x01020304);
  EXPECT_DOUBLE_EQ(0.65619299195623526, drand48());
  EXPECT_DOUBLE_EQ(0.18522597229772941, drand48());
  EXPECT_DOUBLE_EQ(0.42015087072844537, drand48());
  EXPECT_DOUBLE_EQ(0.061637783047395089, drand48());
}

TEST(stdlib, lrand48_random_rand) {
  srand48(0x01020304);
  EXPECT_EQ(1409163720, lrand48());
  EXPECT_EQ(397769746, lrand48());
  EXPECT_EQ(902267124, lrand48());
  EXPECT_EQ(132366131, lrand48());

#if __BIONIC__
  // On bionic, random(3) is equivalent to lrand48...
  srandom(0x01020304);
  EXPECT_EQ(1409163720, random());
  EXPECT_EQ(397769746, random());
  EXPECT_EQ(902267124, random());
  EXPECT_EQ(132366131, random());

  // ...and rand(3) is the bottom 32 bits.
  srand(0x01020304);
  EXPECT_EQ(static_cast<int>(1409163720), rand());
  EXPECT_EQ(static_cast<int>(397769746), rand());
  EXPECT_EQ(static_cast<int>(902267124), rand());
  EXPECT_EQ(static_cast<int>(132366131), rand());
#endif
}

TEST(stdlib, mrand48) {
  srand48(0x01020304);
  EXPECT_EQ(-1476639856, mrand48());
  EXPECT_EQ(795539493, mrand48());
  EXPECT_EQ(1804534249, mrand48());
  EXPECT_EQ(264732262, mrand48());
}

TEST(stdlib, posix_memalign) {
  void* p;

  ASSERT_EQ(0, posix_memalign(&p, 512, 128));
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(p) % 512);
  free(p);

  // Can't align to a non-power of 2.
  ASSERT_EQ(EINVAL, posix_memalign(&p, 81, 128));
}
