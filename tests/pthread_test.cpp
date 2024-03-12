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
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include <unwind.h>

#include <atomic>
#include <future>
#include <vector>

#include <android-base/macros.h>
#include <android-base/parseint.h>
#include <android-base/scopeguard.h>
#include <android-base/silent_death_test.h>
#include <android-base/strings.h>
#include <android-base/test_utils.h>

#include "private/bionic_constants.h"
#include "SignalUtils.h"
#include "utils.h"

using pthread_DeathTest = SilentDeathTest;

TEST(pthread, pthread_key_create) {
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, nullptr));
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

  auto scope_guard = android::base::make_scope_guard([&keys] {
    for (const auto& key : keys) {
      EXPECT_EQ(0, pthread_key_delete(key));
    }
  });

  for (int i = 0; i < nkeys; ++i) {
    pthread_key_t key;
    // If this fails, it's likely that LIBC_PTHREAD_KEY_RESERVED_COUNT is wrong.
    ASSERT_EQ(0, pthread_key_create(&key, nullptr)) << i << " of " << nkeys;
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
    rv = pthread_key_create(&key, nullptr);
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
  ASSERT_EQ(0, pthread_key_create(&key, nullptr));
  ASSERT_EQ(0, pthread_setspecific(key, expected));
  ASSERT_EQ(expected, pthread_getspecific(key));
  ASSERT_EQ(0, pthread_key_delete(key));
  // After deletion, pthread_getspecific returns nullptr.
  ASSERT_EQ(nullptr, pthread_getspecific(key));
  // And you can't use pthread_setspecific with the deleted key.
  ASSERT_EQ(EINVAL, pthread_setspecific(key, expected));
}

TEST(pthread, pthread_key_fork) {
  void* expected = reinterpret_cast<void*>(1234);
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, nullptr));
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
  ASSERT_EQ(0, pthread_key_create(&key, nullptr));

  size_t stack_size = 640 * 1024;
  void* stack = mmap(nullptr, stack_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
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

static void* FnWithStackFrame(void*) {
  int x;
  *const_cast<volatile int*>(&x) = 1;
  return nullptr;
}

TEST(pthread, pthread_heap_allocated_stack) {
  SKIP_WITH_HWASAN; // TODO(b/148982147): Re-enable when fixed.

  size_t stack_size = 640 * 1024;
  std::unique_ptr<char[]> stack(new (std::align_val_t(getpagesize())) char[stack_size]);
  memset(stack.get(), '\xff', stack_size);

  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setstack(&attr, stack.get(), stack_size));

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &attr, FnWithStackFrame, nullptr));

  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
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
  GTEST_SKIP() << "bionic-only test";
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
    return nullptr;
  }
  static std::atomic<bool> spin_flag_;
};

// It doesn't matter if spin_flag_ is used in several tests,
// because it is always set to false after each test. Each thread
// loops on spin_flag_ can find it becomes false at some time.
std::atomic<bool> SpinFunctionHelper::spin_flag_;

static void* JoinFn(void* arg) {
  return reinterpret_cast<void*>(pthread_join(reinterpret_cast<pthread_t>(arg), nullptr));
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
  ASSERT_EQ(0, pthread_create(&t, nullptr, IdFn, nullptr));
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

TEST(pthread, pthread_create) {
  void* expected_result = reinterpret_cast<void*>(123);
  // Can we create a thread?
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, IdFn, expected_result));
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
  ASSERT_EQ(EAGAIN, pthread_create(&t, &attributes, IdFn, nullptr));
}

TEST(pthread, pthread_no_join_after_detach) {
  SpinFunctionHelper spin_helper;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, nullptr, spin_helper.GetFunction(), nullptr));

  // After a pthread_detach...
  ASSERT_EQ(0, pthread_detach(t1));
  AssertDetached(t1, true);

  // ...pthread_join should fail.
  ASSERT_EQ(EINVAL, pthread_join(t1, nullptr));
}

TEST(pthread, pthread_no_op_detach_after_join) {
  SpinFunctionHelper spin_helper;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, nullptr, spin_helper.GetFunction(), nullptr));

  // If thread 2 is already waiting to join thread 1...
  pthread_t t2;
  ASSERT_EQ(0, pthread_create(&t2, nullptr, JoinFn, reinterpret_cast<void*>(t1)));

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
  ASSERT_EQ(EDEADLK, pthread_join(pthread_self(), nullptr));
}

struct TestBug37410 {
  pthread_t main_thread;
  pthread_mutex_t mutex;

  static void main() {
    TestBug37410 data;
    data.main_thread = pthread_self();
    ASSERT_EQ(0, pthread_mutex_init(&data.mutex, nullptr));
    ASSERT_EQ(0, pthread_mutex_lock(&data.mutex));

    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, nullptr, TestBug37410::thread_fn, reinterpret_cast<void*>(&data)));

    // Wait for the thread to be running...
    ASSERT_EQ(0, pthread_mutex_lock(&data.mutex));
    ASSERT_EQ(0, pthread_mutex_unlock(&data.mutex));

    // ...and exit.
    pthread_exit(nullptr);
  }

 private:
  static void* thread_fn(void* arg) {
    TestBug37410* data = reinterpret_cast<TestBug37410*>(arg);

    // Unlocking data->mutex will cause the main thread to exit, invalidating *data. Save the handle.
    pthread_t main_thread = data->main_thread;

    // Let the main thread know we're running.
    pthread_mutex_unlock(&data->mutex);

    // And wait for the main thread to exit.
    pthread_join(main_thread, nullptr);

    return nullptr;
  }
};

// Even though this isn't really a death test, we have to say "DeathTest" here so gtest knows to
// run this test (which exits normally) in its own process.
TEST_F(pthread_DeathTest, pthread_bug_37410) {
  // http://code.google.com/p/android/issues/detail?id=37410
  ASSERT_EXIT(TestBug37410::main(), ::testing::ExitedWithCode(0), "");
}

static void* SignalHandlerFn(void* arg) {
  sigset64_t wait_set;
  sigfillset64(&wait_set);
  return reinterpret_cast<void*>(sigwait64(&wait_set, reinterpret_cast<int*>(arg)));
}

TEST(pthread, pthread_sigmask) {
  // Check that SIGUSR1 isn't blocked.
  sigset_t original_set;
  sigemptyset(&original_set);
  ASSERT_EQ(0, pthread_sigmask(SIG_BLOCK, nullptr, &original_set));
  ASSERT_FALSE(sigismember(&original_set, SIGUSR1));

  // Block SIGUSR1.
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  ASSERT_EQ(0, pthread_sigmask(SIG_BLOCK, &set, nullptr));

  // Check that SIGUSR1 is blocked.
  sigset_t final_set;
  sigemptyset(&final_set);
  ASSERT_EQ(0, pthread_sigmask(SIG_BLOCK, nullptr, &final_set));
  ASSERT_TRUE(sigismember(&final_set, SIGUSR1));
  // ...and that sigprocmask agrees with pthread_sigmask.
  sigemptyset(&final_set);
  ASSERT_EQ(0, sigprocmask(SIG_BLOCK, nullptr, &final_set));
  ASSERT_TRUE(sigismember(&final_set, SIGUSR1));

  // Spawn a thread that calls sigwait and tells us what it received.
  pthread_t signal_thread;
  int received_signal = -1;
  ASSERT_EQ(0, pthread_create(&signal_thread, nullptr, SignalHandlerFn, &received_signal));

  // Send that thread SIGUSR1.
  pthread_kill(signal_thread, SIGUSR1);

  // See what it got.
  void* join_result;
  ASSERT_EQ(0, pthread_join(signal_thread, &join_result));
  ASSERT_EQ(SIGUSR1, received_signal);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(join_result));

  // Restore the original signal mask.
  ASSERT_EQ(0, pthread_sigmask(SIG_SETMASK, &original_set, nullptr));
}

