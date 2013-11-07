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
}
