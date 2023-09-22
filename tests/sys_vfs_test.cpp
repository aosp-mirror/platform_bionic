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

#include <sys/vfs.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>

#include "utils.h"

template <typename StatFsT> void Check(StatFsT& sb) {
  EXPECT_EQ(4096, static_cast<int>(sb.f_bsize));
  EXPECT_EQ(0U, sb.f_bfree);
  EXPECT_EQ(0U, sb.f_ffree);
  EXPECT_EQ(0, sb.f_fsid.__val[0]);
  EXPECT_EQ(0, sb.f_fsid.__val[1]);
  EXPECT_EQ(255, static_cast<int>(sb.f_namelen));

  // The kernel sets a private bit to indicate that f_flags is valid.
  // This flag is not supposed to be exposed to libc clients.
  static const uint32_t ST_VALID = 0x0020;
  EXPECT_TRUE((sb.f_flags & ST_VALID) == 0) << sb.f_flags;
}

TEST(sys_vfs, statfs) {
  struct statfs sb;
  ASSERT_EQ(0, statfs("/proc", &sb));
  Check(sb);
}

TEST(sys_vfs, statfs_failure) {
  struct statfs sb;
  errno = 0;
  ASSERT_EQ(-1, statfs("/does-not-exist", &sb));
  ASSERT_ERRNO(ENOENT);
}

TEST(sys_vfs, statfs64_smoke) {
  struct statfs64 sb;
  ASSERT_EQ(0, statfs64("/proc", &sb));
  Check(sb);
}

TEST(sys_vfs, statfs64_failure) {
  struct statfs64 sb;
  errno = 0;
  ASSERT_EQ(-1, statfs64("/does-not-exist", &sb));
  ASSERT_ERRNO(ENOENT);
}

TEST(sys_vfs, fstatfs) {
  struct statfs sb;
  int fd = open("/proc", O_RDONLY);
  ASSERT_EQ(0, fstatfs(fd, &sb));
  close(fd);
  Check(sb);
}

TEST(sys_vfs, fstatfs_failure) {
  struct statfs sb;
  errno = 0;
  ASSERT_EQ(-1, fstatfs(-1, &sb));
  ASSERT_ERRNO(EBADF);
}

TEST(sys_vfs, fstatfs64_smoke) {
  struct statfs64 sb;
  int fd = open("/proc", O_RDONLY);
  ASSERT_EQ(0, fstatfs64(fd, &sb));
  close(fd);
  Check(sb);
}

TEST(sys_vfs, fstatfs64_failure) {
  struct statfs sb;
  errno = 0;
  ASSERT_EQ(-1, fstatfs(-1, &sb));
  ASSERT_ERRNO(EBADF);
}
