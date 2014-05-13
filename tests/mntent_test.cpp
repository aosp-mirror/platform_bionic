/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <mntent.h>

TEST(mntent, mntent_smoke) {
  FILE* fp = setmntent("/no/mnt/tab/on/android", "r");
  ASSERT_TRUE(fp == NULL);

#if __BIONIC__ // glibc doesn't let you call getmntent/getmntent_r with a NULL FILE*.
  ASSERT_TRUE(getmntent(fp) == NULL);

  struct mntent mbuf;
  char cbuf[32];
  ASSERT_TRUE(getmntent_r(fp, &mbuf, cbuf, sizeof(cbuf)) == NULL);
#endif

  ASSERT_EQ(1, endmntent(fp));
}
