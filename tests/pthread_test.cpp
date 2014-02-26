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
#include <pthread.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

TEST(pthread, pthread_key_create) {
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, NULL));
  ASSERT_EQ(0, pthread_key_delete(key));
  // Can't delete a key that's already been deleted.
  ASSERT_EQ(EINVAL, pthread_key_delete(key));
}

TEST(pthread, pthread_key_create_lots) {
#if defined(__BIONIC__) // glibc uses keys internally that its sysconf value doesn't account for.
  // POSIX says PTHREAD_KEYS_MAX should be at least 128.
  ASSERT_GE(PTHREAD_KEYS_MAX, 128);

  int sysconf_max = sysconf(_SC_THREAD_KEYS_MAX);

  // sysconf shouldn't return a smaller value.
  ASSERT_GE(sysconf_max, PTHREAD_KEYS_MAX);

  // We can allocate _SC_THREAD_KEYS_MAX keys.
  sysconf_max -= 2; // (Except that gtest takes two for itself.)
  std::vector<pthread_key_t> keys;
  for (int i = 0; i < sysconf_max; ++i) {
    pthread_key_t key;
    // If this fails, it's likely that GLOBAL_INIT_THREAD_LOCAL_BUFFER_COUNT is wrong.
    ASSERT_EQ(0, pthread_key_create(&key, NULL)) << i << " of " << sysconf_max;
    keys.push_back(key);
  }

  // ...and that really is the maximum.
  pthread_key_t key;
  ASSERT_EQ(EAGAIN, pthread_key_create(&key, NULL));

  // (Don't leak all those keys!)
  for (size_t i = 0; i < keys.size(); ++i) {
    ASSERT_EQ(0, pthread_key_delete(keys[i]));
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

static void* IdFn(void* arg) {
  return arg;
}

static void* SleepFn(void* arg) {
  sleep(reinterpret_cast<uintptr_t>(arg));
  return NULL;
}

static void* SpinFn(void* arg) {
  volatile bool* b = reinterpret_cast<volatile bool*>(arg);
  while (!*b) {
  }
  return NULL;
}

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
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
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
  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, SleepFn, reinterpret_cast<void*>(5)));

  // After a pthread_detach...
  ASSERT_EQ(0, pthread_detach(t1));
  AssertDetached(t1, true);

  // ...pthread_join should fail.
  void* result;
  ASSERT_EQ(EINVAL, pthread_join(t1, &result));
}

TEST(pthread, pthread_no_op_detach_after_join) {
  bool done = false;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, SpinFn, &done));

  // If thread 2 is already waiting to join thread 1...
  pthread_t t2;
  ASSERT_EQ(0, pthread_create(&t2, NULL, JoinFn, reinterpret_cast<void*>(t1)));

  sleep(1); // (Give t2 a chance to call pthread_join.)

  // ...a call to pthread_detach on thread 1 will "succeed" (silently fail)...
  ASSERT_EQ(0, pthread_detach(t1));
  AssertDetached(t1, false);

  done = true;

  // ...but t2's join on t1 still goes ahead (which we can tell because our join on t2 finishes).
  void* join_result;
  ASSERT_EQ(0, pthread_join(t2, &join_result));
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(join_result));
}

