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

#include <gtest/gtest.h>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <malloc.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include <unwind.h>

#include <atomic>
#include <vector>

#include "private/bionic_constants.h"
#include "private/bionic_macros.h"
#include "private/ScopeGuard.h"
#include "BionicDeathTest.h"
#include "ScopedSignalHandler.h"
#include "utils.h"

TEST(pthread, pthread_key_create) {
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, NULL));
  ASSERT_EQ(0, pthread_key_delete(key));
  // Can't delete a key that's already been deleted.
  ASSERT_EQ(EINVAL, pthread_key_delete(key));
}

TEST(pthread, pthread_keys_max) {
  // POSIX says PTHREAD_KEYS_MAX should be at least _POSIX_THREAD_KEYS_MAX.
  ASSERT_GE(PTHREAD_KEYS_MAX, _POSIX_THREAD_KEYS_MAX);
}

TEST(pthread, sysconf_SC_THREAD_KEYS_MAX_eq_PTHREAD_KEYS_MAX) {
  int sysconf_max = sysconf(_SC_THREAD_KEYS_MAX);
  ASSERT_EQ(sysconf_max, PTHREAD_KEYS_MAX);
}

TEST(pthread, pthread_key_many_distinct) {
  // As gtest uses pthread keys, we can't allocate exactly PTHREAD_KEYS_MAX
  // pthread keys, but We should be able to allocate at least this many keys.
  int nkeys = PTHREAD_KEYS_MAX / 2;
  std::vector<pthread_key_t> keys;

  auto scope_guard = make_scope_guard([&keys]{
    for (const auto& key : keys) {
      EXPECT_EQ(0, pthread_key_delete(key));
    }
  });

  for (int i = 0; i < nkeys; ++i) {
    pthread_key_t key;
    // If this fails, it's likely that LIBC_PTHREAD_KEY_RESERVED_COUNT is wrong.
    ASSERT_EQ(0, pthread_key_create(&key, NULL)) << i << " of " << nkeys;
    keys.push_back(key);
    ASSERT_EQ(0, pthread_setspecific(key, reinterpret_cast<void*>(i)));
  }

  for (int i = keys.size() - 1; i >= 0; --i) {
    ASSERT_EQ(reinterpret_cast<void*>(i), pthread_getspecific(keys.back()));
    pthread_key_t key = keys.back();
    keys.pop_back();
    ASSERT_EQ(0, pthread_key_delete(key));
  }
}

TEST(pthread, pthread_key_not_exceed_PTHREAD_KEYS_MAX) {
  std::vector<pthread_key_t> keys;
  int rv = 0;

  // Pthread keys are used by gtest, so PTHREAD_KEYS_MAX should
  // be more than we are allowed to allocate now.
  for (int i = 0; i < PTHREAD_KEYS_MAX; i++) {
    pthread_key_t key;
    rv = pthread_key_create(&key, NULL);
    if (rv == EAGAIN) {
      break;
    }
    EXPECT_EQ(0, rv);
    keys.push_back(key);
  }

  // Don't leak keys.
  for (const auto& key : keys) {
    EXPECT_EQ(0, pthread_key_delete(key));
  }
  keys.clear();

  // We should have eventually reached the maximum number of keys and received
  // EAGAIN.
  ASSERT_EQ(EAGAIN, rv);
}

TEST(pthread, pthread_key_delete) {
  void* expected = reinterpret_cast<void*>(1234);
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, NULL));
  ASSERT_EQ(0, pthread_setspecific(key, expected));
  ASSERT_EQ(expected, pthread_getspecific(key));
  ASSERT_EQ(0, pthread_key_delete(key));
  // After deletion, pthread_getspecific returns NULL.
  ASSERT_EQ(NULL, pthread_getspecific(key));
  // And you can't use pthread_setspecific with the deleted key.
  ASSERT_EQ(EINVAL, pthread_setspecific(key, expected));
}

TEST(pthread, pthread_key_fork) {
  void* expected = reinterpret_cast<void*>(1234);
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, NULL));
  ASSERT_EQ(0, pthread_setspecific(key, expected));
  ASSERT_EQ(expected, pthread_getspecific(key));

  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    // The surviving thread inherits all the forking thread's TLS values...
    ASSERT_EQ(expected, pthread_getspecific(key));
    _exit(99);
  }

  AssertChildExited(pid, 99);

  ASSERT_EQ(expected, pthread_getspecific(key));
  ASSERT_EQ(0, pthread_key_delete(key));
}

static void* DirtyKeyFn(void* key) {
  return pthread_getspecific(*reinterpret_cast<pthread_key_t*>(key));
}

TEST(pthread, pthread_key_dirty) {
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, NULL));

  size_t stack_size = 640 * 1024;
  void* stack = mmap(NULL, stack_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  ASSERT_NE(MAP_FAILED, stack);
  memset(stack, 0xff, stack_size);

  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setstack(&attr, stack, stack_size));

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &attr, DirtyKeyFn, &key));

  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(nullptr, result); // Not ~0!

  ASSERT_EQ(0, munmap(stack, stack_size));
  ASSERT_EQ(0, pthread_key_delete(key));
}

TEST(pthread, static_pthread_key_used_before_creation) {
#if defined(__BIONIC__)
  // See http://b/19625804. The bug is about a static/global pthread key being used before creation.
  // So here tests if the static/global default value 0 can be detected as invalid key.
  static pthread_key_t key;
  ASSERT_EQ(nullptr, pthread_getspecific(key));
  ASSERT_EQ(EINVAL, pthread_setspecific(key, nullptr));
  ASSERT_EQ(EINVAL, pthread_key_delete(key));
#else
  GTEST_LOG_(INFO) << "This test tests bionic pthread key implementation detail.\n";
#endif
}

static void* IdFn(void* arg) {
  return arg;
}

class SpinFunctionHelper {
 public:
  SpinFunctionHelper() {
    SpinFunctionHelper::spin_flag_ = true;
  }
  ~SpinFunctionHelper() {
    UnSpin();
  }
  auto GetFunction() -> void* (*)(void*) {
    return SpinFunctionHelper::SpinFn;
  }

  void UnSpin() {
    SpinFunctionHelper::spin_flag_ = false;
  }

 private:
  static void* SpinFn(void*) {
    while (spin_flag_) {}
    return NULL;
  }
  static std::atomic<bool> spin_flag_;
};

// It doesn't matter if spin_flag_ is used in several tests,
// because it is always set to false after each test. Each thread
// loops on spin_flag_ can find it becomes false at some time.
std::atomic<bool> SpinFunctionHelper::spin_flag_;

static void* JoinFn(void* arg) {
  return reinterpret_cast<void*>(pthread_join(reinterpret_cast<pthread_t>(arg), NULL));
}

static void AssertDetached(pthread_t t, bool is_detached) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_getattr_np(t, &attr));
  int detach_state;
  ASSERT_EQ(0, pthread_attr_getdetachstate(&attr, &detach_state));
  pthread_attr_destroy(&attr);
  ASSERT_EQ(is_detached, (detach_state == PTHREAD_CREATE_DETACHED));
}

static void MakeDeadThread(pthread_t& t) {
  ASSERT_EQ(0, pthread_create(&t, NULL, IdFn, NULL));
  ASSERT_EQ(0, pthread_join(t, NULL));
}

TEST(pthread, pthread_create) {
  void* expected_result = reinterpret_cast<void*>(123);
  // Can we create a thread?
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, IdFn, expected_result));
  // If we join, do we get the expected value back?
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(expected_result, result);
}

TEST(pthread, pthread_create_EAGAIN) {
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));
  ASSERT_EQ(0, pthread_attr_setstacksize(&attributes, static_cast<size_t>(-1) & ~(getpagesize() - 1)));

  pthread_t t;
  ASSERT_EQ(EAGAIN, pthread_create(&t, &attributes, IdFn, NULL));
}

TEST(pthread, pthread_no_join_after_detach) {
  SpinFunctionHelper spin_helper;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, spin_helper.GetFunction(), NULL));

  // After a pthread_detach...
  ASSERT_EQ(0, pthread_detach(t1));
  AssertDetached(t1, true);

  // ...pthread_join should fail.
  ASSERT_EQ(EINVAL, pthread_join(t1, NULL));
}

TEST(pthread, pthread_no_op_detach_after_join) {
  SpinFunctionHelper spin_helper;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, spin_helper.GetFunction(), NULL));

  // If thread 2 is already waiting to join thread 1...
  pthread_t t2;
  ASSERT_EQ(0, pthread_create(&t2, NULL, JoinFn, reinterpret_cast<void*>(t1)));

  sleep(1); // (Give t2 a chance to call pthread_join.)

#if defined(__BIONIC__)
  ASSERT_EQ(EINVAL, pthread_detach(t1));
#else
  ASSERT_EQ(0, pthread_detach(t1));
#endif
  AssertDetached(t1, false);

  spin_helper.UnSpin();

  // ...but t2's join on t1 still goes ahead (which we can tell because our join on t2 finishes).
  void* join_result;
  ASSERT_EQ(0, pthread_join(t2, &join_result));
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(join_result));
}

TEST(pthread, pthread_join_self) {
  ASSERT_EQ(EDEADLK, pthread_join(pthread_self(), NULL));
}

struct TestBug37410 {
  pthread_t main_thread;
  pthread_mutex_t mutex;

