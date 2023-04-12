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

#include <semaphore.h>

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include <android-base/silent_death_test.h>
#include <gtest/gtest.h>

#include "SignalUtils.h"
#include "private/bionic_constants.h"

using semaphore_DeathTest = SilentDeathTest;

TEST(semaphore, sem_init) {
  sem_t s;

  // Perfectly fine initial values.
  ASSERT_EQ(0, sem_init(&s, 0, 0));
  ASSERT_EQ(0, sem_init(&s, 0, 1));
  ASSERT_EQ(0, sem_init(&s, 0, 123));

  // Too small an initial value.
  errno = 0;
  ASSERT_EQ(-1, sem_init(&s, 0, -1));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(SEM_VALUE_MAX, sysconf(_SC_SEM_VALUE_MAX));

  // The largest initial value.
  ASSERT_EQ(0, sem_init(&s, 0, SEM_VALUE_MAX));

  // Too large an initial value.
  errno = 0;
  ASSERT_EQ(-1, sem_init(&s, 0, SEM_VALUE_MAX + 1));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(0, sem_destroy(&s));
}

TEST(semaphore, sem_trywait) {
  sem_t s;
  ASSERT_EQ(0, sem_init(&s, 0, 3));
  ASSERT_EQ(0, sem_trywait(&s));
  ASSERT_EQ(0, sem_trywait(&s));
  ASSERT_EQ(0, sem_trywait(&s));
  errno = 0;
  ASSERT_EQ(-1, sem_trywait(&s));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(0, sem_destroy(&s));
}

static void SemWaitThreadTestFn(sem_t& sem) {
  ASSERT_EQ(0, sem_wait(&sem));
}

static void* SemWaitThreadFn(void* arg) {
  SemWaitThreadTestFn(*reinterpret_cast<sem_t*>(arg));
  return nullptr;
}

TEST(semaphore, sem_wait__sem_post) {
  sem_t s;
  ASSERT_EQ(0, sem_init(&s, 0, 0));

  pthread_t t1, t2, t3;
  ASSERT_EQ(0, pthread_create(&t1, nullptr, SemWaitThreadFn, &s));
  ASSERT_EQ(0, pthread_create(&t2, nullptr, SemWaitThreadFn, &s));
  ASSERT_EQ(0, pthread_create(&t3, nullptr, SemWaitThreadFn, &s));

  ASSERT_EQ(0, sem_post(&s));
  ASSERT_EQ(0, sem_post(&s));
  ASSERT_EQ(0, sem_post(&s));

  void* result;
  ASSERT_EQ(0, pthread_join(t1, &result));
  ASSERT_EQ(0, pthread_join(t2, &result));
  ASSERT_EQ(0, pthread_join(t3, &result));
}

static inline void timespec_add_ms(timespec& ts, size_t ms) {
  ts.tv_sec  += ms / 1000;
  ts.tv_nsec += (ms % 1000) * 1000000;
  if (ts.tv_nsec >= NS_PER_S) {
    ts.tv_sec++;
    ts.tv_nsec -= NS_PER_S;
  }
}

static void sem_timedwait_helper(clockid_t clock,
                                 int (*wait_function)(sem_t* __sem, const timespec* __ts)) {
  sem_t s;
  ASSERT_EQ(0, sem_init(&s, 0, 0));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(clock, &ts));
  timespec_add_ms(ts, 100);

  errno = 0;
  ASSERT_EQ(-1, wait_function(&s, &ts));
  ASSERT_EQ(ETIMEDOUT, errno);

  // A negative timeout is an error.
  errno = 0;
  ts.tv_nsec = -1;
  ASSERT_EQ(-1, wait_function(&s, &ts));
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(-1, wait_function(&s, &ts));
  ASSERT_EQ(EINVAL, errno);

  errno = 0;
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(-1, wait_function(&s, &ts));
  ASSERT_EQ(ETIMEDOUT, errno);

  ASSERT_EQ(0, sem_destroy(&s));
}

TEST(semaphore, sem_timedwait) {
  sem_timedwait_helper(CLOCK_REALTIME, sem_timedwait);
}

