/*
 * Copyright (C) 2018 The Android Open Source Project
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

// membarrier(2) is only supported for bionic builds (b/111199492).
#if defined(__BIONIC__)

#include <linux/membarrier.h>
#include <sys/syscall.h>

class ScopedErrnoCleaner {
 public:
  ScopedErrnoCleaner() { errno = 0; }
  ~ScopedErrnoCleaner() { errno = 0; }
};

bool HasMembarrier() {
  ScopedErrnoCleaner errno_cleaner;
  bool present = syscall(__NR_membarrier, MEMBARRIER_CMD_QUERY, 0) > 0;
  return present;
}

TEST(membarrier, query) {
  ScopedErrnoCleaner errno_cleaner;
  int supported = syscall(__NR_membarrier, MEMBARRIER_CMD_QUERY, 0);
  if (errno == 0) {
    ASSERT_TRUE(supported >= 0);
  } else {
    ASSERT_TRUE(errno == ENOSYS && supported == -1);
  }
}

TEST(membarrier, global_barrier) {
  if (!HasMembarrier()) {
    return;
  }

  ScopedErrnoCleaner errno_cleaner;
  int supported = syscall(__NR_membarrier, MEMBARRIER_CMD_QUERY, 0);
  ASSERT_LE(0, supported);

  if ((supported & MEMBARRIER_CMD_GLOBAL) != 0) {
    ASSERT_EQ(0, syscall(__NR_membarrier, MEMBARRIER_CMD_GLOBAL, 0));
  }
}

static void TestRegisterAndBarrierCommands(int membarrier_cmd_register,
                                           int membarrier_cmd_barrier) {
  if (!HasMembarrier()) {
    return;
  }

  ScopedErrnoCleaner errno_cleaner;

  // Check barrier use without prior registration.
  if (membarrier_cmd_register == MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED) {
    // Global barrier use is always okay.
    ASSERT_EQ(0, syscall(__NR_membarrier, membarrier_cmd_barrier, 0));
  } else {
    // Private barrier should fail.
    ASSERT_EQ(-1, syscall(__NR_membarrier, membarrier_cmd_barrier, 0));
    ASSERT_EQ(EPERM, errno);
    errno = 0;
  }

  // Check registration for barrier succeeds.
  ASSERT_EQ(0, syscall(__NR_membarrier, membarrier_cmd_register, 0));

  // Check barrier use after registration succeeds.
  ASSERT_EQ(0, syscall(__NR_membarrier, membarrier_cmd_barrier, 0));
}

TEST(membarrier, global_expedited) {
  TestRegisterAndBarrierCommands(MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED,
                                 MEMBARRIER_CMD_GLOBAL_EXPEDITED);
}

TEST(membarrier, private_expedited) {
  TestRegisterAndBarrierCommands(MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED,
                                 MEMBARRIER_CMD_PRIVATE_EXPEDITED);
}

TEST(membarrier, private_expedited_sync_core) {
  TestRegisterAndBarrierCommands(MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_SYNC_CORE,
                                 MEMBARRIER_CMD_PRIVATE_EXPEDITED_SYNC_CORE);
}

#endif  // __BIONIC__