  static void main() {
    TestBug37410 data;
    data.main_thread = pthread_self();
    ASSERT_EQ(0, pthread_mutex_init(&data.mutex, NULL));
    ASSERT_EQ(0, pthread_mutex_lock(&data.mutex));

    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, NULL, TestBug37410::thread_fn, reinterpret_cast<void*>(&data)));

    // Wait for the thread to be running...
    ASSERT_EQ(0, pthread_mutex_lock(&data.mutex));
    ASSERT_EQ(0, pthread_mutex_unlock(&data.mutex));

    // ...and exit.
    pthread_exit(NULL);
  }

 private:
  static void* thread_fn(void* arg) {
    TestBug37410* data = reinterpret_cast<TestBug37410*>(arg);

    // Let the main thread know we're running.
    pthread_mutex_unlock(&data->mutex);

    // And wait for the main thread to exit.
    pthread_join(data->main_thread, NULL);

    return NULL;
  }
};

// Even though this isn't really a death test, we have to say "DeathTest" here so gtest knows to
// run this test (which exits normally) in its own process.

class pthread_DeathTest : public BionicDeathTest {};

TEST_F(pthread_DeathTest, pthread_bug_37410) {
  // http://code.google.com/p/android/issues/detail?id=37410
  ASSERT_EXIT(TestBug37410::main(), ::testing::ExitedWithCode(0), "");
}

static void* SignalHandlerFn(void* arg) {
  sigset_t wait_set;
  sigfillset(&wait_set);
  return reinterpret_cast<void*>(sigwait(&wait_set, reinterpret_cast<int*>(arg)));
}

TEST(pthread, pthread_sigmask) {
  // Check that SIGUSR1 isn't blocked.
  sigset_t original_set;
  sigemptyset(&original_set);
  ASSERT_EQ(0, pthread_sigmask(SIG_BLOCK, NULL, &original_set));
  ASSERT_FALSE(sigismember(&original_set, SIGUSR1));

  // Block SIGUSR1.
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  ASSERT_EQ(0, pthread_sigmask(SIG_BLOCK, &set, NULL));

  // Check that SIGUSR1 is blocked.
  sigset_t final_set;
  sigemptyset(&final_set);
  ASSERT_EQ(0, pthread_sigmask(SIG_BLOCK, NULL, &final_set));
  ASSERT_TRUE(sigismember(&final_set, SIGUSR1));
  // ...and that sigprocmask agrees with pthread_sigmask.
  sigemptyset(&final_set);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, NULL, &final_set));
  ASSERT_TRUE(sigismember(&final_set, SIGUSR1));

  // Spawn a thread that calls sigwait and tells us what it received.
  pthread_t signal_thread;
  int received_signal = -1;
  ASSERT_EQ(0, pthread_create(&signal_thread, NULL, SignalHandlerFn, &received_signal));

  // Send that thread SIGUSR1.
  pthread_kill(signal_thread, SIGUSR1);

  // See what it got.
  void* join_result;
  ASSERT_EQ(0, pthread_join(signal_thread, &join_result));
  ASSERT_EQ(SIGUSR1, received_signal);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(join_result));

  // Restore the original signal mask.
  ASSERT_EQ(0, pthread_sigmask(SIG_SETMASK, &original_set, NULL));
}

static void test_pthread_setname_np__pthread_getname_np(pthread_t t) {
  ASSERT_EQ(0, pthread_setname_np(t, "short"));
  char name[32];
  ASSERT_EQ(0, pthread_getname_np(t, name, sizeof(name)));
  ASSERT_STREQ("short", name);

  // The limit is 15 characters --- the kernel's buffer is 16, but includes a NUL.
  ASSERT_EQ(0, pthread_setname_np(t, "123456789012345"));
  ASSERT_EQ(0, pthread_getname_np(t, name, sizeof(name)));
  ASSERT_STREQ("123456789012345", name);

  ASSERT_EQ(ERANGE, pthread_setname_np(t, "1234567890123456"));

  // The passed-in buffer should be at least 16 bytes.
  ASSERT_EQ(0, pthread_getname_np(t, name, 16));
  ASSERT_EQ(ERANGE, pthread_getname_np(t, name, 15));
}

TEST(pthread, pthread_setname_np__pthread_getname_np__self) {
  test_pthread_setname_np__pthread_getname_np(pthread_self());
}

TEST(pthread, pthread_setname_np__pthread_getname_np__other) {
  SpinFunctionHelper spin_helper;

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, spin_helper.GetFunction(), nullptr));
  test_pthread_setname_np__pthread_getname_np(t);
  spin_helper.UnSpin();
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

// http://b/28051133: a kernel misfeature means that you can't change the
// name of another thread if you've set PR_SET_DUMPABLE to 0.
TEST(pthread, pthread_setname_np__pthread_getname_np__other_PR_SET_DUMPABLE) {
  ASSERT_EQ(0, prctl(PR_SET_DUMPABLE, 0)) << strerror(errno);

  SpinFunctionHelper spin_helper;

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, spin_helper.GetFunction(), nullptr));
  test_pthread_setname_np__pthread_getname_np(t);
  spin_helper.UnSpin();
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

TEST_F(pthread_DeathTest, pthread_setname_np__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_setname_np(dead_thread, "short 3"), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_setname_np__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ENOENT, pthread_setname_np(null_thread, "short 3"));
}

TEST_F(pthread_DeathTest, pthread_getname_np__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  char name[64];
  EXPECT_DEATH(pthread_getname_np(dead_thread, name, sizeof(name)), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_getname_np__null_thread) {
  pthread_t null_thread = 0;

  char name[64];
  EXPECT_EQ(ENOENT, pthread_getname_np(null_thread, name, sizeof(name)));
}

TEST(pthread, pthread_kill__0) {
  // Signal 0 just tests that the thread exists, so it's safe to call on ourselves.
  ASSERT_EQ(0, pthread_kill(pthread_self(), 0));
}

TEST(pthread, pthread_kill__invalid_signal) {
  ASSERT_EQ(EINVAL, pthread_kill(pthread_self(), -1));
}

static void pthread_kill__in_signal_handler_helper(int signal_number) {
  static int count = 0;
  ASSERT_EQ(SIGALRM, signal_number);
  if (++count == 1) {
    // Can we call pthread_kill from a signal handler?
    ASSERT_EQ(0, pthread_kill(pthread_self(), SIGALRM));
  }
}

TEST(pthread, pthread_kill__in_signal_handler) {
  ScopedSignalHandler ssh(SIGALRM, pthread_kill__in_signal_handler_helper);
  ASSERT_EQ(0, pthread_kill(pthread_self(), SIGALRM));
}

TEST_F(pthread_DeathTest, pthread_detach__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_detach(dead_thread), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_detach__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ESRCH, pthread_detach(null_thread));
}

TEST(pthread, pthread_getcpuclockid__clock_gettime) {
  SpinFunctionHelper spin_helper;

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, spin_helper.GetFunction(), NULL));

  clockid_t c;
  ASSERT_EQ(0, pthread_getcpuclockid(t, &c));
  timespec ts;
  ASSERT_EQ(0, clock_gettime(c, &ts));
  spin_helper.UnSpin();
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

TEST_F(pthread_DeathTest, pthread_getcpuclockid__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  clockid_t c;
  EXPECT_DEATH(pthread_getcpuclockid(dead_thread, &c), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_getcpuclockid__null_thread) {
  pthread_t null_thread = 0;
  clockid_t c;
  EXPECT_EQ(ESRCH, pthread_getcpuclockid(null_thread, &c));
}

TEST_F(pthread_DeathTest, pthread_getschedparam__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  int policy;
  sched_param param;
  EXPECT_DEATH(pthread_getschedparam(dead_thread, &policy, &param), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_getschedparam__null_thread) {
  pthread_t null_thread = 0;
  int policy;
  sched_param param;
  EXPECT_EQ(ESRCH, pthread_getschedparam(null_thread, &policy, &param));
}

TEST_F(pthread_DeathTest, pthread_setschedparam__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  int policy = 0;
  sched_param param;
  EXPECT_DEATH(pthread_setschedparam(dead_thread, policy, &param), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_setschedparam__null_thread) {
  pthread_t null_thread = 0;
  int policy = 0;
  sched_param param;
  EXPECT_EQ(ESRCH, pthread_setschedparam(null_thread, policy, &param));
}

TEST_F(pthread_DeathTest, pthread_join__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_join(dead_thread, NULL), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_join__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ESRCH, pthread_join(null_thread, NULL));
}

TEST_F(pthread_DeathTest, pthread_kill__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_kill(dead_thread, 0), "invalid pthread_t");
}

TEST_F(pthread_DeathTest, pthread_kill__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ESRCH, pthread_kill(null_thread, 0));
}

TEST(pthread, pthread_join__multijoin) {
  SpinFunctionHelper spin_helper;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, spin_helper.GetFunction(), NULL));

  pthread_t t2;
  ASSERT_EQ(0, pthread_create(&t2, NULL, JoinFn, reinterpret_cast<void*>(t1)));

  sleep(1); // (Give t2 a chance to call pthread_join.)

  // Multiple joins to the same thread should fail.
  ASSERT_EQ(EINVAL, pthread_join(t1, NULL));

  spin_helper.UnSpin();

  // ...but t2's join on t1 still goes ahead (which we can tell because our join on t2 finishes).
  void* join_result;
  ASSERT_EQ(0, pthread_join(t2, &join_result));
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(join_result));
}

