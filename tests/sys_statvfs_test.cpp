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
#if defined(__x86_64__)
  // On x86_64 based 16kb page size targets, the page size in userspace is simulated to 16kb but
  // the underlying filesystem block size would remain unchanged, i.e., 4kb.
  // For more info:
  // https://source.android.com/docs/core/architecture/16kb-page-size/getting-started-cf-x86-64-pgagnostic
  EXPECT_EQ(4096, static_cast<int>(sb.f_bsize));
#else
  EXPECT_EQ(getpagesize(), static_cast<int>(sb.f_bsize));
#endif
  EXPECT_EQ(0U, sb.f_bfree);
  EXPECT_EQ(0U, sb.f_ffree);
  EXPECT_EQ(255U, sb.f_namemax);

  // Linux 6.7 requires that all filesystems have a non-zero fsid.
  if (sb.f_fsid != 0U) {
    // fs/libfs.c reuses the filesystem's device number.
    struct stat proc_sb;
    ASSERT_EQ(0, stat("/proc", &proc_sb));
    EXPECT_EQ(proc_sb.st_dev, sb.f_fsid);
  } else {
    // Prior to that, the fsid for /proc was just 0.
    EXPECT_EQ(0U, sb.f_fsid);
  }

  // The kernel sets a private bit to indicate that f_flags is valid.
  // This flag is not supposed to be exposed to libc clients.
  static const uint32_t ST_VALID = 0x0020;
  EXPECT_TRUE((sb.f_flag & ST_VALID) == 0) << sb.f_flag;
}

TEST(sys_statvfs, statvfs) {
  struct statvfs sb;
  ASSERT_EQ(0, statvfs("/proc", &sb));
  Check(sb);
}

TEST(sys_statvfs, statvfs64_smoke) {
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

TEST(sys_statvfs, fstatvfs64_smoke) {
  struct statvfs64 sb;
  int fd = open("/proc", O_RDONLY);
  ASSERT_EQ(0, fstatvfs64(fd, &sb));
  close(fd);
  Check(sb);
}