TEST(semaphore, sem_timedwait_monotonic_np) {
#if defined(__BIONIC__)
  sem_timedwait_helper(CLOCK_MONOTONIC, sem_timedwait_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "sem_timedwait_monotonic_np is only supported on bionic";
#endif  // __BIONIC__
}

TEST(semaphore, sem_clockwait) {
#if defined(__BIONIC__)
  sem_timedwait_helper(CLOCK_MONOTONIC, [](sem_t* __sem, const timespec* __ts) {
    return sem_clockwait(__sem, CLOCK_MONOTONIC, __ts);
  });
  sem_timedwait_helper(CLOCK_REALTIME, [](sem_t* __sem, const timespec* __ts) {
    return sem_clockwait(__sem, CLOCK_REALTIME, __ts);
  });
#else   // __BIONIC__
  GTEST_SKIP() << "sem_clockwait is only supported on bionic";
#endif  // __BIONIC__
}

TEST_F(semaphore_DeathTest, sem_timedwait_null_timeout) {
  sem_t s;
  ASSERT_EQ(0, sem_init(&s, 0, 0));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  ASSERT_EXIT(sem_timedwait(&s, nullptr), testing::KilledBySignal(SIGSEGV), "");
#pragma clang diagnostic pop
}

TEST(semaphore, sem_getvalue) {
  sem_t s;
  ASSERT_EQ(0, sem_init(&s, 0, 0));

  int i;
  ASSERT_EQ(0, sem_getvalue(&s, &i));
  ASSERT_EQ(0, i);

  ASSERT_EQ(0, sem_post(&s));
  ASSERT_EQ(0, sem_getvalue(&s, &i));
  ASSERT_EQ(1, i);

  ASSERT_EQ(0, sem_post(&s));
  ASSERT_EQ(0, sem_getvalue(&s, &i));
  ASSERT_EQ(2, i);

  ASSERT_EQ(0, sem_wait(&s));
  ASSERT_EQ(0, sem_getvalue(&s, &i));
  ASSERT_EQ(1, i);
}

extern "C" void android_set_application_target_sdk_version(int target);

static void sem_wait_test_signal_handler(int) {
}

static void* SemWaitEINTRThreadFn(void* arg) {
  sem_t* sem = reinterpret_cast<sem_t*>(arg);
  uintptr_t have_eintr = 0;
  uintptr_t have_error = 0;
  while (true) {
    int result = sem_wait(sem);
    if (result == 0) {
      break;
    }
    if (result == -1) {
      if (errno == EINTR) {
        have_eintr = 1;
      } else {
        have_error = 1;
        break;
      }
    }
  }
  return reinterpret_cast<void*>((have_eintr << 1) | have_error);
}

TEST(semaphore, sem_wait_no_EINTR_in_sdk_less_equal_than_23) {
#if defined(__BIONIC__)
  android_set_application_target_sdk_version(23);
  sem_t s;
  ASSERT_EQ(0, sem_init(&s, 0, 0));
  ScopedSignalHandler handler(SIGUSR1, sem_wait_test_signal_handler);
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr, SemWaitEINTRThreadFn, &s));
  // Give some time for the thread to run sem_wait.
  usleep(500000);
  ASSERT_EQ(0, pthread_kill(thread, SIGUSR1));
  // Give some time for the thread to handle signal.
  usleep(500000);
  ASSERT_EQ(0, sem_post(&s));
  void* result;
  ASSERT_EQ(0, pthread_join(thread, &result));
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(result));
#else
  GTEST_SKIP() << "This test tests sem_wait's compatibility for old sdk versions";
#endif
}

TEST(semaphore, sem_wait_EINTR_in_sdk_greater_than_23) {
#if defined(__BIONIC__)
  android_set_application_target_sdk_version(24U);
#endif
  sem_t s;
  ASSERT_EQ(0, sem_init(&s, 0, 0));
  ScopedSignalHandler handler(SIGUSR1, sem_wait_test_signal_handler);
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr, SemWaitEINTRThreadFn, &s));
  // Give some time for the thread to run sem_wait.
  usleep(500000);
  ASSERT_EQ(0, pthread_kill(thread, SIGUSR1));
  // Give some time for the thread to handle signal.
  usleep(500000);
  ASSERT_EQ(0, sem_post(&s));
  void* result;
  ASSERT_EQ(0, pthread_join(thread, &result));
  ASSERT_EQ(2U, reinterpret_cast<uintptr_t>(result));
}
