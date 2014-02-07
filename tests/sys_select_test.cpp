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
#include <signal.h>
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

TEST(sys_select, select_smoke) {
  fd_set r;
  FD_ZERO(&r);
  fd_set w;
  FD_ZERO(&w);
  fd_set e;
  FD_ZERO(&e);

  FD_SET(STDIN_FILENO, &r);
  FD_SET(STDOUT_FILENO, &w);
  FD_SET(STDERR_FILENO, &w);

  int max = STDERR_FILENO + 1;

  // Invalid max fd.
  ASSERT_EQ(-1, select(-1, &r, &w, &e, NULL));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(2, select(max, &r, &w, &e, NULL));

  // Invalid timeout.
  timeval tv;
  tv.tv_sec = -1;
  tv.tv_usec = 0;
  ASSERT_EQ(-1, select(max, &r, &w, &e, &tv));
  ASSERT_EQ(EINVAL, errno);

  // Valid timeout...
  tv.tv_sec = 1;
  ASSERT_EQ(2, select(max, &r, &w, &e, &tv));
  ASSERT_NE(0, tv.tv_usec); // ...which got updated.
}

TEST(sys_select, pselect_smoke) {
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, SIGPIPE);

  fd_set r;
  FD_ZERO(&r);
  fd_set w;
  FD_ZERO(&w);
  fd_set e;
  FD_ZERO(&e);

  FD_SET(STDIN_FILENO, &r);
  FD_SET(STDOUT_FILENO, &w);
  FD_SET(STDERR_FILENO, &w);

  int max = STDERR_FILENO + 1;

  // Invalid max fd.
  ASSERT_EQ(-1, pselect(-1, &r, &w, &e, NULL, &ss));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(2, pselect(max, &r, &w, &e, NULL, &ss));

  // Invalid timeout.
  timespec tv;
  tv.tv_sec = -1;
  tv.tv_nsec = 0;
  ASSERT_EQ(-1, pselect(max, &r, &w, &e, &tv, &ss));
  ASSERT_EQ(EINVAL, errno);

  // Valid timeout...
  tv.tv_sec = 1;
  ASSERT_EQ(2, pselect(max, &r, &w, &e, &tv, &ss));
  ASSERT_EQ(0, tv.tv_nsec); // ...which did _not_ get updated.
}