TEST(pthread, pthread_join__race) {
  // http://b/11693195 --- pthread_join could return before the thread had actually exited.
  // If the joiner unmapped the thread's stack, that could lead to SIGSEGV in the thread.
  for (size_t i = 0; i < 1024; ++i) {
    size_t stack_size = 640*1024;
    void* stack = mmap(NULL, stack_size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

    pthread_attr_t a;
    pthread_attr_init(&a);
    pthread_attr_setstack(&a, stack, stack_size);

    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, &a, IdFn, NULL));
    ASSERT_EQ(0, pthread_join(t, NULL));
    ASSERT_EQ(0, munmap(stack, stack_size));
  }
}

static void* GetActualGuardSizeFn(void* arg) {
  pthread_attr_t attributes;
  pthread_getattr_np(pthread_self(), &attributes);
  pthread_attr_getguardsize(&attributes, reinterpret_cast<size_t*>(arg));
  return NULL;
}

static size_t GetActualGuardSize(const pthread_attr_t& attributes) {
  size_t result;
  pthread_t t;
  pthread_create(&t, &attributes, GetActualGuardSizeFn, &result);
  pthread_join(t, NULL);
  return result;
}

static void* GetActualStackSizeFn(void* arg) {
  pthread_attr_t attributes;
  pthread_getattr_np(pthread_self(), &attributes);
  pthread_attr_getstacksize(&attributes, reinterpret_cast<size_t*>(arg));
  return NULL;
}

static size_t GetActualStackSize(const pthread_attr_t& attributes) {
  size_t result;
  pthread_t t;
  pthread_create(&t, &attributes, GetActualStackSizeFn, &result);
  pthread_join(t, NULL);
  return result;
}

TEST(pthread, pthread_attr_setguardsize) {
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));

  // Get the default guard size.
  size_t default_guard_size;
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &default_guard_size));

  // No such thing as too small: will be rounded up to one page by pthread_create.
  ASSERT_EQ(0, pthread_attr_setguardsize(&attributes, 128));
  size_t guard_size;
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(128U, guard_size);
  ASSERT_EQ(4096U, GetActualGuardSize(attributes));

  // Large enough and a multiple of the page size.
  ASSERT_EQ(0, pthread_attr_setguardsize(&attributes, 32*1024));
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(32*1024U, guard_size);

  // Large enough but not a multiple of the page size; will be rounded up by pthread_create.
  ASSERT_EQ(0, pthread_attr_setguardsize(&attributes, 32*1024 + 1));
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(32*1024U + 1, guard_size);
}

TEST(pthread, pthread_attr_setstacksize) {
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));

  // Get the default stack size.
  size_t default_stack_size;
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &default_stack_size));

  // Too small.
  ASSERT_EQ(EINVAL, pthread_attr_setstacksize(&attributes, 128));
  size_t stack_size;
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size));
  ASSERT_EQ(default_stack_size, stack_size);
  ASSERT_GE(GetActualStackSize(attributes), default_stack_size);

  // Large enough and a multiple of the page size; may be rounded up by pthread_create.
  ASSERT_EQ(0, pthread_attr_setstacksize(&attributes, 32*1024));
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size));
  ASSERT_EQ(32*1024U, stack_size);
  ASSERT_GE(GetActualStackSize(attributes), 32*1024U);

  // Large enough but not aligned; will be rounded up by pthread_create.
  ASSERT_EQ(0, pthread_attr_setstacksize(&attributes, 32*1024 + 1));
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size));
  ASSERT_EQ(32*1024U + 1, stack_size);
#if defined(__BIONIC__)
  ASSERT_GT(GetActualStackSize(attributes), 32*1024U + 1);
#else // __BIONIC__
  // glibc rounds down, in violation of POSIX. They document this in their BUGS section.
  ASSERT_EQ(GetActualStackSize(attributes), 32*1024U);
#endif // __BIONIC__
}

TEST(pthread, pthread_rwlockattr_smoke) {
  pthread_rwlockattr_t attr;
  ASSERT_EQ(0, pthread_rwlockattr_init(&attr));

  int pshared_value_array[] = {PTHREAD_PROCESS_PRIVATE, PTHREAD_PROCESS_SHARED};
  for (size_t i = 0; i < sizeof(pshared_value_array) / sizeof(pshared_value_array[0]); ++i) {
    ASSERT_EQ(0, pthread_rwlockattr_setpshared(&attr, pshared_value_array[i]));
    int pshared;
    ASSERT_EQ(0, pthread_rwlockattr_getpshared(&attr, &pshared));
    ASSERT_EQ(pshared_value_array[i], pshared);
  }

  int kind_array[] = {PTHREAD_RWLOCK_PREFER_READER_NP,
                      PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP};
  for (size_t i = 0; i < sizeof(kind_array) / sizeof(kind_array[0]); ++i) {
    ASSERT_EQ(0, pthread_rwlockattr_setkind_np(&attr, kind_array[i]));
    int kind;
    ASSERT_EQ(0, pthread_rwlockattr_getkind_np(&attr, &kind));
    ASSERT_EQ(kind_array[i], kind);
  }

  ASSERT_EQ(0, pthread_rwlockattr_destroy(&attr));
}

TEST(pthread, pthread_rwlock_init_same_as_PTHREAD_RWLOCK_INITIALIZER) {
  pthread_rwlock_t lock1 = PTHREAD_RWLOCK_INITIALIZER;
  pthread_rwlock_t lock2;
  ASSERT_EQ(0, pthread_rwlock_init(&lock2, NULL));
  ASSERT_EQ(0, memcmp(&lock1, &lock2, sizeof(lock1)));
}

TEST(pthread, pthread_rwlock_smoke) {
  pthread_rwlock_t l;
  ASSERT_EQ(0, pthread_rwlock_init(&l, NULL));

  // Single read lock
  ASSERT_EQ(0, pthread_rwlock_rdlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  // Multiple read lock
  ASSERT_EQ(0, pthread_rwlock_rdlock(&l));
  ASSERT_EQ(0, pthread_rwlock_rdlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  // Write lock
  ASSERT_EQ(0, pthread_rwlock_wrlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  // Try writer lock
  ASSERT_EQ(0, pthread_rwlock_trywrlock(&l));
  ASSERT_EQ(EBUSY, pthread_rwlock_trywrlock(&l));
  ASSERT_EQ(EBUSY, pthread_rwlock_tryrdlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  // Try reader lock
  ASSERT_EQ(0, pthread_rwlock_tryrdlock(&l));
  ASSERT_EQ(0, pthread_rwlock_tryrdlock(&l));
  ASSERT_EQ(EBUSY, pthread_rwlock_trywrlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  // Try writer lock after unlock
  ASSERT_EQ(0, pthread_rwlock_wrlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  // EDEADLK in "read after write"
  ASSERT_EQ(0, pthread_rwlock_wrlock(&l));
  ASSERT_EQ(EDEADLK, pthread_rwlock_rdlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  // EDEADLK in "write after write"
  ASSERT_EQ(0, pthread_rwlock_wrlock(&l));
  ASSERT_EQ(EDEADLK, pthread_rwlock_wrlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  ASSERT_EQ(0, pthread_rwlock_destroy(&l));
}

struct RwlockWakeupHelperArg {
  pthread_rwlock_t lock;
  enum Progress {
    LOCK_INITIALIZED,
    LOCK_WAITING,
    LOCK_RELEASED,
    LOCK_ACCESSED,
    LOCK_TIMEDOUT,
  };
  std::atomic<Progress> progress;
  std::atomic<pid_t> tid;
  std::function<int (pthread_rwlock_t*)> trylock_function;
  std::function<int (pthread_rwlock_t*)> lock_function;
  std::function<int (pthread_rwlock_t*, const timespec*)> timed_lock_function;
};

static void pthread_rwlock_wakeup_helper(RwlockWakeupHelperArg* arg) {
  arg->tid = gettid();
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_INITIALIZED, arg->progress);
  arg->progress = RwlockWakeupHelperArg::LOCK_WAITING;

  ASSERT_EQ(EBUSY, arg->trylock_function(&arg->lock));
  ASSERT_EQ(0, arg->lock_function(&arg->lock));
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_RELEASED, arg->progress);
  ASSERT_EQ(0, pthread_rwlock_unlock(&arg->lock));

  arg->progress = RwlockWakeupHelperArg::LOCK_ACCESSED;
}

static void test_pthread_rwlock_reader_wakeup_writer(std::function<int (pthread_rwlock_t*)> lock_function) {
  RwlockWakeupHelperArg wakeup_arg;
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, NULL));
  ASSERT_EQ(0, pthread_rwlock_rdlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = pthread_rwlock_trywrlock;
  wakeup_arg.lock_function = lock_function;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL,
    reinterpret_cast<void* (*)(void*)>(pthread_rwlock_wakeup_helper), &wakeup_arg));
  WaitUntilThreadSleep(wakeup_arg.tid);
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, wakeup_arg.progress);

  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_RELEASED;
  ASSERT_EQ(0, pthread_rwlock_unlock(&wakeup_arg.lock));

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_ACCESSED, wakeup_arg.progress);
  ASSERT_EQ(0, pthread_rwlock_destroy(&wakeup_arg.lock));
}

TEST(pthread, pthread_rwlock_reader_wakeup_writer) {
  test_pthread_rwlock_reader_wakeup_writer(pthread_rwlock_wrlock);
}

TEST(pthread, pthread_rwlock_reader_wakeup_writer_timedwait) {
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_reader_wakeup_writer([&](pthread_rwlock_t* lock) {
    return pthread_rwlock_timedwrlock(lock, &ts);
  });
}

static void test_pthread_rwlock_writer_wakeup_reader(std::function<int (pthread_rwlock_t*)> lock_function) {
  RwlockWakeupHelperArg wakeup_arg;
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, NULL));
  ASSERT_EQ(0, pthread_rwlock_wrlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = pthread_rwlock_tryrdlock;
  wakeup_arg.lock_function = lock_function;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL,
    reinterpret_cast<void* (*)(void*)>(pthread_rwlock_wakeup_helper), &wakeup_arg));
  WaitUntilThreadSleep(wakeup_arg.tid);
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, wakeup_arg.progress);

  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_RELEASED;
  ASSERT_EQ(0, pthread_rwlock_unlock(&wakeup_arg.lock));

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_ACCESSED, wakeup_arg.progress);
  ASSERT_EQ(0, pthread_rwlock_destroy(&wakeup_arg.lock));
}

