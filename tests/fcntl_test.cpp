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

#include "TemporaryFile.h"

TEST(fcntl, fcntl_smoke) {
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);

  int flags = fcntl(fd, F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(0, flags & FD_CLOEXEC);

  int rc = fcntl(fd, F_SETFD, FD_CLOEXEC);
  ASSERT_EQ(0, rc);

  flags = fcntl(fd, F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(FD_CLOEXEC, flags & FD_CLOEXEC);

  close(fd);
}

TEST(fcntl, open_open64) {
  int fd;

  fd = open("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);
  close(fd);

  fd = open64("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);
  close(fd);
}

TEST(fcntl, openat_openat64) {
  int fd;

  fd = openat(AT_FDCWD, "/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);
  close(fd);

  fd = openat64(AT_FDCWD, "/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);
  close(fd);
}

TEST(fcntl, creat_creat64) {
  ASSERT_EQ(-1, creat("", 0666));
  ASSERT_EQ(ENOENT, errno);
  ASSERT_EQ(-1, creat64("", 0666));
  ASSERT_EQ(ENOENT, errno);
}

TEST(fcntl, fallocate_EINVAL) {
  TemporaryFile tf;

#if defined(__BIONIC__)
  errno = 0;
  ASSERT_EQ(-1, fallocate(tf.fd, 0, 0, -1));
  ASSERT_EQ(EINVAL, errno);

  errno = 0;
  ASSERT_EQ(-1, fallocate64(tf.fd, 0, 0, -1));
  ASSERT_EQ(EINVAL, errno);
#endif

  errno = 0;
  ASSERT_EQ(EINVAL, posix_fallocate(tf.fd, 0, -1));
  ASSERT_EQ(0, errno);

  errno = 0;
  ASSERT_EQ(EINVAL, posix_fallocate64(tf.fd, 0, -1));
  ASSERT_EQ(0, errno);
}

TEST(fcntl, fallocate) {
  TemporaryFile tf;
  struct stat sb;
  ASSERT_EQ(0, fstat(tf.fd, &sb));
  ASSERT_EQ(0, sb.st_size);

#if defined(__BIONIC__)
  ASSERT_EQ(0, fallocate(tf.fd, 0, 0, 1));
  ASSERT_EQ(0, fstat(tf.fd, &sb));
  ASSERT_EQ(1, sb.st_size);

  ASSERT_EQ(0, fallocate64(tf.fd, 0, 0, 2));
  ASSERT_EQ(0, fstat(tf.fd, &sb));
  ASSERT_EQ(2, sb.st_size);
#endif

  ASSERT_EQ(0, posix_fallocate(tf.fd, 0, 3));
  ASSERT_EQ(0, fstat(tf.fd, &sb));
  ASSERT_EQ(3, sb.st_size);

  ASSERT_EQ(0, posix_fallocate64(tf.fd, 0, 4));
  ASSERT_EQ(0, fstat(tf.fd, &sb));
  ASSERT_EQ(4, sb.st_size);
}

TEST(fcntl, f_getlk64) {
  int fd = open64("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);

  struct flock64 check_lock;
  check_lock.l_type = F_WRLCK;
  check_lock.l_start = 0;
  check_lock.l_whence = SEEK_SET;
  check_lock.l_len = 0;

  int rc = fcntl(fd, F_GETLK64, &check_lock);
  ASSERT_EQ(0, rc);

  close(fd);
}
