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

TEST(fcntl, posix_fadvise) {
  TemporaryFile tf;
  errno = 0;

  EXPECT_EQ(EBADF, posix_fadvise(-1, 0, 0, POSIX_FADV_NORMAL));
  EXPECT_EQ(0, errno);

  EXPECT_EQ(EBADF, posix_fadvise64(-1, 0, 0, POSIX_FADV_NORMAL));
  EXPECT_EQ(0, errno);

  EXPECT_EQ(EINVAL, posix_fadvise(tf.fd, 0, 0, -1));
  EXPECT_EQ(0, errno);

  EXPECT_EQ(EINVAL, posix_fadvise64(tf.fd, 0, 0, -1));
  EXPECT_EQ(0, errno);

  EXPECT_EQ(0, posix_fadvise(tf.fd, 0, 0, POSIX_FADV_NORMAL));
  EXPECT_EQ(0, posix_fadvise64(tf.fd, 0, 0, POSIX_FADV_NORMAL));
}

TEST(fcntl, fallocate_EINVAL) {
  TemporaryFile tf;

  // fallocate/fallocate64 set errno.
  // posix_fallocate/posix_fallocate64 return an errno value.

  errno = 0;
  ASSERT_EQ(-1, fallocate(tf.fd, 0, 0, -1));
  ASSERT_EQ(EINVAL, errno);

  errno = 0;
  ASSERT_EQ(-1, fallocate64(tf.fd, 0, 0, -1));
  ASSERT_EQ(EINVAL, errno);

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

TEST(fcntl, splice) {
  int pipe_fds[2];
  ASSERT_EQ(0, pipe(pipe_fds));

  int in = open("/proc/cpuinfo", O_RDONLY);
  ASSERT_NE(in, -1);

  TemporaryFile tf;

  ssize_t bytes_read = splice(in, 0, pipe_fds[1], NULL, 8*1024, SPLICE_F_MORE | SPLICE_F_MOVE);
  ASSERT_NE(bytes_read, -1);

  ssize_t bytes_written = splice(pipe_fds[0], NULL, tf.fd, 0, bytes_read, SPLICE_F_MORE | SPLICE_F_MOVE);
  ASSERT_EQ(bytes_read, bytes_written);

  close(pipe_fds[0]);
  close(pipe_fds[1]);
  close(in);
}

TEST(fcntl, vmsplice) {
  int pipe_fds[2];
  ASSERT_EQ(0, pipe(pipe_fds));

  iovec v[2];
  v[0].iov_base = const_cast<char*>("hello ");
  v[0].iov_len = 6;
  v[1].iov_base = const_cast<char*>("world\n");
  v[1].iov_len = 6;
  ssize_t bytes_written = vmsplice(pipe_fds[1], v, sizeof(v)/sizeof(iovec), 0);
  ASSERT_EQ(v[0].iov_len + v[1].iov_len, static_cast<size_t>(bytes_written));
  close(pipe_fds[1]);

  char buf[BUFSIZ];
  FILE* fp = fdopen(pipe_fds[0], "r");
  ASSERT_TRUE(fp != NULL);
  ASSERT_TRUE(fgets(buf, sizeof(buf), fp) != NULL);
  fclose(fp);
  ASSERT_STREQ("hello world\n", buf);
}

TEST(fcntl, tee) {
  char expected[256];
  FILE* expected_fp = fopen("/proc/version", "r");
  ASSERT_TRUE(expected_fp != NULL);
  ASSERT_TRUE(fgets(expected, sizeof(expected), expected_fp) != NULL);
  fclose(expected_fp);

  int pipe1[2];
  ASSERT_EQ(0, pipe(pipe1));

  int pipe2[2];
  ASSERT_EQ(0, pipe(pipe2));

  int in = open("/proc/version", O_RDONLY);
  ASSERT_NE(in, -1);

  // Write /proc/version into pipe1.
  ssize_t bytes_read = splice(in, 0, pipe1[1], NULL, 8*1024, SPLICE_F_MORE | SPLICE_F_MOVE);
  ASSERT_NE(bytes_read, -1);
  close(pipe1[1]);

  // Tee /proc/version from pipe1 into pipe2.
  ssize_t bytes_teed = tee(pipe1[0], pipe2[1], SIZE_MAX, 0);
  ASSERT_EQ(bytes_read, bytes_teed);
  close(pipe2[1]);

  // The out fds of both pipe1 and pipe2 should now contain /proc/version.
  char buf1[BUFSIZ];
  FILE* fp1 = fdopen(pipe1[0], "r");
  ASSERT_TRUE(fp1 != NULL);
  ASSERT_TRUE(fgets(buf1, sizeof(buf1), fp1) != NULL);
  fclose(fp1);

  char buf2[BUFSIZ];
  FILE* fp2 = fdopen(pipe2[0], "r");
  ASSERT_TRUE(fp2 != NULL);
  ASSERT_TRUE(fgets(buf2, sizeof(buf2), fp2) != NULL);
  fclose(fp2);

  ASSERT_STREQ(expected, buf1);
  ASSERT_STREQ(expected, buf2);
}