TEST(pthread, pthread_sigmask64_SIGTRMIN) {
  // Check that SIGRTMIN isn't blocked.
  sigset64_t original_set;
  sigemptyset64(&original_set);
  ASSERT_EQ(0, pthread_sigmask64(SIG_BLOCK, nullptr, &original_set));
  ASSERT_FALSE(sigismember64(&original_set, SIGRTMIN));

  // Block SIGRTMIN.
  sigset64_t set;
  sigemptyset64(&set);
  sigaddset64(&set, SIGRTMIN);
  ASSERT_EQ(0, pthread_sigmask64(SIG_BLOCK, &set, nullptr));

  // Check that SIGRTMIN is blocked.
  sigset64_t final_set;
  sigemptyset64(&final_set);
  ASSERT_EQ(0, pthread_sigmask64(SIG_BLOCK, nullptr, &final_set));
  ASSERT_TRUE(sigismember64(&final_set, SIGRTMIN));
  // ...and that sigprocmask64 agrees with pthread_sigmask64.
  sigemptyset64(&final_set);
  ASSERT_EQ(0, sigprocmask64(SIG_BLOCK, nullptr, &final_set));
  ASSERT_TRUE(sigismember64(&final_set, SIGRTMIN));

  // Spawn a thread that calls sigwait64 and tells us what it received.
  pthread_t signal_thread;
  int received_signal = -1;
  ASSERT_EQ(0, pthread_create(&signal_thread, nullptr, SignalHandlerFn, &received_signal));

  // Send that thread SIGRTMIN.
  pthread_kill(signal_thread, SIGRTMIN);

  // See what it got.
  void* join_result;
  ASSERT_EQ(0, pthread_join(signal_thread, &join_result));
  ASSERT_EQ(SIGRTMIN, received_signal);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(join_result));

  // Restore the original signal mask.
  ASSERT_EQ(0, pthread_sigmask64(SIG_SETMASK, &original_set, nullptr));
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

  EXPECT_DEATH(pthread_setname_np(dead_thread, "short 3"),
               "invalid pthread_t (.*) passed to pthread_setname_np");
}

TEST_F(pthread_DeathTest, pthread_setname_np__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ENOENT, pthread_setname_np(null_thread, "short 3"));
}

TEST_F(pthread_DeathTest, pthread_getname_np__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  char name[64];
  EXPECT_DEATH(pthread_getname_np(dead_thread, name, sizeof(name)),
               "invalid pthread_t (.*) passed to pthread_getname_np");
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

TEST(pthread, pthread_kill__exited_thread) {
  static std::promise<pid_t> tid_promise;
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr,
                              [](void*) -> void* {
                                tid_promise.set_value(gettid());
                                return nullptr;
                              },
                              nullptr));

  pid_t tid = tid_promise.get_future().get();
  while (TEMP_FAILURE_RETRY(syscall(__NR_tgkill, getpid(), tid, 0)) != -1) {
    continue;
  }
  ASSERT_ERRNO(ESRCH);

  ASSERT_EQ(ESRCH, pthread_kill(thread, 0));
}

TEST_F(pthread_DeathTest, pthread_detach__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_detach(dead_thread),
               "invalid pthread_t (.*) passed to pthread_detach");
}

TEST_F(pthread_DeathTest, pthread_detach__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ESRCH, pthread_detach(null_thread));
}

TEST(pthread, pthread_getcpuclockid__clock_gettime) {
  SpinFunctionHelper spin_helper;

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, spin_helper.GetFunction(), nullptr));

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
  EXPECT_DEATH(pthread_getcpuclockid(dead_thread, &c),
               "invalid pthread_t (.*) passed to pthread_getcpuclockid");
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
  EXPECT_DEATH(pthread_getschedparam(dead_thread, &policy, &param),
               "invalid pthread_t (.*) passed to pthread_getschedparam");
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
  EXPECT_DEATH(pthread_setschedparam(dead_thread, policy, &param),
               "invalid pthread_t (.*) passed to pthread_setschedparam");
}

TEST_F(pthread_DeathTest, pthread_setschedparam__null_thread) {
  pthread_t null_thread = 0;
  int policy = 0;
  sched_param param;
  EXPECT_EQ(ESRCH, pthread_setschedparam(null_thread, policy, &param));
}

TEST_F(pthread_DeathTest, pthread_setschedprio__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_setschedprio(dead_thread, 123),
               "invalid pthread_t (.*) passed to pthread_setschedprio");
}

TEST_F(pthread_DeathTest, pthread_setschedprio__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ESRCH, pthread_setschedprio(null_thread, 123));
}

TEST_F(pthread_DeathTest, pthread_join__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_join(dead_thread, nullptr),
               "invalid pthread_t (.*) passed to pthread_join");
}

TEST_F(pthread_DeathTest, pthread_join__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ESRCH, pthread_join(null_thread, nullptr));
}

TEST_F(pthread_DeathTest, pthread_kill__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  EXPECT_DEATH(pthread_kill(dead_thread, 0),
               "invalid pthread_t (.*) passed to pthread_kill");
}

TEST_F(pthread_DeathTest, pthread_kill__null_thread) {
  pthread_t null_thread = 0;
  EXPECT_EQ(ESRCH, pthread_kill(null_thread, 0));
}

TEST(pthread, pthread_join__multijoin) {
  SpinFunctionHelper spin_helper;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, nullptr, spin_helper.GetFunction(), nullptr));

  pthread_t t2;
  ASSERT_EQ(0, pthread_create(&t2, nullptr, JoinFn, reinterpret_cast<void*>(t1)));

  sleep(1); // (Give t2 a chance to call pthread_join.)

  // Multiple joins to the same thread should fail.
  ASSERT_EQ(EINVAL, pthread_join(t1, nullptr));

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
    void* stack = mmap(nullptr, stack_size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

    pthread_attr_t a;
    pthread_attr_init(&a);
    pthread_attr_setstack(&a, stack, stack_size);

    pthread_t t;
    ASSERT_EQ(0, pthread_create(&t, &a, IdFn, nullptr));
    ASSERT_EQ(0, pthread_join(t, nullptr));
    ASSERT_EQ(0, munmap(stack, stack_size));
  }
}

static void* GetActualGuardSizeFn(void* arg) {
  pthread_attr_t attributes;
  pthread_getattr_np(pthread_self(), &attributes);
  pthread_attr_getguardsize(&attributes, reinterpret_cast<size_t*>(arg));
  return nullptr;
}

static size_t GetActualGuardSize(const pthread_attr_t& attributes) {
  size_t result;
  pthread_t t;
  pthread_create(&t, &attributes, GetActualGuardSizeFn, &result);
  pthread_join(t, nullptr);
  return result;
}

static void* GetActualStackSizeFn(void* arg) {
  pthread_attr_t attributes;
  pthread_getattr_np(pthread_self(), &attributes);
  pthread_attr_getstacksize(&attributes, reinterpret_cast<size_t*>(arg));
  return nullptr;
}

static size_t GetActualStackSize(const pthread_attr_t& attributes) {
  size_t result;
  pthread_t t;
  pthread_create(&t, &attributes, GetActualStackSizeFn, &result);
  pthread_join(t, nullptr);
  return result;
}

TEST(pthread, pthread_attr_setguardsize_tiny) {
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));

  // No such thing as too small: will be rounded up to one page by pthread_create.
  ASSERT_EQ(0, pthread_attr_setguardsize(&attributes, 128));
  size_t guard_size;
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(128U, guard_size);
  ASSERT_EQ(static_cast<unsigned long>(getpagesize()), GetActualGuardSize(attributes));
}

TEST(pthread, pthread_attr_setguardsize_reasonable) {
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));

  // Large enough and a multiple of the page size.
  ASSERT_EQ(0, pthread_attr_setguardsize(&attributes, 32*1024));
  size_t guard_size;
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(32*1024U, guard_size);
  ASSERT_EQ(32*1024U, GetActualGuardSize(attributes));
}

TEST(pthread, pthread_attr_setguardsize_needs_rounding) {
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));

  // Large enough but not a multiple of the page size.
  ASSERT_EQ(0, pthread_attr_setguardsize(&attributes, 32*1024 + 1));
  size_t guard_size;
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(32*1024U + 1, guard_size);
  ASSERT_EQ(roundup(32 * 1024U + 1, getpagesize()), GetActualGuardSize(attributes));
}

TEST(pthread, pthread_attr_setguardsize_enormous) {
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));

  // Larger than the stack itself. (Historically we mistakenly carved
  // the guard out of the stack itself, rather than adding it after the
  // end.)
  ASSERT_EQ(0, pthread_attr_setguardsize(&attributes, 32*1024*1024));
  size_t guard_size;
  ASSERT_EQ(0, pthread_attr_getguardsize(&attributes, &guard_size));
  ASSERT_EQ(32*1024*1024U, guard_size);
  ASSERT_EQ(32*1024*1024U, GetActualGuardSize(attributes));
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

#if !defined(ANDROID_HOST_MUSL)
  // musl doesn't have pthread_rwlockattr_setkind_np
  int kind_array[] = {PTHREAD_RWLOCK_PREFER_READER_NP,
                      PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP};
  for (size_t i = 0; i < sizeof(kind_array) / sizeof(kind_array[0]); ++i) {
    ASSERT_EQ(0, pthread_rwlockattr_setkind_np(&attr, kind_array[i]));
    int kind;
    ASSERT_EQ(0, pthread_rwlockattr_getkind_np(&attr, &kind));
    ASSERT_EQ(kind_array[i], kind);
  }
#endif

  ASSERT_EQ(0, pthread_rwlockattr_destroy(&attr));
}

