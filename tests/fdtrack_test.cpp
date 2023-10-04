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
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__BIONIC__)
#include <sys/pidfd.h>

#include "platform/bionic/fdtrack.h"
#include "platform/bionic/reserved_signals.h"
#endif

#include <vector>

#include <android-base/cmsg.h>
#include <android-base/logging.h>
#include <android-base/unique_fd.h>

#include "utils.h"

using android::base::ReceiveFileDescriptors;
using android::base::SendFileDescriptors;
using android::base::unique_fd;

#if defined(__BIONIC__)
void DumpEvent(std::vector<android_fdtrack_event>* events, size_t index) {
  auto& event = (*events)[index];
  if (event.type == ANDROID_FDTRACK_EVENT_TYPE_CREATE) {
    fprintf(stderr, "  event %zu: fd %d created by %s\n", index, event.fd,
            event.data.create.function_name);
  } else if (event.type == ANDROID_FDTRACK_EVENT_TYPE_CLOSE) {
    fprintf(stderr, "  event %zu: fd %d closed\n", index, event.fd);
  } else {
    errx(1, "unexpected fdtrack event type: %d", event.type);
  }
}

std::vector<android_fdtrack_event> FdtrackRun(void (*func)(), bool reenable = true) {
  // Each bionic test is run in separate process, so we can safely use a static here.
  // However, since they're all forked, we need to reenable fdtrack.
  if (reenable) {
    android_fdtrack_set_globally_enabled(true);
  }

  static std::vector<android_fdtrack_event> events;
  events.clear();

  android_fdtrack_hook_t previous = nullptr;
  android_fdtrack_hook_t hook = [](android_fdtrack_event* event) { events.push_back(*event); };

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

  // Filter out temporary fds created and closed as a result of the call.
  // (e.g. accept creating a socket to tell netd about the newly accepted socket)
  size_t i = 0;
  while (i + 1 < events.size()) {
    auto& event = events[i];
    if (event.type == ANDROID_FDTRACK_EVENT_TYPE_CREATE) {
      for (size_t j = i + 1; j < events.size(); ++j) {
        if (event.fd == events[j].fd) {
          if (events[j].type == ANDROID_FDTRACK_EVENT_TYPE_CREATE) {
            fprintf(stderr, "error: multiple create events for the same fd:\n");
            DumpEvent(&events, i);
            DumpEvent(&events, j);
            exit(1);
          }

          events.erase(events.begin() + j);
          events.erase(events.begin() + i);
          continue;
        }
      }
    }
    ++i;
  }

  return std::move(events);
}

const char* FdtrackEventTypeToName(android_fdtrack_event_type event_type) {
  switch (event_type) {
    case ANDROID_FDTRACK_EVENT_TYPE_CREATE:
      return "created";
    case ANDROID_FDTRACK_EVENT_TYPE_CLOSE:
      return "closed";
  }
}
#endif

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