TEST(pthread, pthread_join_self) {
  void* result;
  ASSERT_EQ(EDEADLK, pthread_join(pthread_self(), &result));
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
TEST(pthread_DeathTest, pthread_bug_37410) {
  // http://code.google.com/p/android/issues/detail?id=37410
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
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

#if defined(__BIONIC__)
extern "C" pid_t __bionic_clone(int flags, void* child_stack, pid_t* parent_tid, void* tls, pid_t* child_tid, int (*fn)(void*), void* arg);
#endif // __BIONIC__

TEST(pthread, __bionic_clone) {
#if defined(__BIONIC__)
  // Check that our hand-written clone assembler sets errno correctly on failure.
  uintptr_t fake_child_stack[16];
  errno = 0;
  ASSERT_EQ(-1, __bionic_clone(CLONE_THREAD, &fake_child_stack[16], NULL, NULL, NULL, NULL, NULL));
  ASSERT_EQ(EINVAL, errno);
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(pthread, pthread_setname_np__too_long) {
#if defined(__BIONIC__) // Not all build servers have a new enough glibc? TODO: remove when they're on gprecise.
  ASSERT_EQ(ERANGE, pthread_setname_np(pthread_self(), "this name is far too long for linux"));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(pthread, pthread_setname_np__self) {
#if defined(__BIONIC__) // Not all build servers have a new enough glibc? TODO: remove when they're on gprecise.
  ASSERT_EQ(0, pthread_setname_np(pthread_self(), "short 1"));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(pthread, pthread_setname_np__other) {
#if defined(__BIONIC__) // Not all build servers have a new enough glibc? TODO: remove when they're on gprecise.
  // Emulator kernels don't currently support setting the name of other threads.
  char* filename = NULL;
  asprintf(&filename, "/proc/self/task/%d/comm", gettid());
  struct stat sb;
  bool has_comm = (stat(filename, &sb) != -1);
  free(filename);

  if (has_comm) {
    pthread_t t1;
    ASSERT_EQ(0, pthread_create(&t1, NULL, SleepFn, reinterpret_cast<void*>(5)));
    ASSERT_EQ(0, pthread_setname_np(t1, "short 2"));
  } else {
    fprintf(stderr, "skipping test: this kernel doesn't have /proc/self/task/tid/comm files!\n");
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
}

TEST(pthread, pthread_setname_np__no_such_thread) {
#if defined(__BIONIC__) // Not all build servers have a new enough glibc? TODO: remove when they're on gprecise.
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  // Call pthread_setname_np after thread has already exited.
  ASSERT_EQ(ESRCH, pthread_setname_np(dead_thread, "short 3"));
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif // __BIONIC__
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
  struct sigaction action;
  struct sigaction original_action;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  action.sa_handler = pthread_kill__in_signal_handler_helper;
  ASSERT_EQ(0, sigaction(SIGALRM, &action, &original_action));
  ASSERT_EQ(0, pthread_kill(pthread_self(), SIGALRM));
  ASSERT_EQ(0, sigaction(SIGALRM, &original_action, NULL));
}

TEST(pthread, pthread_detach__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  ASSERT_EQ(ESRCH, pthread_detach(dead_thread));
}

TEST(pthread, pthread_getcpuclockid__clock_gettime) {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, SleepFn, reinterpret_cast<void*>(5)));

  clockid_t c;
  ASSERT_EQ(0, pthread_getcpuclockid(t, &c));
  timespec ts;
  ASSERT_EQ(0, clock_gettime(c, &ts));
}

TEST(pthread, pthread_getcpuclockid__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  clockid_t c;
  ASSERT_EQ(ESRCH, pthread_getcpuclockid(dead_thread, &c));
}

TEST(pthread, pthread_getschedparam__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  int policy;
  sched_param param;
  ASSERT_EQ(ESRCH, pthread_getschedparam(dead_thread, &policy, &param));
}

TEST(pthread, pthread_setschedparam__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  int policy = 0;
  sched_param param;
  ASSERT_EQ(ESRCH, pthread_setschedparam(dead_thread, policy, &param));
}

TEST(pthread, pthread_join__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  void* result;
  ASSERT_EQ(ESRCH, pthread_join(dead_thread, &result));
}

TEST(pthread, pthread_kill__no_such_thread) {
  pthread_t dead_thread;
  MakeDeadThread(dead_thread);

  ASSERT_EQ(ESRCH, pthread_kill(dead_thread, 0));
}

TEST(pthread, pthread_join__multijoin) {
  bool done = false;

  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, SpinFn, &done));

  pthread_t t2;
  ASSERT_EQ(0, pthread_create(&t2, NULL, JoinFn, reinterpret_cast<void*>(t1)));

  sleep(1); // (Give t2 a chance to call pthread_join.)

  // Multiple joins to the same thread should fail.
  ASSERT_EQ(EINVAL, pthread_join(t1, NULL));

  done = true;

  // ...but t2's join on t1 still goes ahead (which we can tell because our join on t2 finishes).
  void* join_result;
  ASSERT_EQ(0, pthread_join(t2, &join_result));
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(join_result));
}

TEST(pthread, pthread_join__race) {
  // http://b/11693195 --- pthread_join could return before the thread had actually exited.
  // If the joiner unmapped the thread's stack, that could lead to SIGSEGV in the thread.
  for (size_t i = 0; i < 1024; ++i) {
    size_t stack_size = 64*1024;
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
  void* join_result;
  pthread_join(t, &join_result);
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
  void* join_result;
  pthread_join(t, &join_result);
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

  // Large enough and a multiple of the page size.
  ASSERT_EQ(0, pthread_attr_setstacksize(&attributes, 32*1024));
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size));
  ASSERT_EQ(32*1024U, stack_size);
  ASSERT_EQ(GetActualStackSize(attributes), 32*1024U);

  // Large enough but not a multiple of the page size; will be rounded up by pthread_create.
  ASSERT_EQ(0, pthread_attr_setstacksize(&attributes, 32*1024 + 1));
  ASSERT_EQ(0, pthread_attr_getstacksize(&attributes, &stack_size));
  ASSERT_EQ(32*1024U + 1, stack_size);
#if defined(__BIONIC__)
  // Bionic rounds up, which is what POSIX allows.
  ASSERT_EQ(GetActualStackSize(attributes), (32 + 4)*1024U);
#else // __BIONIC__
  // glibc rounds down, in violation of POSIX. They document this in their BUGS section.
  ASSERT_EQ(GetActualStackSize(attributes), 32*1024U);
#endif // __BIONIC__
}

