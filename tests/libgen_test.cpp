/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <libgen.h>

#include <errno.h>
#include <gtest/gtest.h>

static void TestDirname(const char* in, const char* expected_out) {
  char* writable_in = (in != nullptr) ? strdup(in) : nullptr;
  errno = 0;
  const char* out = dirname(&writable_in[0]);
  ASSERT_STREQ(expected_out, out) << in;
  ASSERT_EQ(0, errno) << in;
  free(writable_in);
}

TEST(libgen, dirname) {
  TestDirname(nullptr, ".");
  TestDirname("", ".");
  TestDirname("/usr/lib", "/usr");
  TestDirname("/usr/", "/");
  TestDirname("usr", ".");
  TestDirname(".", ".");
  TestDirname("..", ".");
  TestDirname("/", "/");
}

#if defined(__BIONIC__) && !defined(__LP64__)
static void TestBasename(const char* in, const char* expected_out, int expected_rc,
                         char* buf, size_t buf_size, int expected_errno) {
  errno = 0;
  int rc = basename_r(in, buf, buf_size);
  ASSERT_EQ(expected_rc, rc) << in;
  if (rc != -1 && buf != nullptr) {
    ASSERT_STREQ(expected_out, buf) << in;
  }
  ASSERT_EQ(expected_errno, errno) << in;
}

static void TestDirname(const char* in, const char* expected_out, int expected_rc,
                        char* buf, size_t buf_size, int expected_errno) {
  errno = 0;
  int rc = dirname_r(in, buf, buf_size);
  ASSERT_EQ(expected_rc, rc) << in;
  if (rc != -1 && buf != nullptr) {
    ASSERT_STREQ(expected_out, buf) << in;
  }
  ASSERT_EQ(expected_errno, errno) << in;
}
#endif // __BIONIC__

TEST(libgen, basename_r) {
#if defined(__BIONIC__) && !defined(__LP64__)
  char buf[256];
  TestBasename("", ".",  1, nullptr, 0, 0);
  TestBasename("", ".", -1, buf, 0, ERANGE);
  TestBasename("", ".", -1, buf, 1, ERANGE);
  TestBasename("", ".", 1, buf, 2, 0);
  TestBasename("", ".", 1, buf, sizeof(buf), 0);
  TestBasename("/usr/lib", "lib", 3, buf, sizeof(buf), 0);
  TestBasename("/usr/", "usr", 3, buf, sizeof(buf), 0);
  TestBasename("usr", "usr", 3, buf, sizeof(buf), 0);
  TestBasename("/", "/", 1, buf, sizeof(buf), 0);
  TestBasename(".", ".", 1, buf, sizeof(buf), 0);
  TestBasename("..", "..", 2, buf, sizeof(buf), 0);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(libgen, dirname_r) {
#if defined(__BIONIC__) && !defined(__LP64__)
  char buf[256];
  TestDirname("", ".",  1, nullptr, 0, 0);
  TestDirname("", ".", -1, buf, 0, ERANGE);
  TestDirname("", ".", -1, buf, 1, ERANGE);
  TestDirname("", ".", 1, buf, 2, 0);
  TestDirname("/usr/lib", "/usr", 4, buf, sizeof(buf), 0);
  TestDirname("/usr/", "/", 1, buf, sizeof(buf), 0);
  TestDirname("usr", ".", 1, buf, sizeof(buf), 0);
  TestDirname(".", ".", 1, buf, sizeof(buf), 0);
  TestDirname("..", ".", 1, buf, sizeof(buf), 0);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}
