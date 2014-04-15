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

#if defined(__BIONIC__)
  #define ACCEPT4_SUPPORTED 1
  #define RECVMMSG_SUPPORTED 1
  #define SENDMMSG_SUPPORTED 1
#elif defined(__GLIBC_PREREQ)
  #if __GLIBC_PREREQ(2, 9)
    #define ACCEPT4_SUPPORTED 1
  #endif
  #if __GLIBC_PREREQ(2, 12)
    #define RECVMMSG_SUPPORTED 1
  #endif
  #if __GLIBC_PREREQ(2, 14)
    #define SENDMMSG_SUPPORTED 1
  #endif
#endif

#if defined(ACCEPT4_SUPPORTED) || defined(RECVMMSG_SUPPORTED) || defined(SENDMMSG_SUPPORTED)

#define SOCK_PATH "test"

static void* ConnectFn(void* data) {
  bool (*callback_fn)(int) = reinterpret_cast<bool (*)(int)>(data);
  void* return_value = NULL;

  int fd = socket(PF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
  if (fd < 0) {
    GTEST_LOG_(ERROR) << "socket call failed: " << strerror(errno);
    return reinterpret_cast<void*>(-1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  addr.sun_path[0] = '\0';
  strcpy(addr.sun_path + 1, SOCK_PATH);

  if (connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
    GTEST_LOG_(ERROR) << "connect call failed: " << strerror(errno);
    return_value = reinterpret_cast<void*>(-1);
  }
  else if (callback_fn != NULL && !callback_fn(fd)) {
    return_value = reinterpret_cast<void*>(-1);
  }

  close(fd);

  return return_value;
}

static void RunTest(void (*test_fn)(struct sockaddr_un*, int),
                    bool (*callback_fn)(int fd)) {
  int fd = socket(PF_UNIX, SOCK_SEQPACKET, 0);
  ASSERT_NE(fd, -1) << strerror(errno);

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  addr.sun_path[0] = '\0';
  strcpy(addr.sun_path + 1, SOCK_PATH);

  ASSERT_NE(-1, bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr))) << strerror(errno);

  ASSERT_NE(-1, listen(fd, 1)) << strerror(errno);

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, ConnectFn, reinterpret_cast<void*>(callback_fn)));

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd, &read_set);
  timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd+1, &read_set, NULL, NULL, &tv));

  test_fn(&addr, fd);

  void* ret_val;
  ASSERT_EQ(0, pthread_join(thread, &ret_val));
  ASSERT_EQ(NULL, ret_val);

  close(fd);
}
#endif

TEST(sys_socket, accept4_error) {
#if defined(ACCEPT4_SUPPORTED)
  ASSERT_EQ(-1, accept4(-1, NULL, NULL, 0));
  ASSERT_EQ(EBADF, errno);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

#if defined(ACCEPT4_SUPPORTED)
static void TestAccept4(struct sockaddr_un* addr, int fd) {
  socklen_t len = sizeof(*addr);
  int fd_acc = accept4(fd, reinterpret_cast<struct sockaddr*>(addr), &len, SOCK_CLOEXEC);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  // Check that the flag was set properly.
  ASSERT_EQ(FD_CLOEXEC, fcntl(fd_acc, F_GETFD) & FD_CLOEXEC);

  close(fd_acc);
}
#endif

TEST(sys_socket, accept4_smoke) {
#if defined(ACCEPT4_SUPPORTED)
  RunTest(TestAccept4, NULL);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

#if defined(RECVMMSG_SUPPORTED)
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
#endif

TEST(sys_socket, recvmmsg_smoke) {
#if defined(RECVMMSG_SUPPORTED)
  RunTest(TestRecvMMsg, SendMultiple);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

TEST(sys_socket, recvmmsg_error) {
#if defined(RECVMMSG_SUPPORTED)
  ASSERT_EQ(-1, recvmmsg(-1, NULL, 0, 0, NULL));
  ASSERT_EQ(EBADF, errno);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

#if defined(SENDMMSG_SUPPORTED)
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
    ASSERT_LT(0, select(fd_acc+1, &read_set, NULL, NULL, &tv));
    char buffer[100];
    ASSERT_EQ(strlen(g_SendMsgs[i]) + 1,
              static_cast<size_t>(recv(fd_acc, buffer, sizeof(buffer), 0)));
    ASSERT_STREQ(g_SendMsgs[i], buffer);
  }

  close(fd_acc);
}
#endif

TEST(sys_socket, sendmmsg_smoke) {
#if defined(SENDMMSG_SUPPORTED)
  RunTest(TestSendMMsg, SendMMsg);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

TEST(sys_socket, sendmmsg_error) {
#if defined(SENDMMSG_SUPPORTED)
  ASSERT_EQ(-1, sendmmsg(-1, NULL, 0, 0));
  ASSERT_EQ(EBADF, errno);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}
