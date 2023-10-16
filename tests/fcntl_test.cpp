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
#include <string.h>
#include <sys/utsname.h>
#include <sys/vfs.h>

#include <android-base/file.h>
#include <android-base/silent_death_test.h>
#include <android-base/stringprintf.h>

// Glibc v2.19 doesn't include these in fcntl.h so host builds will fail without.
#if !defined(FALLOC_FL_PUNCH_HOLE) || !defined(FALLOC_FL_KEEP_SIZE)
#include <linux/falloc.h>
#endif
#if !defined(EXT4_SUPER_MAGIC)
#include <linux/magic.h>
#endif

#include "utils.h"

using fcntl_DeathTest = SilentDeathTest;

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
  ASSERT_ERRNO(ENOENT);
  ASSERT_EQ(-1, creat64("", 0666));
  ASSERT_ERRNO(ENOENT);
}

TEST(fcntl, posix_fadvise) {
  TemporaryFile tf;
  errno = 0;

  EXPECT_EQ(EBADF, posix_fadvise(-1, 0, 0, POSIX_FADV_NORMAL));
  EXPECT_ERRNO(0);

  EXPECT_EQ(EBADF, posix_fadvise64(-1, 0, 0, POSIX_FADV_NORMAL));
  EXPECT_ERRNO(0);

  EXPECT_EQ(EINVAL, posix_fadvise(tf.fd, 0, 0, -1));
  EXPECT_ERRNO(0);

  EXPECT_EQ(EINVAL, posix_fadvise64(tf.fd, 0, 0, -1));
  EXPECT_ERRNO(0);

  EXPECT_EQ(0, posix_fadvise(tf.fd, 0, 0, POSIX_FADV_NORMAL));
  EXPECT_EQ(0, posix_fadvise64(tf.fd, 0, 0, POSIX_FADV_NORMAL));
}

TEST(fcntl, fallocate_EINVAL) {
  TemporaryFile tf;

  // fallocate/fallocate64 set errno.
  // posix_fallocate/posix_fallocate64 return an errno value.

  errno = 0;
  ASSERT_EQ(-1, fallocate(tf.fd, 0, 0, -1));
  ASSERT_ERRNO(EINVAL);

  errno = 0;
  ASSERT_EQ(-1, fallocate64(tf.fd, 0, 0, -1));
  ASSERT_ERRNO(EINVAL);

  errno = 0;
  ASSERT_EQ(EINVAL, posix_fallocate(tf.fd, 0, -1));
  ASSERT_ERRNO(0);

  errno = 0;
  ASSERT_EQ(EINVAL, posix_fallocate64(tf.fd, 0, -1));
  ASSERT_ERRNO(0);
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

TEST(fcntl, f_getlk) {
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);

  struct flock check_lock;
  check_lock.l_type = F_WRLCK;
  check_lock.l_start = 0;
  check_lock.l_whence = SEEK_SET;
  check_lock.l_len = 0;

  ASSERT_EQ(0, fcntl(fd, F_GETLK, &check_lock));
  close(fd);
}

