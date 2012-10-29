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

#include <gtest/gtest.h>

#include <libgen.h>

#include <errno.h>

static void TestBasename(const char* in, const char* expected_out) {
  char* writable_in = (in != NULL) ? strdup(in) : NULL;
  errno = 0;
  const char* out = basename(&writable_in[0]);
  ASSERT_STREQ(expected_out, out) << in;
  ASSERT_EQ(0, errno) << in;
  free(writable_in);
}

static void TestDirname(const char* in, const char* expected_out) {
  char* writable_in = (in != NULL) ? strdup(in) : NULL;
  errno = 0;
  const char* out = dirname(&writable_in[0]);
  ASSERT_STREQ(expected_out, out) << in;
  ASSERT_EQ(0, errno) << in;
  free(writable_in);
}

TEST(libgen, basename) {
  TestBasename(NULL, ".");
  TestBasename("", ".");
  TestBasename("/usr/lib", "lib");
  TestBasename("/usr/", "usr");
  TestBasename("usr", "usr");
  TestBasename("/", "/");
  TestBasename(".", ".");
  TestBasename("..", "..");
  TestBasename("///", "/");
  TestBasename("//usr//lib//", "lib");
}

TEST(libgen, dirname) {
  TestDirname(NULL, ".");
  TestDirname("", ".");
  TestDirname("/usr/lib", "/usr");
  TestDirname("/usr/", "/");
  TestDirname("usr", ".");
  TestDirname(".", ".");
  TestDirname("..", ".");
  TestDirname("/", "/");
}

#if __BIONIC__

static void TestBasename(const char* in, const char* expected_out, int expected_rc,
                         char* buf, size_t buf_size, int expected_errno) {
  errno = 0;
  int rc = basename_r(in, buf, buf_size);
  ASSERT_EQ(expected_rc, rc) << in;
  if (rc != -1 && buf != NULL) {
    ASSERT_STREQ(expected_out, buf) << in;
  }
  ASSERT_EQ(expected_errno, errno) << in;
}

static void TestDirname(const char* in, const char* expected_out, int expected_rc,
                        char* buf, size_t buf_size, int expected_errno) {
  errno = 0;
  int rc = dirname_r(in, buf, buf_size);
  ASSERT_EQ(expected_rc, rc) << in;
  if (rc != -1 && buf != NULL) {
    ASSERT_STREQ(expected_out, buf) << in;
  }
  ASSERT_EQ(expected_errno, errno) << in;
}

TEST(libgen, basename_r) {
  char buf[256];
  TestBasename("", ".",  1, NULL, 0, 0);
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
}

TEST(libgen, dirname_r) {
  char buf[256];
  TestDirname("", ".",  1, NULL, 0, 0);
  TestDirname("", ".", -1, buf, 0, ERANGE);
  TestDirname("", ".", -1, buf, 1, ERANGE);
  TestDirname("", ".", 1, buf, 2, 0);
  TestDirname("/usr/lib", "/usr", 4, buf, sizeof(buf), 0);
  TestDirname("/usr/", "/", 1, buf, sizeof(buf), 0);
  TestDirname("usr", ".", 1, buf, sizeof(buf), 0);
  TestDirname(".", ".", 1, buf, sizeof(buf), 0);
  TestDirname("..", ".", 1, buf, sizeof(buf), 0);
}

#endif
