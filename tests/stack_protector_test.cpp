/*
 * Copyright (C) 2012 The Android Open Source Project
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
 * Contributed by: Intel Corporation
 */

#include <gtest/gtest.h>

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <set>

#ifdef __GLIBC__

// glibc doesn't expose gettid(2).
pid_t gettid() { return syscall(__NR_gettid); }

#endif

#ifdef __i386__

// For x86, bionic and glibc have per-thread stack guard values.

static uint32_t GetGuardFromTls() {
  uint32_t guard;
  asm ("mov %%gs:0x14, %0": "=d" (guard));
  return guard;
}

struct stack_protector_checker {
  std::set<pid_t> tids;
  std::set<uint32_t> guards;

  void Check() {
    pid_t tid = gettid();
    uint32_t guard = GetGuardFromTls();

    printf("[thread %d] %%gs:0x14 = 0x%08x\n", tid, guard);

    // Duplicate tid. gettid(2) bug? Seeing this would be very upsetting.
    ASSERT_TRUE(tids.find(tid) == tids.end());
#ifdef __GLIBC__
    // glibc uses the same guard for every thread. bionic uses a different guard for each one.
#else
    // Duplicate guard. Our bug. Note this is potentially flaky; we _could_ get the
    // same guard for two threads, but it should be vanishingly unlikely.
    ASSERT_TRUE(guards.find(guard) == guards.end());
#endif
    // Uninitialized guard. Our bug. Note this is potentially flaky; we _could_ get
    // four random zero bytes, but it should be vanishingly unlikely.
    ASSERT_NE(guard, 0U);

    tids.insert(tid);
    guards.insert(guard);
  }
};

static void* ThreadGuardHelper(void* arg) {
  stack_protector_checker* checker = reinterpret_cast<stack_protector_checker*>(arg);
  checker->Check();
  return NULL;
}

TEST(stack_protector, guard_per_thread) {
  stack_protector_checker checker;
  size_t thread_count = 10;
  for (size_t i = 0; i < thread_count; ++i) {
    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, NULL, ThreadGuardHelper, &checker));
    void* result;
    ASSERT_EQ(0, pthread_join(t, &result));
    ASSERT_EQ(NULL, result);
  }
  ASSERT_EQ(thread_count, checker.tids.size());

  // glibc uses the same guard for every thread. bionic uses a different guard for each one.
#ifdef __BIONIC__
  ASSERT_EQ(thread_count, checker.guards.size());
#else
  ASSERT_EQ(1U, checker.guards.size());
#endif
}

#endif

#if defined(__BIONIC__) || defined(__arm__) || defined(__mips__)

// For ARM and MIPS, glibc has a global stack check guard value.

// Bionic has the global for x86 too, to support binaries that can run on
// Android releases that didn't implement the TLS guard value.

extern "C" void* __stack_chk_guard;

TEST(stack_protector, global_guard) {
  ASSERT_NE(0, gettid());
  ASSERT_NE(0U, reinterpret_cast<uintptr_t>(__stack_chk_guard));
}

#endif