TEST(pthread, pthread_rwlock_writer_wakeup_reader) {
  test_pthread_rwlock_writer_wakeup_reader(pthread_rwlock_rdlock);
}

TEST(pthread, pthread_rwlock_writer_wakeup_reader_timedwait) {
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_writer_wakeup_reader([&](pthread_rwlock_t* lock) {
    return pthread_rwlock_timedrdlock(lock, &ts);
  });
}

static void pthread_rwlock_wakeup_timeout_helper(RwlockWakeupHelperArg* arg) {
  arg->tid = gettid();
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_INITIALIZED, arg->progress);
  arg->progress = RwlockWakeupHelperArg::LOCK_WAITING;

  ASSERT_EQ(EBUSY, arg->trylock_function(&arg->lock));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ASSERT_EQ(ETIMEDOUT, arg->timed_lock_function(&arg->lock, &ts));
  ts.tv_nsec = -1;
  ASSERT_EQ(EINVAL, arg->timed_lock_function(&arg->lock, &ts));
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(EINVAL, arg->timed_lock_function(&arg->lock, &ts));
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(ETIMEDOUT, arg->timed_lock_function(&arg->lock, &ts));
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  ASSERT_EQ(ETIMEDOUT, arg->timed_lock_function(&arg->lock, &ts));
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, arg->progress);
  arg->progress = RwlockWakeupHelperArg::LOCK_TIMEDOUT;
}

TEST(pthread, pthread_rwlock_timedrdlock_timeout) {
  RwlockWakeupHelperArg wakeup_arg;
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, nullptr));
  ASSERT_EQ(0, pthread_rwlock_wrlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = pthread_rwlock_tryrdlock;
  wakeup_arg.timed_lock_function = pthread_rwlock_timedrdlock;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr,
      reinterpret_cast<void* (*)(void*)>(pthread_rwlock_wakeup_timeout_helper), &wakeup_arg));
  WaitUntilThreadSleep(wakeup_arg.tid);
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, wakeup_arg.progress);

  ASSERT_EQ(0, pthread_join(thread, nullptr));
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_TIMEDOUT, wakeup_arg.progress);
  ASSERT_EQ(0, pthread_rwlock_unlock(&wakeup_arg.lock));
  ASSERT_EQ(0, pthread_rwlock_destroy(&wakeup_arg.lock));
}

TEST(pthread, pthread_rwlock_timedwrlock_timeout) {
  RwlockWakeupHelperArg wakeup_arg;
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, nullptr));
  ASSERT_EQ(0, pthread_rwlock_rdlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = pthread_rwlock_trywrlock;
  wakeup_arg.timed_lock_function = pthread_rwlock_timedwrlock;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr,
      reinterpret_cast<void* (*)(void*)>(pthread_rwlock_wakeup_timeout_helper), &wakeup_arg));
  WaitUntilThreadSleep(wakeup_arg.tid);
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, wakeup_arg.progress);

  ASSERT_EQ(0, pthread_join(thread, nullptr));
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_TIMEDOUT, wakeup_arg.progress);
  ASSERT_EQ(0, pthread_rwlock_unlock(&wakeup_arg.lock));
  ASSERT_EQ(0, pthread_rwlock_destroy(&wakeup_arg.lock));
}

class RwlockKindTestHelper {
 private:
  struct ThreadArg {
    RwlockKindTestHelper* helper;
    std::atomic<pid_t>& tid;

    ThreadArg(RwlockKindTestHelper* helper, std::atomic<pid_t>& tid)
      : helper(helper), tid(tid) { }
  };

 public:
  pthread_rwlock_t lock;

 public:
  explicit RwlockKindTestHelper(int kind_type) {
    InitRwlock(kind_type);
  }

  ~RwlockKindTestHelper() {
    DestroyRwlock();
  }

  void CreateWriterThread(pthread_t& thread, std::atomic<pid_t>& tid) {
    tid = 0;
    ThreadArg* arg = new ThreadArg(this, tid);
    ASSERT_EQ(0, pthread_create(&thread, NULL,
                                reinterpret_cast<void* (*)(void*)>(WriterThreadFn), arg));
  }

  void CreateReaderThread(pthread_t& thread, std::atomic<pid_t>& tid) {
    tid = 0;
    ThreadArg* arg = new ThreadArg(this, tid);
    ASSERT_EQ(0, pthread_create(&thread, NULL,
                                reinterpret_cast<void* (*)(void*)>(ReaderThreadFn), arg));
  }

 private:
  void InitRwlock(int kind_type) {
    pthread_rwlockattr_t attr;
    ASSERT_EQ(0, pthread_rwlockattr_init(&attr));
    ASSERT_EQ(0, pthread_rwlockattr_setkind_np(&attr, kind_type));
    ASSERT_EQ(0, pthread_rwlock_init(&lock, &attr));
    ASSERT_EQ(0, pthread_rwlockattr_destroy(&attr));
  }

  void DestroyRwlock() {
    ASSERT_EQ(0, pthread_rwlock_destroy(&lock));
  }

  static void WriterThreadFn(ThreadArg* arg) {
    arg->tid = gettid();

    RwlockKindTestHelper* helper = arg->helper;
    ASSERT_EQ(0, pthread_rwlock_wrlock(&helper->lock));
    ASSERT_EQ(0, pthread_rwlock_unlock(&helper->lock));
    delete arg;
  }

  static void ReaderThreadFn(ThreadArg* arg) {
    arg->tid = gettid();

    RwlockKindTestHelper* helper = arg->helper;
    ASSERT_EQ(0, pthread_rwlock_rdlock(&helper->lock));
    ASSERT_EQ(0, pthread_rwlock_unlock(&helper->lock));
    delete arg;
  }
};

TEST(pthread, pthread_rwlock_kind_PTHREAD_RWLOCK_PREFER_READER_NP) {
  RwlockKindTestHelper helper(PTHREAD_RWLOCK_PREFER_READER_NP);
  ASSERT_EQ(0, pthread_rwlock_rdlock(&helper.lock));

  pthread_t writer_thread;
  std::atomic<pid_t> writer_tid;
  helper.CreateWriterThread(writer_thread, writer_tid);
  WaitUntilThreadSleep(writer_tid);

  pthread_t reader_thread;
  std::atomic<pid_t> reader_tid;
  helper.CreateReaderThread(reader_thread, reader_tid);
  ASSERT_EQ(0, pthread_join(reader_thread, NULL));

  ASSERT_EQ(0, pthread_rwlock_unlock(&helper.lock));
  ASSERT_EQ(0, pthread_join(writer_thread, NULL));
}

TEST(pthread, pthread_rwlock_kind_PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP) {
  RwlockKindTestHelper helper(PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
  ASSERT_EQ(0, pthread_rwlock_rdlock(&helper.lock));

  pthread_t writer_thread;
  std::atomic<pid_t> writer_tid;
  helper.CreateWriterThread(writer_thread, writer_tid);
  WaitUntilThreadSleep(writer_tid);

  pthread_t reader_thread;
  std::atomic<pid_t> reader_tid;
  helper.CreateReaderThread(reader_thread, reader_tid);
  WaitUntilThreadSleep(reader_tid);

  ASSERT_EQ(0, pthread_rwlock_unlock(&helper.lock));
  ASSERT_EQ(0, pthread_join(writer_thread, NULL));
  ASSERT_EQ(0, pthread_join(reader_thread, NULL));
}

static int g_once_fn_call_count = 0;
static void OnceFn() {
  ++g_once_fn_call_count;
}

TEST(pthread, pthread_once_smoke) {
  pthread_once_t once_control = PTHREAD_ONCE_INIT;
  ASSERT_EQ(0, pthread_once(&once_control, OnceFn));
  ASSERT_EQ(0, pthread_once(&once_control, OnceFn));
  ASSERT_EQ(1, g_once_fn_call_count);
}

static std::string pthread_once_1934122_result = "";

static void Routine2() {
  pthread_once_1934122_result += "2";
}

static void Routine1() {
  pthread_once_t once_control_2 = PTHREAD_ONCE_INIT;
  pthread_once_1934122_result += "1";
  pthread_once(&once_control_2, &Routine2);
}

TEST(pthread, pthread_once_1934122) {
  // Very old versions of Android couldn't call pthread_once from a
  // pthread_once init routine. http://b/1934122.
  pthread_once_t once_control_1 = PTHREAD_ONCE_INIT;
  ASSERT_EQ(0, pthread_once(&once_control_1, &Routine1));
  ASSERT_EQ("12", pthread_once_1934122_result);
}

static int g_atfork_prepare_calls = 0;
static void AtForkPrepare1() { g_atfork_prepare_calls = (g_atfork_prepare_calls * 10) + 1; }
static void AtForkPrepare2() { g_atfork_prepare_calls = (g_atfork_prepare_calls * 10) + 2; }
static int g_atfork_parent_calls = 0;
static void AtForkParent1() { g_atfork_parent_calls = (g_atfork_parent_calls * 10) + 1; }
static void AtForkParent2() { g_atfork_parent_calls = (g_atfork_parent_calls * 10) + 2; }
static int g_atfork_child_calls = 0;
static void AtForkChild1() { g_atfork_child_calls = (g_atfork_child_calls * 10) + 1; }
static void AtForkChild2() { g_atfork_child_calls = (g_atfork_child_calls * 10) + 2; }

TEST(pthread, pthread_atfork_smoke) {
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare1, AtForkParent1, AtForkChild1));
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare2, AtForkParent2, AtForkChild2));

  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  // Child and parent calls are made in the order they were registered.
  if (pid == 0) {
    ASSERT_EQ(12, g_atfork_child_calls);
    _exit(0);
  }
  ASSERT_EQ(12, g_atfork_parent_calls);

  // Prepare calls are made in the reverse order.
  ASSERT_EQ(21, g_atfork_prepare_calls);
  AssertChildExited(pid, 0);
}

