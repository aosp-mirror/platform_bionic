/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#include "utils.h"

struct ConnectData {
  bool (*callback_fn)(int);
  const char* sock_path;
  ConnectData(bool (*callback_func)(int), const char* socket_path)
   : callback_fn(callback_func), sock_path(socket_path) {}
};

static void* ConnectFn(void* data) {
  ConnectData* pdata = reinterpret_cast<ConnectData*>(data);
  bool (*callback_fn)(int) = pdata->callback_fn;
  void* return_value = nullptr;

  int fd = socket(PF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
  if (fd < 0) {
    GTEST_LOG_(ERROR) << "socket call failed: " << strerror(errno);
    return reinterpret_cast<void*>(-1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  addr.sun_path[0] = '\0';
  strcpy(addr.sun_path + 1, pdata->sock_path);

  if (connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
    GTEST_LOG_(ERROR) << "connect call failed: " << strerror(errno);
    return_value = reinterpret_cast<void*>(-1);
  }
  else if (callback_fn != nullptr && !callback_fn(fd)) {
    return_value = reinterpret_cast<void*>(-1);
  }

  close(fd);

  return return_value;
}

static void RunTest(void (*test_fn)(struct sockaddr_un*, int),
                    bool (*callback_fn)(int fd), const char* sock_path) {
  int fd = socket(PF_UNIX, SOCK_SEQPACKET, 0);
  ASSERT_NE(fd, -1) << strerror(errno);

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  addr.sun_path[0] = '\0';
  strcpy(addr.sun_path + 1, sock_path);

  ASSERT_NE(-1, bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr))) << strerror(errno);

  ASSERT_NE(-1, listen(fd, 1)) << strerror(errno);

  ConnectData connect_data(callback_fn, sock_path);

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr, ConnectFn, &connect_data));

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd, &read_set);
  timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd+1, &read_set, nullptr, nullptr, &tv));

  test_fn(&addr, fd);

  void* ret_val;
  ASSERT_EQ(0, pthread_join(thread, &ret_val));
  ASSERT_EQ(nullptr, ret_val);

  close(fd);
}

TEST(sys_socket, accept4_error) {
  ASSERT_EQ(-1, accept4(-1, nullptr, nullptr, 0));
  ASSERT_EQ(EBADF, errno);
}

static void TestAccept4(struct sockaddr_un* addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept4(fd, reinterpret_cast<struct sockaddr*>(addr), &len, SOCK_CLOEXEC);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  // Check that SOCK_CLOEXEC was set properly.
  AssertCloseOnExec(fd_acc, true);

  close(fd_acc);
}

TEST(sys_socket, accept4_smoke) {
  RunTest(TestAccept4, nullptr, "test_accept");
}

const char* g_RecvMsgs[] = {
  "RECVMMSG_ONE",
  "RECVMMSG_TWO",
  "RECVMMSG_THREE",
};
#define NUM_RECV_MSGS (sizeof(g_RecvMsgs)/sizeof(const char*))

static bool SendMultiple(int fd) {
  for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
    if (send(fd, g_RecvMsgs[i], strlen(g_RecvMsgs[i]) + 1, 0) < 0) {
      GTEST_LOG_(ERROR) << "send call failed: " << strerror(errno);
      return false;
    }
  }

  return true;
}

static void TestRecvMMsg(struct sockaddr_un *addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept(fd, reinterpret_cast<struct sockaddr*>(addr), &len);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  struct mmsghdr msgs[NUM_RECV_MSGS];
  memset(msgs, 0, sizeof(struct mmsghdr)*NUM_RECV_MSGS);

  struct iovec io[NUM_RECV_MSGS];
  char bufs[NUM_RECV_MSGS][100];
  for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
    io[i].iov_base = reinterpret_cast<void*>(bufs[i]);
    io[i].iov_len = strlen(g_RecvMsgs[i]) + 1;

    msgs[i].msg_hdr.msg_iov = &io[i];
    msgs[i].msg_hdr.msg_iovlen = 1;
    msgs[i].msg_len = sizeof(struct msghdr);
  }

  struct timespec ts;
  memset(&ts, 0, sizeof(ts));
  ts.tv_sec = 5;
  ts.tv_nsec = 0;
  ASSERT_EQ(NUM_RECV_MSGS,
            static_cast<size_t>(recvmmsg(fd_acc, msgs, NUM_RECV_MSGS, 0, &ts)))
           << strerror(errno);
  for (size_t i = 0; i < NUM_RECV_MSGS; i++) {
    ASSERT_STREQ(g_RecvMsgs[i], bufs[i]);
  }

  close(fd_acc);
}

TEST(sys_socket, recvmmsg_smoke) {
  RunTest(TestRecvMMsg, SendMultiple, "test_revmmsg");
}

TEST(sys_socket, recvmmsg_error) {
  ASSERT_EQ(-1, recvmmsg(-1, nullptr, 0, 0, nullptr));
  ASSERT_EQ(EBADF, errno);
}

const char* g_SendMsgs[] = {
  "MSG_ONE",
  "MSG_TWO",
  "MSG_THREE"
};
#define NUM_SEND_MSGS (sizeof(g_SendMsgs)/sizeof(const char*))

static bool SendMMsg(int fd) {
  struct mmsghdr msgs[NUM_SEND_MSGS];
  memset(msgs, 0, sizeof(struct mmsghdr)*NUM_SEND_MSGS);
  struct iovec io[NUM_SEND_MSGS];
  for (size_t i = 0; i < NUM_SEND_MSGS; i++) {
    io[i].iov_base = reinterpret_cast<void*>(const_cast<char*>(g_SendMsgs[i]));
    io[i].iov_len = strlen(g_SendMsgs[i]) + 1;
    msgs[i].msg_hdr.msg_iov = &io[i];
    msgs[i].msg_hdr.msg_iovlen = 1;
    msgs[i].msg_len = sizeof(struct msghdr);
  }

  if (sendmmsg(fd, msgs, NUM_SEND_MSGS, 0) < 0) {
    GTEST_LOG_(ERROR) << "sendmmsg call failed: " << strerror(errno);
    return false;
  }
  return true;
}

static void TestSendMMsg(struct sockaddr_un *addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept(fd, reinterpret_cast<struct sockaddr*>(addr), &len);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd_acc, &read_set);

  for (size_t i = 0; i < NUM_SEND_MSGS; i++) {
    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    ASSERT_LT(0, select(fd_acc+1, &read_set, nullptr, nullptr, &tv));
    char buffer[100];
    ASSERT_EQ(strlen(g_SendMsgs[i]) + 1,
              static_cast<size_t>(recv(fd_acc, buffer, sizeof(buffer), 0)));
    ASSERT_STREQ(g_SendMsgs[i], buffer);
  }

  close(fd_acc);
}

TEST(sys_socket, sendmmsg_smoke) {
  RunTest(TestSendMMsg, SendMMsg, "test_sendmmsg");
}

TEST(sys_socket, sendmmsg_error) {
  ASSERT_EQ(-1, sendmmsg(-1, nullptr, 0, 0));
  ASSERT_EQ(EBADF, errno);
}
