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

#if defined(__GLIBC__)
// glibc doesn't expose gettid(2).
pid_t gettid() { return syscall(__NR_gettid); }
#endif // __GLIBC__

// For x86, bionic and glibc have per-thread stack guard values (all identical).
#if defined(__i386__)
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
#endif // __i386__

TEST(stack_protector, same_guard_per_thread) {
#if defined(__i386__)
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

  // bionic and glibc use the same guard for every thread.
  ASSERT_EQ(1U, checker.guards.size());
#else // __i386__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __i386__
}

// For ARM and MIPS, glibc has a global stack check guard value.
#if defined(__BIONIC__) || defined(__arm__) || defined(__mips__)
#define TEST_STACK_CHK_GUARD

// Bionic has the global for x86 too, to support binaries that can run on
// Android releases that didn't implement the TLS guard value.
extern "C" uintptr_t __stack_chk_guard;

/*
 * When this function returns, the stack canary will be inconsistent
 * with the previous value, which will generate a call to __stack_chk_fail(),
 * eventually resulting in a SIGABRT.
 *
 * This must be marked with "__attribute__ ((noinline))", to ensure the
 * compiler generates the proper stack guards around this function.
 */
__attribute__ ((noinline))
static void do_modify_stack_chk_guard() {
  __stack_chk_guard = 0x12345678;
}
#endif

TEST(stack_protector, global_guard) {
#if defined(TEST_STACK_CHK_GUARD)
  ASSERT_NE(0, gettid());
  ASSERT_NE(0U, __stack_chk_guard);
#else // TEST_STACK_CHK_GUARD
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // TEST_STACK_CHK_GUARD
}

TEST(stack_protector_DeathTest, modify_stack_protector) {
#if defined(TEST_STACK_CHK_GUARD)
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(do_modify_stack_chk_guard(), testing::KilledBySignal(SIGABRT), "");
#else // TEST_STACK_CHK_GUARD
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // TEST_STACK_CHK_GUARD
}