TEST(pthread, pthread_attr_getscope) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  int scope;
  ASSERT_EQ(0, pthread_attr_getscope(&attr, &scope));
  ASSERT_EQ(PTHREAD_SCOPE_SYSTEM, scope);
}

TEST(pthread, pthread_condattr_init) {
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);

  clockid_t clock;
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_REALTIME, clock);

  int pshared;
  ASSERT_EQ(0, pthread_condattr_getpshared(&attr, &pshared));
  ASSERT_EQ(PTHREAD_PROCESS_PRIVATE, pshared);
}

TEST(pthread, pthread_condattr_setclock) {
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);

  ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_REALTIME));
  clockid_t clock;
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_REALTIME, clock);

  ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_MONOTONIC, clock);

  ASSERT_EQ(EINVAL, pthread_condattr_setclock(&attr, CLOCK_PROCESS_CPUTIME_ID));
}

TEST(pthread, pthread_cond_broadcast__preserves_condattr_flags) {
#if defined(__BIONIC__)
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);

  ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
  ASSERT_EQ(0, pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));

  pthread_cond_t cond_var;
  ASSERT_EQ(0, pthread_cond_init(&cond_var, &attr));

  ASSERT_EQ(0, pthread_cond_signal(&cond_var));
  ASSERT_EQ(0, pthread_cond_broadcast(&cond_var));

  attr = static_cast<pthread_condattr_t>(*reinterpret_cast<uint32_t*>(cond_var.__private));
  clockid_t clock;
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_MONOTONIC, clock);
  int pshared;
  ASSERT_EQ(0, pthread_condattr_getpshared(&attr, &pshared));
  ASSERT_EQ(PTHREAD_PROCESS_SHARED, pshared);
#else  // !defined(__BIONIC__)
  GTEST_LOG_(INFO) << "This tests a bionic implementation detail.\n";
#endif  // !defined(__BIONIC__)
}

class pthread_CondWakeupTest : public ::testing::Test {
 protected:
  pthread_mutex_t mutex;
  pthread_cond_t cond;

  enum Progress {
    INITIALIZED,
    WAITING,
    SIGNALED,
    FINISHED,
  };
  std::atomic<Progress> progress;
  pthread_t thread;
  std::function<int (pthread_cond_t* cond, pthread_mutex_t* mutex)> wait_function;

 protected:
  void SetUp() override {
    ASSERT_EQ(0, pthread_mutex_init(&mutex, nullptr));
  }

  void InitCond(clockid_t clock=CLOCK_REALTIME) {
    pthread_condattr_t attr;
    ASSERT_EQ(0, pthread_condattr_init(&attr));
    ASSERT_EQ(0, pthread_condattr_setclock(&attr, clock));
    ASSERT_EQ(0, pthread_cond_init(&cond, &attr));
    ASSERT_EQ(0, pthread_condattr_destroy(&attr));
  }

  void StartWaitingThread(std::function<int (pthread_cond_t* cond, pthread_mutex_t* mutex)> wait_function) {
    progress = INITIALIZED;
    this->wait_function = wait_function;
    ASSERT_EQ(0, pthread_create(&thread, NULL, reinterpret_cast<void* (*)(void*)>(WaitThreadFn), this));
    while (progress != WAITING) {
      usleep(5000);
    }
    usleep(5000);
  }

  void TearDown() override {
    ASSERT_EQ(0, pthread_join(thread, nullptr));
    ASSERT_EQ(FINISHED, progress);
    ASSERT_EQ(0, pthread_cond_destroy(&cond));
    ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
  }

 private:
  static void WaitThreadFn(pthread_CondWakeupTest* test) {
    ASSERT_EQ(0, pthread_mutex_lock(&test->mutex));
    test->progress = WAITING;
    while (test->progress == WAITING) {
      ASSERT_EQ(0, test->wait_function(&test->cond, &test->mutex));
    }
    ASSERT_EQ(SIGNALED, test->progress);
    test->progress = FINISHED;
    ASSERT_EQ(0, pthread_mutex_unlock(&test->mutex));
  }
};

TEST_F(pthread_CondWakeupTest, signal_wait) {
  InitCond();
  StartWaitingThread([](pthread_cond_t* cond, pthread_mutex_t* mutex) {
    return pthread_cond_wait(cond, mutex);
  });
  progress = SIGNALED;
  ASSERT_EQ(0, pthread_cond_signal(&cond));
}

TEST_F(pthread_CondWakeupTest, broadcast_wait) {
  InitCond();
  StartWaitingThread([](pthread_cond_t* cond, pthread_mutex_t* mutex) {
    return pthread_cond_wait(cond, mutex);
  });
  progress = SIGNALED;
  ASSERT_EQ(0, pthread_cond_broadcast(&cond));
}

TEST_F(pthread_CondWakeupTest, signal_timedwait_CLOCK_REALTIME) {
  InitCond(CLOCK_REALTIME);
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  StartWaitingThread([&](pthread_cond_t* cond, pthread_mutex_t* mutex) {
    return pthread_cond_timedwait(cond, mutex, &ts);
  });
  progress = SIGNALED;
  ASSERT_EQ(0, pthread_cond_signal(&cond));
}

TEST_F(pthread_CondWakeupTest, signal_timedwait_CLOCK_MONOTONIC) {
  InitCond(CLOCK_MONOTONIC);
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &ts));
  ts.tv_sec += 1;
  StartWaitingThread([&](pthread_cond_t* cond, pthread_mutex_t* mutex) {
    return pthread_cond_timedwait(cond, mutex, &ts);
  });
  progress = SIGNALED;
  ASSERT_EQ(0, pthread_cond_signal(&cond));
}

TEST(pthread, pthread_cond_timedwait_timeout) {
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, nullptr));
  pthread_cond_t cond;
  ASSERT_EQ(0, pthread_cond_init(&cond, nullptr));
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ASSERT_EQ(ETIMEDOUT, pthread_cond_timedwait(&cond, &mutex, &ts));
  ts.tv_nsec = -1;
  ASSERT_EQ(EINVAL, pthread_cond_timedwait(&cond, &mutex, &ts));
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(EINVAL, pthread_cond_timedwait(&cond, &mutex, &ts));
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(ETIMEDOUT, pthread_cond_timedwait(&cond, &mutex, &ts));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
}

TEST(pthread, pthread_attr_getstack__main_thread) {
  // This test is only meaningful for the main thread, so make sure we're running on it!
  ASSERT_EQ(getpid(), syscall(__NR_gettid));

  // Get the main thread's attributes.
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_getattr_np(pthread_self(), &attributes));

  // Check that we correctly report that the main thread has no guard page.
  size_t guard_size;
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(0U, guard_size); // The main thread has no guard page.

  // Get the stack base and the stack size (both ways).
  void* stack_base;
  size_t stack_size;
  ASSERT_EQ(0, pthread_attr_getstack(&attributes, &stack_base, &stack_size));
  size_t stack_size2;
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size2));

  // The two methods of asking for the stack size should agree.
  EXPECT_EQ(stack_size, stack_size2);

