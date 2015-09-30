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

#include <gtest/gtest.h>

#include <sys/uio.h>

#include "TemporaryFile.h"

TEST(sys_uio, readv_writev) {
  TemporaryFile tf;

  char buf1[] = "hello";
  char buf2[] = "world";
  iovec ios[] = { { buf1, 5 }, { buf2, 5 } };

  ASSERT_EQ(10, writev(tf.fd, ios, 2));

  ASSERT_EQ(0, lseek(tf.fd, 0, SEEK_SET));

  memset(buf1, '1', sizeof(buf1));
  memset(buf2, '2', sizeof(buf2));

  ASSERT_EQ(10, readv(tf.fd, ios, 2));
  buf1[5] = buf2[5] = '\0';
  ASSERT_STREQ("hello", buf1);
  ASSERT_STREQ("world", buf2);
}

template <typename ReadFn, typename WriteFn>
void TestPreadVPwriteV(ReadFn read_fn, WriteFn write_fn) {
  TemporaryFile tf;

  char buf[] = "world";
  iovec ios[] = { { buf, 5 } };

  ASSERT_EQ(5, write_fn(tf.fd, ios, 1, 5));
  ASSERT_EQ(0, lseek(tf.fd, 0, SEEK_CUR));

  strcpy(buf, "hello");
  ASSERT_EQ(5, write_fn(tf.fd, ios, 1, 0));
  ASSERT_EQ(0, lseek(tf.fd, 0, SEEK_CUR));

  ASSERT_EQ(5, read_fn(tf.fd, ios, 1, 5));
  ASSERT_STREQ("world", buf);
  ASSERT_EQ(5, read_fn(tf.fd, ios, 1, 0));
  ASSERT_STREQ("hello", buf);
}

TEST(sys_uio, preadv_pwritev) {
  TestPreadVPwriteV(preadv, pwritev);
}

TEST(sys_uio, preadv64_pwritev64) {
  TestPreadVPwriteV(preadv64, pwritev64);
}

TEST(sys_uio, process_vm_readv) {
  ASSERT_EQ(0, process_vm_readv(0, nullptr, 0, nullptr, 0, 0));
}

TEST(sys_uio, process_vm_writev) {
  ASSERT_EQ(0, process_vm_writev(0, nullptr, 0, nullptr, 0, 0));
}
