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

#if !defined(__GLIBC__) // Android's prebuilt gcc's header files don't include <sys/eventfd.h>.

#include <sys/eventfd.h>

TEST(eventfd, smoke) {
  unsigned int initial_value = 2;
  int fd = eventfd(initial_value, O_NONBLOCK);
  ASSERT_NE(fd, -1);

  eventfd_t value = 123;
  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(initial_value, value);

  // Reading clears the counter.
  ASSERT_EQ(-1, eventfd_read(fd, &value));
  ASSERT_EQ(EAGAIN, errno);

  // Values written are added until the next read.
  ASSERT_EQ(0, eventfd_write(fd, 1));
  ASSERT_EQ(0, eventfd_write(fd, 1));
  ASSERT_EQ(0, eventfd_write(fd, 1));

  ASSERT_EQ(0, eventfd_read(fd, &value));
  ASSERT_EQ(3U, value);

  close(fd);
}

#endif
