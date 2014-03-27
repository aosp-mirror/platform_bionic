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

/*
 * Contributed by: Intel Corporation, 2014
 */

#include <gtest/gtest.h>

#if defined(__BIONIC__)
  #define SOCK_CLOEXEC_SUPPORTED 1
#elif defined(__GLIBC_PREREQ)
  #if __GLIBC_PREREQ(2, 9)
    #define SOCK_CLOEXEC_SUPPORTED 1
  #endif
#endif

#if defined(SOCK_CLOEXEC_SUPPORTED)

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_PATH "test"

static void* ConnectFn(void*) {
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
    close(fd);
    return reinterpret_cast<void*>(-1);
  }

  close(fd);

  return NULL;
}
#endif

TEST(accept4, smoke) {
#if defined(SOCK_CLOEXEC_SUPPORTED)
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
  ASSERT_EQ(0, pthread_create(&thread, NULL, ConnectFn, NULL));

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd, &read_set);
  timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  ASSERT_LT(0, select(fd+1, &read_set, NULL, NULL, &tv));

  socklen_t len = sizeof(addr);
  int fd_acc = accept4(fd, reinterpret_cast<struct sockaddr*>(&addr), &len, SOCK_CLOEXEC);
  ASSERT_NE(fd_acc, -1) << strerror(errno);

  // Check that the flag was set properly.
  ASSERT_EQ(FD_CLOEXEC, fcntl(fd_acc, F_GETFD) & FD_CLOEXEC);

  void* ret_val;
  pthread_join(thread, &ret_val);
  ASSERT_EQ(NULL, ret_val);

  close(fd_acc);
  close(fd);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}
