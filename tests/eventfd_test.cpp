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

#include <errno.h>
#include <fcntl.h>

#include <gtest/gtest.h>

#include <sys/eventfd.h>

#include "utils.h"

TEST(eventfd, smoke) {
  constexpr unsigned int kInitialValue = 2;
  int fd = eventfd(kInitialValue, EFD_NONBLOCK);
  ASSERT_NE(-1, fd);

  eventfd_t value = 123;
  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(kInitialValue, value);

  // Reading clears the counter.
  ASSERT_EQ(-1, eventfd_read(fd, &value));
  ASSERT_ERRNO(EAGAIN);

  // Values written are added until the next read.
  ASSERT_EQ(0, eventfd_write(fd, 1));
  ASSERT_EQ(0, eventfd_write(fd, 1));
  ASSERT_EQ(0, eventfd_write(fd, 1));

  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(3U, value);

  close(fd);
}

TEST(eventfd, cloexec) {
  constexpr unsigned int kInitialValue = 2;
  int fd = eventfd(kInitialValue, EFD_CLOEXEC);
  ASSERT_NE(-1, fd);
  ASSERT_TRUE(CloseOnExec(fd));

  eventfd_t value = 123;
  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(kInitialValue, value);

  close(fd);

  fd = eventfd(kInitialValue, EFD_NONBLOCK | EFD_CLOEXEC);
  ASSERT_NE(-1, fd);
  ASSERT_TRUE(CloseOnExec(fd));

  value = 123;
  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(kInitialValue, value);

  close(fd);
}

TEST(eventfd, semaphore) {
  int fd = eventfd(3, EFD_NONBLOCK | EFD_SEMAPHORE);
  ASSERT_NE(-1, fd);

  eventfd_t value = 123;
  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(1U, value);

  value = 123;
  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(1U, value);

  value = 123;
  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(1U, value);

  // The counter is cleared after the initial value decrements to 0.
  ASSERT_EQ(-1, eventfd_read(fd, &value));
  ASSERT_ERRNO(EAGAIN);

  close(fd);
}