#if defined(__BIONIC__)
  // What does /proc/self/maps' [stack] line say?
  void* maps_stack_hi = NULL;
  std::vector<map_record> maps;
  ASSERT_TRUE(Maps::parse_maps(&maps));
  for (const auto& map : maps) {
    if (map.pathname == "[stack]") {
      maps_stack_hi = reinterpret_cast<void*>(map.addr_end);
      break;
    }
  }

  // The high address of the /proc/self/maps [stack] region should equal stack_base + stack_size.
  // Remember that the stack grows down (and is mapped in on demand), so the low address of the
  // region isn't very interesting.
  EXPECT_EQ(maps_stack_hi, reinterpret_cast<uint8_t*>(stack_base) + stack_size);

  // The stack size should correspond to RLIMIT_STACK.
  rlimit rl;
  ASSERT_EQ(0, getrlimit(RLIMIT_STACK, &rl));
  uint64_t original_rlim_cur = rl.rlim_cur;
  if (rl.rlim_cur == RLIM_INFINITY) {
    rl.rlim_cur = 8 * 1024 * 1024; // Bionic reports unlimited stacks as 8MiB.
  }
  EXPECT_EQ(rl.rlim_cur, stack_size);

  auto guard = make_scope_guard([&rl, original_rlim_cur]() {
    rl.rlim_cur = original_rlim_cur;
    ASSERT_EQ(0, setrlimit(RLIMIT_STACK, &rl));
  });

  //
  // What if RLIMIT_STACK is smaller than the stack's current extent?
  //
  rl.rlim_cur = rl.rlim_max = 1024; // 1KiB. We know the stack must be at least a page already.
  rl.rlim_max = RLIM_INFINITY;
  ASSERT_EQ(0, setrlimit(RLIMIT_STACK, &rl));

  ASSERT_EQ(0, pthread_getattr_np(pthread_self(), &attributes));
  ASSERT_EQ(0, pthread_attr_getstack(&attributes, &stack_base, &stack_size));
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size2));

  EXPECT_EQ(stack_size, stack_size2);
  ASSERT_EQ(1024U, stack_size);

  //
  // What if RLIMIT_STACK isn't a whole number of pages?
  //
  rl.rlim_cur = rl.rlim_max = 6666; // Not a whole number of pages.
  rl.rlim_max = RLIM_INFINITY;
  ASSERT_EQ(0, setrlimit(RLIMIT_STACK, &rl));

  ASSERT_EQ(0, pthread_getattr_np(pthread_self(), &attributes));
  ASSERT_EQ(0, pthread_attr_getstack(&attributes, &stack_base, &stack_size));
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size2));

  EXPECT_EQ(stack_size, stack_size2);
  ASSERT_EQ(6666U, stack_size);
#endif
}

struct GetStackSignalHandlerArg {
  volatile bool done;
  void* signal_stack_base;
  size_t signal_stack_size;
  void* main_stack_base;
  size_t main_stack_size;
};

static GetStackSignalHandlerArg getstack_signal_handler_arg;

static void getstack_signal_handler(int sig) {
  ASSERT_EQ(SIGUSR1, sig);
  // Use sleep() to make current thread be switched out by the kernel to provoke the error.
  sleep(1);
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_getattr_np(pthread_self(), &attr));
  void* stack_base;
  size_t stack_size;
  ASSERT_EQ(0, pthread_attr_getstack(&attr, &stack_base, &stack_size));

  // Verify if the stack used by the signal handler is the alternate stack just registered.
  ASSERT_LE(getstack_signal_handler_arg.signal_stack_base, &attr);
  ASSERT_LT(static_cast<void*>(&attr),
            static_cast<char*>(getstack_signal_handler_arg.signal_stack_base) +
            getstack_signal_handler_arg.signal_stack_size);

  // Verify if the main thread's stack got in the signal handler is correct.
  ASSERT_EQ(getstack_signal_handler_arg.main_stack_base, stack_base);
  ASSERT_LE(getstack_signal_handler_arg.main_stack_size, stack_size);

  getstack_signal_handler_arg.done = true;
}

// The previous code obtained the main thread's stack by reading the entry in
// /proc/self/task/<pid>/maps that was labeled [stack]. Unfortunately, on x86/x86_64, the kernel
// relies on sp0 in task state segment(tss) to label the stack map with [stack]. If the kernel
// switches a process while the main thread is in an alternate stack, then the kernel will label
// the wrong map with [stack]. This test verifies that when the above situation happens, the main
// thread's stack is found correctly.
TEST(pthread, pthread_attr_getstack_in_signal_handler) {
  // This test is only meaningful for the main thread, so make sure we're running on it!
  ASSERT_EQ(getpid(), syscall(__NR_gettid));

  const size_t sig_stack_size = 16 * 1024;
  void* sig_stack = mmap(NULL, sig_stack_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
                         -1, 0);
  ASSERT_NE(MAP_FAILED, sig_stack);
  stack_t ss;
  ss.ss_sp = sig_stack;
  ss.ss_size = sig_stack_size;
  ss.ss_flags = 0;
  stack_t oss;
  ASSERT_EQ(0, sigaltstack(&ss, &oss));

  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_getattr_np(pthread_self(), &attr));
  void* main_stack_base;
  size_t main_stack_size;
  ASSERT_EQ(0, pthread_attr_getstack(&attr, &main_stack_base, &main_stack_size));

  ScopedSignalHandler handler(SIGUSR1, getstack_signal_handler, SA_ONSTACK);
  getstack_signal_handler_arg.done = false;
  getstack_signal_handler_arg.signal_stack_base = sig_stack;
  getstack_signal_handler_arg.signal_stack_size = sig_stack_size;
  getstack_signal_handler_arg.main_stack_base = main_stack_base;
  getstack_signal_handler_arg.main_stack_size = main_stack_size;
  kill(getpid(), SIGUSR1);
  ASSERT_EQ(true, getstack_signal_handler_arg.done);

  ASSERT_EQ(0, sigaltstack(&oss, nullptr));
  ASSERT_EQ(0, munmap(sig_stack, sig_stack_size));
}

static void pthread_attr_getstack_18908062_helper(void*) {
  char local_variable;
  pthread_attr_t attributes;
  pthread_getattr_np(pthread_self(), &attributes);
  void* stack_base;
  size_t stack_size;
  pthread_attr_getstack(&attributes, &stack_base, &stack_size);

  // Test whether &local_variable is in [stack_base, stack_base + stack_size).
  ASSERT_LE(reinterpret_cast<char*>(stack_base), &local_variable);
  ASSERT_LT(&local_variable, reinterpret_cast<char*>(stack_base) + stack_size);
}

// Check whether something on stack is in the range of
// [stack_base, stack_base + stack_size). see b/18908062.
TEST(pthread, pthread_attr_getstack_18908062) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL,
            reinterpret_cast<void* (*)(void*)>(pthread_attr_getstack_18908062_helper),
            NULL));
  pthread_join(t, NULL);
}

#if defined(__BIONIC__)
static pthread_mutex_t pthread_gettid_np_mutex = PTHREAD_MUTEX_INITIALIZER;

static void* pthread_gettid_np_helper(void* arg) {
  *reinterpret_cast<pid_t*>(arg) = gettid();

  // Wait for our parent to call pthread_gettid_np on us before exiting.
  pthread_mutex_lock(&pthread_gettid_np_mutex);
  pthread_mutex_unlock(&pthread_gettid_np_mutex);
  return NULL;
}
#endif

TEST(pthread, pthread_gettid_np) {
#if defined(__BIONIC__)
  ASSERT_EQ(gettid(), pthread_gettid_np(pthread_self()));

  // Ensure the other thread doesn't exit until after we've called
  // pthread_gettid_np on it.
  pthread_mutex_lock(&pthread_gettid_np_mutex);

  pid_t t_gettid_result;
  pthread_t t;
  pthread_create(&t, NULL, pthread_gettid_np_helper, &t_gettid_result);

  pid_t t_pthread_gettid_np_result = pthread_gettid_np(t);

  // Release the other thread and wait for it to exit.
  pthread_mutex_unlock(&pthread_gettid_np_mutex);
  pthread_join(t, NULL);

  ASSERT_EQ(t_gettid_result, t_pthread_gettid_np_result);
#else
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
}

static size_t cleanup_counter = 0;

static void AbortCleanupRoutine(void*) {
  abort();
}

static void CountCleanupRoutine(void*) {
  ++cleanup_counter;
}

static void PthreadCleanupTester() {
  pthread_cleanup_push(CountCleanupRoutine, NULL);
  pthread_cleanup_push(CountCleanupRoutine, NULL);
  pthread_cleanup_push(AbortCleanupRoutine, NULL);

  pthread_cleanup_pop(0); // Pop the abort without executing it.
  pthread_cleanup_pop(1); // Pop one count while executing it.
  ASSERT_EQ(1U, cleanup_counter);
  // Exit while the other count is still on the cleanup stack.
  pthread_exit(NULL);

  // Calls to pthread_cleanup_pop/pthread_cleanup_push must always be balanced.
  pthread_cleanup_pop(0);
}

static void* PthreadCleanupStartRoutine(void*) {
  PthreadCleanupTester();
  return NULL;
}

TEST(pthread, pthread_cleanup_push__pthread_cleanup_pop) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, PthreadCleanupStartRoutine, NULL));
  pthread_join(t, NULL);
  ASSERT_EQ(2U, cleanup_counter);
}

TEST(pthread, PTHREAD_MUTEX_DEFAULT_is_PTHREAD_MUTEX_NORMAL) {
  ASSERT_EQ(PTHREAD_MUTEX_NORMAL, PTHREAD_MUTEX_DEFAULT);
}

TEST(pthread, pthread_mutexattr_gettype) {
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));

  int attr_type;

  ASSERT_EQ(0, pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL));
  ASSERT_EQ(0, pthread_mutexattr_gettype(&attr, &attr_type));
  ASSERT_EQ(PTHREAD_MUTEX_NORMAL, attr_type);

  ASSERT_EQ(0, pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK));
  ASSERT_EQ(0, pthread_mutexattr_gettype(&attr, &attr_type));
  ASSERT_EQ(PTHREAD_MUTEX_ERRORCHECK, attr_type);

  ASSERT_EQ(0, pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
  ASSERT_EQ(0, pthread_mutexattr_gettype(&attr, &attr_type));
  ASSERT_EQ(PTHREAD_MUTEX_RECURSIVE, attr_type);

  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
}

