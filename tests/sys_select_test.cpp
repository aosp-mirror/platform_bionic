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
#include <sys/types.h>
#include <sys/wait.h>

#include "utils.h"

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

#define DELAY_MSG "1234"

static void DelayedWrite(int* pid, int* fd) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  if ((*pid = fork()) == 0) {
    close(fds[0]);
    usleep(5000);
    EXPECT_EQ(5, write(fds[1], DELAY_MSG, sizeof(DELAY_MSG)));
    close(fds[1]);
    exit(0);
  }
  ASSERT_LT(0, *pid);
  close(fds[1]);

  *fd = fds[0];
}

static void DelayedWriteCleanup(int pid, int fd) {
  char buf[sizeof(DELAY_MSG)];
  ASSERT_EQ(static_cast<ssize_t>(sizeof(DELAY_MSG)), read(fd, buf, sizeof(DELAY_MSG)));
  ASSERT_STREQ(DELAY_MSG, buf);

  AssertChildExited(pid, 0);
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

  int num_fds = select(max, &r, &w, &e, NULL);
  // If there is data to be read on STDIN, then the number of
  // fds ready will be 3 instead of 2. Allow this case, but verify
  // every fd that is set.
  ASSERT_TRUE(num_fds == 2 || num_fds == 3) << "Num fds returned " << num_fds;
  ASSERT_TRUE(FD_ISSET(STDOUT_FILENO, &w));
  ASSERT_TRUE(FD_ISSET(STDERR_FILENO, &w));
  if (num_fds == 3) {
    ASSERT_TRUE(FD_ISSET(STDIN_FILENO, &r));
  }

  // Invalid timeout.
  timeval tv;
  tv.tv_sec = -1;
  tv.tv_usec = 0;
  ASSERT_EQ(-1, select(max, &r, &w, &e, &tv));
  ASSERT_EQ(EINVAL, errno);

  // Valid timeout...
  tv.tv_sec = 1;
  int pid, fd;
  DelayedWrite(&pid, &fd);

  FD_ZERO(&r);
  FD_SET(fd, &r);
  ASSERT_EQ(1, select(fd+1, &r, NULL, NULL, &tv));
  // Both tv_sec and tv_nsec should have been updated.
  ASSERT_EQ(0, tv.tv_sec);
  ASSERT_NE(0, tv.tv_usec);

  DelayedWriteCleanup(pid, fd);
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

  // If there is data to be read on STDIN, then the number of
  // fds ready will be 3 instead of 2. Allow this case, but verify
  // every fd that is set.
  int num_fds = pselect(max, &r, &w, &e, NULL, &ss);
  ASSERT_TRUE(num_fds == 2 || num_fds == 3) << "Num fds returned " << num_fds;
  ASSERT_TRUE(FD_ISSET(STDOUT_FILENO, &w));
  ASSERT_TRUE(FD_ISSET(STDERR_FILENO, &w));
  if (num_fds == 3) {
    ASSERT_TRUE(FD_ISSET(STDIN_FILENO, &r));
  }

  // Invalid timeout.
  timespec tv;
  tv.tv_sec = -1;
  tv.tv_nsec = 0;
  ASSERT_EQ(-1, pselect(max, &r, &w, &e, &tv, &ss));
  ASSERT_EQ(EINVAL, errno);

  // Valid timeout...
  tv.tv_sec = 1;
  int pid, fd;
  DelayedWrite(&pid, &fd);

  FD_ZERO(&r);
  FD_SET(fd, &r);
  ASSERT_EQ(1, pselect(fd+1, &r, NULL, NULL, &tv, NULL));
  // Neither tv_sec nor tv_nsec should have been updated.
  ASSERT_EQ(1, tv.tv_sec);
  ASSERT_EQ(0, tv.tv_nsec);

  DelayedWriteCleanup(pid, fd);
}
