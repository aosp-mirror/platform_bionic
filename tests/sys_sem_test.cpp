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
#include <sys/sem.h>

#include <android-base/file.h>

TEST(sys_sem, smoke) {
  if (semctl(-1, 0, IPC_RMID) == -1 && errno == ENOSYS) {
    GTEST_SKIP() << "no <sys/sem.h> support in this kernel";
  }

  // Create a semaphore.
  TemporaryDir dir;
  key_t key = ftok(dir.path, 1);
  int id = semget(key, 1, IPC_CREAT|0666);
  ASSERT_NE(id, -1);

  // Check semaphore info.
  semid_ds ds;
  memset(&ds, 0, sizeof(ds));
  ASSERT_EQ(0, semctl(id, 0, IPC_STAT, &ds));
  ASSERT_EQ(1U, ds.sem_nsems);

  ASSERT_EQ(0, semctl(id, 0, GETVAL));

  // Increment.
  sembuf ops[] = {{ .sem_num = 0, .sem_op = 1, .sem_flg = 0 }};
  ASSERT_EQ(0, semop(id, ops, 1));
  ASSERT_EQ(1, semctl(id, 0, GETVAL));

  // Test timeouts.
  timespec ts = { .tv_sec = 0, .tv_nsec = 100 };
  ops[0] = { .sem_num = 0, .sem_op = 0, .sem_flg = 0 };
  errno = 0;
  ASSERT_EQ(-1, semtimedop(id, ops, 1, &ts));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(1, semctl(id, 0, GETVAL));

  // Decrement.
  ops[0] = { .sem_num = 0, .sem_op = -1, .sem_flg = 0 };
  ASSERT_EQ(0, semop(id, ops, 1));
  ASSERT_EQ(0, semctl(id, 0, GETVAL));

  // Destroy the semaphore.
  ASSERT_EQ(0, semctl(id, 0, IPC_RMID));
}

TEST(sys_sem, semget_failure) {
  errno = 0;
  ASSERT_EQ(-1, semget(-1, -1, 0));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
}

TEST(sys_sem, semctl_failure) {
  errno = 0;
  ASSERT_EQ(-1, semctl(-1, 0, IPC_RMID));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
}

TEST(sys_sem, semop_failure) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  errno = 0;
  ASSERT_EQ(-1, semop(-1, nullptr, 0));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
#pragma clang diagnostic pop
}

TEST(sys_sem, semtimedop_failure) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  errno = 0;
  ASSERT_EQ(-1, semtimedop(-1, nullptr, 0, nullptr));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
#pragma clang diagnostic pop
}

TEST(sys_sem, union_semun) {
  // https://github.com/android-ndk/ndk/issues/400
#if defined(__BIONIC__)
  semun arg;
  semid_ds i1;
  seminfo i2;
  unsigned short a[] = { 1u, 2u };
  arg.val = 123;
  arg.buf = &i1;
  arg.array = a;
  arg.__buf = &i2;
#else
  // glibc already mostly removed this cruft (although it's still in <linux/sem.h>).
#endif
}
