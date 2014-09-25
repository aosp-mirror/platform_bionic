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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "TemporaryFile.h"

TEST(sys_stat, futimens) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

  int fd = fileno(fp);
  ASSERT_NE(fd, -1);

  timespec times[2];
  times[0].tv_sec = 123;
  times[0].tv_nsec = 0;
  times[1].tv_sec = 456;
  times[1].tv_nsec = 0;
  ASSERT_EQ(0, futimens(fd, times)) << strerror(errno);

  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(times[0].tv_sec, static_cast<long>(sb.st_atime));
  ASSERT_EQ(times[1].tv_sec, static_cast<long>(sb.st_mtime));

  fclose(fp);
}

TEST(sys_stat, futimens_EBADF) {
  timespec times[2];
  times[0].tv_sec = 123;
  times[0].tv_nsec = 0;
  times[1].tv_sec = 456;
  times[1].tv_nsec = 0;
  ASSERT_EQ(-1, futimens(-1, times));
  ASSERT_EQ(EBADF, errno);
}

TEST(sys_stat, mkfifo) {
  if (getuid() == 0) {
    // Racy but probably sufficient way to get a suitable filename.
    std::string path;
    {
      TemporaryFile tf;
      path = tf.filename;
    }

    ASSERT_EQ(0, mkfifo(path.c_str(), 0666));
    struct stat sb;
    ASSERT_EQ(0, stat(path.c_str(), &sb));
    ASSERT_TRUE(S_ISFIFO(sb.st_mode));
    unlink(path.c_str());
  } else {
    GTEST_LOG_(INFO) << "This test only performs a test when run as root.";
  }
}

TEST(sys_stat, stat64_lstat64_fstat64) {
  struct stat64 sb;
  ASSERT_EQ(0, stat64("/proc/version", &sb));
  ASSERT_EQ(0, lstat64("/proc/version", &sb));
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_EQ(0, fstat64(fd, &sb));
  close(fd);
}
