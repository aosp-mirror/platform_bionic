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

#include <sys/statvfs.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>

template <typename StatVfsT> void Check(StatVfsT& sb) {
  EXPECT_EQ(4096U, sb.f_bsize);
  EXPECT_EQ(0U, sb.f_bfree);
  EXPECT_EQ(0U, sb.f_ffree);
  EXPECT_EQ(0U, sb.f_fsid);
  EXPECT_EQ(255U, sb.f_namemax);
}

TEST(sys_statvfs, statvfs) {
  struct statvfs sb;
  ASSERT_EQ(0, statvfs("/proc", &sb));
  Check(sb);
}

TEST(sys_statvfs, statvfs64) {
  struct statvfs64 sb;
  ASSERT_EQ(0, statvfs64("/proc", &sb));
  Check(sb);
}

TEST(sys_statvfs, fstatvfs) {
  struct statvfs sb;
  int fd = open("/proc", O_RDONLY);
  ASSERT_EQ(0, fstatvfs(fd, &sb));
  close(fd);
  Check(sb);
}
TEST(sys_statvfs, fstatvfs64) {
  struct statvfs64 sb;
  int fd = open("/proc", O_RDONLY);
  ASSERT_EQ(0, fstatvfs64(fd, &sb));
  close(fd);
  Check(sb);
}