TEST(pthread, pthread_rwlock_init_same_as_PTHREAD_RWLOCK_INITIALIZER) {
  pthread_rwlock_t lock1 = PTHREAD_RWLOCK_INITIALIZER;
  pthread_rwlock_t lock2;
  ASSERT_EQ(0, pthread_rwlock_init(&lock2, nullptr));
  ASSERT_EQ(0, memcmp(&lock1, &lock2, sizeof(lock1)));
}

TEST(pthread, pthread_rwlock_smoke) {
  pthread_rwlock_t l;
  ASSERT_EQ(0, pthread_rwlock_init(&l, nullptr));

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
  clockid_t clock;
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
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, nullptr));
  ASSERT_EQ(0, pthread_rwlock_rdlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = &pthread_rwlock_trywrlock;
  wakeup_arg.lock_function = lock_function;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr,
    reinterpret_cast<void* (*)(void*)>(pthread_rwlock_wakeup_helper), &wakeup_arg));
  WaitUntilThreadSleep(wakeup_arg.tid);
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, wakeup_arg.progress);

  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_RELEASED;
  ASSERT_EQ(0, pthread_rwlock_unlock(&wakeup_arg.lock));

  ASSERT_EQ(0, pthread_join(thread, nullptr));
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

TEST(pthread, pthread_rwlock_reader_wakeup_writer_timedwait_monotonic_np) {
#if defined(__BIONIC__)
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_reader_wakeup_writer(
      [&](pthread_rwlock_t* lock) { return pthread_rwlock_timedwrlock_monotonic_np(lock, &ts); });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_timedwrlock_monotonic_np not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_reader_wakeup_writer_clockwait) {
#if defined(__BIONIC__)
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_reader_wakeup_writer([&](pthread_rwlock_t* lock) {
    return pthread_rwlock_clockwrlock(lock, CLOCK_MONOTONIC, &ts);
  });

  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_reader_wakeup_writer([&](pthread_rwlock_t* lock) {
    return pthread_rwlock_clockwrlock(lock, CLOCK_REALTIME, &ts);
  });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockwrlock not available";
#endif  // __BIONIC__
}

static void test_pthread_rwlock_writer_wakeup_reader(std::function<int (pthread_rwlock_t*)> lock_function) {
  RwlockWakeupHelperArg wakeup_arg;
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, nullptr));
  ASSERT_EQ(0, pthread_rwlock_wrlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = &pthread_rwlock_tryrdlock;
  wakeup_arg.lock_function = lock_function;

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr,
    reinterpret_cast<void* (*)(void*)>(pthread_rwlock_wakeup_helper), &wakeup_arg));
  WaitUntilThreadSleep(wakeup_arg.tid);
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, wakeup_arg.progress);

  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_RELEASED;
  ASSERT_EQ(0, pthread_rwlock_unlock(&wakeup_arg.lock));

  ASSERT_EQ(0, pthread_join(thread, nullptr));
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

TEST(pthread, pthread_rwlock_writer_wakeup_reader_timedwait_monotonic_np) {
#if defined(__BIONIC__)
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_writer_wakeup_reader(
      [&](pthread_rwlock_t* lock) { return pthread_rwlock_timedrdlock_monotonic_np(lock, &ts); });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_timedrdlock_monotonic_np not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_writer_wakeup_reader_clockwait) {
#if defined(__BIONIC__)
  timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_writer_wakeup_reader([&](pthread_rwlock_t* lock) {
    return pthread_rwlock_clockrdlock(lock, CLOCK_MONOTONIC, &ts);
  });

  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ts.tv_sec += 1;
  test_pthread_rwlock_writer_wakeup_reader([&](pthread_rwlock_t* lock) {
    return pthread_rwlock_clockrdlock(lock, CLOCK_REALTIME, &ts);
  });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockrdlock not available";
#endif  // __BIONIC__
}

static void pthread_rwlock_wakeup_timeout_helper(RwlockWakeupHelperArg* arg) {
  arg->tid = gettid();
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_INITIALIZED, arg->progress);
  arg->progress = RwlockWakeupHelperArg::LOCK_WAITING;

  ASSERT_EQ(EBUSY, arg->trylock_function(&arg->lock));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(arg->clock, &ts));
  ASSERT_EQ(ETIMEDOUT, arg->timed_lock_function(&arg->lock, &ts));
  ts.tv_nsec = -1;
  ASSERT_EQ(EINVAL, arg->timed_lock_function(&arg->lock, &ts));
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(EINVAL, arg->timed_lock_function(&arg->lock, &ts));
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(ETIMEDOUT, arg->timed_lock_function(&arg->lock, &ts));
  ASSERT_EQ(0, clock_gettime(arg->clock, &ts));
  ts.tv_sec += 1;
  ASSERT_EQ(ETIMEDOUT, arg->timed_lock_function(&arg->lock, &ts));
  ASSERT_EQ(RwlockWakeupHelperArg::LOCK_WAITING, arg->progress);
  arg->progress = RwlockWakeupHelperArg::LOCK_TIMEDOUT;
}

