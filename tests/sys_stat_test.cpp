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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <android-base/file.h>
#include <gtest/gtest.h>

#if defined(__BIONIC__)
#define HAVE_STATX
#elif defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 28)
#define HAVE_STATX
#endif
#endif

TEST(sys_stat, futimens) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != nullptr);

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

TEST(sys_stat, mkfifo_failure) {
  errno = 0;
  ASSERT_EQ(-1, mkfifo("/", 0666));
  ASSERT_EQ(EEXIST, errno);
}

TEST(sys_stat, mkfifoat_failure) {
  errno = 0;
  ASSERT_EQ(-1, mkfifoat(-2, "x", 0666));
  ASSERT_EQ(EBADF, errno);
}

TEST(sys_stat, mkfifo) {
  if (getuid() == 0) {
    // Racy but probably sufficient way to get a suitable filename.
    std::string path;
    {
      TemporaryFile tf;
      path = tf.path;
    }

    ASSERT_EQ(0, mkfifo(path.c_str(), 0666));
    struct stat sb;
    ASSERT_EQ(0, stat(path.c_str(), &sb));
    ASSERT_TRUE(S_ISFIFO(sb.st_mode));
    unlink(path.c_str());
  } else {
    // SELinux policy forbids us from creating FIFOs. http://b/17646702.
    GTEST_SKIP() << "SELinux policy forbids non-root from creating FIFOs";
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

TEST(sys_stat, statx) {
#if defined(HAVE_STATX)
  struct statx sx;
  int rc = statx(AT_FDCWD, "/proc/version", AT_STATX_SYNC_AS_STAT, STATX_ALL, &sx);
  if (rc == -1 && errno == ENOSYS) {
    GTEST_SKIP() << "statx returned ENOSYS";
    return;
  }
  ASSERT_EQ(0, rc);
  struct stat64 sb;
  ASSERT_EQ(0, stat64("/proc/version", &sb));
  EXPECT_EQ(sb.st_ino, sx.stx_ino);
  EXPECT_EQ(sb.st_mode, sx.stx_mode);
#else
  GTEST_SKIP() << "statx not available";
#endif
}

TEST(sys_stat, fchmodat_EFAULT_file) {
  ASSERT_EQ(-1, fchmodat(AT_FDCWD, (char *) 0x1, 0751, 0));
  ASSERT_EQ(EFAULT, errno);
}

TEST(sys_stat, fchmodat_AT_SYMLINK_NOFOLLOW_EFAULT_file) {
  ASSERT_EQ(-1, fchmodat(AT_FDCWD, (char *) 0x1, 0751, AT_SYMLINK_NOFOLLOW));
#if defined(__BIONIC__)
  ASSERT_EQ(EFAULT, errno);
#else
  // glibc 2.19 does not implement AT_SYMLINK_NOFOLLOW and always
  // returns ENOTSUP
  ASSERT_EQ(ENOTSUP, errno);
#endif
}

TEST(sys_stat, fchmodat_bad_flags) {
  ASSERT_EQ(-1, fchmodat(AT_FDCWD, "/blah", 0751, ~AT_SYMLINK_NOFOLLOW));
  ASSERT_EQ(EINVAL, errno);
}

TEST(sys_stat, fchmodat_bad_flags_ALL) {
  ASSERT_EQ(-1, fchmodat(AT_FDCWD, "/blah", 0751, ~0));
  ASSERT_EQ(EINVAL, errno);
}

TEST(sys_stat, fchmodat_nonexistant_file) {
  ASSERT_EQ(-1, fchmodat(AT_FDCWD, "/blah", 0751, 0));
  ASSERT_EQ(ENOENT, errno);
}

TEST(sys_stat, fchmodat_AT_SYMLINK_NOFOLLOW_nonexistant_file) {
  ASSERT_EQ(-1, fchmodat(AT_FDCWD, "/blah", 0751, AT_SYMLINK_NOFOLLOW));
#if defined(__BIONIC__)
  ASSERT_EQ(ENOENT, errno);
#else
  // glibc 2.19 does not implement AT_SYMLINK_NOFOLLOW and always
  // returns ENOTSUP
  ASSERT_EQ(ENOTSUP, errno);
#endif
}

static void AssertFileModeEquals(mode_t expected_mode, const char* filename) {
  struct stat sb;
  ASSERT_EQ(0, stat(filename, &sb));
  mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
  ASSERT_EQ(expected_mode & mask, static_cast<mode_t>(sb.st_mode) & mask);
}

TEST(sys_stat, fchmodat_file) {
  TemporaryFile tf;

  ASSERT_EQ(0, fchmodat(AT_FDCWD, tf.path, 0751, 0));
  AssertFileModeEquals(0751, tf.path);
}

TEST(sys_stat, fchmodat_AT_SYMLINK_NOFOLLOW_file) {
  TemporaryFile tf;
  errno = 0;
  int result = fchmodat(AT_FDCWD, tf.path, 0751, AT_SYMLINK_NOFOLLOW);

#if defined(__BIONIC__)
  ASSERT_EQ(0, result);
  ASSERT_EQ(0, errno);
  AssertFileModeEquals(0751, tf.path);
#else
  // glibc 2.19 does not implement AT_SYMLINK_NOFOLLOW and always
  // returns ENOTSUP
  ASSERT_EQ(-1, result);
  ASSERT_EQ(ENOTSUP, errno);
#endif
}

TEST(sys_stat, fchmodat_symlink) {
  TemporaryFile tf;
  char linkname[255];

  snprintf(linkname, sizeof(linkname), "%s.link", tf.path);

  ASSERT_EQ(0, symlink(tf.path, linkname));
  ASSERT_EQ(0, fchmodat(AT_FDCWD, linkname, 0751, 0));
  AssertFileModeEquals(0751, tf.path);
  unlink(linkname);
}

TEST(sys_stat, fchmodat_dangling_symlink) {
  TemporaryFile tf;
  char linkname[255];
  char target[255];

  snprintf(linkname, sizeof(linkname), "%s.link", tf.path);
  snprintf(target, sizeof(target), "%s.doesnotexist", tf.path);

  ASSERT_EQ(0, symlink(target, linkname));
  ASSERT_EQ(-1, fchmodat(AT_FDCWD, linkname, 0751, 0));
  ASSERT_EQ(ENOENT, errno);
  unlink(linkname);
}

static void AssertSymlinkModeEquals(mode_t expected_mode, const char* linkname) {
  struct stat sb;
  ASSERT_EQ(0, fstatat(AT_FDCWD, linkname, &sb, AT_SYMLINK_NOFOLLOW));
  mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
  ASSERT_EQ(expected_mode & mask, static_cast<mode_t>(sb.st_mode) & mask);
}

TEST(sys_stat, fchmodat_AT_SYMLINK_NOFOLLOW_with_symlink) {
  TemporaryFile tf;
  struct stat tf_sb;
  ASSERT_EQ(0, stat(tf.path, &tf_sb));

  char linkname[255];
  snprintf(linkname, sizeof(linkname), "%s.link", tf.path);

  ASSERT_EQ(0, symlink(tf.path, linkname));
  int result = fchmodat(AT_FDCWD, linkname, 0751, AT_SYMLINK_NOFOLLOW);
  // It depends on the kernel whether chmod operation on symlink is allowed.
  if (result == 0) {
    AssertSymlinkModeEquals(0751, linkname);
  } else {
    ASSERT_EQ(-1, result);
    ASSERT_EQ(ENOTSUP, errno);
  }

  // Target file mode shouldn't be modified.
  AssertFileModeEquals(tf_sb.st_mode, tf.path);
  unlink(linkname);
}

TEST(sys_stat, fchmodat_AT_SYMLINK_NOFOLLOW_with_dangling_symlink) {
  TemporaryFile tf;

  char linkname[255];
  char target[255];
  snprintf(linkname, sizeof(linkname), "%s.link", tf.path);
  snprintf(target, sizeof(target), "%s.doesnotexist", tf.path);

  ASSERT_EQ(0, symlink(target, linkname));
  int result = fchmodat(AT_FDCWD, linkname, 0751, AT_SYMLINK_NOFOLLOW);
  // It depends on the kernel whether chmod operation on symlink is allowed.
  if (result == 0) {
    AssertSymlinkModeEquals(0751, linkname);
  } else {
    ASSERT_EQ(-1, result);
    ASSERT_EQ(ENOTSUP, errno);
  }

  unlink(linkname);
}

TEST(sys_stat, faccessat_EINVAL) {
  ASSERT_EQ(-1, faccessat(AT_FDCWD, "/dev/null", F_OK, ~AT_SYMLINK_NOFOLLOW));
  ASSERT_EQ(EINVAL, errno);
#if defined(__BIONIC__)
  ASSERT_EQ(-1, faccessat(AT_FDCWD, "/dev/null", ~(R_OK | W_OK | X_OK), 0));
  ASSERT_EQ(EINVAL, errno);
#else
  ASSERT_EQ(0, faccessat(AT_FDCWD, "/dev/null", ~(R_OK | W_OK | X_OK), AT_SYMLINK_NOFOLLOW));
  ASSERT_EQ(-1, faccessat(AT_FDCWD, "/dev/null", ~(R_OK | W_OK | X_OK), 0));
  ASSERT_EQ(EINVAL, errno);
#endif
}

TEST(sys_stat, faccessat_AT_SYMLINK_NOFOLLOW_EINVAL) {
#if defined(__BIONIC__)
  // Android doesn't support AT_SYMLINK_NOFOLLOW
  ASSERT_EQ(-1, faccessat(AT_FDCWD, "/dev/null", F_OK, AT_SYMLINK_NOFOLLOW));
  ASSERT_EQ(EINVAL, errno);
#else
  ASSERT_EQ(0, faccessat(AT_FDCWD, "/dev/null", F_OK, AT_SYMLINK_NOFOLLOW));
#endif
}

TEST(sys_stat, faccessat_dev_null) {
  ASSERT_EQ(0, faccessat(AT_FDCWD, "/dev/null", F_OK, 0));
  ASSERT_EQ(0, faccessat(AT_FDCWD, "/dev/null", R_OK, 0));
  ASSERT_EQ(0, faccessat(AT_FDCWD, "/dev/null", W_OK, 0));
  ASSERT_EQ(0, faccessat(AT_FDCWD, "/dev/null", R_OK|W_OK, 0));
}

TEST(sys_stat, faccessat_nonexistant) {
  ASSERT_EQ(-1, faccessat(AT_FDCWD, "/blah", F_OK, AT_SYMLINK_NOFOLLOW));
#if defined(__BIONIC__)
  // Android doesn't support AT_SYMLINK_NOFOLLOW
  ASSERT_EQ(EINVAL, errno);
#else
  ASSERT_EQ(ENOENT, errno);
#endif
}
