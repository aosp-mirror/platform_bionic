/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__BIONIC__)
#include <android/fdtrack.h>
#endif

#include <vector>

#include <android-base/unique_fd.h>

#if defined(__BIONIC__)
std::vector<android_fdtrack_event> FdtrackRun(void (*func)()) {
  // Each bionic test is run in separate process, so we can safely use a static here.
  static std::vector<android_fdtrack_event> events;
  events.clear();

  android_fdtrack_hook_t previous = nullptr;
  android_fdtrack_hook_t hook = [](android_fdtrack_event* event) {
    events.push_back(*event);
  };

  if (!android_fdtrack_compare_exchange_hook(&previous, hook)) {
    errx(1, "failed to exchange hook: previous hook was %p", previous);
  }

  if (previous) {
    errx(1, "hook was already registered?");
    abort();
  }

  func();

  if (!android_fdtrack_compare_exchange_hook(&hook, nullptr)) {
    errx(1, "failed to reset hook");
  }

  return std::move(events);
}
#endif

TEST(fdtrack, open) {
#if defined(__BIONIC__)
  static int fd = -1;
  auto events = FdtrackRun([]() { fd = open("/dev/null", O_WRONLY | O_CLOEXEC); });
  ASSERT_NE(-1, fd);
  ASSERT_EQ(1U, events.size());
  ASSERT_EQ(fd, events[0].fd);
  ASSERT_EQ(ANDROID_FDTRACK_EVENT_TYPE_CREATE, events[0].type);
  ASSERT_STREQ("open", events[0].data.create.function_name);
#endif
}

TEST(fdtrack, close) {
#if defined(__BIONIC__)
  static int fd = open("/dev/null", O_WRONLY | O_CLOEXEC);
  ASSERT_NE(-1, fd);

  auto events = FdtrackRun([]() { close(fd); });
  ASSERT_EQ(1U, events.size());
  ASSERT_EQ(fd, events[0].fd);
  ASSERT_EQ(ANDROID_FDTRACK_EVENT_TYPE_CLOSE, events[0].type);
#endif
}

TEST(fdtrack, enable_disable) {
#if defined(__BIONIC__)
  static int fd1 = -1;
  static int fd2 = -1;
  static int fd3 = -1;

  auto events = FdtrackRun([]() {
    if (!android_fdtrack_get_enabled()) {
      errx(1, "fdtrack is disabled");
    }
    fd1 = open("/dev/null", O_WRONLY | O_CLOEXEC);
    android_fdtrack_set_enabled(false);
    fd2 = open("/dev/null", O_WRONLY | O_CLOEXEC);
    android_fdtrack_set_enabled(true);
    fd3 = open("/dev/null", O_WRONLY | O_CLOEXEC);
  });

  if (fd1 == -1 || fd2 == -1 || fd3 == -1) {
    errx(1, "failed to open /dev/null");
  }

  ASSERT_EQ(2U, events.size());

  ASSERT_EQ(fd1, events[0].fd);
  ASSERT_EQ(ANDROID_FDTRACK_EVENT_TYPE_CREATE, events[0].type);
  ASSERT_STREQ("open", events[0].data.create.function_name);

  ASSERT_EQ(fd3, events[1].fd);
  ASSERT_EQ(ANDROID_FDTRACK_EVENT_TYPE_CREATE, events[1].type);
  ASSERT_STREQ("open", events[1].data.create.function_name);
#endif
}
