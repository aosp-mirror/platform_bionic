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
#include <signal.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "utils.h"

TEST(sys_epoll, epoll_wait) {
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd);

  // Regular epoll_wait.
  epoll_event events[1] = {};
  ASSERT_EQ(0, epoll_wait(epoll_fd, events, 1, 1));
}

TEST(sys_epoll, epoll_pwait_no_sigset) {
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd);

  // epoll_pwait without a sigset (which is equivalent to epoll_wait).
  epoll_event events[1] = {};
  ASSERT_EQ(0, epoll_pwait(epoll_fd, events, 1, 1, nullptr));
}

TEST(sys_epoll, epoll_pwait64_no_sigset) {
#if defined(__BIONIC__)
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd);

  // epoll_pwait64 without a sigset (which is equivalent to epoll_wait).
  epoll_event events[1] = {};
  ASSERT_EQ(0, epoll_pwait64(epoll_fd, events, 1, 1, nullptr));
#else
  GTEST_SKIP() << "epoll_pwait64 is bionic-only";
#endif
}

TEST(sys_epoll, epoll_pwait2_no_sigset) {
#if defined(__BIONIC__)
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd);

  // epoll_pwait2 without a sigset (which is equivalent to epoll_wait).
  epoll_event events[1] = {};
  timespec ts = {.tv_nsec = 500};
  int rc = epoll_pwait2(epoll_fd, events, 1, &ts, nullptr);
  if (rc == -1 && errno == ENOSYS) GTEST_SKIP() << "no epoll_pwait2() in this kernel";
  ASSERT_EQ(0, rc) << strerror(errno);
#else
  GTEST_SKIP() << "epoll_pwait2 is only in glibc 2.35+";
#endif
}

TEST(sys_epoll, epoll_pwait_with_sigset) {
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd);

  // epoll_pwait with a sigset.
  epoll_event events[1] = {};
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, SIGPIPE);
  ASSERT_EQ(0, epoll_pwait(epoll_fd, events, 1, 1, &ss));
}

TEST(sys_epoll, epoll_pwait2_with_sigset) {
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd);

#if defined(__BIONIC__)
  epoll_event events[1] = {};
  timespec ts = {.tv_nsec = 500};
  sigset_t ss2;
  sigemptyset(&ss2);
  sigaddset(&ss2, SIGPIPE);
  int rc = epoll_pwait2(epoll_fd, events, 1, &ts, &ss2);
  if (rc == -1 && errno == ENOSYS) GTEST_SKIP() << "no epoll_pwait2() in this kernel";
  ASSERT_EQ(0, rc) << strerror(errno);
#else
  GTEST_SKIP() << "epoll_pwait2 is only in glibc 2.35+";
#endif
}

TEST(sys_epoll, epoll_pwait2_64_with_sigset) {
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd);

#if defined(__BIONIC__)
  epoll_event events[1] = {};
  timespec ts = {.tv_nsec = 500};
  sigset64_t ss2;
  sigemptyset64(&ss2);
  sigaddset64(&ss2, SIGPIPE);
  int rc = epoll_pwait2_64(epoll_fd, events, 1, &ts, &ss2);
  if (rc == -1 && errno == ENOSYS) GTEST_SKIP() << "no epoll_pwait2() in this kernel";
  ASSERT_EQ(0, rc) << strerror(errno);
#else
  GTEST_SKIP() << "epoll_pwait2_64 is bionic-only";
#endif
}

TEST(sys_epoll, epoll_create_invalid_size) {
  errno = 0;
  ASSERT_EQ(-1, epoll_create(0));
  ASSERT_ERRNO(EINVAL);
}

TEST(sys_epoll, epoll_event_data) {
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd) << strerror(errno);

  int fds[2];
  ASSERT_NE(-1, pipe(fds));

  const uint64_t expected = 0x123456789abcdef0;

  // Get ready to poll on read end of pipe.
  epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.u64 = expected;
  ASSERT_NE(-1, epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fds[0], &ev));

  // Ensure there's something in the pipe.
  ASSERT_EQ(1, write(fds[1], "\n", 1));

  // Poll.
  epoll_event events[1];
  ASSERT_EQ(1, epoll_wait(epoll_fd, events, 1, 1));
  ASSERT_EQ(expected, events[0].data.u64);

  close(fds[0]);
  close(fds[1]);
}

TEST(sys_epoll, epoll_create1) {
  int fd;
  fd = epoll_create(1);
  ASSERT_FALSE(CloseOnExec(fd));
  close(fd);

  fd = epoll_create1(0);
  ASSERT_FALSE(CloseOnExec(fd));
  close(fd);

  fd = epoll_create1(EPOLL_CLOEXEC);
  ASSERT_TRUE(CloseOnExec(fd));
  close(fd);
}
