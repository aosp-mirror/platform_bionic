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

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include "utils.h"

TEST(getcwd, auto_full) {
  // If we let the library do all the work, everything's fine.
  errno = 0;
  char* cwd = getcwd(nullptr, 0);
  ASSERT_TRUE(cwd != nullptr);
  ASSERT_EQ(0, errno);
  ASSERT_GE(strlen(cwd), 1U);
  free(cwd);
}

TEST(getcwd, auto_reasonable) {
  // If we ask the library to allocate a reasonable buffer, everything's fine.
  errno = 0;
  char* cwd = getcwd(nullptr, PATH_MAX);
  ASSERT_TRUE(cwd != nullptr);
  ASSERT_EQ(0, errno);
  ASSERT_GE(strlen(cwd), 1U);
  free(cwd);
}

TEST(getcwd, auto_too_small) {
  // If we ask the library to allocate a too-small buffer, ERANGE.
  errno = 0;
  char* cwd = getcwd(nullptr, 1);
  ASSERT_TRUE(cwd == nullptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(getcwd, auto_too_large) {
  SKIP_WITH_HWASAN; // allocation size too large
  // If we ask the library to allocate an unreasonably large buffer, ERANGE.
  errno = 0;
  char* cwd = getcwd(nullptr, static_cast<size_t>(-1));
  ASSERT_TRUE(cwd == nullptr);
  ASSERT_EQ(ENOMEM, errno);
}

TEST(getcwd, manual_too_small) {
  // If we allocate a too-small buffer, ERANGE.
  char tiny_buf[1];
  errno = 0;
  char* cwd = getcwd(tiny_buf, sizeof(tiny_buf));
  ASSERT_TRUE(cwd == nullptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(getcwd, manual_zero) {
  // If we allocate a zero-length buffer, EINVAL.
  char tiny_buf[1];
  errno = 0;
  char* cwd = getcwd(tiny_buf, 0);
  ASSERT_TRUE(cwd == nullptr);
  ASSERT_EQ(EINVAL, errno);
}

TEST(getcwd, manual_path_max) {
  char* buf = new char[PATH_MAX];
  errno = 0;
  char* cwd = getcwd(buf, PATH_MAX);
  ASSERT_TRUE(cwd == buf);
  ASSERT_EQ(0, errno);
  ASSERT_GE(strlen(cwd), 1U);
  delete[] cwd;
}
