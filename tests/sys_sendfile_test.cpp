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
#include "TemporaryFile.h"

#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

TEST(sys_sendfile, sendfile) {
  TemporaryFile src_file;
  ASSERT_EQ(5, TEMP_FAILURE_RETRY(write(src_file.fd, "hello", 5)));

  TemporaryFile dst_file;

  off_t offset = 2;
  size_t count = 2;
  ssize_t rc = sendfile(dst_file.fd, src_file.fd, &offset, count);
  ASSERT_EQ(2, rc);
  ASSERT_EQ(4, offset);

  ASSERT_EQ(0, lseek(dst_file.fd, 0, SEEK_SET));
  char buf[3];
  buf[2] = '\0';
  ASSERT_EQ(2, TEMP_FAILURE_RETRY(read(dst_file.fd, &buf, 2)));
  ASSERT_STREQ("ll", buf);
}

TEST(sys_sendfile, sendfile64) {
  TemporaryFile src_file;
  ASSERT_EQ(5, TEMP_FAILURE_RETRY(write(src_file.fd, "hello", 5)));

  TemporaryFile dst_file;

  off64_t offset = 2;
  size_t count = 2;
  ssize_t rc = sendfile64(dst_file.fd, src_file.fd, &offset, count);
  ASSERT_EQ(2, rc);
  ASSERT_EQ(4, offset);

  ASSERT_EQ(0, lseek(dst_file.fd, 0, SEEK_SET));
  char buf[3];
  buf[2] = '\0';
  ASSERT_EQ(2, TEMP_FAILURE_RETRY(read(dst_file.fd, &buf, 2)));
  ASSERT_STREQ("ll", buf);
}
