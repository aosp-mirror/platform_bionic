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
  // Read all the entries with getmntent().
  FILE* fp = setmntent("/proc/mounts", "r");
  ASSERT_TRUE(fp != nullptr);

  std::vector<std::string> fsnames;
  std::vector<std::string> dirs;
  mntent* me;
  while ((me = getmntent(fp)) != nullptr) {
    fsnames.push_back(me->mnt_fsname);
    dirs.push_back(me->mnt_dir);
  }

  ASSERT_EQ(1, endmntent(fp));

  // Then again with getmntent_r(), checking they match.
  fp = setmntent("/proc/mounts", "r");
  ASSERT_TRUE(fp != nullptr);

  struct mntent entry;
  char buf[BUFSIZ];
  size_t i = 0;
  while (getmntent_r(fp, &entry, buf, sizeof(buf)) != nullptr) {
    ASSERT_EQ(fsnames[i], entry.mnt_fsname);
    ASSERT_EQ(dirs[i], entry.mnt_dir);
    i++;
  }

  ASSERT_EQ(1, endmntent(fp));

  // And just for good measure: we did see a /proc entry, right?
  auto it = std::find(fsnames.begin(), fsnames.end(), "proc");
  ASSERT_TRUE(it != fsnames.end());
  size_t proc_index = it - fsnames.begin();
  ASSERT_EQ("/proc", dirs[proc_index]);
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
