/*
 * Copyright (C) 2015 The Android Open Source Project
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
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>

#include "private/ScopeGuard.h"

extern "C" pid_t gettid();

static void ProcSelfReadlinkBody() {
  char buf[100];
  char buf2[1024];
  int fd = open("/dev/null", O_RDWR | O_CLOEXEC);
  ASSERT_NE(-1, fd);
  snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd);
  const char* ERRORMSG = "Please apply the following two kernel patches:\n"
    "* https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=73af963f9f3036dffed55c3a2898598186db1045\n"
    "* https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=96d0df79f2644fc823f26c06491e182d87a90c2a\n";
  ASSERT_NE(-1, readlink(buf, buf2, sizeof(buf2))) << ERRORMSG;
  ASSERT_STREQ("/dev/null", buf2);
  close(fd);
}

static void* ProcSelfReadlink(void*) {
  ProcSelfReadlinkBody();
  return NULL;
}

TEST(bug_26110743, ProcSelfReadlink) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, ProcSelfReadlink, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(NULL, result);
}

TEST(bug_26110743, ProcSelfReadlink_NotDumpable) {
  int dumpable = prctl(PR_GET_DUMPABLE, 0, 0, 0, 0);
  prctl(PR_SET_DUMPABLE, 0, 0, 0, 0);
  auto guard = make_scope_guard([&]() {
    // restore dumpable
    prctl(PR_SET_DUMPABLE, dumpable, 0, 0, 0);
  });

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, ProcSelfReadlink, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(NULL, result);
}

static void ProcTaskFdReadlinkBody() {
  char buf[200];
  char buf2[1024];
  int fd = open("/dev/null", O_RDWR | O_CLOEXEC);
  ASSERT_NE(-1, fd);
  pid_t mypid = getpid();
  pid_t mytid = gettid();
  ASSERT_NE(mypid, mytid);
  snprintf(buf, sizeof(buf), "/proc/%d/task/%d/fd/%d", mypid, mytid, fd);
  const char* ERRORMSG = "Please apply the following kernel patch:\n"
    "* https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=54708d2858e79a2bdda10bf8a20c80eb96c20613\n";
  ASSERT_NE(-1, readlink(buf, buf2, sizeof(buf2))) << ERRORMSG;
  ASSERT_STREQ("/dev/null", buf2);
  close(fd);
}

static void* ProcTaskFdReadlink(void*) {
  ProcTaskFdReadlinkBody();
  return NULL;
}

TEST(bug_26110743, ProcTaskFdReadlink) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, ProcTaskFdReadlink, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(NULL, result);
}

TEST(bug_26110743, ProcTaskFdReadlink_NotDumpable) {
  int dumpable = prctl(PR_GET_DUMPABLE, 0, 0, 0, 0);
  prctl(PR_SET_DUMPABLE, 0, 0, 0, 0);
  auto guard = make_scope_guard([&]() {
    // restore dumpable
    prctl(PR_SET_DUMPABLE, dumpable, 0, 0, 0);
  });

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, ProcTaskFdReadlink, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(NULL, result);
}
