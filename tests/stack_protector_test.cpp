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
#include "BionicDeathTest.h"

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <set>

#include "private/bionic_tls.h"

extern "C" pid_t gettid(); // glibc defines this but doesn't declare it anywhere.

#if defined(__BIONIC__)
extern uintptr_t __stack_chk_guard;
#endif

struct stack_protector_checker {
  std::set<pid_t> tids;
  std::set<void*> guards;

  void Check() {
    pid_t tid = gettid();
    void* guard = __get_tls()[TLS_SLOT_STACK_GUARD];

    printf("[thread %d] TLS stack guard = %p\n", tid, guard);

    // Duplicate tid. gettid(2) bug? Seeing this would be very upsetting.
    ASSERT_TRUE(tids.find(tid) == tids.end());

    // Uninitialized guard. Our bug. Note this is potentially flaky; we _could_
    // get four random zero bytes, but it should be vanishingly unlikely.
    ASSERT_NE(guard, nullptr);

#if defined(__BIONIC__)
    // bionic always has the global too.
    ASSERT_EQ(__stack_chk_guard, reinterpret_cast<uintptr_t>(guard));
#endif

    tids.insert(tid);
    guards.insert(guard);
  }
};

TEST(stack_protector, same_guard_per_thread) {
  // Everyone has the TLS slot set, even if their stack protector
  // implementation doesn't yet use it.
  stack_protector_checker checker;

  // Check the main thread.
  ASSERT_EQ(getpid(), gettid()); // We are the main thread, right?
  checker.Check();

  size_t thread_count = 9;
  for (size_t i = 1; i < thread_count; ++i) {
    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, nullptr, [](void* arg) -> void* {
      stack_protector_checker* checker = reinterpret_cast<stack_protector_checker*>(arg);
      checker->Check();
      return nullptr;
    }, &checker));
    void* result;
    ASSERT_EQ(0, pthread_join(t, &result));
    ASSERT_EQ(nullptr, result);
  }
  ASSERT_EQ(thread_count, checker.tids.size());

  // Both bionic and glibc use the same guard for every thread.
  ASSERT_EQ(1U, checker.guards.size());
}

TEST(stack_protector, global_guard) {
#if defined(__BIONIC__)
  // Bionic always has a global, even if it's using TLS.
  ASSERT_NE(0, gettid());
  ASSERT_NE(0U, __stack_chk_guard);
#else
  GTEST_SKIP() << "glibc doesn't have a global __stack_chk_guard";
#endif
}

class stack_protector_DeathTest : public BionicDeathTest {};

TEST_F(stack_protector_DeathTest, modify_stack_protector) {
  // In another file to prevent inlining, which removes stack protection.
  extern void modify_stack_protector_test();
#if __has_feature(hwaddress_sanitizer)
  ASSERT_EXIT(modify_stack_protector_test(),
              testing::KilledBySignal(SIGABRT), "tag-mismatch");
#else
  ASSERT_EXIT(modify_stack_protector_test(),
              testing::KilledBySignal(SIGABRT), "stack corruption detected");
#endif
}
