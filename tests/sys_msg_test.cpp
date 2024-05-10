/*
 * Copyright (C) 2016 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <gtest/gtest.h>

#include <errno.h>
#include <sys/msg.h>

#include <android-base/file.h>

TEST(sys_msg, smoke) {
  if (msgctl(-1, IPC_STAT, nullptr) == -1 && errno == ENOSYS) {
    GTEST_SKIP() << "no <sys/msg.h> support in this kernel";
  }

  // Create a queue.
  TemporaryDir dir;
  key_t key = ftok(dir.path, 1);
  int id = msgget(key, IPC_CREAT|0666);
  ASSERT_NE(id, -1);

  // Queue should be empty.
  msqid_ds ds;
  memset(&ds, 0, sizeof(ds));
  ASSERT_EQ(0, msgctl(id, IPC_STAT, &ds));
  ASSERT_EQ(0U, ds.msg_qnum);
  ASSERT_EQ(0U, ds.msg_cbytes);

  // Send a message.
  struct {
    long type;
    char data[32];
  } msg = { 1, "hello world" };
  ASSERT_EQ(0, msgsnd(id, &msg, sizeof(msg.data), 0));

  // Queue should be non-empty.
  ASSERT_EQ(0, msgctl(id, IPC_STAT, &ds));
  ASSERT_EQ(1U, ds.msg_qnum);
  ASSERT_EQ(sizeof(msg.data), ds.msg_cbytes);

  // Read the message.
  memset(&msg, 0, sizeof(msg));
  ASSERT_EQ(static_cast<ssize_t>(sizeof(msg.data)),
            msgrcv(id, &msg, sizeof(msg.data), 0, 0));
  ASSERT_EQ(1, msg.type);
  ASSERT_STREQ("hello world", msg.data);

  // Destroy the queue.
  ASSERT_EQ(0, msgctl(id, IPC_RMID, nullptr));
}

TEST(sys_msg, msgctl_failure) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  errno = 0;
  ASSERT_EQ(-1, msgctl(-1, IPC_STAT, nullptr));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
#pragma clang diagnostic pop
}

TEST(sys_msg, msgget_failure) {
  errno = 0;
  ASSERT_EQ(-1, msgget(-1, 0));
  ASSERT_TRUE(errno == ENOENT || errno == ENOSYS);
}

TEST(sys_msg, msgrcv_failure) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  errno = 0;
  ASSERT_EQ(-1, msgrcv(-1, nullptr, 0, 0, 0));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
#pragma clang diagnostic pop
}

TEST(sys_msg, msgsnd_failure) {
  struct {
    long type;
    char data[1];
  } msg = { 1, "" };
  errno = 0;
  ASSERT_EQ(-1, msgsnd(-1, &msg, sizeof(msg.data), 0));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
}