TEST(pthread, pthread_rwlock_smoke) {
  pthread_rwlock_t l;
  ASSERT_EQ(0, pthread_rwlock_init(&l, NULL));

  ASSERT_EQ(0, pthread_rwlock_rdlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  ASSERT_EQ(0, pthread_rwlock_wrlock(&l));
  ASSERT_EQ(0, pthread_rwlock_unlock(&l));

  ASSERT_EQ(0, pthread_rwlock_destroy(&l));
}

static int gOnceFnCallCount = 0;
static void OnceFn() {
  ++gOnceFnCallCount;
}

TEST(pthread, pthread_once_smoke) {
  pthread_once_t once_control = PTHREAD_ONCE_INIT;
  ASSERT_EQ(0, pthread_once(&once_control, OnceFn));
  ASSERT_EQ(0, pthread_once(&once_control, OnceFn));
  ASSERT_EQ(1, gOnceFnCallCount);
}

static int gAtForkPrepareCalls = 0;
static void AtForkPrepare1() { gAtForkPrepareCalls = (gAtForkPrepareCalls << 4) | 1; }
static void AtForkPrepare2() { gAtForkPrepareCalls = (gAtForkPrepareCalls << 4) | 2; }
static int gAtForkParentCalls = 0;
static void AtForkParent1() { gAtForkParentCalls = (gAtForkParentCalls << 4) | 1; }
static void AtForkParent2() { gAtForkParentCalls = (gAtForkParentCalls << 4) | 2; }
static int gAtForkChildCalls = 0;
static void AtForkChild1() { gAtForkChildCalls = (gAtForkChildCalls << 4) | 1; }
static void AtForkChild2() { gAtForkChildCalls = (gAtForkChildCalls << 4) | 2; }

TEST(pthread, pthread_atfork) {
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare1, AtForkParent1, AtForkChild1));
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare2, AtForkParent2, AtForkChild2));

  int pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  // Child and parent calls are made in the order they were registered.
  if (pid == 0) {
    ASSERT_EQ(0x12, gAtForkChildCalls);
    _exit(0);
  }
  ASSERT_EQ(0x12, gAtForkParentCalls);

  // Prepare calls are made in the reverse order.
  ASSERT_EQ(0x21, gAtForkPrepareCalls);
}

TEST(pthread, pthread_attr_getscope) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  int scope;
  ASSERT_EQ(0, pthread_attr_getscope(&attr, &scope));
  ASSERT_EQ(PTHREAD_SCOPE_SYSTEM, scope);
}
