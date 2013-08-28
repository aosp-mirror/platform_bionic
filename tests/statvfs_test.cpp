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

TEST(statvfs, statvfs) {
  struct statvfs sb;
  memset(&sb, 0, sizeof(sb));

  ASSERT_EQ(0, statvfs("/", &sb));
#if __BIONIC__
  ASSERT_EQ(0U, sb.f_bfree);
  ASSERT_EQ(0U, sb.f_ffree);
  ASSERT_EQ(0U, sb.f_fsid);
  ASSERT_TRUE((sb.f_flag & ST_RDONLY) != 0);
#endif

#if __BIONIC__
  ASSERT_EQ(0, statvfs("/data/local/tmp", &sb));
  ASSERT_NE(0U, sb.f_bfree);
  ASSERT_NE(0U, sb.f_ffree);
  ASSERT_NE(0U, sb.f_fsid);
  ASSERT_FALSE((sb.f_flag & ST_RDONLY) != 0);
  ASSERT_TRUE((sb.f_flag & ST_NOSUID) != 0);
#endif
}

TEST(statvfs, fstatvfs) {
  struct statvfs sb;
  memset(&sb, 0, sizeof(sb));

  int fd = open("/", O_RDONLY);
  ASSERT_EQ(0, fstatvfs(fd, &sb));
  close(fd);
#if __BIONIC__
  ASSERT_EQ(0U, sb.f_bfree);
  ASSERT_EQ(0U, sb.f_ffree);
  ASSERT_EQ(0U, sb.f_fsid);
  ASSERT_TRUE((sb.f_flag & ST_RDONLY) != 0);
#endif

#if __BIONIC__
  fd = open("/data/local/tmp", O_RDONLY);
  ASSERT_EQ(0, fstatvfs(fd, &sb));
  close(fd);
  ASSERT_NE(0U, sb.f_bfree);
  ASSERT_NE(0U, sb.f_ffree);
  ASSERT_NE(0U, sb.f_fsid);
  ASSERT_FALSE((sb.f_flag & ST_RDONLY) != 0);
  ASSERT_TRUE((sb.f_flag & ST_NOSUID) != 0);
#endif
}
