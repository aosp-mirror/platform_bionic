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

#include <errno.h>
#include <sys/shm.h>

#include <android-base/file.h>
#include <gtest/gtest.h>

TEST(sys_shm, smoke) {
  if (shmctl(-1, IPC_STAT, nullptr) == -1 && errno == ENOSYS) {
    GTEST_LOG_(INFO) << "no <sys/shm.h> support in this kernel\n";
    return;
  }

  // Create a segment.
  TemporaryDir dir;
  key_t key = ftok(dir.path, 1);
  int id = shmget(key, 1234, IPC_CREAT|0666);
  ASSERT_NE(id, -1);

  // Check segment info.
  shmid_ds ds;
  memset(&ds, 0, sizeof(ds));
  ASSERT_EQ(0, shmctl(id, IPC_STAT, &ds));
  ASSERT_EQ(1234U, ds.shm_segsz);

  // Attach.
  void* p = shmat(id, nullptr, SHM_RDONLY);
  ASSERT_NE(p, nullptr);

  // Detach.
  ASSERT_EQ(0, shmdt(p));

  // Destroy the segment.
  ASSERT_EQ(0, shmctl(id, IPC_RMID, nullptr));
}

TEST(sys_shm, shmat_failure) {
  errno = 0;
  ASSERT_EQ(reinterpret_cast<void*>(-1), shmat(-1, nullptr, SHM_RDONLY));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
}

TEST(sys_shm, shmctl_failure) {
  errno = 0;
  ASSERT_EQ(-1, shmctl(-1, IPC_STAT, nullptr));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
}

TEST(sys_shm, shmdt_failure) {
  errno = 0;
  ASSERT_EQ(-1, shmdt(nullptr));
  ASSERT_TRUE(errno == EINVAL || errno == ENOSYS);
}

TEST(sys_shm, shmget_failure) {
  errno = 0;
  ASSERT_EQ(-1, shmget(-1, 1234, 0));
  ASSERT_TRUE(errno == ENOENT || errno == ENOSYS);
}