TEST(fdtrack, fork) {
#if defined(__BIONIC__)
  ASSERT_EXIT(
      []() {
        static int fd = open("/dev/null", O_WRONLY | O_CLOEXEC);
        ASSERT_NE(-1, fd);

        auto events = FdtrackRun([]() { close(fd); }, false);
        ASSERT_EQ(0U, events.size());
        exit(0);
      }(),
      testing::ExitedWithCode(0), "");
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

struct require_semicolon;

#if defined(__BIONIC__)
void SetFdResult(std::vector<int>* output, int fd) {
  output->push_back(fd);
}

void SetFdResult(std::vector<int>* output, std::vector<int> fds) {
  *output = fds;
}

#define FDTRACK_TEST_NAME(test_name, fdtrack_name, expression)                                   \
  TEST(fdtrack, test_name) {                                                                     \
    static std::vector<int> expected_fds;                                                        \
    auto events = FdtrackRun([]() { SetFdResult(&expected_fds, expression); });                  \
    for (auto& fd : expected_fds) {                                                              \
      ASSERT_NE(-1, fd) << strerror(errno);                                                      \
    }                                                                                            \
    if (events.size() != expected_fds.size()) {                                                  \
      fprintf(stderr, "too many events received: expected %zu, got %zu:\n", expected_fds.size(), \
              events.size());                                                                    \
      for (size_t i = 0; i < events.size(); ++i) {                                               \
        DumpEvent(&events, i);                                                                   \
      }                                                                                          \
      FAIL();                                                                                    \
      return;                                                                                    \
    }                                                                                            \
    for (auto& event : events) {                                                                 \
      ASSERT_NE(expected_fds.end(),                                                              \
                std::find(expected_fds.begin(), expected_fds.end(), events[0].fd));              \
      ASSERT_EQ(ANDROID_FDTRACK_EVENT_TYPE_CREATE, event.type);                                  \
      ASSERT_STREQ(fdtrack_name, event.data.create.function_name);                               \
    }                                                                                            \
  }                                                                                              \
  struct require_semicolon
#else
#define FDTRACK_TEST_NAME(name, fdtrack_name, expression) \
  TEST(fdtrack, name) {}                                  \
  struct require_semicolon
#endif

#define FDTRACK_TEST(name, expression) FDTRACK_TEST_NAME(name, #name, expression)

// clang-format misformats statement expressions pretty badly here:
// clang-format off
FDTRACK_TEST(open, open("/dev/null", O_WRONLY | O_CLOEXEC));
FDTRACK_TEST(openat, openat(AT_EMPTY_PATH, "/dev/null", O_WRONLY | O_CLOEXEC));
FDTRACK_TEST(socket, socket(AF_UNIX, SOCK_STREAM, 0));

FDTRACK_TEST(pidfd_open, ({
  int rc = pidfd_open(getpid(), 0);
  if (rc == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_open() in this kernel";
  ASSERT_NE(-1, rc) << strerror(errno);
  rc;
}));

FDTRACK_TEST(pidfd_getfd, ({
  android_fdtrack_set_enabled(false);
  int pidfd_self = pidfd_open(getpid(), 0);
  if (pidfd_self == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_open() in this kernel";
  ASSERT_NE(-1, pidfd_self) << strerror(errno);

  android_fdtrack_set_enabled(true);

  int rc = pidfd_getfd(pidfd_self, STDIN_FILENO, 0);
  if (rc == -1 && errno == ENOSYS) GTEST_SKIP() << "no pidfd_getfd() in this kernel";
  ASSERT_NE(-1, rc) << strerror(errno);

  android_fdtrack_set_enabled(false);
  close(pidfd_self);
  android_fdtrack_set_enabled(true);

  rc;
}));

FDTRACK_TEST(dup, dup(STDOUT_FILENO));
FDTRACK_TEST(dup2, dup2(STDOUT_FILENO, STDERR_FILENO));
FDTRACK_TEST(dup3, dup3(STDOUT_FILENO, STDERR_FILENO, 0));
FDTRACK_TEST_NAME(fcntl_F_DUPFD, "F_DUPFD", fcntl(STDOUT_FILENO, F_DUPFD, 0));
FDTRACK_TEST_NAME(fcntl_F_DUPFD_CLOEXEC, "F_DUPFD_CLOEXEC", fcntl(STDOUT_FILENO, F_DUPFD_CLOEXEC, 0));

FDTRACK_TEST(pipe, ({
  std::vector<int> fds = { -1, -1};
  if (pipe(fds.data()) != 0) {
    err(1, "pipe failed");
  }
  fds;
}));

FDTRACK_TEST(pipe2, ({
  std::vector<int> fds = { -1, -1};
  if (pipe2(fds.data(), O_CLOEXEC) != 0) {
    err(1, "pipe failed");
  }
  fds;
}));

FDTRACK_TEST(socketpair, ({
  std::vector<int> fds = { -1, -1};
  if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds.data()) != 0) {
    err(1, "socketpair failed");
  }
  fds;
}));

FDTRACK_TEST(epoll_create, epoll_create(1));
FDTRACK_TEST(epoll_create1, epoll_create1(0));

FDTRACK_TEST(eventfd, eventfd(0, 0));

#if defined(__BIONIC__)
static int CreateListener() {
  android_fdtrack_set_enabled(false);
  int listener = socket(AF_INET, SOCK_STREAM, 0);
  CHECK_NE(-1, listener);

  sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = 0,
      .sin_addr = {htonl(INADDR_LOOPBACK)},
  };
  socklen_t addrlen = sizeof(addr);

  CHECK_NE(-1, bind(listener, reinterpret_cast<sockaddr*>(&addr), addrlen)) << strerror(errno);
  CHECK_NE(-1, getsockname(listener, reinterpret_cast<sockaddr*>(&addr), &addrlen));
  CHECK_EQ(static_cast<size_t>(addrlen), sizeof(addr));
  CHECK_NE(-1, listen(listener, 1));

  int connector = socket(AF_INET, SOCK_STREAM, 0);
  CHECK_NE(-1, connector);
  CHECK_NE(-1, connect(connector, reinterpret_cast<sockaddr*>(&addr), addrlen));
  android_fdtrack_set_enabled(true);

  return listener;
}
#endif

FDTRACK_TEST_NAME(accept, "accept4", accept(CreateListener(), nullptr, nullptr));
FDTRACK_TEST(accept4, accept4(CreateListener(), nullptr, nullptr, 0));

FDTRACK_TEST(recvmsg, ({
  android_fdtrack_set_enabled(false);
  int sockets[2];
  ASSERT_NE(-1, socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sockets));
  ASSERT_EQ(3, SendFileDescriptors(sockets[0], "foo", 3, STDIN_FILENO));
  android_fdtrack_set_enabled(true);

  char buf[4];
  unique_fd received_fd;
  ASSERT_EQ(3, ReceiveFileDescriptors(sockets[1], buf, sizeof(buf), &received_fd));
  received_fd.release();
}));

FDTRACK_TEST_NAME(vfork, "open", ({
  int fd = open("/dev/null", O_RDONLY);

  pid_t rc = vfork();
  ASSERT_NE(-1, rc);

  if (rc == 0) {
    close(fd);
    _exit(0);
  }

  int status;
  pid_t wait_result = waitpid(rc, &status, 0);
  ASSERT_EQ(wait_result, rc);
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(0, WEXITSTATUS(status));

  fd;
}));
// clang-format on
