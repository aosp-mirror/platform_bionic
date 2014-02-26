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
#include <sys/epoll.h>

TEST(sys_epoll, smoke) {
  int epoll_fd = epoll_create(1);
  ASSERT_NE(-1, epoll_fd) << strerror(errno);
  epoll_event events[1];

  // Regular epoll_wait.
  ASSERT_EQ(0, epoll_wait(epoll_fd, events, 1, 1));

  // epoll_pwait without a sigset (which is equivalent to epoll_wait).
  ASSERT_EQ(0, epoll_pwait(epoll_fd, events, 1, 1, NULL));

  // epoll_pwait with a sigset.
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, SIGPIPE);
  ASSERT_EQ(0, epoll_pwait(epoll_fd, events, 1, 1, &ss));
}
