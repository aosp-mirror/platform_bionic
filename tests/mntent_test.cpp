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
  FILE* fp = setmntent("/proc/mounts", "r");
  ASSERT_TRUE(fp != NULL);

  ASSERT_TRUE(getmntent(fp) != NULL);

  bool saw_proc = false;

  struct mntent entry;
  char buf[BUFSIZ];
  while (getmntent_r(fp, &entry, buf, sizeof(buf)) != NULL) {
    if (strcmp(entry.mnt_fsname, "proc") == 0 && strcmp(entry.mnt_dir, "/proc") == 0) {
      saw_proc = true;
    }
  }

  ASSERT_TRUE(saw_proc);

  ASSERT_EQ(1, endmntent(fp));
}

TEST(mntent, hasmntopt) {
  // indices                  1  1
  // of keys:      0    5   9 1  4
  char mnt_opts[]{"aa=b,a=b,b,bb,c=d"};
  struct mntent ent;
  memset(&ent, 0, sizeof(ent));
  ent.mnt_opts = mnt_opts;

  EXPECT_EQ(mnt_opts, hasmntopt(&ent, "aa"));
  EXPECT_EQ(mnt_opts + 5, hasmntopt(&ent, "a"));
  EXPECT_EQ(mnt_opts + 9, hasmntopt(&ent, "b"));
  EXPECT_EQ(mnt_opts + 11, hasmntopt(&ent, "bb"));
  EXPECT_EQ(mnt_opts + 14, hasmntopt(&ent, "c"));
  EXPECT_EQ(nullptr, hasmntopt(&ent, "d"));
  EXPECT_EQ(nullptr, hasmntopt(&ent, "e"));
}