static void pthread_rwlock_timedrdlock_timeout_helper(
    clockid_t clock, int (*lock_function)(pthread_rwlock_t* __rwlock, const timespec* __timeout)) {
  RwlockWakeupHelperArg wakeup_arg;
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, nullptr));
  ASSERT_EQ(0, pthread_rwlock_wrlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = &pthread_rwlock_tryrdlock;
  wakeup_arg.timed_lock_function = lock_function;
  wakeup_arg.clock = clock;

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

TEST(pthread, pthread_rwlock_timedrdlock_timeout) {
  pthread_rwlock_timedrdlock_timeout_helper(CLOCK_REALTIME, pthread_rwlock_timedrdlock);
}

TEST(pthread, pthread_rwlock_timedrdlock_monotonic_np_timeout) {
#if defined(__BIONIC__)
  pthread_rwlock_timedrdlock_timeout_helper(CLOCK_MONOTONIC,
                                            pthread_rwlock_timedrdlock_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_timedrdlock_monotonic_np not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_clockrdlock_monotonic_timeout) {
#if defined(__BIONIC__)
  pthread_rwlock_timedrdlock_timeout_helper(
      CLOCK_MONOTONIC, [](pthread_rwlock_t* __rwlock, const timespec* __timeout) {
        return pthread_rwlock_clockrdlock(__rwlock, CLOCK_MONOTONIC, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockrdlock not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_clockrdlock_realtime_timeout) {
#if defined(__BIONIC__)
  pthread_rwlock_timedrdlock_timeout_helper(
      CLOCK_REALTIME, [](pthread_rwlock_t* __rwlock, const timespec* __timeout) {
        return pthread_rwlock_clockrdlock(__rwlock, CLOCK_REALTIME, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockrdlock not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_clockrdlock_invalid) {
#if defined(__BIONIC__)
  pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
  timespec ts;
  EXPECT_EQ(EINVAL, pthread_rwlock_clockrdlock(&lock, CLOCK_PROCESS_CPUTIME_ID, &ts));
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockrdlock not available";
#endif  // __BIONIC__
}

static void pthread_rwlock_timedwrlock_timeout_helper(
    clockid_t clock, int (*lock_function)(pthread_rwlock_t* __rwlock, const timespec* __timeout)) {
  RwlockWakeupHelperArg wakeup_arg;
  ASSERT_EQ(0, pthread_rwlock_init(&wakeup_arg.lock, nullptr));
  ASSERT_EQ(0, pthread_rwlock_rdlock(&wakeup_arg.lock));
  wakeup_arg.progress = RwlockWakeupHelperArg::LOCK_INITIALIZED;
  wakeup_arg.tid = 0;
  wakeup_arg.trylock_function = &pthread_rwlock_trywrlock;
  wakeup_arg.timed_lock_function = lock_function;
  wakeup_arg.clock = clock;

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
  pthread_rwlock_timedwrlock_timeout_helper(CLOCK_REALTIME, pthread_rwlock_timedwrlock);
}

TEST(pthread, pthread_rwlock_timedwrlock_monotonic_np_timeout) {
#if defined(__BIONIC__)
  pthread_rwlock_timedwrlock_timeout_helper(CLOCK_MONOTONIC,
                                            pthread_rwlock_timedwrlock_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_timedwrlock_monotonic_np not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_clockwrlock_monotonic_timeout) {
#if defined(__BIONIC__)
  pthread_rwlock_timedwrlock_timeout_helper(
      CLOCK_MONOTONIC, [](pthread_rwlock_t* __rwlock, const timespec* __timeout) {
        return pthread_rwlock_clockwrlock(__rwlock, CLOCK_MONOTONIC, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockwrlock not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_clockwrlock_realtime_timeout) {
#if defined(__BIONIC__)
  pthread_rwlock_timedwrlock_timeout_helper(
      CLOCK_REALTIME, [](pthread_rwlock_t* __rwlock, const timespec* __timeout) {
        return pthread_rwlock_clockwrlock(__rwlock, CLOCK_REALTIME, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockwrlock not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_rwlock_clockwrlock_invalid) {
#if defined(__BIONIC__)
  pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
  timespec ts;
  EXPECT_EQ(EINVAL, pthread_rwlock_clockwrlock(&lock, CLOCK_PROCESS_CPUTIME_ID, &ts));
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_rwlock_clockrwlock not available";
#endif  // __BIONIC__
}

#if !defined(ANDROID_HOST_MUSL)
// musl doesn't have pthread_rwlockattr_setkind_np
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
    ASSERT_EQ(0, pthread_create(&thread, nullptr,
                                reinterpret_cast<void* (*)(void*)>(WriterThreadFn), arg));
  }

  void CreateReaderThread(pthread_t& thread, std::atomic<pid_t>& tid) {
    tid = 0;
    ThreadArg* arg = new ThreadArg(this, tid);
    ASSERT_EQ(0, pthread_create(&thread, nullptr,
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
#endif

TEST(pthread, pthread_rwlock_kind_PTHREAD_RWLOCK_PREFER_READER_NP) {
#if !defined(ANDROID_HOST_MUSL)
  RwlockKindTestHelper helper(PTHREAD_RWLOCK_PREFER_READER_NP);
  ASSERT_EQ(0, pthread_rwlock_rdlock(&helper.lock));

  pthread_t writer_thread;
  std::atomic<pid_t> writer_tid;
  helper.CreateWriterThread(writer_thread, writer_tid);
  WaitUntilThreadSleep(writer_tid);

  pthread_t reader_thread;
  std::atomic<pid_t> reader_tid;
  helper.CreateReaderThread(reader_thread, reader_tid);
  ASSERT_EQ(0, pthread_join(reader_thread, nullptr));

  ASSERT_EQ(0, pthread_rwlock_unlock(&helper.lock));
  ASSERT_EQ(0, pthread_join(writer_thread, nullptr));
#else
  GTEST_SKIP() << "musl doesn't have pthread_rwlockattr_setkind_np";
#endif
}

TEST(pthread, pthread_rwlock_kind_PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP) {
#if !defined(ANDROID_HOST_MUSL)
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
  ASSERT_EQ(0, pthread_join(writer_thread, nullptr));
  ASSERT_EQ(0, pthread_join(reader_thread, nullptr));
#else
  GTEST_SKIP() << "musl doesn't have pthread_rwlockattr_setkind_np";
#endif
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

TEST(pthread, pthread_atfork_smoke_fork) {
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare1, AtForkParent1, AtForkChild1));
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare2, AtForkParent2, AtForkChild2));

  g_atfork_prepare_calls = g_atfork_parent_calls = g_atfork_child_calls = 0;
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

TEST(pthread, pthread_atfork_smoke_vfork) {
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare1, AtForkParent1, AtForkChild1));
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare2, AtForkParent2, AtForkChild2));

  g_atfork_prepare_calls = g_atfork_parent_calls = g_atfork_child_calls = 0;
  pid_t pid = vfork();
  ASSERT_NE(-1, pid) << strerror(errno);

  // atfork handlers are not called.
  if (pid == 0) {
    ASSERT_EQ(0, g_atfork_child_calls);
    _exit(0);
  }
  ASSERT_EQ(0, g_atfork_parent_calls);
  ASSERT_EQ(0, g_atfork_prepare_calls);
  AssertChildExited(pid, 0);
}

TEST(pthread, pthread_atfork_smoke__Fork) {
#if defined(__BIONIC__)
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare1, AtForkParent1, AtForkChild1));
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare2, AtForkParent2, AtForkChild2));

  g_atfork_prepare_calls = g_atfork_parent_calls = g_atfork_child_calls = 0;
  pid_t pid = _Fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  // atfork handlers are not called.
  if (pid == 0) {
    ASSERT_EQ(0, g_atfork_child_calls);
    _exit(0);
  }
  ASSERT_EQ(0, g_atfork_parent_calls);
  ASSERT_EQ(0, g_atfork_prepare_calls);
  AssertChildExited(pid, 0);
#endif
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
  GTEST_SKIP() << "bionic-only test";
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
  timespec ts;
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

  void StartWaitingThread(
      std::function<int(pthread_cond_t* cond, pthread_mutex_t* mutex)> wait_function) {
    progress = INITIALIZED;
    this->wait_function = wait_function;
    ASSERT_EQ(0, pthread_create(&thread, nullptr, reinterpret_cast<void* (*)(void*)>(WaitThreadFn),
                                this));
    while (progress != WAITING) {
      usleep(5000);
    }
    usleep(5000);
  }

  void RunTimedTest(
      clockid_t clock,
      std::function<int(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* timeout)>
          wait_function) {
    ASSERT_EQ(0, clock_gettime(clock, &ts));
    ts.tv_sec += 1;

    StartWaitingThread([&wait_function, this](pthread_cond_t* cond, pthread_mutex_t* mutex) {
      return wait_function(cond, mutex, &ts);
    });

    progress = SIGNALED;
    ASSERT_EQ(0, pthread_cond_signal(&cond));
  }

  void RunTimedTest(clockid_t clock, std::function<int(pthread_cond_t* cond, pthread_mutex_t* mutex,
                                                       clockid_t clock, const timespec* timeout)>
                                         wait_function) {
    RunTimedTest(clock, [clock, &wait_function](pthread_cond_t* cond, pthread_mutex_t* mutex,
                                                const timespec* timeout) {
      return wait_function(cond, mutex, clock, timeout);
    });
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
  RunTimedTest(CLOCK_REALTIME, pthread_cond_timedwait);
}

TEST_F(pthread_CondWakeupTest, signal_timedwait_CLOCK_MONOTONIC) {
  InitCond(CLOCK_MONOTONIC);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_timedwait);
}

TEST_F(pthread_CondWakeupTest, signal_timedwait_CLOCK_MONOTONIC_np) {
#if defined(__BIONIC__)
  InitCond(CLOCK_REALTIME);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_timedwait_monotonic_np not available";
#endif  // __BIONIC__
}

TEST_F(pthread_CondWakeupTest, signal_clockwait_monotonic_monotonic) {
#if defined(__BIONIC__)
  InitCond(CLOCK_MONOTONIC);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

TEST_F(pthread_CondWakeupTest, signal_clockwait_monotonic_realtime) {
#if defined(__BIONIC__)
  InitCond(CLOCK_MONOTONIC);
  RunTimedTest(CLOCK_REALTIME, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

TEST_F(pthread_CondWakeupTest, signal_clockwait_realtime_monotonic) {
#if defined(__BIONIC__)
  InitCond(CLOCK_REALTIME);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

TEST_F(pthread_CondWakeupTest, signal_clockwait_realtime_realtime) {
#if defined(__BIONIC__)
  InitCond(CLOCK_REALTIME);
  RunTimedTest(CLOCK_REALTIME, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

static void pthread_cond_timedwait_timeout_helper(bool init_monotonic, clockid_t clock,
                                                  int (*wait_function)(pthread_cond_t* __cond,
                                                                       pthread_mutex_t* __mutex,
                                                                       const timespec* __timeout)) {
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, nullptr));
  pthread_cond_t cond;

  if (init_monotonic) {
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);

    ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
    clockid_t clock;
    ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
    ASSERT_EQ(CLOCK_MONOTONIC, clock);

    ASSERT_EQ(0, pthread_cond_init(&cond, &attr));
  } else {
    ASSERT_EQ(0, pthread_cond_init(&cond, nullptr));
  }
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(clock, &ts));
  ASSERT_EQ(ETIMEDOUT, wait_function(&cond, &mutex, &ts));
  ts.tv_nsec = -1;
  ASSERT_EQ(EINVAL, wait_function(&cond, &mutex, &ts));
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(EINVAL, wait_function(&cond, &mutex, &ts));
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(ETIMEDOUT, wait_function(&cond, &mutex, &ts));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
}

TEST(pthread, pthread_cond_timedwait_timeout) {
  pthread_cond_timedwait_timeout_helper(false, CLOCK_REALTIME, pthread_cond_timedwait);
}

TEST(pthread, pthread_cond_timedwait_monotonic_np_timeout) {
#if defined(__BIONIC__)
  pthread_cond_timedwait_timeout_helper(false, CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
  pthread_cond_timedwait_timeout_helper(true, CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_timedwait_monotonic_np not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_cond_clockwait_timeout) {
#if defined(__BIONIC__)
  pthread_cond_timedwait_timeout_helper(
      false, CLOCK_MONOTONIC,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_MONOTONIC, __timeout);
      });
  pthread_cond_timedwait_timeout_helper(
      true, CLOCK_MONOTONIC,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_MONOTONIC, __timeout);
      });
  pthread_cond_timedwait_timeout_helper(
      false, CLOCK_REALTIME,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_REALTIME, __timeout);
      });
  pthread_cond_timedwait_timeout_helper(
      true, CLOCK_REALTIME,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_REALTIME, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_cond_clockwait_invalid) {
#if defined(__BIONIC__)
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  timespec ts;
  EXPECT_EQ(EINVAL, pthread_cond_clockwait(&cond, &mutex, CLOCK_PROCESS_CPUTIME_ID, &ts));

#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
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
  // Find stack in /proc/self/maps using a pointer to the stack.
  //
  // We do not use "[stack]" label because in native-bridge environment it is not
  // guaranteed to point to the right stack. A native bridge implementation may
  // keep separate stack for the guest code.
  void* maps_stack_hi = nullptr;
  std::vector<map_record> maps;
  ASSERT_TRUE(Maps::parse_maps(&maps));
  uintptr_t stack_address = reinterpret_cast<uintptr_t>(untag_address(&maps_stack_hi));
  for (const auto& map : maps) {
    if (map.addr_start <= stack_address && map.addr_end > stack_address){
      maps_stack_hi = reinterpret_cast<void*>(map.addr_end);
      break;
    }
  }

  // The high address of the /proc/self/maps stack region should equal stack_base + stack_size.
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

  auto guard = android::base::make_scope_guard([&rl, original_rlim_cur]() {
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
  ASSERT_LT(static_cast<void*>(untag_address(&attr)),
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
  void* sig_stack = mmap(nullptr, sig_stack_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
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
  ASSERT_LT(untag_address(&local_variable), reinterpret_cast<char*>(stack_base) + stack_size);
}

// Check whether something on stack is in the range of
// [stack_base, stack_base + stack_size). see b/18908062.
TEST(pthread, pthread_attr_getstack_18908062) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr,
            reinterpret_cast<void* (*)(void*)>(pthread_attr_getstack_18908062_helper),
            nullptr));
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

#if defined(__BIONIC__)
static pthread_mutex_t pthread_gettid_np_mutex = PTHREAD_MUTEX_INITIALIZER;

static void* pthread_gettid_np_helper(void* arg) {
  *reinterpret_cast<pid_t*>(arg) = gettid();

  // Wait for our parent to call pthread_gettid_np on us before exiting.
  pthread_mutex_lock(&pthread_gettid_np_mutex);
  pthread_mutex_unlock(&pthread_gettid_np_mutex);
  return nullptr;
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
  pthread_create(&t, nullptr, pthread_gettid_np_helper, &t_gettid_result);

  pid_t t_pthread_gettid_np_result = pthread_gettid_np(t);

  // Release the other thread and wait for it to exit.
  pthread_mutex_unlock(&pthread_gettid_np_mutex);
  ASSERT_EQ(0, pthread_join(t, nullptr));

  ASSERT_EQ(t_gettid_result, t_pthread_gettid_np_result);
#else
  GTEST_SKIP() << "pthread_gettid_np not available";
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
  pthread_cleanup_push(CountCleanupRoutine, nullptr);
  pthread_cleanup_push(CountCleanupRoutine, nullptr);
  pthread_cleanup_push(AbortCleanupRoutine, nullptr);

  pthread_cleanup_pop(0); // Pop the abort without executing it.
  pthread_cleanup_pop(1); // Pop one count while executing it.
  ASSERT_EQ(1U, cleanup_counter);
  // Exit while the other count is still on the cleanup stack.
  pthread_exit(nullptr);

  // Calls to pthread_cleanup_pop/pthread_cleanup_push must always be balanced.
  pthread_cleanup_pop(0);
}

static void* PthreadCleanupStartRoutine(void*) {
  PthreadCleanupTester();
  return nullptr;
}

TEST(pthread, pthread_cleanup_push__pthread_cleanup_pop) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, PthreadCleanupStartRoutine, nullptr));
  ASSERT_EQ(0, pthread_join(t, nullptr));
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

TEST(pthread, pthread_mutexattr_protocol) {
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));

  int protocol;
  ASSERT_EQ(0, pthread_mutexattr_getprotocol(&attr, &protocol));
  ASSERT_EQ(PTHREAD_PRIO_NONE, protocol);
  for (size_t repeat = 0; repeat < 2; ++repeat) {
    for (int set_protocol : {PTHREAD_PRIO_NONE, PTHREAD_PRIO_INHERIT}) {
      ASSERT_EQ(0, pthread_mutexattr_setprotocol(&attr, set_protocol));
      ASSERT_EQ(0, pthread_mutexattr_getprotocol(&attr, &protocol));
      ASSERT_EQ(protocol, set_protocol);
    }
  }
}

struct PthreadMutex {
  pthread_mutex_t lock;

  explicit PthreadMutex(int mutex_type, int protocol = PTHREAD_PRIO_NONE) {
    init(mutex_type, protocol);
  }

  ~PthreadMutex() {
    destroy();
  }

 private:
  void init(int mutex_type, int protocol) {
    pthread_mutexattr_t attr;
    ASSERT_EQ(0, pthread_mutexattr_init(&attr));
    ASSERT_EQ(0, pthread_mutexattr_settype(&attr, mutex_type));
    ASSERT_EQ(0, pthread_mutexattr_setprotocol(&attr, protocol));
    ASSERT_EQ(0, pthread_mutex_init(&lock, &attr));
    ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
  }

  void destroy() {
    ASSERT_EQ(0, pthread_mutex_destroy(&lock));
  }

  DISALLOW_COPY_AND_ASSIGN(PthreadMutex);
};

static int UnlockFromAnotherThread(pthread_mutex_t* mutex) {
  pthread_t thread;
  pthread_create(&thread, nullptr, [](void* mutex_voidp) -> void* {
    pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(mutex_voidp);
    intptr_t result = pthread_mutex_unlock(mutex);
    return reinterpret_cast<void*>(result);
  }, mutex);
  void* result;
  EXPECT_EQ(0, pthread_join(thread, &result));
  return reinterpret_cast<intptr_t>(result);
};

static void TestPthreadMutexLockNormal(int protocol) {
  PthreadMutex m(PTHREAD_MUTEX_NORMAL, protocol);

  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  if (protocol == PTHREAD_PRIO_INHERIT) {
    ASSERT_EQ(EPERM, UnlockFromAnotherThread(&m.lock));
  }
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(EBUSY, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
}

static void TestPthreadMutexLockErrorCheck(int protocol) {
  PthreadMutex m(PTHREAD_MUTEX_ERRORCHECK, protocol);

  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(EPERM, UnlockFromAnotherThread(&m.lock));
  ASSERT_EQ(EDEADLK, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  if (protocol == PTHREAD_PRIO_NONE) {
    ASSERT_EQ(EBUSY, pthread_mutex_trylock(&m.lock));
  } else {
    ASSERT_EQ(EDEADLK, pthread_mutex_trylock(&m.lock));
  }
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(EPERM, pthread_mutex_unlock(&m.lock));
}

static void TestPthreadMutexLockRecursive(int protocol) {
  PthreadMutex m(PTHREAD_MUTEX_RECURSIVE, protocol);

  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(EPERM, UnlockFromAnotherThread(&m.lock));
  ASSERT_EQ(0, pthread_mutex_lock(&m.lock));
  ASSERT_EQ(EPERM, UnlockFromAnotherThread(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_trylock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
  ASSERT_EQ(EPERM, pthread_mutex_unlock(&m.lock));
}

TEST(pthread, pthread_mutex_lock_NORMAL) {
  TestPthreadMutexLockNormal(PTHREAD_PRIO_NONE);
}

TEST(pthread, pthread_mutex_lock_ERRORCHECK) {
  TestPthreadMutexLockErrorCheck(PTHREAD_PRIO_NONE);
}

TEST(pthread, pthread_mutex_lock_RECURSIVE) {
  TestPthreadMutexLockRecursive(PTHREAD_PRIO_NONE);
}

TEST(pthread, pthread_mutex_lock_pi) {
  TestPthreadMutexLockNormal(PTHREAD_PRIO_INHERIT);
  TestPthreadMutexLockErrorCheck(PTHREAD_PRIO_INHERIT);
  TestPthreadMutexLockRecursive(PTHREAD_PRIO_INHERIT);
}

TEST(pthread, pthread_mutex_pi_count_limit) {
#if defined(__BIONIC__) && !defined(__LP64__)
  // Bionic only supports 65536 pi mutexes in 32-bit programs.
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));
  ASSERT_EQ(0, pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT));
  std::vector<pthread_mutex_t> mutexes(65536);
  // Test if we can use 65536 pi mutexes at the same time.
  // Run 2 times to check if freed pi mutexes can be recycled.
  for (int repeat = 0; repeat < 2; ++repeat) {
    for (auto& m : mutexes) {
      ASSERT_EQ(0, pthread_mutex_init(&m, &attr));
    }
    pthread_mutex_t m;
    ASSERT_EQ(ENOMEM, pthread_mutex_init(&m, &attr));
    for (auto& m : mutexes) {
      ASSERT_EQ(0, pthread_mutex_lock(&m));
    }
    for (auto& m : mutexes) {
      ASSERT_EQ(0, pthread_mutex_unlock(&m));
    }
    for (auto& m : mutexes) {
      ASSERT_EQ(0, pthread_mutex_destroy(&m));
    }
  }
  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
#else
  GTEST_SKIP() << "pi mutex count not limited to 64Ki";
#endif
}

TEST(pthread, pthread_mutex_init_same_as_static_initializers) {
  pthread_mutex_t lock_normal = PTHREAD_MUTEX_INITIALIZER;
  PthreadMutex m1(PTHREAD_MUTEX_NORMAL);
  ASSERT_EQ(0, memcmp(&lock_normal, &m1.lock, sizeof(pthread_mutex_t)));
  pthread_mutex_destroy(&lock_normal);

#if !defined(ANDROID_HOST_MUSL)
  // musl doesn't support PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP or
  // PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP.
  pthread_mutex_t lock_errorcheck = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
  PthreadMutex m2(PTHREAD_MUTEX_ERRORCHECK);
  ASSERT_EQ(0, memcmp(&lock_errorcheck, &m2.lock, sizeof(pthread_mutex_t)));
  pthread_mutex_destroy(&lock_errorcheck);

  pthread_mutex_t lock_recursive = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
  PthreadMutex m3(PTHREAD_MUTEX_RECURSIVE);
  ASSERT_EQ(0, memcmp(&lock_recursive, &m3.lock, sizeof(pthread_mutex_t)));
  ASSERT_EQ(0, pthread_mutex_destroy(&lock_recursive));
#endif
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
    ASSERT_EQ(0, pthread_create(&thread, nullptr,
      reinterpret_cast<void* (*)(void*)>(MutexWakeupHelper::thread_fn), this));

    WaitUntilThreadSleep(tid);
    ASSERT_EQ(LOCK_WAITING, progress);

    progress = LOCK_RELEASED;
    ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));

    ASSERT_EQ(0, pthread_join(thread, nullptr));
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

static int GetThreadPriority(pid_t tid) {
  // sched_getparam() returns the static priority of a thread, which can't reflect a thread's
  // priority after priority inheritance. So read /proc/<pid>/stat to get the dynamic priority.
  std::string filename = android::base::StringPrintf("/proc/%d/stat", tid);
  std::string content;
  int result = INT_MAX;
  if (!android::base::ReadFileToString(filename, &content)) {
    return result;
  }
  std::vector<std::string> strs = android::base::Split(content, " ");
  if (strs.size() < 18) {
    return result;
  }
  if (!android::base::ParseInt(strs[17], &result)) {
    return INT_MAX;
  }
  return result;
}

class PIMutexWakeupHelper {
private:
  PthreadMutex m;
  int protocol;
  enum Progress {
    LOCK_INITIALIZED,
    LOCK_CHILD_READY,
    LOCK_WAITING,
    LOCK_RELEASED,
  };
  std::atomic<Progress> progress;
  std::atomic<pid_t> main_tid;
  std::atomic<pid_t> child_tid;
  PthreadMutex start_thread_m;

  static void thread_fn(PIMutexWakeupHelper* helper) {
    helper->child_tid = gettid();
    ASSERT_EQ(LOCK_INITIALIZED, helper->progress);
    ASSERT_EQ(0, setpriority(PRIO_PROCESS, gettid(), 1));
    ASSERT_EQ(21, GetThreadPriority(gettid()));
    ASSERT_EQ(0, pthread_mutex_lock(&helper->m.lock));
    helper->progress = LOCK_CHILD_READY;
    ASSERT_EQ(0, pthread_mutex_lock(&helper->start_thread_m.lock));

    ASSERT_EQ(0, pthread_mutex_unlock(&helper->start_thread_m.lock));
    WaitUntilThreadSleep(helper->main_tid);
    ASSERT_EQ(LOCK_WAITING, helper->progress);

    if (helper->protocol == PTHREAD_PRIO_INHERIT) {
      ASSERT_EQ(20, GetThreadPriority(gettid()));
    } else {
      ASSERT_EQ(21, GetThreadPriority(gettid()));
    }
    helper->progress = LOCK_RELEASED;
    ASSERT_EQ(0, pthread_mutex_unlock(&helper->m.lock));
  }

public:
  explicit PIMutexWakeupHelper(int mutex_type, int protocol)
      : m(mutex_type, protocol), protocol(protocol), start_thread_m(PTHREAD_MUTEX_NORMAL) {
  }

  void test() {
    ASSERT_EQ(0, pthread_mutex_lock(&start_thread_m.lock));
    main_tid = gettid();
    ASSERT_EQ(20, GetThreadPriority(main_tid));
    progress = LOCK_INITIALIZED;
    child_tid = 0;

    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, nullptr,
              reinterpret_cast<void* (*)(void*)>(PIMutexWakeupHelper::thread_fn), this));

    WaitUntilThreadSleep(child_tid);
    ASSERT_EQ(LOCK_CHILD_READY, progress);
    ASSERT_EQ(0, pthread_mutex_unlock(&start_thread_m.lock));
    progress = LOCK_WAITING;
    ASSERT_EQ(0, pthread_mutex_lock(&m.lock));

    ASSERT_EQ(LOCK_RELEASED, progress);
    ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
    ASSERT_EQ(0, pthread_join(thread, nullptr));
  }
};

TEST(pthread, pthread_mutex_pi_wakeup) {
  for (int type : {PTHREAD_MUTEX_NORMAL, PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_ERRORCHECK}) {
    for (int protocol : {PTHREAD_PRIO_INHERIT}) {
      PIMutexWakeupHelper helper(type, protocol);
      helper.test();
    }
  }
}

TEST(pthread, pthread_mutex_owner_tid_limit) {
#if defined(__BIONIC__) && !defined(__LP64__)
  FILE* fp = fopen("/proc/sys/kernel/pid_max", "r");
  ASSERT_TRUE(fp != nullptr);
  long pid_max;
  ASSERT_EQ(1, fscanf(fp, "%ld", &pid_max));
  fclose(fp);
  // Bionic's pthread_mutex implementation on 32-bit devices uses 16 bits to represent owner tid.
  ASSERT_LE(pid_max, 65536);
#else
  GTEST_SKIP() << "pthread_mutex supports 32-bit tid";
#endif
}

static void pthread_mutex_timedlock_helper(clockid_t clock,
                                           int (*lock_function)(pthread_mutex_t* __mutex,
                                                                const timespec* __timeout)) {
  pthread_mutex_t m;
  ASSERT_EQ(0, pthread_mutex_init(&m, nullptr));

  // If the mutex is already locked, pthread_mutex_timedlock should time out.
  ASSERT_EQ(0, pthread_mutex_lock(&m));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(clock, &ts));
  ASSERT_EQ(ETIMEDOUT, lock_function(&m, &ts));
  ts.tv_nsec = -1;
  ASSERT_EQ(EINVAL, lock_function(&m, &ts));
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(EINVAL, lock_function(&m, &ts));
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(ETIMEDOUT, lock_function(&m, &ts));

  // check we wait long enough for the lock.
  ASSERT_EQ(0, clock_gettime(clock, &ts));
  const int64_t start_ns = ts.tv_sec * NS_PER_S + ts.tv_nsec;

  // add a second to get deadline.
  ts.tv_sec += 1;

  ASSERT_EQ(ETIMEDOUT, lock_function(&m, &ts));

  // The timedlock must have waited at least 1 second before returning.
  clock_gettime(clock, &ts);
  const int64_t end_ns = ts.tv_sec * NS_PER_S + ts.tv_nsec;
  ASSERT_GT(end_ns - start_ns, NS_PER_S);

  // If the mutex is unlocked, pthread_mutex_timedlock should succeed.
  ASSERT_EQ(0, pthread_mutex_unlock(&m));

  ASSERT_EQ(0, clock_gettime(clock, &ts));
  ts.tv_sec += 1;
  ASSERT_EQ(0, lock_function(&m, &ts));

  ASSERT_EQ(0, pthread_mutex_unlock(&m));
  ASSERT_EQ(0, pthread_mutex_destroy(&m));
}

TEST(pthread, pthread_mutex_timedlock) {
  pthread_mutex_timedlock_helper(CLOCK_REALTIME, pthread_mutex_timedlock);
}

TEST(pthread, pthread_mutex_timedlock_monotonic_np) {
#if defined(__BIONIC__)
  pthread_mutex_timedlock_helper(CLOCK_MONOTONIC, pthread_mutex_timedlock_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_mutex_timedlock_monotonic_np not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_mutex_clocklock) {
#if defined(__BIONIC__)
  pthread_mutex_timedlock_helper(
      CLOCK_MONOTONIC, [](pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_mutex_clocklock(__mutex, CLOCK_MONOTONIC, __timeout);
      });
  pthread_mutex_timedlock_helper(
      CLOCK_REALTIME, [](pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_mutex_clocklock(__mutex, CLOCK_REALTIME, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_mutex_clocklock not available";
#endif  // __BIONIC__
}

static void pthread_mutex_timedlock_pi_helper(clockid_t clock,
                                              int (*lock_function)(pthread_mutex_t* __mutex,
                                                                   const timespec* __timeout)) {
  PthreadMutex m(PTHREAD_MUTEX_NORMAL, PTHREAD_PRIO_INHERIT);

  timespec ts;
  clock_gettime(clock, &ts);
  const int64_t start_ns = ts.tv_sec * NS_PER_S + ts.tv_nsec;

  // add a second to get deadline.
  ts.tv_sec += 1;

  ASSERT_EQ(0, lock_function(&m.lock, &ts));

  struct ThreadArgs {
    clockid_t clock;
    int (*lock_function)(pthread_mutex_t* __mutex, const timespec* __timeout);
    PthreadMutex& m;
  };

  ThreadArgs thread_args = {
    .clock = clock,
    .lock_function = lock_function,
    .m = m,
  };

  auto ThreadFn = [](void* arg) -> void* {
    auto args = static_cast<ThreadArgs*>(arg);
    timespec ts;
    clock_gettime(args->clock, &ts);
    ts.tv_sec += 1;
    intptr_t result = args->lock_function(&args->m.lock, &ts);
    return reinterpret_cast<void*>(result);
  };

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, nullptr, ThreadFn, &thread_args));
  void* result;
  ASSERT_EQ(0, pthread_join(thread, &result));
  ASSERT_EQ(ETIMEDOUT, reinterpret_cast<intptr_t>(result));

  // The timedlock must have waited at least 1 second before returning.
  clock_gettime(clock, &ts);
  const int64_t end_ns = ts.tv_sec * NS_PER_S + ts.tv_nsec;
  ASSERT_GT(end_ns - start_ns, NS_PER_S);

  ASSERT_EQ(0, pthread_mutex_unlock(&m.lock));
}

TEST(pthread, pthread_mutex_timedlock_pi) {
  pthread_mutex_timedlock_pi_helper(CLOCK_REALTIME, pthread_mutex_timedlock);
}

TEST(pthread, pthread_mutex_timedlock_monotonic_np_pi) {
#if defined(__BIONIC__)
  pthread_mutex_timedlock_pi_helper(CLOCK_MONOTONIC, pthread_mutex_timedlock_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_mutex_timedlock_monotonic_np not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_mutex_clocklock_pi) {
#if defined(__BIONIC__)
  pthread_mutex_timedlock_pi_helper(
      CLOCK_MONOTONIC, [](pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_mutex_clocklock(__mutex, CLOCK_MONOTONIC, __timeout);
      });
  pthread_mutex_timedlock_pi_helper(
      CLOCK_REALTIME, [](pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_mutex_clocklock(__mutex, CLOCK_REALTIME, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_mutex_clocklock not available";
#endif  // __BIONIC__
}

TEST(pthread, pthread_mutex_clocklock_invalid) {
#if defined(__BIONIC__)
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  timespec ts;
  EXPECT_EQ(EINVAL, pthread_mutex_clocklock(&mutex, CLOCK_PROCESS_CPUTIME_ID, &ts));
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_mutex_clocklock not available";
#endif  // __BIONIC__
}

TEST_F(pthread_DeathTest, pthread_mutex_using_destroyed_mutex) {
#if defined(__BIONIC__)
  pthread_mutex_t m;
  ASSERT_EQ(0, pthread_mutex_init(&m, nullptr));
  ASSERT_EQ(0, pthread_mutex_destroy(&m));
  ASSERT_EXIT(pthread_mutex_lock(&m), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_lock called on a destroyed mutex");
  ASSERT_EXIT(pthread_mutex_unlock(&m), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_unlock called on a destroyed mutex");
  ASSERT_EXIT(pthread_mutex_trylock(&m), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_trylock called on a destroyed mutex");
  timespec ts;
  ASSERT_EXIT(pthread_mutex_timedlock(&m, &ts), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_timedlock called on a destroyed mutex");
  ASSERT_EXIT(pthread_mutex_timedlock_monotonic_np(&m, &ts), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_timedlock_monotonic_np called on a destroyed mutex");
  ASSERT_EXIT(pthread_mutex_clocklock(&m, CLOCK_MONOTONIC, &ts), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_clocklock called on a destroyed mutex");
  ASSERT_EXIT(pthread_mutex_clocklock(&m, CLOCK_REALTIME, &ts), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_clocklock called on a destroyed mutex");
  ASSERT_EXIT(pthread_mutex_clocklock(&m, CLOCK_PROCESS_CPUTIME_ID, &ts),
              ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_clocklock called on a destroyed mutex");
  ASSERT_EXIT(pthread_mutex_destroy(&m), ::testing::KilledBySignal(SIGABRT),
              "pthread_mutex_destroy called on a destroyed mutex");
#else
  GTEST_SKIP() << "bionic-only test";
#endif
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
  ASSERT_EQ(0, pthread_mutex_init(mutex, nullptr));
  ASSERT_EQ(0, pthread_mutex_lock(mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(mutex));
  ASSERT_EQ(0, pthread_mutex_destroy(mutex));

  pthread_cond_t* cond = reinterpret_cast<pthread_cond_t*>(
                           allocator.allocate(sizeof(pthread_cond_t), 4));
  ASSERT_EQ(0, pthread_cond_init(cond, nullptr));
  ASSERT_EQ(0, pthread_cond_signal(cond));
  ASSERT_EQ(0, pthread_cond_broadcast(cond));
  ASSERT_EQ(0, pthread_cond_destroy(cond));

  pthread_rwlock_t* rwlock = reinterpret_cast<pthread_rwlock_t*>(
                               allocator.allocate(sizeof(pthread_rwlock_t), 4));
  ASSERT_EQ(0, pthread_rwlock_init(rwlock, nullptr));
  ASSERT_EQ(0, pthread_rwlock_rdlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_wrlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_unlock(rwlock));
  ASSERT_EQ(0, pthread_rwlock_destroy(rwlock));

#else
  GTEST_SKIP() << "bionic-only test";
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
  GTEST_SKIP() << "32-bit bionic-only test";
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
  GTEST_SKIP() << "32-bit bionic-only test";
#endif
}

TEST_F(pthread_DeathTest, pthread_mutex_lock_null_64) {
#if defined(__BIONIC__) && defined(__LP64__)
  pthread_mutex_t* null_value = nullptr;
  ASSERT_EXIT(pthread_mutex_lock(null_value), testing::KilledBySignal(SIGSEGV), "");
#else
  GTEST_SKIP() << "64-bit bionic-only test";
#endif
}

TEST_F(pthread_DeathTest, pthread_mutex_unlock_null_64) {
#if defined(__BIONIC__) && defined(__LP64__)
  pthread_mutex_t* null_value = nullptr;
  ASSERT_EXIT(pthread_mutex_unlock(null_value), testing::KilledBySignal(SIGSEGV), "");
#else
  GTEST_SKIP() << "64-bit bionic-only test";
#endif
}

extern _Unwind_Reason_Code FrameCounter(_Unwind_Context* ctx, void* arg);

static volatile bool signal_handler_on_altstack_done;

__attribute__((__noinline__))
static void signal_handler_backtrace() {
  // Check if we have enough stack space for unwinding.
  int count = 0;
  _Unwind_Backtrace(FrameCounter, &count);
  ASSERT_GT(count, 0);
}

__attribute__((__noinline__))
static void signal_handler_logging() {
  // Check if we have enough stack space for logging.
  std::string s(2048, '*');
  GTEST_LOG_(INFO) << s;
  signal_handler_on_altstack_done = true;
}

__attribute__((__noinline__))
static void signal_handler_snprintf() {
  // Check if we have enough stack space for snprintf to a PATH_MAX buffer, plus some extra.
  char buf[PATH_MAX + 2048];
  ASSERT_GT(snprintf(buf, sizeof(buf), "/proc/%d/status", getpid()), 0);
}

static void SignalHandlerOnAltStack(int signo, siginfo_t*, void*) {
  ASSERT_EQ(SIGUSR1, signo);
  signal_handler_backtrace();
  signal_handler_logging();
  signal_handler_snprintf();
}

TEST(pthread, big_enough_signal_stack) {
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
    int mask = arg->data->finished_mask.fetch_or(1 << arg->id);
    mask |= 1 << arg->id;
    if (mask == ((1 << arg->data->thread_count) - 1)) {
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

TEST(pthread, pthread_barrier_init_zero_count) {
  pthread_barrier_t barrier;
  ASSERT_EQ(EINVAL, pthread_barrier_init(&barrier, nullptr, 0));
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

TEST(pthread, pthread_attr_getdetachstate__pthread_attr_setdetachstate) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  int state;
  ASSERT_EQ(0, pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
  ASSERT_EQ(0, pthread_attr_getdetachstate(&attr, &state));
  ASSERT_EQ(PTHREAD_CREATE_DETACHED, state);

  ASSERT_EQ(0, pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));
  ASSERT_EQ(0, pthread_attr_getdetachstate(&attr, &state));
  ASSERT_EQ(PTHREAD_CREATE_JOINABLE, state);

  ASSERT_EQ(EINVAL, pthread_attr_setdetachstate(&attr, 123));
  ASSERT_EQ(0, pthread_attr_getdetachstate(&attr, &state));
  ASSERT_EQ(PTHREAD_CREATE_JOINABLE, state);
}

TEST(pthread, pthread_create__mmap_failures) {
  // After thread is successfully created, native_bridge might need more memory to run it.
  SKIP_WITH_NATIVE_BRIDGE;

  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));

  const auto kPageSize = sysconf(_SC_PAGE_SIZE);

  // Use up all the VMAs. By default this is 64Ki (though some will already be in use).
  std::vector<void*> pages;
  pages.reserve(64 * 1024);
  int prot = PROT_NONE;
  while (true) {
    void* page = mmap(nullptr, kPageSize, prot, MAP_ANON|MAP_PRIVATE, -1, 0);
    if (page == MAP_FAILED) break;
    pages.push_back(page);
    prot = (prot == PROT_NONE) ? PROT_READ : PROT_NONE;
  }

  // Try creating threads, freeing up a page each time we fail.
  size_t EAGAIN_count = 0;
  size_t i = 0;
  for (; i < pages.size(); ++i) {
    pthread_t t;
    int status = pthread_create(&t, &attr, IdFn, nullptr);
    if (status != EAGAIN) break;
    ++EAGAIN_count;
    ASSERT_EQ(0, munmap(pages[i], kPageSize));
  }

  // Creating a thread uses at least three VMAs: the combined stack and TLS, and a guard on each
  // side. So we should have seen at least three failures.
  ASSERT_GE(EAGAIN_count, 3U);

  for (; i < pages.size(); ++i) {
    ASSERT_EQ(0, munmap(pages[i], kPageSize));
  }
}

TEST(pthread, pthread_setschedparam) {
  sched_param p = { .sched_priority = INT_MIN };
  ASSERT_EQ(EINVAL, pthread_setschedparam(pthread_self(), INT_MIN, &p));
}

TEST(pthread, pthread_setschedprio) {
  ASSERT_EQ(EINVAL, pthread_setschedprio(pthread_self(), INT_MIN));
}

TEST(pthread, pthread_attr_getinheritsched__pthread_attr_setinheritsched) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  int state;
  ASSERT_EQ(0, pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED));
  ASSERT_EQ(0, pthread_attr_getinheritsched(&attr, &state));
  ASSERT_EQ(PTHREAD_INHERIT_SCHED, state);

  ASSERT_EQ(0, pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
  ASSERT_EQ(0, pthread_attr_getinheritsched(&attr, &state));
  ASSERT_EQ(PTHREAD_EXPLICIT_SCHED, state);

  ASSERT_EQ(EINVAL, pthread_attr_setinheritsched(&attr, 123));
  ASSERT_EQ(0, pthread_attr_getinheritsched(&attr, &state));
  ASSERT_EQ(PTHREAD_EXPLICIT_SCHED, state);
}

TEST(pthread, pthread_attr_setinheritsched__PTHREAD_INHERIT_SCHED__PTHREAD_EXPLICIT_SCHED) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  // If we set invalid scheduling attributes but choose to inherit, everything's fine...
  sched_param param = { .sched_priority = sched_get_priority_max(SCHED_FIFO) + 1 };
  ASSERT_EQ(0, pthread_attr_setschedparam(&attr, &param));
  ASSERT_EQ(0, pthread_attr_setschedpolicy(&attr, SCHED_FIFO));
  ASSERT_EQ(0, pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED));

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &attr, IdFn, nullptr));
  ASSERT_EQ(0, pthread_join(t, nullptr));

#if defined(__LP64__)
  // If we ask to use them, though, we'll see a failure...
  ASSERT_EQ(0, pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
  ASSERT_EQ(EINVAL, pthread_create(&t, &attr, IdFn, nullptr));
#else
  // For backwards compatibility with broken apps, we just ignore failures
  // to set scheduler attributes on LP32.
#endif
}

TEST(pthread, pthread_attr_setinheritsched_PTHREAD_INHERIT_SCHED_takes_effect) {
  sched_param param = { .sched_priority = sched_get_priority_min(SCHED_FIFO) };
  int rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
  if (rc == EPERM) GTEST_SKIP() << "pthread_setschedparam failed with EPERM";
  ASSERT_EQ(0, rc);

  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED));

  SpinFunctionHelper spin_helper;
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &attr, spin_helper.GetFunction(), nullptr));
  int actual_policy;
  sched_param actual_param;
  ASSERT_EQ(0, pthread_getschedparam(t, &actual_policy, &actual_param));
  ASSERT_EQ(SCHED_FIFO, actual_policy);
  spin_helper.UnSpin();
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

TEST(pthread, pthread_attr_setinheritsched_PTHREAD_EXPLICIT_SCHED_takes_effect) {
  sched_param param = { .sched_priority = sched_get_priority_min(SCHED_FIFO) };
  int rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
  if (rc == EPERM) GTEST_SKIP() << "pthread_setschedparam failed with EPERM";
  ASSERT_EQ(0, rc);

  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
  ASSERT_EQ(0, pthread_attr_setschedpolicy(&attr, SCHED_OTHER));

  SpinFunctionHelper spin_helper;
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &attr, spin_helper.GetFunction(), nullptr));
  int actual_policy;
  sched_param actual_param;
  ASSERT_EQ(0, pthread_getschedparam(t, &actual_policy, &actual_param));
  ASSERT_EQ(SCHED_OTHER, actual_policy);
  spin_helper.UnSpin();
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

TEST(pthread, pthread_attr_setinheritsched__takes_effect_despite_SCHED_RESET_ON_FORK) {
  sched_param param = { .sched_priority = sched_get_priority_min(SCHED_FIFO) };
  int rc = pthread_setschedparam(pthread_self(), SCHED_FIFO | SCHED_RESET_ON_FORK, &param);
  if (rc == EPERM) GTEST_SKIP() << "pthread_setschedparam failed with EPERM";
  ASSERT_EQ(0, rc);

  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED));

  SpinFunctionHelper spin_helper;
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &attr, spin_helper.GetFunction(), nullptr));
  int actual_policy;
  sched_param actual_param;
  ASSERT_EQ(0, pthread_getschedparam(t, &actual_policy, &actual_param));
  ASSERT_EQ(SCHED_FIFO  | SCHED_RESET_ON_FORK, actual_policy);
  spin_helper.UnSpin();
  ASSERT_EQ(0, pthread_join(t, nullptr));
}

extern "C" bool android_run_on_all_threads(bool (*func)(void*), void* arg);

TEST(pthread, run_on_all_threads) {
#if defined(__BIONIC__)
  pthread_t t;
  ASSERT_EQ(
      0, pthread_create(
             &t, nullptr,
             [](void*) -> void* {
               pthread_attr_t detached;
               if (pthread_attr_init(&detached) != 0 ||
                   pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED) != 0) {
                 return reinterpret_cast<void*>(errno);
               }

               for (int i = 0; i != 1000; ++i) {
                 pthread_t t1, t2;
                 if (pthread_create(
                         &t1, &detached, [](void*) -> void* { return nullptr; }, nullptr) != 0 ||
                     pthread_create(
                         &t2, nullptr, [](void*) -> void* { return nullptr; }, nullptr) != 0 ||
                     pthread_join(t2, nullptr) != 0) {
                   return reinterpret_cast<void*>(errno);
                 }
               }

               if (pthread_attr_destroy(&detached) != 0) {
                 return reinterpret_cast<void*>(errno);
               }
               return nullptr;
             },
             nullptr));

  for (int i = 0; i != 1000; ++i) {
    ASSERT_TRUE(android_run_on_all_threads([](void* arg) { return arg == nullptr; }, nullptr));
  }

  void *retval;
  ASSERT_EQ(0, pthread_join(t, &retval));
  ASSERT_EQ(nullptr, retval);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}
