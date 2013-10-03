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
#include <stdlib.h>
#include <sys/select.h>

TEST(sys_select, fd_set_smoke) {
  fd_set fds;
  FD_ZERO(&fds);

  for (size_t i = 0; i < 1024; ++i) {
    EXPECT_FALSE(FD_ISSET(i, &fds));
  }

  FD_SET(0, &fds);
  EXPECT_TRUE(FD_ISSET(0, &fds));
  EXPECT_FALSE(FD_ISSET(1, &fds));
  FD_SET(1, &fds);
  EXPECT_TRUE(FD_ISSET(0, &fds));
  EXPECT_TRUE(FD_ISSET(1, &fds));
  FD_CLR(0, &fds);
  EXPECT_FALSE(FD_ISSET(0, &fds));
  EXPECT_TRUE(FD_ISSET(1, &fds));
  FD_CLR(1, &fds);
  EXPECT_FALSE(FD_ISSET(0, &fds));
  EXPECT_FALSE(FD_ISSET(1, &fds));
}
