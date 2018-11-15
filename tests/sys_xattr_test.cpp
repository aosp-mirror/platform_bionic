/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include <android-base/file.h>
#include <gtest/gtest.h>

TEST(sys_xattr, setxattr) {
  TemporaryFile tf;
  char buf[10];
  ASSERT_EQ(0, setxattr(tf.path, "user.foo", "bar", 4, 0));
  ASSERT_EQ(4, getxattr(tf.path, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
  buf[0] = '\0';
  ASSERT_EQ(4, lgetxattr(tf.path, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
}

TEST(sys_xattr, fsetxattr) {
  TemporaryFile tf;
  char buf[10];
  ASSERT_EQ(0, fsetxattr(tf.fd, "user.foo", "bar", 4, 0));
  ASSERT_EQ(4, fgetxattr(tf.fd, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
}

TEST(sys_xattr, fsetxattr_zerobuf) {
  TemporaryFile tf;
  char buf[10];
  ASSERT_EQ(0, fsetxattr(tf.fd, "user.foo", "", 0, 0));
  ASSERT_EQ(0, fgetxattr(tf.fd, "user.foo", buf, sizeof(buf)));
}

TEST(sys_xattr, fsetxattr_toosmallbuf) {
  TemporaryFile tf;
  char buf[10];
  ASSERT_EQ(0, fsetxattr(tf.fd, "user.foo", "01234567890123456789", 21, 0));
  ASSERT_EQ(-1, fgetxattr(tf.fd, "user.foo", buf, sizeof(buf)));
  ASSERT_EQ(ERANGE, errno);
}

TEST(sys_xattr, fsetxattr_invalidfd) {
  char buf[10];
  errno = 0;
  ASSERT_EQ(-1, fsetxattr(65535, "user.foo", "0123", 5, 0));
  ASSERT_EQ(EBADF, errno);
  errno = 0;
  ASSERT_EQ(-1, fgetxattr(65535, "user.foo", buf, sizeof(buf)));
  ASSERT_EQ(EBADF, errno);
}

TEST(sys_xattr, fsetxattr_with_opath) {
  TemporaryFile tf;
  int fd = open(tf.path, O_PATH);
  ASSERT_NE(-1, fd);

  int res = fsetxattr(fd, "user.foo", "bar", 4, 0);
#if defined(__BIONIC__)
  char buf[10];
  ASSERT_EQ(0, res);
  ASSERT_EQ(4, fgetxattr(fd, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
#else
  ASSERT_EQ(-1, res);
  ASSERT_EQ(EBADF, errno);
#endif
  close(fd);
}

TEST(sys_xattr, fsetxattr_with_opath_toosmall) {
  TemporaryFile tf;
  int fd = open(tf.path, O_PATH);
  ASSERT_NE(-1, fd);

  int res = fsetxattr(fd, "user.foo", "01234567890123456789", 21, 0);
#if defined(__BIONIC__)
  char buf[10];
  ASSERT_EQ(0, res);
  ASSERT_EQ(-1, fgetxattr(fd, "user.foo", buf, sizeof(buf)));
  ASSERT_EQ(ERANGE, errno);
#else
  ASSERT_EQ(-1, res);
  ASSERT_EQ(EBADF, errno);
#endif
  close(fd);
}

TEST(sys_xattr, flistattr) {
  TemporaryFile tf;
  char buf[65536];  // 64kB is max possible xattr list size. See "man 7 xattr".
  ASSERT_EQ(0, fsetxattr(tf.fd, "user.foo", "bar", 4, 0));
  ssize_t result = flistxattr(tf.fd, buf, sizeof(buf));
  ASSERT_TRUE(result >= 9);
  ASSERT_TRUE(memmem(buf, sizeof(buf), "user.foo", 9) != nullptr);
}

TEST(sys_xattr, flistattr_opath) {
  TemporaryFile tf;
  char buf[65536];  // 64kB is max possible xattr list size. See "man 7 xattr".
  ASSERT_EQ(0, fsetxattr(tf.fd, "user.foo", "bar", 4, 0));
  int fd = open(tf.path, O_PATH);
  ASSERT_NE(-1, fd);
  ssize_t res = flistxattr(fd, buf, sizeof(buf));
#if defined(__BIONIC__)
  ASSERT_TRUE(res >= 9);
  ASSERT_TRUE(static_cast<size_t>(res) <= sizeof(buf));
  ASSERT_TRUE(memmem(buf, res, "user.foo", 9) != nullptr);
#else
  ASSERT_EQ(-1, res);
  ASSERT_EQ(EBADF, errno);
#endif
  close(fd);
}
