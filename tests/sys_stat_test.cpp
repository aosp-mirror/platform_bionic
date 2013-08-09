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
#include <stdlib.h>
#include <sys/stat.h>

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