struct PthreadMutex {
  pthread_mutex_t lock;

  explicit PthreadMutex(int mutex_type) {
    init(mutex_type);
  }

  ~PthreadMutex() {
    destroy();
  }

 private:
  void init(int mutex_type) {
    pthread_mutexattr_t attr;
    ASSERT_EQ(0, pthread_mutexattr_init(&attr));
    ASSERT_EQ(0, pthread_mutexattr_settype(&attr, mutex_type));
    ASSERT_EQ(0, pthread_mutex_init(&lock, &attr));
    ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
  }

  void destroy() {
    ASSERT_EQ(0, pthread_mutex_destroy(&lock));
  }

  DISALLOW_COPY_AND_ASSIGN(PthreadMutex);
};

TEST(pthread, pthread_mutex_lock_NORMAL) {
  PthreadMutex m(PTHREAD_MUTEX_NORMAL);

  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(EBUSY, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
}

TEST(pthread, pthread_mutex_lock_ERRORCHECK) {
  PthreadMutex m(PTHREAD_MUTEX_ERRORCHECK);

  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(EDEADLK, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(EBUSY, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(EPERM, pthread_mutex_unlock(&m.lock));
}

TEST(pthread, pthread_mutex_lock_RECURSIVE) {
  PthreadMutex m(PTHREAD_MUTEX_RECURSIVE);

  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(EPERM, pthread_mutex_unlock(&m.lock));
}

TEST(pthread, pthread_mutex_init_same_as_static_initializers) {
  pthread_mutex_t lock_normal = PTHREAD_MUTEX_INITIALIZER;
  PthreadMutex m1(PTHREAD_MUTEX_NORMAL);
  ASSERT_EQ(0, memcmp(&lock_normal, &m1.lock, sizeof(pthread_mutex_t)));
  pthread_mutex_destroy(&lock_normal);

  pthread_mutex_t lock_errorcheck = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
  PthreadMutex m2(PTHREAD_MUTEX_ERRORCHECK);
  ASSERT_EQ(0, memcmp(&lock_errorcheck, &m2.lock, sizeof(pthread_mutex_t)));
  pthread_mutex_destroy(&lock_errorcheck);

  pthread_mutex_t lock_recursive = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
  PthreadMutex m3(PTHREAD_MUTEX_RECURSIVE);
  ASSERT_EQ(0, memcmp(&lock_recursive, &m3.lock, sizeof(pthread_mutex_t)));
  ASSERT_EQ(0, pthread_mutex_destroy(&lock_recursive));
}
class MutexWakeupHelper {
 private:
  PthreadMutex m;
  enum Progress {
    LOCK_INITIALIZED,
    LOCK_WAITING,
    LOCK_RELEASED,
    LOCK_ACCESSED
  };
  std::atomic<Progress> progress;
  std::atomic<pid_t> tid;

  static void thread_fn(MutexWakeupHelper* helper) {
    helper->tid = gettid();
    ASSERT_EQ(LOCK_INITIALIZED, helper->progress);
    helper->progress = LOCK_WAITING;

    ASSERT_EQ(0, pthread_mutex_lock(&helper->m.lock));
    ASSERT_EQ(LOCK_RELEASED, helper->progress);
    ASSERT_EQ(0, pthread_mutex_unlock(&helper->m.lock));

    helper->progress = LOCK_ACCESSED;
  }

 public:
  explicit MutexWakeupHelper(int mutex_type) : m(mutex_type) {
  }

  void test() {
    ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
    progress = LOCK_INITIALIZED;
    tid = 0;

    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, NULL,
      reinterpret_cast<void* (*)(void*)>(MutexWakeupHelper::thread_fn), this));

    WaitUntilThreadSleep(tid);
    ASSERT_EQ(LOCK_WAITING, progress);

    progress = LOCK_RELEASED;
    ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));

    ASSERT_EQ(0, pthread_join(thread, NULL));
    ASSERT_EQ(LOCK_ACCESSED, progress);
  }
};

TEST(pthread, pthread_mutex_NORMAL_wakeup) {
  MutexWakeupHelper helper(PTHREAD_MUTEX_NORMAL);
  helper.test();
}

TEST(pthread, pthread_mutex_ERRORCHECK_wakeup) {
  MutexWakeupHelper helper(PTHREAD_MUTEX_ERRORCHECK);
  helper.test();
}

TEST(pthread, pthread_mutex_RECURSIVE_wakeup) {
  MutexWakeupHelper helper(PTHREAD_MUTEX_RECURSIVE);
  helper.test();
}

TEST(pthread, pthread_mutex_owner_tid_limit) {
#if defined(__BIONIC__) && !defined(__LP64__)
  FILE* fp = fopen("/proc/sys/kernel/pid_max", "r");
  ASSERT_TRUE(fp != NULL);
  long pid_max;
  ASSERT_EQ(1, fscanf(fp, "%ld", &pid_max));
  fclose(fp);
  // Bionic's pthread_mutex implementation on 32-bit devices uses 16 bits to represent owner tid.
  ASSERT_LE(pid_max, 65536);
#else
  GTEST_LOG_(INFO) << "This test does nothing as 32-bit tid is supported by pthread_mutex.\n";
#endif
}

TEST(pthread, pthread_mutex_timedlock) {
  pthread_mutex_t m;
  ASSERT_EQ(0, pthread_mutex_init(&m, nullptr));

  // If the mutex is already locked, pthread_mutex_timedlock should time out.
  ASSERT_EQ(0, pthread_mutex_lock(&m));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ASSERT_EQ(ETIMEDOUT, pthread_mutex_timedlock(&m, &ts));
  ts.tv_nsec = -1;
  ASSERT_EQ(EINVAL, pthread_mutex_timedlock(&m, &ts));
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(EINVAL, pthread_mutex_timedlock(&m, &ts));
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(ETIMEDOUT, pthread_mutex_timedlock(&m, &ts));

  // If the mutex is unlocked, pthread_mutex_timedlock should succeed.
  ASSERT_EQ(0, pthread_mutex_unlock(&m));

  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  ASSERT_EQ(0, pthread_mutex_timedlock(&m, &ts));

  ASSERT_EQ(0, pthread_mutex_unlock(&m));
  ASSERT_EQ(0, pthread_mutex_destroy(&m));
}

class StrictAlignmentAllocator {
 public:
  void* allocate(size_t size, size_t alignment) {
    char* p = new char[size + alignment * 2];
    allocated_array.push_back(p);
    while (!is_strict_aligned(p, alignment)) {
      ++p;
    }
    return p;
  }

  ~StrictAlignmentAllocator() {
    for (const auto& p : allocated_array) {
      delete[] p;
    }
  }

 private:
  bool is_strict_aligned(char* p, size_t alignment) {
    return (reinterpret_cast<uintptr_t>(p) % (alignment * 2)) == alignment;
  }

  std::vector<char*> allocated_array;
};