TEST(fcntl, f_getlk64) {
  int fd = open64("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);

  struct flock64 check_lock;
  check_lock.l_type = F_WRLCK;
  check_lock.l_start = 0;
  check_lock.l_whence = SEEK_SET;
  check_lock.l_len = 0;

  ASSERT_EQ(0, fcntl(fd, F_GETLK64, &check_lock));
  close(fd);
}

TEST(fcntl, splice) {
  int pipe_fds[2];
  ASSERT_EQ(0, pipe(pipe_fds));

  int in = open("/proc/cpuinfo", O_RDONLY);
  ASSERT_NE(in, -1);

  TemporaryFile tf;

  ssize_t bytes_read = splice(in, nullptr, pipe_fds[1], nullptr, 8*1024, SPLICE_F_MORE | SPLICE_F_MOVE);
  ASSERT_NE(bytes_read, -1);

  ssize_t bytes_written = splice(pipe_fds[0], nullptr, tf.fd, nullptr, bytes_read, SPLICE_F_MORE | SPLICE_F_MOVE);
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
  ASSERT_TRUE(fp != nullptr);
  ASSERT_TRUE(fgets(buf, sizeof(buf), fp) != nullptr);
  fclose(fp);
  ASSERT_STREQ("hello world\n", buf);
}

TEST(fcntl, tee) {
  char expected[BUFSIZ];
  FILE* expected_fp = fopen("/proc/version", "r");
  ASSERT_TRUE(expected_fp != nullptr);
  ASSERT_TRUE(fgets(expected, sizeof(expected), expected_fp) != nullptr);
  fclose(expected_fp);

  int pipe1[2];
  ASSERT_EQ(0, pipe(pipe1));

  int pipe2[2];
  ASSERT_EQ(0, pipe(pipe2));

  int in = open("/proc/version", O_RDONLY);
  ASSERT_NE(in, -1);

  // Write /proc/version into pipe1.
  ssize_t bytes_read = splice(in, nullptr, pipe1[1], nullptr, 8*1024, SPLICE_F_MORE | SPLICE_F_MOVE);
  ASSERT_NE(bytes_read, -1);
  close(pipe1[1]);

  // Tee /proc/version from pipe1 into pipe2.
  ssize_t bytes_teed = tee(pipe1[0], pipe2[1], SIZE_MAX, 0);
  ASSERT_EQ(bytes_read, bytes_teed);
  close(pipe2[1]);

  // The out fds of both pipe1 and pipe2 should now contain /proc/version.
  char buf1[BUFSIZ];
  FILE* fp1 = fdopen(pipe1[0], "r");
  ASSERT_TRUE(fp1 != nullptr);
  ASSERT_TRUE(fgets(buf1, sizeof(buf1), fp1) != nullptr);
  fclose(fp1);

  char buf2[BUFSIZ];
  FILE* fp2 = fdopen(pipe2[0], "r");
  ASSERT_TRUE(fp2 != nullptr);
  ASSERT_TRUE(fgets(buf2, sizeof(buf2), fp2) != nullptr);
  fclose(fp2);

  ASSERT_STREQ(expected, buf1);
  ASSERT_STREQ(expected, buf2);
}

TEST(fcntl, readahead) {
  // Just check that the function is available.
  errno = 0;
  ASSERT_EQ(-1, readahead(-1, 0, 123));
  ASSERT_ERRNO(EBADF);
}

TEST(fcntl, sync_file_range) {
  // Just check that the function is available.
  errno = 0;
  ASSERT_EQ(-1, sync_file_range(-1, 0, 0, 0));
  ASSERT_ERRNO(EBADF);

  TemporaryFile tf;
  ASSERT_EQ(0, sync_file_range(tf.fd, 0, 0, 0));

  // The arguments to the underlying system call are in a different order on 32-bit ARM.
  // Check that the `flags` argument gets passed to the kernel correctly.
  errno = 0;
  ASSERT_EQ(-1, sync_file_range(tf.fd, 0, 0, ~0));
  ASSERT_ERRNO(EINVAL);
}

static bool parse_kernel_release(long* const major, long* const minor) {
  struct utsname buf;
  if (uname(&buf) == -1) {
    return false;
  }
  return sscanf(buf.release, "%ld.%ld", major, minor) == 2;
}

/*
 * b/28760453:
 * Kernels older than 4.1 should have ext4 FALLOC_FL_PUNCH_HOLE disabled due to CVE-2015-8839.
 * Devices that fail this test should cherry-pick the following commit:
 * https://android.googlesource.com/kernel/msm/+/bdba352e898cbf57c8620ad68c8abf749c784d1f
 */
TEST(fcntl, falloc_punch) {
  long major = 0, minor = 0;
  ASSERT_TRUE(parse_kernel_release(&major, &minor));

  if (major < 4 || (major == 4 && minor < 1)) {
    TemporaryFile tf;
    struct statfs sfs;
    ASSERT_EQ(0, fstatfs(tf.fd, &sfs));
    if (sfs.f_type == EXT4_SUPER_MAGIC) {
      ASSERT_EQ(-1, fallocate(tf.fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE, 0, 1));
      ASSERT_ERRNO(EOPNOTSUPP);
    }
  }
}

TEST(fcntl, open_O_TMPFILE_mode) {
#if defined(__BIONIC__)  // Our glibc is too old for O_TMPFILE.
  TemporaryDir dir;
  // Without O_EXCL, we're allowed to give this a name later.
  // (This is unrelated to the O_CREAT interaction with O_EXCL.)
  const mode_t perms = S_IRUSR | S_IWUSR;
  int fd = open(dir.path, O_TMPFILE | O_RDWR, perms);

  // Ignore kernels without O_TMPFILE support (< 3.11).
  if (fd == -1 && (errno == EISDIR || errno == EINVAL || errno == EOPNOTSUPP)) return;

  ASSERT_TRUE(fd != -1) << strerror(errno);

  // Does the fd claim to have the mode we set?
  struct stat sb = {};
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(perms, (sb.st_mode & ~S_IFMT));

  // On Android if we're not root, we won't be able to create links anyway...
  if (getuid() != 0) return;

  std::string final_path = android::base::StringPrintf("%s/named_now", dir.path);
  ASSERT_EQ(0, linkat(AT_FDCWD, android::base::StringPrintf("/proc/self/fd/%d", fd).c_str(),
                      AT_FDCWD, final_path.c_str(),
                      AT_SYMLINK_FOLLOW));
  ASSERT_EQ(0, close(fd));

  // Does the resulting file claim to have the mode we set?
  ASSERT_EQ(0, stat(final_path.c_str(), &sb));
  ASSERT_EQ(perms, (sb.st_mode & ~S_IFMT));

  // With O_EXCL, you're not allowed to add a name later.
  fd = open(dir.path, O_TMPFILE | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
  ASSERT_TRUE(fd != -1) << strerror(errno);
  errno = 0;
  ASSERT_EQ(-1, linkat(AT_FDCWD, android::base::StringPrintf("/proc/self/fd/%d", fd).c_str(),
                       AT_FDCWD, android::base::StringPrintf("%s/no_chance", dir.path).c_str(),
                       AT_SYMLINK_FOLLOW));
  ASSERT_ERRNO(ENOENT);
  ASSERT_EQ(0, close(fd));
#endif
}

TEST_F(fcntl_DeathTest, fcntl_F_SETFD) {
  EXPECT_DEATH(fcntl(0, F_SETFD, O_NONBLOCK), "only supports FD_CLOEXEC");
}
