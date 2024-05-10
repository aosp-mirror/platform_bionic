/*
 * Copyright (C) 2021 The Android Open Source Project
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
#include <sys/types.h>
#include <unistd.h>

#if defined(__BIONIC__)
#include <sys/pidfd.h>
#endif

#include <android-base/silent_death_test.h>
#include <android-base/unique_fd.h>

#include "utils.h"

using android::base::unique_fd;
using namespace std::chrono_literals;

using pidfd_DeathTest = SilentDeathTest;

TEST(pidfd, pidfd_open) {
#if defined(__BIONIC__)
  pid_t child = fork();
  ASSERT_NE(-1, child);
  if (child == 0) {
    _exit(42);
  }

  unique_fd pidfd(pidfd_open(child, 0));
  if (pidfd.get() == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_open() in this kernel";
  ASSERT_NE(-1, pidfd.get()) << strerror(errno);

  siginfo_t siginfo;
  int rc = waitid(P_PIDFD, pidfd.get(), &siginfo, WEXITED);
  if (rc == -1) {
    ASSERT_ERRNO(EINVAL);
    GTEST_SKIP() << "P_PIDFD not available";
  }

  ASSERT_EQ(child, siginfo.si_pid);
#endif
}

TEST(pidfd, pidfd_getfd) {
#if defined(__BIONIC__)
  unique_fd r, w;
  ASSERT_TRUE(android::base::Pipe(&r, &w));
  unique_fd self(pidfd_open(getpid(), 0));
  if (self.get() == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_open() in this kernel";
  ASSERT_NE(-1, self.get()) << strerror(errno);

  unique_fd dup(pidfd_getfd(self.get(), r.get(), 0));
  if (dup.get() == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_getfd() in this kernel";
  ASSERT_NE(-1, dup.get()) << strerror(errno);

  ASSERT_NE(r.get(), dup.get());
  ASSERT_EQ(3, write(w.get(), "foo", 3));
  char buf[4];
  ASSERT_EQ(3, read(dup.get(), buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "foo", 3));
#endif
}

TEST_F(pidfd_DeathTest, pidfd_send_signal) {
#if defined(__BIONIC__)
  unique_fd self(pidfd_open(getpid(), 0));
  if (self.get() == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_open() in this kernel";
  ASSERT_NE(-1, self.get()) << strerror(errno);

  int rc = pidfd_send_signal(self.get(), 0, nullptr, 0);
  if (rc == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_send_signal() in this kernel";
  ASSERT_EQ(0, rc) << strerror(errno);

  ASSERT_EXIT(({
                // gtest will fork a child off for ASSERT_EXIT: `self` refers to the parent.
                unique_fd child(pidfd_open(getpid(), 0));
                pidfd_send_signal(child.get(), SIGINT, nullptr, 0);
              }),
              testing::KilledBySignal(SIGINT), "");

#endif
}
