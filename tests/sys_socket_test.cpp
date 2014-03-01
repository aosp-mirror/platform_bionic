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
#include <sys/socket.h>

TEST(sys_socket, recvmmsg) {
#if !defined(__GLIBC__) // TODO: Android's prebuilt gcc is too old for recvmmsg/sendmmsg.
  ASSERT_EQ(-1, recvmmsg(-1, NULL, 0, 0, NULL));
  ASSERT_EQ(EBADF, errno);
#endif
}

TEST(sys_socket, sendmmsg) {
#if !defined(__GLIBC__) // TODO: Android's prebuilt gcc is too old for recvmmsg/sendmmsg.
  ASSERT_EQ(-1, sendmmsg(-1, NULL, 0, 0));
  ASSERT_EQ(EBADF, errno);
#endif
}