TEST(pthread, pthread_types_allow_four_bytes_alignment) {
#if defined(__BIONIC__)
  // For binary compatibility with old version, we need to allow 4-byte aligned data for pthread types.
  StrictAlignmentAllocator allocator;
  pthread_mutex_t* mutex = reinterpret_cast<pthread_mutex_t*>(
                             allocator.allocate(sizeof(pthread_mutex_t), 4));
  ASSERT_EQ(0, pthread_mutex_init(mutex, NULL));
  ASSERT_EQ(0, pthread_mutex_lock(mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(mutex));
  ASSERT_EQ(0, pthread_mutex_destroy(mutex));

  pthread_cond_t* cond = reinterpret_cast<pthread_cond_t*>(
                           allocator.allocate(sizeof(pthread_cond_t), 4));
  ASSERT_EQ(0, pthread_cond_init(cond, NULL));
  ASSERT_EQ(0, pthread_cond_signal(cond));
  ASSERT_EQ(0, pthread_cond_broadcast(cond));
  ASSERT_EQ(0, pthread_cond_destroy(cond));

  pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(
                               allocator.allocate(sizeof(pthread_rwlock_t), 4));
  ASSERT_EQ(0, pthread_rwlock_init(rwlock, NULL));
  ASSERT_EQ(0, pthread_rwlock_rdlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_wrlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_destroy(rwlock));

#else
  GTEST_LOG_(INFO) << "This test tests bionic implementation details.";
#endif
}

TEST(pthread, pthread_mutex_lock_null_32) {
#if defined(__BIONIC__) && !defined(__LP64__)
  // For LP32, the pthread lock/unlock functions allow a NULL mutex and return
  // EINVAL in that case: http://b/19995172.
  //
  // We decorate the public defintion with _Nonnull so that people recompiling
  // their code with get a warning and might fix their bug, but need to pass
  // NULL here to test that we remain compatible.
  pthread_mutex_t* null_value = nullptr;
  ASSERT_EQ(EINVAL, pthread_mutex_lock(null_value));
#else
  GTEST_LOG_(INFO) << "This test tests bionic implementation details on 32 bit devices.";
#endif
}

TEST(pthread, pthread_mutex_unlock_null_32) {
#if defined(__BIONIC__) && !defined(__LP64__)
  // For LP32, the pthread lock/unlock functions allow a NULL mutex and return
  // EINVAL in that case: http://b/19995172.
  //
  // We decorate the public defintion with _Nonnull so that people recompiling
  // their code with get a warning and might fix their bug, but need to pass
  // NULL here to test that we remain compatible.
  pthread_mutex_t* null_value = nullptr;
  ASSERT_EQ(EINVAL, pthread_mutex_unlock(null_value));
#else
  GTEST_LOG_(INFO) << "This test tests bionic implementation details on 32 bit devices.";
#endif
}

TEST_F(pthread_DeathTest, pthread_mutex_lock_null_64) {
#if defined(__BIONIC__) && defined(__LP64__)
  pthread_mutex_t* null_value = nullptr;
  ASSERT_EXIT(pthread_mutex_lock(null_value), testing::KilledBySignal(SIGSEGV), "");
#else
  GTEST_LOG_(INFO) << "This test tests bionic implementation details on 64 bit devices.";
#endif
}

TEST_F(pthread_DeathTest, pthread_mutex_unlock_null_64) {
#if defined(__BIONIC__) && defined(__LP64__)
  pthread_mutex_t* null_value = nullptr;
  ASSERT_EXIT(pthread_mutex_unlock(null_value), testing::KilledBySignal(SIGSEGV), "");
#else
  GTEST_LOG_(INFO) << "This test tests bionic implementation details on 64 bit devices.";
#endif
}

extern _Unwind_Reason_Code FrameCounter(_Unwind_Context* ctx, void* arg);

static volatile bool signal_handler_on_altstack_done;

static void SignalHandlerOnAltStack(int signo, siginfo_t*, void*) {
  ASSERT_EQ(SIGUSR1, signo);
  // Check if we have enough stack space for unwinding.
  int count = 0;
  _Unwind_Backtrace(FrameCounter, &count);
  ASSERT_GT(count, 0);
  // Check if we have enough stack space for logging.
  std::string s(2048, '*');
  GTEST_LOG_(INFO) << s;
  signal_handler_on_altstack_done = true;
}

TEST(pthread, big_enough_signal_stack_for_64bit_arch) {
  signal_handler_on_altstack_done = false;
  ScopedSignalHandler handler(SIGUSR1, SignalHandlerOnAltStack, SA_SIGINFO | SA_ONSTACK);
  kill(getpid(), SIGUSR1);
  ASSERT_TRUE(signal_handler_on_altstack_done);
}

TEST(pthread, pthread_barrierattr_smoke) {
  pthread_barrierattr_t attr;
  ASSERT_EQ(0, pthread_barrierattr_init(&attr));
  int pshared;
  ASSERT_EQ(0, pthread_barrierattr_getpshared(&attr, &pshared));
  ASSERT_EQ(PTHREAD_PROCESS_PRIVATE, pshared);
  ASSERT_EQ(0, pthread_barrierattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));
  ASSERT_EQ(0, pthread_barrierattr_getpshared(&attr, &pshared));
  ASSERT_EQ(PTHREAD_PROCESS_SHARED, pshared);
  ASSERT_EQ(0, pthread_barrierattr_destroy(&attr));
}

struct BarrierTestHelperData {
  size_t thread_count;
  pthread_barrier_t barrier;
  std::atomic<int> finished_mask;
  std::atomic<int> serial_thread_count;
  size_t iteration_count;
  std::atomic<size_t> finished_iteration_count;

  BarrierTestHelperData(size_t thread_count, size_t iteration_count)
      : thread_count(thread_count), finished_mask(0), serial_thread_count(0),
        iteration_count(iteration_count), finished_iteration_count(0) {
  }
};

struct BarrierTestHelperArg {
  int id;
  BarrierTestHelperData* data;
};

static void BarrierTestHelper(BarrierTestHelperArg* arg) {
  for (size_t i = 0; i < arg->data->iteration_count; ++i) {
    int result = pthread_barrier_wait(&arg->data->barrier);
    if (result == PTHREAD_BARRIER_SERIAL_THREAD) {
      arg->data->serial_thread_count++;
    } else {
      ASSERT_EQ(0, result);
    }
    arg->data->finished_mask |= (1 << arg->id);
    if (arg->data->finished_mask == ((1 << arg->data->thread_count) - 1)) {
      ASSERT_EQ(1, arg->data->serial_thread_count);
      arg->data->finished_iteration_count++;
      arg->data->finished_mask = 0;
      arg->data->serial_thread_count = 0;
    }
  }
}

TEST(pthread, pthread_barrier_smoke) {
  const size_t BARRIER_ITERATION_COUNT = 10;
  const size_t BARRIER_THREAD_COUNT = 10;
  BarrierTestHelperData data(BARRIER_THREAD_COUNT, BARRIER_ITERATION_COUNT);
  ASSERT_EQ(0, pthread_barrier_init(&data.barrier, nullptr, data.thread_count));
  std::vector<pthread_t> threads(data.thread_count);
  std::vector<BarrierTestHelperArg> args(threads.size());
  for (size_t i = 0; i < threads.size(); ++i) {
    args[i].id = i;
    args[i].data = &data;
    ASSERT_EQ(0, pthread_create(&threads[i], nullptr,
                                reinterpret_cast<void* (*)(void*)>(BarrierTestHelper), &args[i]));
  }
  for (size_t i = 0; i < threads.size(); ++i) {
    ASSERT_EQ(0, pthread_join(threads[i], nullptr));
  }
  ASSERT_EQ(data.iteration_count, data.finished_iteration_count);
  ASSERT_EQ(0, pthread_barrier_destroy(&data.barrier));
}

struct BarrierDestroyTestArg {
  std::atomic<int> tid;
  pthread_barrier_t* barrier;
};

static void BarrierDestroyTestHelper(BarrierDestroyTestArg* arg) {
  arg->tid = gettid();
  ASSERT_EQ(0, pthread_barrier_wait(arg->barrier));
}

TEST(pthread, pthread_barrier_destroy) {
  pthread_barrier_t barrier;
  ASSERT_EQ(0, pthread_barrier_init(&barrier, nullptr, 2));
  pthread_t thread;
  BarrierDestroyTestArg arg;
  arg.tid = 0;
  arg.barrier = &barrier;
  ASSERT_EQ(0, pthread_create(&thread, nullptr,
                              reinterpret_cast<void* (*)(void*)>(BarrierDestroyTestHelper), &arg));
  WaitUntilThreadSleep(arg.tid);
  ASSERT_EQ(EBUSY, pthread_barrier_destroy(&barrier));
  ASSERT_EQ(PTHREAD_BARRIER_SERIAL_THREAD, pthread_barrier_wait(&barrier));
  // Verify if the barrier can be destroyed directly after pthread_barrier_wait().
  ASSERT_EQ(0, pthread_barrier_destroy(&barrier));
  ASSERT_EQ(0, pthread_join(thread, nullptr));
#if defined(__BIONIC__)
  ASSERT_EQ(EINVAL, pthread_barrier_destroy(&barrier));
#endif
}

struct BarrierOrderingTestHelperArg {
  pthread_barrier_t* barrier;
  size_t* array;
  size_t array_length;
  size_t id;
};

void BarrierOrderingTestHelper(BarrierOrderingTestHelperArg* arg) {
  const size_t ITERATION_COUNT = 10000;
  for (size_t i = 1; i <= ITERATION_COUNT; ++i) {
    arg->array[arg->id] = i;
    int result = pthread_barrier_wait(arg->barrier);
    ASSERT_TRUE(result == 0 || result == PTHREAD_BARRIER_SERIAL_THREAD);
    for (size_t j = 0; j < arg->array_length; ++j) {
      ASSERT_EQ(i, arg->array[j]);
    }
    result = pthread_barrier_wait(arg->barrier);
    ASSERT_TRUE(result == 0 || result == PTHREAD_BARRIER_SERIAL_THREAD);
  }
}

TEST(pthread, pthread_barrier_check_ordering) {
  const size_t THREAD_COUNT = 4;
  pthread_barrier_t barrier;
  ASSERT_EQ(0, pthread_barrier_init(&barrier, nullptr, THREAD_COUNT));
  size_t array[THREAD_COUNT];
  std::vector<pthread_t> threads(THREAD_COUNT);
  std::vector<BarrierOrderingTestHelperArg> args(THREAD_COUNT);
  for (size_t i = 0; i < THREAD_COUNT; ++i) {
    args[i].barrier = &barrier;
    args[i].array = array;
    args[i].array_length = THREAD_COUNT;
    args[i].id = i;
    ASSERT_EQ(0, pthread_create(&threads[i], nullptr,
                                reinterpret_cast<void* (*)(void*)>(BarrierOrderingTestHelper),
                                &args[i]));
  }
  for (size_t i = 0; i < THREAD_COUNT; ++i) {
    ASSERT_EQ(0, pthread_join(threads[i], nullptr));
  }
}

TEST(pthread, pthread_spinlock_smoke) {
  pthread_spinlock_t lock;
  ASSERT_EQ(0, pthread_spin_init(&lock, 0));
  ASSERT_EQ(0, pthread_spin_trylock(&lock));
  ASSERT_EQ(0, pthread_spin_unlock(&lock));
  ASSERT_EQ(0, pthread_spin_lock(&lock));
  ASSERT_EQ(EBUSY, pthread_spin_trylock(&lock));
  ASSERT_EQ(0, pthread_spin_unlock(&lock));
  ASSERT_EQ(0, pthread_spin_destroy(&lock));
}
