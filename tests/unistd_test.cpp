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

#include "SignalUtils.h"
#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdint.h>
#include <sys/capability.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>

#include <android-base/file.h>
#include <android-base/silent_death_test.h>
#include <android-base/strings.h>

#include "private/get_cpu_count_from_string.h"

#if defined(__BIONIC__)
#include "bionic/pthread_internal.h"
#endif

#if defined(NOFORTIFY)
#define UNISTD_TEST unistd_nofortify
#define UNISTD_DEATHTEST unistd_nofortify_DeathTest
#else
#define UNISTD_TEST unistd
#define UNISTD_DEATHTEST unistd_DeathTest
#endif

using UNISTD_DEATHTEST = SilentDeathTest;

using namespace std::chrono_literals;

static void* get_brk() {
  return sbrk(0);
}

static void* page_align(uintptr_t addr) {
  uintptr_t mask = sysconf(_SC_PAGE_SIZE) - 1;
  return reinterpret_cast<void*>((addr + mask) & ~mask);
}

TEST(UNISTD_TEST, brk) {
  void* initial_break = get_brk();

  void* new_break = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(initial_break) + 1);
  int ret = brk(new_break);
  if (ret == -1) {
    ASSERT_ERRNO(ENOMEM);
  } else {
    ASSERT_EQ(0, ret);
    ASSERT_GE(get_brk(), new_break);
  }

  // Expand by a full page to force the mapping to expand
  new_break = page_align(reinterpret_cast<uintptr_t>(initial_break) + sysconf(_SC_PAGE_SIZE));
  ret = brk(new_break);
  if (ret == -1) {
    ASSERT_ERRNO(ENOMEM);
  } else {
    ASSERT_EQ(0, ret);
    ASSERT_EQ(get_brk(), new_break);
  }
}

TEST(UNISTD_TEST, brk_ENOMEM) {
  ASSERT_EQ(-1, brk(reinterpret_cast<void*>(-1)));
  ASSERT_ERRNO(ENOMEM);
}

#if defined(__GLIBC__)
#define SBRK_MIN INTPTR_MIN
#define SBRK_MAX INTPTR_MAX
#else
#define SBRK_MIN PTRDIFF_MIN
#define SBRK_MAX PTRDIFF_MAX
#endif

TEST(UNISTD_TEST, sbrk_ENOMEM) {
#if defined(__BIONIC__) && !defined(__LP64__)
  // There is no way to guarantee that all overflow conditions can be tested
  // without manipulating the underlying values of the current break.
  extern void* __bionic_brk;

  class ScopedBrk {
  public:
    ScopedBrk() : saved_brk_(__bionic_brk) {}
    virtual ~ScopedBrk() { __bionic_brk = saved_brk_; }

  private:
    void* saved_brk_;
  };

  ScopedBrk scope_brk;

  // Set the current break to a point that will cause an overflow.
  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX) + 2);

  // Can't increase by so much that we'd overflow.
  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MAX));
  ASSERT_ERRNO(ENOMEM);

  // Set the current break to a point that will cause an overflow.
  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX));

  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MIN));
  ASSERT_ERRNO(ENOMEM);

  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX) - 1);

  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MIN + 1));
  ASSERT_ERRNO(ENOMEM);
#else
  class ScopedBrk {
  public:
    ScopedBrk() : saved_brk_(get_brk()) {}
    virtual ~ScopedBrk() { brk(saved_brk_); }

  private:
    void* saved_brk_;
  };

  ScopedBrk scope_brk;

  uintptr_t cur_brk = reinterpret_cast<uintptr_t>(get_brk());
  if (cur_brk < static_cast<uintptr_t>(-(SBRK_MIN+1))) {
    // Do the overflow test for a max negative increment.
    ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(SBRK_MIN));
#if defined(__BIONIC__)
    // GLIBC does not set errno in overflow case.
    ASSERT_ERRNO(ENOMEM);
#endif
  }

  uintptr_t overflow_brk = static_cast<uintptr_t>(SBRK_MAX) + 2;
  if (cur_brk < overflow_brk) {
    // Try and move the value to PTRDIFF_MAX + 2.
    cur_brk = reinterpret_cast<uintptr_t>(sbrk(overflow_brk));
  }
  if (cur_brk >= overflow_brk) {
    ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(SBRK_MAX));
#if defined(__BIONIC__)
    // GLIBC does not set errno in overflow case.
    ASSERT_ERRNO(ENOMEM);
#endif
  }
#endif
}

TEST(UNISTD_TEST, truncate) {
  TemporaryFile tf;
  ASSERT_EQ(0, close(tf.fd));
  ASSERT_EQ(0, truncate(tf.path, 123));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.path, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, truncate64_smoke) {
  TemporaryFile tf;
  ASSERT_EQ(0, close(tf.fd));
  ASSERT_EQ(0, truncate64(tf.path, 123));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.path, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, ftruncate) {
  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, 123));
  ASSERT_EQ(0, close(tf.fd));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.path, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, ftruncate64_smoke) {
  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate64(tf.fd, 123));
  ASSERT_EQ(0, close(tf.fd));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.path, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, ftruncate_negative) {
  TemporaryFile tf;
  errno = 0;
  ASSERT_EQ(-1, ftruncate(tf.fd, -123));
  ASSERT_ERRNO(EINVAL);
}

static bool g_pause_test_flag = false;
static void PauseTestSignalHandler(int) {
  g_pause_test_flag = true;
}

TEST(UNISTD_TEST, pause) {
  ScopedSignalHandler handler(SIGALRM, PauseTestSignalHandler);

  alarm(1);
  ASSERT_FALSE(g_pause_test_flag);
  ASSERT_EQ(-1, pause());
  ASSERT_TRUE(g_pause_test_flag);
}

TEST(UNISTD_TEST, read) {
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);

  char buf[5];
  ASSERT_EQ(5, read(fd, buf, 5));
  ASSERT_EQ(buf[0], 'L');
  ASSERT_EQ(buf[1], 'i');
  ASSERT_EQ(buf[2], 'n');
  ASSERT_EQ(buf[3], 'u');
  ASSERT_EQ(buf[4], 'x');
  close(fd);
}

TEST(UNISTD_TEST, read_EBADF) {
  // read returns ssize_t which is 64-bits on LP64, so it's worth explicitly checking that
  // our syscall stubs correctly return a 64-bit -1.
  char buf[1];
  ASSERT_EQ(-1, read(-1, buf, sizeof(buf)));
  ASSERT_ERRNO(EBADF);
}

TEST(UNISTD_TEST, syscall_long) {
  // Check that syscall(3) correctly returns long results.
  // https://code.google.com/p/android/issues/detail?id=73952
  // We assume that the break is > 4GiB, but this is potentially flaky.
  uintptr_t p = reinterpret_cast<uintptr_t>(sbrk(0));
  ASSERT_EQ(p, static_cast<uintptr_t>(syscall(__NR_brk, 0)));
}

TEST(UNISTD_TEST, alarm) {
  ASSERT_EQ(0U, alarm(0));
}

TEST(UNISTD_TEST, _exit) {
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    _exit(99);
  }

  AssertChildExited(pid, 99);
}

TEST(UNISTD_TEST, getenv_unsetenv) {
  ASSERT_EQ(0, setenv("test-variable", "hello", 1));
  ASSERT_STREQ("hello", getenv("test-variable"));
  ASSERT_EQ(0, unsetenv("test-variable"));
  ASSERT_TRUE(getenv("test-variable") == nullptr);
}

TEST(UNISTD_TEST, unsetenv_EINVAL) {
  EXPECT_EQ(-1, unsetenv(""));
  EXPECT_ERRNO(EINVAL);
  EXPECT_EQ(-1, unsetenv("a=b"));
  EXPECT_ERRNO(EINVAL);
}

TEST(UNISTD_TEST, setenv_EINVAL) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  EXPECT_EQ(-1, setenv(nullptr, "value", 0));
  EXPECT_ERRNO(EINVAL);
  EXPECT_EQ(-1, setenv(nullptr, "value", 1));
  EXPECT_ERRNO(EINVAL);
#pragma clang diagnostic pop
  EXPECT_EQ(-1, setenv("", "value", 0));
  EXPECT_ERRNO(EINVAL);
  EXPECT_EQ(-1, setenv("", "value", 1));
  EXPECT_ERRNO(EINVAL);
  EXPECT_EQ(-1, setenv("a=b", "value", 0));
  EXPECT_ERRNO(EINVAL);
  EXPECT_EQ(-1, setenv("a=b", "value", 1));
  EXPECT_ERRNO(EINVAL);
}

TEST(UNISTD_TEST, setenv) {
  ASSERT_EQ(0, unsetenv("test-variable"));

  char a[] = "a";
  char b[] = "b";
  char c[] = "c";

  // New value.
  EXPECT_EQ(0, setenv("test-variable", a, 0));
  EXPECT_STREQ(a, getenv("test-variable"));

  // Existing value, no overwrite.
  EXPECT_EQ(0, setenv("test-variable", b, 0));
  EXPECT_STREQ(a, getenv("test-variable"));

  // Existing value, overwrite.
  EXPECT_EQ(0, setenv("test-variable", c, 1));
  EXPECT_STREQ(c, getenv("test-variable"));
  // But the arrays backing the values are unchanged.
  EXPECT_EQ('a', a[0]);
  EXPECT_EQ('b', b[0]);
  EXPECT_EQ('c', c[0]);

  ASSERT_EQ(0, unsetenv("test-variable"));
}

TEST(UNISTD_TEST, putenv) {
  ASSERT_EQ(0, unsetenv("a"));

  char* s1 = strdup("a=b");
  ASSERT_EQ(0, putenv(s1));

  ASSERT_STREQ("b", getenv("a"));
  s1[2] = 'c';
  ASSERT_STREQ("c", getenv("a"));

  char* s2 = strdup("a=b");
  ASSERT_EQ(0, putenv(s2));

  ASSERT_STREQ("b", getenv("a"));
  ASSERT_EQ('c', s1[2]);

  ASSERT_EQ(0, unsetenv("a"));
  free(s1);
  free(s2);
}

TEST(UNISTD_TEST, clearenv) {
  extern char** environ;

  // Guarantee that environ is not initially empty...
  ASSERT_EQ(0, setenv("test-variable", "a", 1));

  // Stash a copy.
  std::vector<char*> old_environ;
  for (size_t i = 0; environ[i] != nullptr; ++i) {
    old_environ.push_back(strdup(environ[i]));
  }

  ASSERT_EQ(0, clearenv());

  EXPECT_TRUE(environ == nullptr || environ[0] == nullptr);
  EXPECT_EQ(nullptr, getenv("test-variable"));
  EXPECT_EQ(0, setenv("test-variable", "post-clear", 1));
  EXPECT_STREQ("post-clear", getenv("test-variable"));

  // Put the old environment back.
  for (size_t i = 0; i < old_environ.size(); ++i) {
    EXPECT_EQ(0, putenv(old_environ[i]));
  }

  // Check it wasn't overwritten.
  EXPECT_STREQ("a", getenv("test-variable"));

  EXPECT_EQ(0, unsetenv("test-variable"));
}

static void TestSyncFunction(int (*fn)(int)) {
  int fd;

  // Can't sync an invalid fd.
  errno = 0;
  EXPECT_EQ(-1, fn(-1));
  EXPECT_ERRNO(EBADF);

  // It doesn't matter whether you've opened a file for write or not.
  TemporaryFile tf;
  ASSERT_NE(-1, tf.fd);

  EXPECT_EQ(0, fn(tf.fd));

  ASSERT_NE(-1, fd = open(tf.path, O_RDONLY));
  EXPECT_EQ(0, fn(fd));
  close(fd);

  ASSERT_NE(-1, fd = open(tf.path, O_RDWR));
  EXPECT_EQ(0, fn(fd));
  close(fd);

  // The fd can even be a directory.
  ASSERT_NE(-1, fd = open("/data/local/tmp", O_RDONLY));
  EXPECT_EQ(0, fn(fd));
  close(fd);
}

static void TestFsyncFunction(int (*fn)(int)) {
  TestSyncFunction(fn);

  // But some file systems are fussy about fsync/fdatasync...
  errno = 0;
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_NE(-1, fd);
  EXPECT_EQ(-1, fn(fd));
  EXPECT_ERRNO(EINVAL);
  close(fd);
}

TEST(UNISTD_TEST, fdatasync) {
  TestFsyncFunction(fdatasync);
}

TEST(UNISTD_TEST, fsync) {
  TestFsyncFunction(fsync);
}

TEST(UNISTD_TEST, syncfs) {
  TestSyncFunction(syncfs);
}

TEST(UNISTD_TEST, vfork) {
#if defined(__BIONIC__)
  pthread_internal_t* self = __get_thread();

  pid_t cached_pid;
  ASSERT_TRUE(self->get_cached_pid(&cached_pid));
  ASSERT_EQ(syscall(__NR_getpid), cached_pid);
  ASSERT_FALSE(self->is_vforked());

  pid_t rc = vfork();
  ASSERT_NE(-1, rc);
  if (rc == 0) {
    if (self->get_cached_pid(&cached_pid)) {
      const char* error = "__get_thread()->cached_pid_ set after vfork\n";
      write(STDERR_FILENO, error, strlen(error));
      _exit(1);
    }

    if (!self->is_vforked()) {
      const char* error = "__get_thread()->vforked_ not set after vfork\n";
      write(STDERR_FILENO, error, strlen(error));
      _exit(1);
    }

    _exit(0);
  } else {
    ASSERT_TRUE(self->get_cached_pid(&cached_pid));
    ASSERT_EQ(syscall(__NR_getpid), cached_pid);
    ASSERT_FALSE(self->is_vforked());

    int status;
    pid_t wait_result = waitpid(rc, &status, 0);
    ASSERT_EQ(wait_result, rc);
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_EQ(0, WEXITSTATUS(status));
  }
#endif
}

static void AssertGetPidCorrect() {
  // The loop is just to make manual testing/debugging with strace easier.
  pid_t getpid_syscall_result = syscall(__NR_getpid);
  for (size_t i = 0; i < 128; ++i) {
    ASSERT_EQ(getpid_syscall_result, getpid());
  }
}

static void TestGetPidCachingWithFork(int (*fork_fn)(), void (*exit_fn)(int)) {
  pid_t parent_pid = getpid();
  ASSERT_EQ(syscall(__NR_getpid), parent_pid);

  pid_t fork_result = fork_fn();
  ASSERT_NE(fork_result, -1);
  if (fork_result == 0) {
    // We're the child.
    ASSERT_NO_FATAL_FAILURE(AssertGetPidCorrect());
    ASSERT_EQ(parent_pid, getppid());
    exit_fn(123);
  } else {
    // We're the parent.
    ASSERT_EQ(parent_pid, getpid());
    AssertChildExited(fork_result, 123);
  }
}

// gettid() is marked as __attribute_const__, which will have the compiler
// optimize out multiple calls to gettid in the same function. This wrapper
// defeats that optimization.
static __attribute__((__noinline__)) pid_t GetTidForTest() {
  __asm__("");
  return gettid();
}

static void AssertGetTidCorrect() {
  // The loop is just to make manual testing/debugging with strace easier.
  pid_t gettid_syscall_result = syscall(__NR_gettid);
  for (size_t i = 0; i < 128; ++i) {
    ASSERT_EQ(gettid_syscall_result, GetTidForTest());
  }
}

static void TestGetTidCachingWithFork(int (*fork_fn)(), void (*exit_fn)(int)) {
  pid_t parent_tid = GetTidForTest();
  ASSERT_EQ(syscall(__NR_gettid), parent_tid);

  pid_t fork_result = fork_fn();
  ASSERT_NE(fork_result, -1);
  if (fork_result == 0) {
    // We're the child.
    EXPECT_EQ(syscall(__NR_getpid), syscall(__NR_gettid));
    EXPECT_EQ(getpid(), GetTidForTest()) << "real tid is " << syscall(__NR_gettid)
                                         << ", pid is " << syscall(__NR_getpid);
    ASSERT_NO_FATAL_FAILURE(AssertGetTidCorrect());
    exit_fn(123);
  } else {
    // We're the parent.
    ASSERT_EQ(parent_tid, GetTidForTest());
    AssertChildExited(fork_result, 123);
  }
}

TEST(UNISTD_TEST, getpid_caching_and_fork) {
  TestGetPidCachingWithFork(fork, exit);
}

TEST(UNISTD_TEST, gettid_caching_and_fork) {
  TestGetTidCachingWithFork(fork, exit);
}

TEST(UNISTD_TEST, getpid_caching_and_vfork) {
  TestGetPidCachingWithFork(vfork, _exit);
}

static int CloneLikeFork() {
  return clone(nullptr, nullptr, SIGCHLD, nullptr);
}

TEST(UNISTD_TEST, getpid_caching_and_clone_process) {
  TestGetPidCachingWithFork(CloneLikeFork, exit);
}

TEST(UNISTD_TEST, gettid_caching_and_clone_process) {
  TestGetTidCachingWithFork(CloneLikeFork, exit);
}

static int CloneAndSetTid() {
  pid_t child_tid = 0;
  pid_t parent_tid = GetTidForTest();

  int rv = clone(nullptr, nullptr, CLONE_CHILD_SETTID | SIGCHLD, nullptr, nullptr, nullptr, &child_tid);
  EXPECT_NE(-1, rv);

  if (rv == 0) {
    // Child.
    EXPECT_EQ(child_tid, GetTidForTest());
    EXPECT_NE(child_tid, parent_tid);
  } else {
    EXPECT_NE(child_tid, GetTidForTest());
    EXPECT_NE(child_tid, parent_tid);
    EXPECT_EQ(GetTidForTest(), parent_tid);
  }

  return rv;
}

TEST(UNISTD_TEST, gettid_caching_and_clone_process_settid) {
  TestGetTidCachingWithFork(CloneAndSetTid, exit);
}

__attribute__((no_sanitize("hwaddress", "memtag")))
static int CloneStartRoutine(int (*start_routine)(void*)) {
  void* child_stack[1024];
  return clone(start_routine, &child_stack[1024], SIGCHLD, nullptr);
}

static int GetPidCachingCloneStartRoutine(void*) {
  AssertGetPidCorrect();
  return 123;
}

TEST(UNISTD_TEST, getpid_caching_and_clone) {
  pid_t parent_pid = getpid();
  ASSERT_EQ(syscall(__NR_getpid), parent_pid);

  int clone_result = CloneStartRoutine(GetPidCachingCloneStartRoutine);
  ASSERT_NE(clone_result, -1);

  ASSERT_EQ(parent_pid, getpid());

  AssertChildExited(clone_result, 123);
}

static int GetTidCachingCloneStartRoutine(void*) {
  AssertGetTidCorrect();
  return 123;
}

TEST(UNISTD_TEST, gettid_caching_and_clone) {
  pid_t parent_tid = GetTidForTest();
  ASSERT_EQ(syscall(__NR_gettid), parent_tid);

  int clone_result = CloneStartRoutine(GetTidCachingCloneStartRoutine);
  ASSERT_NE(clone_result, -1);

  ASSERT_EQ(parent_tid, GetTidForTest());

  AssertChildExited(clone_result, 123);
}

static int CloneChildExit(void*) {
  AssertGetPidCorrect();
  AssertGetTidCorrect();
  exit(33);
}

TEST(UNISTD_TEST, clone_fn_and_exit) {
  int clone_result = CloneStartRoutine(CloneChildExit);
  ASSERT_NE(-1, clone_result);

  AssertGetPidCorrect();
  AssertGetTidCorrect();

  AssertChildExited(clone_result, 33);
}

static void* GetPidCachingPthreadStartRoutine(void*) {
  AssertGetPidCorrect();
  return nullptr;
}

TEST(UNISTD_TEST, getpid_caching_and_pthread_create) {
  pid_t parent_pid = getpid();

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, GetPidCachingPthreadStartRoutine, nullptr));

  ASSERT_EQ(parent_pid, getpid());

  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(nullptr, result);
}

static void* GetTidCachingPthreadStartRoutine(void*) {
  AssertGetTidCorrect();
  uint64_t tid = GetTidForTest();
  return reinterpret_cast<void*>(tid);
}

TEST(UNISTD_TEST, gettid_caching_and_pthread_create) {
  pid_t parent_tid = GetTidForTest();

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, GetTidCachingPthreadStartRoutine, &parent_tid));

  ASSERT_EQ(parent_tid, GetTidForTest());

  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_NE(static_cast<uint64_t>(parent_tid), reinterpret_cast<uint64_t>(result));
}

__attribute__((noinline)) static void HwasanVforkTestChild() {
  // Allocate a tagged region on stack and leave it there.
  char x[10000];
  DoNotOptimize(x);
  _exit(0);
}

__attribute__((noinline)) static void HwasanReadMemory(const char* p, size_t size) {
  // Read memory byte-by-byte. This will blow up if the pointer tag in p does not match any memory
  // tag in [p, p+size).
  char z;
  for (size_t i = 0; i < size; ++i) {
    DoNotOptimize(z = p[i]);
  }
}

__attribute__((noinline, no_sanitize("hwaddress"))) static void HwasanVforkTestParent() {
  // Allocate a region on stack, but don't tag it (see the function attribute).
  // This depends on unallocated stack space at current function entry being untagged.
  char x[10000];
  DoNotOptimize(x);
  // Verify that contents of x[] are untagged.
  HwasanReadMemory(x, sizeof(x));
}

TEST(UNISTD_TEST, hwasan_vfork) {
  // Test hwasan annotation in vfork. This test is only interesting when built with hwasan, but it
  // is supposed to work correctly either way.
  if (vfork()) {
    HwasanVforkTestParent();
  } else {
    HwasanVforkTestChild();
  }
}

TEST_F(UNISTD_DEATHTEST, abort) {
  ASSERT_EXIT(abort(), testing::KilledBySignal(SIGABRT), "");
}

TEST(UNISTD_TEST, sethostname) {
  // The permissions check happens before the argument check, so this will
  // fail for a different reason if you're running as root than if you're
  // not, but it'll fail either way. Checking that we have the symbol is about
  // all we can do for sethostname(2).
  ASSERT_EQ(-1, sethostname("", -1));
}

TEST(UNISTD_TEST, gethostname) {
  char hostname[HOST_NAME_MAX + 1];
  memset(hostname, 0, sizeof(hostname));

  // Can we get the hostname with a big buffer?
  ASSERT_EQ(0, gethostname(hostname, HOST_NAME_MAX));

  // Can we get the hostname with a right-sized buffer?
  ASSERT_EQ(0, gethostname(hostname, strlen(hostname) + 1));

  // Does uname(2) agree?
  utsname buf;
  ASSERT_EQ(0, uname(&buf));
  ASSERT_EQ(0, strncmp(hostname, buf.nodename, sizeof(buf.nodename)));
  ASSERT_GT(strlen(hostname), 0U);

  // Do we correctly detect truncation?
  errno = 0;
  ASSERT_EQ(-1, gethostname(hostname, strlen(hostname)));
  ASSERT_ERRNO(ENAMETOOLONG);
}

TEST(UNISTD_TEST, pathconf_fpathconf) {
  TemporaryFile tf;
  long rc = 0L;
  // As a file system's block size is always power of 2, the configure values
  // for ALLOC and XFER should be power of 2 as well.
  rc = pathconf(tf.path, _PC_ALLOC_SIZE_MIN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = pathconf(tf.path, _PC_REC_MIN_XFER_SIZE);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = pathconf(tf.path, _PC_REC_XFER_ALIGN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));

  rc = fpathconf(tf.fd, _PC_ALLOC_SIZE_MIN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = fpathconf(tf.fd, _PC_REC_MIN_XFER_SIZE);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = fpathconf(tf.fd, _PC_REC_XFER_ALIGN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
}

TEST(UNISTD_TEST, _POSIX_constants) {
  // Make a tight verification of _POSIX_* / _POSIX2_* / _XOPEN_* macros, to prevent change by mistake.
  // Verify according to POSIX.1-2008.
  EXPECT_EQ(200809L, _POSIX_VERSION);

  EXPECT_EQ(2, _POSIX_AIO_LISTIO_MAX);
  EXPECT_EQ(1, _POSIX_AIO_MAX);
  EXPECT_EQ(4096, _POSIX_ARG_MAX);
  EXPECT_EQ(25, _POSIX_CHILD_MAX);
  EXPECT_EQ(20000000, _POSIX_CLOCKRES_MIN);
  EXPECT_EQ(32, _POSIX_DELAYTIMER_MAX);
  EXPECT_EQ(255, _POSIX_HOST_NAME_MAX);
  EXPECT_EQ(8, _POSIX_LINK_MAX);
  EXPECT_EQ(9, _POSIX_LOGIN_NAME_MAX);
  EXPECT_EQ(255, _POSIX_MAX_CANON);
  EXPECT_EQ(255, _POSIX_MAX_INPUT);
  EXPECT_EQ(8, _POSIX_MQ_OPEN_MAX);
  EXPECT_EQ(32, _POSIX_MQ_PRIO_MAX);
  EXPECT_EQ(14, _POSIX_NAME_MAX);
  EXPECT_EQ(8, _POSIX_NGROUPS_MAX);
  EXPECT_EQ(20, _POSIX_OPEN_MAX);
  EXPECT_EQ(256, _POSIX_PATH_MAX);
  EXPECT_EQ(512, _POSIX_PIPE_BUF);
  EXPECT_EQ(255, _POSIX_RE_DUP_MAX);
  EXPECT_EQ(8, _POSIX_RTSIG_MAX);
  EXPECT_EQ(256, _POSIX_SEM_NSEMS_MAX);
  EXPECT_EQ(32767, _POSIX_SEM_VALUE_MAX);
  EXPECT_EQ(32, _POSIX_SIGQUEUE_MAX);
  EXPECT_EQ(32767, _POSIX_SSIZE_MAX);
  EXPECT_EQ(8, _POSIX_STREAM_MAX);
#if !defined(__GLIBC__)
  EXPECT_EQ(4, _POSIX_SS_REPL_MAX);
#endif
  EXPECT_EQ(255, _POSIX_SYMLINK_MAX);
  EXPECT_EQ(8, _POSIX_SYMLOOP_MAX);
  EXPECT_EQ(4, _POSIX_THREAD_DESTRUCTOR_ITERATIONS);
  EXPECT_EQ(128, _POSIX_THREAD_KEYS_MAX);
  EXPECT_EQ(64, _POSIX_THREAD_THREADS_MAX);
  EXPECT_EQ(32, _POSIX_TIMER_MAX);
#if !defined(__GLIBC__)
  EXPECT_EQ(30, _POSIX_TRACE_EVENT_NAME_MAX);
  EXPECT_EQ(8, _POSIX_TRACE_NAME_MAX);
  EXPECT_EQ(8, _POSIX_TRACE_SYS_MAX);
  EXPECT_EQ(32, _POSIX_TRACE_USER_EVENT_MAX);
#endif
  EXPECT_EQ(9, _POSIX_TTY_NAME_MAX);
  EXPECT_EQ(6, _POSIX_TZNAME_MAX);
  EXPECT_EQ(99, _POSIX2_BC_BASE_MAX);
  EXPECT_EQ(2048, _POSIX2_BC_DIM_MAX);
  EXPECT_EQ(99, _POSIX2_BC_SCALE_MAX);
  EXPECT_EQ(1000, _POSIX2_BC_STRING_MAX);
  EXPECT_EQ(14, _POSIX2_CHARCLASS_NAME_MAX);
  EXPECT_EQ(2, _POSIX2_COLL_WEIGHTS_MAX);
  EXPECT_EQ(32, _POSIX2_EXPR_NEST_MAX);
  EXPECT_EQ(2048, _POSIX2_LINE_MAX);
  EXPECT_EQ(255, _POSIX2_RE_DUP_MAX);

  EXPECT_EQ(16, _XOPEN_IOV_MAX);
#if !defined(__GLIBC__)
  EXPECT_EQ(255, _XOPEN_NAME_MAX);
  EXPECT_EQ(1024, _XOPEN_PATH_MAX);
#endif
}

TEST(UNISTD_TEST, _POSIX_options) {
  EXPECT_EQ(_POSIX_VERSION, _POSIX_ADVISORY_INFO);
  EXPECT_GT(_POSIX_BARRIERS, 0);
  EXPECT_GT(_POSIX_SPIN_LOCKS, 0);
  EXPECT_NE(_POSIX_CHOWN_RESTRICTED, -1);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_CLOCK_SELECTION);
#if !defined(__GLIBC__) // glibc supports ancient kernels.
  EXPECT_EQ(_POSIX_VERSION, _POSIX_CPUTIME);
#endif
  EXPECT_EQ(_POSIX_VERSION, _POSIX_FSYNC);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_IPV6);
  EXPECT_GT(_POSIX_JOB_CONTROL, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MAPPED_FILES);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MEMLOCK);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MEMLOCK_RANGE);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MEMORY_PROTECTION);
#if !defined(__GLIBC__) // glibc supports ancient kernels.
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MONOTONIC_CLOCK);
#endif
  EXPECT_GT(_POSIX_NO_TRUNC, 0);
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(_POSIX_VERSION, _POSIX_PRIORITY_SCHEDULING);
#endif
  EXPECT_EQ(_POSIX_VERSION, _POSIX_RAW_SOCKETS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_READER_WRITER_LOCKS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_REALTIME_SIGNALS);
  EXPECT_GT(_POSIX_REGEXP, 0);
  EXPECT_GT(_POSIX_SAVED_IDS, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_SEMAPHORES);
  EXPECT_GT(_POSIX_SHELL, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_SPAWN);
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(-1, _POSIX_SPORADIC_SERVER);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_SYNCHRONIZED_IO);
#endif
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREADS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_ATTR_STACKADDR);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_ATTR_STACKSIZE);
#if !defined(__GLIBC__) // glibc supports ancient kernels.
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_CPUTIME);
#endif
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_PRIORITY_SCHEDULING);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_PROCESS_SHARED);
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(-1, _POSIX_THREAD_ROBUST_PRIO_PROTECT);
#endif
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_SAFE_FUNCTIONS);
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(-1, _POSIX_THREAD_SPORADIC_SERVER);
#endif
  EXPECT_EQ(_POSIX_VERSION, _POSIX_TIMEOUTS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_TIMERS);
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(-1, _POSIX_TRACE);
  EXPECT_EQ(-1, _POSIX_TRACE_EVENT_FILTER);
  EXPECT_EQ(-1, _POSIX_TRACE_INHERIT);
  EXPECT_EQ(-1, _POSIX_TRACE_LOG);
  EXPECT_EQ(-1, _POSIX_TYPED_MEMORY_OBJECTS);
#endif
  EXPECT_NE(-1, _POSIX_VDISABLE);

  EXPECT_EQ(_POSIX_VERSION, _POSIX2_VERSION);
  EXPECT_EQ(_POSIX_VERSION, _POSIX2_C_BIND);
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(_POSIX_VERSION, _POSIX2_CHAR_TERM);
#endif

  EXPECT_EQ(700, _XOPEN_VERSION);
  EXPECT_EQ(1, _XOPEN_ENH_I18N);
#if !defined(ANDROID_HOST_MUSL)
  EXPECT_EQ(1, _XOPEN_REALTIME);
  EXPECT_EQ(1, _XOPEN_REALTIME_THREADS);
  EXPECT_EQ(1, _XOPEN_SHM);
#endif
  EXPECT_EQ(1, _XOPEN_UNIX);

#if defined(__BIONIC__)
  // These tests only pass on bionic, as bionic and glibc has different support on these macros.
  // Macros like _POSIX_ASYNCHRONOUS_IO are not supported on bionic yet.
  EXPECT_EQ(-1, _POSIX_ASYNCHRONOUS_IO);
  EXPECT_EQ(-1, _POSIX_MESSAGE_PASSING);
  EXPECT_EQ(-1, _POSIX_PRIORITIZED_IO);
  EXPECT_EQ(-1, _POSIX_SHARED_MEMORY_OBJECTS);
  EXPECT_EQ(-1, _POSIX_THREAD_PRIO_INHERIT);
  EXPECT_EQ(-1, _POSIX_THREAD_PRIO_PROTECT);
  EXPECT_EQ(-1, _POSIX_THREAD_ROBUST_PRIO_INHERIT);

  EXPECT_EQ(-1, _POSIX2_C_DEV);
  EXPECT_EQ(-1, _POSIX2_FORT_DEV);
  EXPECT_EQ(-1, _POSIX2_FORT_RUN);
  EXPECT_EQ(-1, _POSIX2_LOCALEDEF);
  EXPECT_EQ(-1, _POSIX2_SW_DEV);
  EXPECT_EQ(-1, _POSIX2_UPE);

  EXPECT_EQ(-1, _XOPEN_CRYPT);
  EXPECT_EQ(-1, _XOPEN_LEGACY);
  EXPECT_EQ(-1, _XOPEN_STREAMS);
#endif // defined(__BIONIC__)
}

#define VERIFY_SYSCONF_UNKNOWN(name) \
  VerifySysconf(name, #name, [](long v){return v == -1 && errno == EINVAL;})

#define VERIFY_SYSCONF_UNSUPPORTED(name) \
  VerifySysconf(name, #name, [](long v){return v == -1 && errno == 0;})

// sysconf() means unlimited when it returns -1 with errno unchanged.
#define VERIFY_SYSCONF_POSITIVE(name) \
  VerifySysconf(name, #name, [](long v){return (v > 0 || v == -1) && errno == 0;})

#define VERIFY_SYSCONF_POSIX_VERSION(name) \
  VerifySysconf(name, #name, [](long v){return v == _POSIX_VERSION && errno == 0;})

static void VerifySysconf(int option, const char *option_name, bool (*verify)(long)) {
  errno = 0;
  long ret = sysconf(option);
  EXPECT_TRUE(verify(ret)) << "name = " << option_name << ", ret = "
      << ret <<", Error Message: " << strerror(errno);
}

TEST(UNISTD_TEST, sysconf) {
  VERIFY_SYSCONF_POSIX_VERSION(_SC_ADVISORY_INFO);
  VERIFY_SYSCONF_POSITIVE(_SC_ARG_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_BARRIERS);
  VERIFY_SYSCONF_POSITIVE(_SC_BC_BASE_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_BC_DIM_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_BC_SCALE_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_CHILD_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_CLK_TCK);
  VERIFY_SYSCONF_POSITIVE(_SC_COLL_WEIGHTS_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_CPUTIME);
  VERIFY_SYSCONF_POSITIVE(_SC_EXPR_NEST_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_LINE_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_NGROUPS_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_OPEN_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_PASS_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_2_C_BIND);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_FORT_DEV);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_FORT_RUN);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_UPE);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_2_VERSION);
  VERIFY_SYSCONF_POSITIVE(_SC_JOB_CONTROL);
  VERIFY_SYSCONF_POSITIVE(_SC_SAVED_IDS);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_VERSION);
  VERIFY_SYSCONF_POSITIVE(_SC_RE_DUP_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_STREAM_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_TZNAME_MAX);
  VerifySysconf(_SC_XOPEN_VERSION, "_SC_XOPEN_VERSION", [](long v){return v == _XOPEN_VERSION && errno == 0;});
  VERIFY_SYSCONF_POSITIVE(_SC_ATEXIT_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_IOV_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_UIO_MAXIOV);
  EXPECT_EQ(sysconf(_SC_IOV_MAX), sysconf(_SC_UIO_MAXIOV));
  VERIFY_SYSCONF_POSITIVE(_SC_PAGESIZE);
  VERIFY_SYSCONF_POSITIVE(_SC_PAGE_SIZE);
  VerifySysconf(_SC_PAGE_SIZE, "_SC_PAGE_SIZE",
                [](long v){return v == sysconf(_SC_PAGESIZE) && errno == 0 && v == getpagesize();});
  VERIFY_SYSCONF_POSITIVE(_SC_XOPEN_UNIX);
  VERIFY_SYSCONF_POSITIVE(_SC_AIO_LISTIO_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_AIO_MAX);
  VerifySysconf(_SC_AIO_PRIO_DELTA_MAX, "_SC_AIO_PRIO_DELTA_MAX", [](long v){return v >= 0 && errno == 0;});
  VERIFY_SYSCONF_POSITIVE(_SC_DELAYTIMER_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_MQ_OPEN_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_MQ_PRIO_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_RTSIG_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_SEM_NSEMS_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_SEM_VALUE_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_SPIN_LOCKS);
  VERIFY_SYSCONF_POSITIVE(_SC_TIMER_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_FSYNC);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_MAPPED_FILES);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_MEMLOCK);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_MEMLOCK_RANGE);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_MEMORY_PROTECTION);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_PRIORITY_SCHEDULING);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_REALTIME_SIGNALS);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_SEMAPHORES);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_SYNCHRONIZED_IO);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_TIMERS);
  VERIFY_SYSCONF_POSITIVE(_SC_GETGR_R_SIZE_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_GETPW_R_SIZE_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_LOGIN_NAME_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_THREAD_DESTRUCTOR_ITERATIONS);
  VERIFY_SYSCONF_POSITIVE(_SC_THREAD_KEYS_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_THREAD_STACK_MIN);
  VERIFY_SYSCONF_POSITIVE(_SC_THREAD_THREADS_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_TTY_NAME_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREADS);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_ATTR_STACKADDR);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_ATTR_STACKSIZE);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_PRIORITY_SCHEDULING);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_THREAD_PRIO_INHERIT);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_THREAD_PRIO_PROTECT);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_SAFE_FUNCTIONS);
  VERIFY_SYSCONF_POSITIVE(_SC_NPROCESSORS_CONF);
  VERIFY_SYSCONF_POSITIVE(_SC_NPROCESSORS_ONLN);
  VERIFY_SYSCONF_POSITIVE(_SC_PHYS_PAGES);
  VERIFY_SYSCONF_POSITIVE(_SC_AVPHYS_PAGES);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_MONOTONIC_CLOCK);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_PBS);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_PBS_ACCOUNTING);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_PBS_CHECKPOINT);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_PBS_LOCATE);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_PBS_MESSAGE);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_PBS_TRACK);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_CLOCK_SELECTION);
  VERIFY_SYSCONF_POSITIVE(_SC_HOST_NAME_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_IPV6);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_RAW_SOCKETS);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_READER_WRITER_LOCKS);
  VERIFY_SYSCONF_POSITIVE(_SC_REGEXP);
  VERIFY_SYSCONF_POSITIVE(_SC_SHELL);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_SPAWN);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_SPORADIC_SERVER);
  VERIFY_SYSCONF_POSITIVE(_SC_SYMLOOP_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_CPUTIME);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_PROCESS_SHARED);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_THREAD_SPORADIC_SERVER);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_TIMEOUTS);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE_EVENT_FILTER);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE_EVENT_NAME_MAX);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE_INHERIT);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE_LOG);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE_NAME_MAX);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE_SYS_MAX);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TRACE_USER_EVENT_MAX);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_TYPED_MEMORY_OBJECTS);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_XOPEN_STREAMS);

#if defined(__LP64__)
  VERIFY_SYSCONF_UNSUPPORTED(_SC_V7_ILP32_OFF32);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_V7_ILP32_OFFBIG);
  VERIFY_SYSCONF_POSITIVE(_SC_V7_LP64_OFF64);
  VERIFY_SYSCONF_POSITIVE(_SC_V7_LPBIG_OFFBIG);
#else
  VERIFY_SYSCONF_POSITIVE(_SC_V7_ILP32_OFF32);
#if defined(__BIONIC__)
  // bionic does not support 64 bits off_t type on 32bit machine.
  VERIFY_SYSCONF_UNSUPPORTED(_SC_V7_ILP32_OFFBIG);
#endif
  VERIFY_SYSCONF_UNSUPPORTED(_SC_V7_LP64_OFF64);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_V7_LPBIG_OFFBIG);
#endif

#if defined(__BIONIC__)
  // Tests can only run on bionic, as bionic and glibc have different support for these options.
  // Below options are not supported on bionic yet.
  VERIFY_SYSCONF_UNSUPPORTED(_SC_ASYNCHRONOUS_IO);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_MESSAGE_PASSING);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_PRIORITIZED_IO);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_SHARED_MEMORY_OBJECTS);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_THREAD_ROBUST_PRIO_INHERIT);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_THREAD_ROBUST_PRIO_PROTECT);

  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_C_DEV);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_LOCALEDEF);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_2_SW_DEV);

  VERIFY_SYSCONF_UNSUPPORTED(_SC_XOPEN_CRYPT);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_XOPEN_LEGACY);
  VERIFY_SYSCONF_UNSUPPORTED(_SC_XOPEN_UUCP);
#endif // defined(__BIONIC__)
}

TEST(UNISTD_TEST, get_cpu_count_from_string) {
  ASSERT_EQ(0, GetCpuCountFromString(" "));
  ASSERT_EQ(1, GetCpuCountFromString("0"));
  ASSERT_EQ(40, GetCpuCountFromString("0-39"));
  ASSERT_EQ(4, GetCpuCountFromString("0, 1-2, 4\n"));
}

TEST(UNISTD_TEST, sysconf_SC_NPROCESSORS_make_sense) {
  ASSERT_LE(sysconf(_SC_NPROCESSORS_ONLN), sysconf(_SC_NPROCESSORS_CONF));
}

TEST(UNISTD_TEST, sysconf_SC_NPROCESSORS_ONLN) {
  std::string line;
  ASSERT_TRUE(android::base::ReadFileToString("/sys/devices/system/cpu/online", &line));
  long online_cpus = 0;
  for (const std::string& s : android::base::Split(line, ",")) {
    std::vector<std::string> numbers = android::base::Split(s, "-");
    if (numbers.size() == 1u) {
      online_cpus++;
    } else {
      online_cpus += atoi(numbers[1].c_str()) - atoi(numbers[0].c_str()) + 1;
    }
  }
  ASSERT_EQ(online_cpus, sysconf(_SC_NPROCESSORS_ONLN));
}

TEST(UNISTD_TEST, sysconf_SC_ARG_MAX) {
  // Since Linux 2.6.23, ARG_MAX isn't a constant and depends on RLIMIT_STACK.
  // See setup_arg_pages() in the kernel for the gory details:
  // https://elixir.bootlin.com/linux/v6.6.4/source/fs/exec.c#L749

  // Get our current limit, and set things up so we restore the limit.
  rlimit rl;
  ASSERT_EQ(0, getrlimit(RLIMIT_STACK, &rl));
  uint64_t original_rlim_cur = rl.rlim_cur;
  if (rl.rlim_cur == RLIM_INFINITY) {
    rl.rlim_cur = 8 * 1024 * 1024; // Bionic reports unlimited stacks as 8MiB.
  }
  auto guard = android::base::make_scope_guard([&rl, original_rlim_cur]() {
    rl.rlim_cur = original_rlim_cur;
    ASSERT_EQ(0, setrlimit(RLIMIT_STACK, &rl));
  });

  // _SC_ARG_MAX should be 1/4 the stack size.
  EXPECT_EQ(static_cast<long>(rl.rlim_cur / 4), sysconf(_SC_ARG_MAX));

  // If you have a really small stack, the kernel still guarantees a stack
  // expansion of 128KiB (see setup_arg_pages() in fs/exec.c).
  rl.rlim_cur = 1024;
  rl.rlim_max = RLIM_INFINITY;
  ASSERT_EQ(0, setrlimit(RLIMIT_STACK, &rl));

  // The stack expansion number is defined in fs/exec.c.
  // https://elixir.bootlin.com/linux/v6.6.4/source/fs/exec.c#L845
  constexpr long kernel_stack_expansion = 131072;
  EXPECT_EQ(kernel_stack_expansion, sysconf(_SC_ARG_MAX));

  // If you have a large stack, the kernel will keep the stack
  // expansion to 128KiB (see setup_arg_pages() in fs/exec.c).
  rl.rlim_cur = 524288;
  rl.rlim_max = RLIM_INFINITY;
  ASSERT_EQ(0, setrlimit(RLIMIT_STACK, &rl));

  EXPECT_EQ(kernel_stack_expansion, sysconf(_SC_ARG_MAX));
}

TEST(UNISTD_TEST, sysconf_unknown) {
  VERIFY_SYSCONF_UNKNOWN(-1);
  VERIFY_SYSCONF_UNKNOWN(666);
}

[[maybe_unused]] static void show_cache(const char* name, long size, long assoc, long line_size) {
  printf("%s cache size: %ld bytes, line size %ld bytes, ", name, size, line_size);
  if (assoc == 0) {
    printf("fully");
  } else {
    printf("%ld-way", assoc);
  }
  printf(" associative\n");
}

TEST(UNISTD_TEST, sysconf_cache) {
#if defined(ANDROID_HOST_MUSL)
  GTEST_SKIP() << "musl does not have _SC_LEVEL?_?CACHE_SIZE";
#else
  // It's not obvious we can _test_ any of these, but we can at least
  // show the output for humans to inspect.
  show_cache("L1D", sysconf(_SC_LEVEL1_DCACHE_SIZE), sysconf(_SC_LEVEL1_DCACHE_ASSOC), sysconf(_SC_LEVEL1_DCACHE_LINESIZE));
  show_cache("L1I", sysconf(_SC_LEVEL1_ICACHE_SIZE), sysconf(_SC_LEVEL1_ICACHE_ASSOC), sysconf(_SC_LEVEL1_ICACHE_LINESIZE));
  show_cache("L2", sysconf(_SC_LEVEL2_CACHE_SIZE), sysconf(_SC_LEVEL2_CACHE_ASSOC), sysconf(_SC_LEVEL2_CACHE_LINESIZE));
  show_cache("L3", sysconf(_SC_LEVEL3_CACHE_SIZE), sysconf(_SC_LEVEL3_CACHE_ASSOC), sysconf(_SC_LEVEL3_CACHE_LINESIZE));
  show_cache("L4", sysconf(_SC_LEVEL4_CACHE_SIZE), sysconf(_SC_LEVEL4_CACHE_ASSOC), sysconf(_SC_LEVEL4_CACHE_LINESIZE));
#endif
}

TEST(UNISTD_TEST, dup2_same) {
  // POSIX says of dup2:
  // If fildes2 is already a valid open file descriptor ...
  // [and] fildes is equal to fildes2 ... dup2() shall return
  // fildes2 without closing it.
  // This isn't true of dup3(2), so we need to manually implement that.

  // Equal and valid.
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_TRUE(fd != -1);
  ASSERT_EQ(fd, dup2(fd, fd));
  ASSERT_EQ(0, close(fd)); // Check that dup2 didn't close fd.

  // Equal, but invalid.
  errno = 0;
  ASSERT_EQ(-1, dup2(fd, fd));
  ASSERT_ERRNO(EBADF);
}

TEST(UNISTD_TEST, dup3) {
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_EQ(666, dup3(fd, 666, 0));
  ASSERT_FALSE(CloseOnExec(666));
  close(666);
  ASSERT_EQ(667, dup3(fd, 667, O_CLOEXEC));
  ASSERT_TRUE(CloseOnExec(667));
  close(667);
  close(fd);
}

TEST(UNISTD_TEST, lockf_smoke) {
  constexpr off64_t file_size = 32*1024LL;

  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, file_size));

  // Lock everything.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_LOCK, file_size));

  // Try-lock everything, this should succeed too.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_TLOCK, file_size));

  // Check status.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_TEST, file_size));

  // Unlock file.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_ULOCK, file_size));
}

TEST(UNISTD_TEST, lockf_zero) {
  constexpr off64_t file_size = 32*1024LL;

  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, file_size));

  // Lock everything by specifying a size of 0 (meaning "to the end, even if it changes").
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_LOCK, 0));

  // Check that it's locked.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_TEST, file_size));

  // Move the end.
  ASSERT_EQ(0, ftruncate(tf.fd, 2*file_size));

  // Check that the new section is locked too.
  ASSERT_EQ(file_size, lseek64(tf.fd, file_size, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_TEST, 2*file_size));
}

TEST(UNISTD_TEST, lockf_negative) {
  constexpr off64_t file_size = 32*1024LL;

  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, file_size));

  // Lock everything, but specifying the range in reverse.
  ASSERT_EQ(file_size, lseek64(tf.fd, file_size, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_LOCK, -file_size));

  // Check that it's locked.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_TEST, file_size));
}

TEST(UNISTD_TEST, lockf_with_child) {
  constexpr off64_t file_size = 32*1024LL;

  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, file_size));

  // Lock everything.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_LOCK, file_size));

  // Fork a child process
  pid_t pid = fork();
  ASSERT_NE(-1, pid);
  if (pid == 0) {
    // Check that the child cannot lock the file.
    ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
    ASSERT_EQ(-1, lockf64(tf.fd, F_TLOCK, file_size));
    ASSERT_ERRNO(EAGAIN);
    // Check also that it reports itself as locked.
    ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
    ASSERT_EQ(-1, lockf64(tf.fd, F_TEST, file_size));
    ASSERT_ERRNO(EACCES);
    _exit(0);
  }
  AssertChildExited(pid, 0);
}

TEST(UNISTD_TEST, lockf_partial_with_child) {
  constexpr off64_t file_size = 32*1024LL;

  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, file_size));

  // Lock the first half of the file.
  ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_LOCK, file_size/2));

  // Fork a child process.
  pid_t pid = fork();
  ASSERT_NE(-1, pid);
  if (pid == 0) {
    // Check that the child can lock the other half.
    ASSERT_EQ(file_size/2, lseek64(tf.fd, file_size/2, SEEK_SET));
    ASSERT_EQ(0, lockf64(tf.fd, F_TLOCK, file_size/2));
    // Check that the child cannot lock the first half.
    ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
    ASSERT_EQ(-1, lockf64(tf.fd, F_TEST, file_size/2));
    ASSERT_ERRNO(EACCES);
    // Check also that it reports itself as locked.
    ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
    ASSERT_EQ(-1, lockf64(tf.fd, F_TEST, file_size/2));
    ASSERT_ERRNO(EACCES);
    _exit(0);
  }
  AssertChildExited(pid, 0);

  // The second half was locked by the child, but the lock disappeared
  // when the process exited, so check it can be locked now.
  ASSERT_EQ(file_size/2, lseek64(tf.fd, file_size/2, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_TLOCK, file_size/2));
}

TEST(UNISTD_TEST, getdomainname) {
  struct utsname u;
  ASSERT_EQ(0, uname(&u));

  char buf[sizeof(u.domainname)];
  ASSERT_EQ(0, getdomainname(buf, sizeof(buf)));
  EXPECT_STREQ(u.domainname, buf);

#if defined(__BIONIC__)
  // bionic and glibc have different behaviors when len is too small
  ASSERT_EQ(-1, getdomainname(buf, strlen(u.domainname)));
  EXPECT_ERRNO(EINVAL);
#endif
}

TEST(UNISTD_TEST, setdomainname) {
  __user_cap_header_struct header;
  memset(&header, 0, sizeof(header));
  header.version = _LINUX_CAPABILITY_VERSION_3;

  __user_cap_data_struct old_caps[_LINUX_CAPABILITY_U32S_3];
  ASSERT_EQ(0, capget(&header, &old_caps[0]));

  auto admin_idx = CAP_TO_INDEX(CAP_SYS_ADMIN);
  auto admin_mask = CAP_TO_MASK(CAP_SYS_ADMIN);
  bool has_admin = old_caps[admin_idx].effective & admin_mask;
  if (has_admin) {
    __user_cap_data_struct new_caps[_LINUX_CAPABILITY_U32S_3];
    memcpy(new_caps, old_caps, sizeof(new_caps));
    new_caps[admin_idx].effective &= ~admin_mask;

    ASSERT_EQ(0, capset(&header, &new_caps[0])) << "failed to drop admin privileges";
  }

  const char* name = "newdomainname";
  ASSERT_EQ(-1, setdomainname(name, strlen(name)));
  ASSERT_ERRNO(EPERM);

  if (has_admin) {
    ASSERT_EQ(0, capset(&header, &old_caps[0])) << "failed to restore admin privileges";
  }
}

TEST(UNISTD_TEST, execve_failure) {
  ExecTestHelper eth;
  errno = 0;
  ASSERT_EQ(-1, execve("/", eth.GetArgs(), eth.GetEnv()));
  ASSERT_ERRNO(EACCES);
}

static void append_llvm_cov_env_var(std::string& env_str) {
  if (getenv("LLVM_PROFILE_FILE") != nullptr)
    env_str.append("__LLVM_PROFILE_RT_INIT_ONCE=__LLVM_PROFILE_RT_INIT_ONCE\n");
}

TEST(UNISTD_TEST, execve_args) {
  // int execve(const char* path, char* argv[], char* envp[]);

  // Test basic argument passing.
  ExecTestHelper eth;
  eth.SetArgs({"echo", "hello", "world", nullptr});
  eth.Run([&]() { execve(BIN_DIR "echo", eth.GetArgs(), eth.GetEnv()); }, 0, "hello world\n");

  // Test environment variable setting too.
  eth.SetArgs({"printenv", nullptr});
  eth.SetEnv({"A=B", nullptr});

  std::string expected_output("A=B\n");
  append_llvm_cov_env_var(expected_output);

  eth.Run([&]() { execve(BIN_DIR "printenv", eth.GetArgs(), eth.GetEnv()); }, 0,
          expected_output.c_str());
}

TEST(UNISTD_TEST, execl_failure) {
  errno = 0;
  ASSERT_EQ(-1, execl("/", "/", nullptr));
  ASSERT_ERRNO(EACCES);
}

TEST(UNISTD_TEST, execl) {
  ExecTestHelper eth;
  // int execl(const char* path, const char* arg, ...);
  eth.Run([&]() { execl(BIN_DIR "echo", "echo", "hello", "world", nullptr); }, 0, "hello world\n");
}

TEST(UNISTD_TEST, execle_failure) {
  ExecTestHelper eth;
  errno = 0;
  ASSERT_EQ(-1, execle("/", "/", nullptr, eth.GetEnv()));
  ASSERT_ERRNO(EACCES);
}

TEST(UNISTD_TEST, execle) {
  ExecTestHelper eth;
  eth.SetEnv({"A=B", nullptr});

  std::string expected_output("A=B\n");
  append_llvm_cov_env_var(expected_output);

  // int execle(const char* path, const char* arg, ..., char* envp[]);
  eth.Run([&]() { execle(BIN_DIR "printenv", "printenv", nullptr, eth.GetEnv()); }, 0,
          expected_output.c_str());
}

TEST(UNISTD_TEST, execv_failure) {
  ExecTestHelper eth;
  errno = 0;
  ASSERT_EQ(-1, execv("/", eth.GetArgs()));
  ASSERT_ERRNO(EACCES);
}

TEST(UNISTD_TEST, execv) {
  ExecTestHelper eth;
  eth.SetArgs({"echo", "hello", "world", nullptr});
  // int execv(const char* path, char* argv[]);
  eth.Run([&]() { execv(BIN_DIR "echo", eth.GetArgs()); }, 0, "hello world\n");
}

TEST(UNISTD_TEST, execlp_failure) {
  errno = 0;
  ASSERT_EQ(-1, execlp("/", "/", nullptr));
  ASSERT_ERRNO(EACCES);
}

TEST(UNISTD_TEST, execlp) {
  ExecTestHelper eth;
  // int execlp(const char* file, const char* arg, ...);
  eth.Run([&]() { execlp("echo", "echo", "hello", "world", nullptr); }, 0, "hello world\n");
}

TEST(UNISTD_TEST, execvp_failure) {
  ExecTestHelper eth;
  eth.SetArgs({nullptr});
  errno = 0;
  ASSERT_EQ(-1, execvp("/", eth.GetArgs()));
  ASSERT_ERRNO(EACCES);
}

TEST(UNISTD_TEST, execvp) {
  ExecTestHelper eth;
  eth.SetArgs({"echo", "hello", "world", nullptr});
  // int execvp(const char* file, char* argv[]);
  eth.Run([&]() { execvp("echo", eth.GetArgs()); }, 0, "hello world\n");
}

TEST(UNISTD_TEST, execvpe_failure) {
  ExecTestHelper eth;
  errno = 0;
  ASSERT_EQ(-1, execvpe("this-does-not-exist", eth.GetArgs(), eth.GetEnv()));
  // Running in CTS we might not even be able to search all directories in $PATH.
  ASSERT_TRUE(errno == ENOENT || errno == EACCES) << strerror(errno);
}

TEST(UNISTD_TEST, execvpe) {
  // int execvpe(const char* file, char* argv[], char* envp[]);

  // Test basic argument passing.
  ExecTestHelper eth;
  eth.SetArgs({"echo", "hello", "world", nullptr});
  eth.Run([&]() { execvpe("echo", eth.GetArgs(), eth.GetEnv()); }, 0, "hello world\n");

  // Test environment variable setting too.
  eth.SetArgs({"printenv", nullptr});
  eth.SetEnv({"A=B", nullptr});

  std::string expected_output("A=B\n");
  append_llvm_cov_env_var(expected_output);

  eth.Run([&]() { execvpe("printenv", eth.GetArgs(), eth.GetEnv()); }, 0, expected_output.c_str());
}

TEST(UNISTD_TEST, execvpe_ENOEXEC) {
  // Create a shell script with #!.
  TemporaryFile tf;
  ASSERT_TRUE(android::base::WriteStringToFile("#!" BIN_DIR "sh\necho script\n", tf.path));

  // Set $PATH so we can find it.
  setenv("PATH", dirname(tf.path), 1);

  ExecTestHelper eth;
  eth.SetArgs({basename(tf.path), nullptr});

  // It's not inherently executable.
  errno = 0;
  ASSERT_EQ(-1, execvpe(basename(tf.path), eth.GetArgs(), eth.GetEnv()));
  ASSERT_ERRNO(EACCES);

  // Make it executable (and keep it writable because we're going to rewrite it below).
  ASSERT_EQ(0, chmod(tf.path, 0777));

  // TemporaryFile will have a writable fd, so we can test ETXTBSY while we're here...
  errno = 0;
  ASSERT_EQ(-1, execvpe(basename(tf.path), eth.GetArgs(), eth.GetEnv()));
  ASSERT_ERRNO(ETXTBSY);

  // 1. The simplest test: the kernel should handle this.
  ASSERT_EQ(0, close(tf.fd));
  eth.Run([&]() { execvpe(basename(tf.path), eth.GetArgs(), eth.GetEnv()); }, 0, "script\n");

  // 2. Try again without a #!. We should have to handle this ourselves.
  ASSERT_TRUE(android::base::WriteStringToFile("echo script\n", tf.path));
  eth.Run([&]() { execvpe(basename(tf.path), eth.GetArgs(), eth.GetEnv()); }, 0, "script\n");

  // 3. Again without a #!, but also with a leading '/', since that's a special case in the
  // implementation.
  eth.Run([&]() { execvpe(tf.path, eth.GetArgs(), eth.GetEnv()); }, 0, "script\n");
}

TEST(UNISTD_TEST, execvp_libcore_test_55017) {
  ExecTestHelper eth;
  eth.SetArgs({"/system/bin/does-not-exist", nullptr});

  errno = 0;
  ASSERT_EQ(-1, execvp("/system/bin/does-not-exist", eth.GetArgs()));
  ASSERT_ERRNO(ENOENT);
}

TEST(UNISTD_TEST, exec_argv0_null) {
  // http://b/33276926 and http://b/227498625.
  //
  // With old kernels, bionic will see the null pointer and use "<unknown>" but
  // with new (5.18+) kernels, the kernel will already have substituted the
  // empty string, so we don't make any assertion here about what (if anything)
  // comes before the first ':'.
  //
  // If this ever causes trouble, we could change bionic to replace _either_ the
  // null pointer or the empty string. We could also use the actual name from
  // readlink() on /proc/self/exe if we ever had reason to disallow programs
  // from trying to hide like this.
  char* args[] = {nullptr};
  char* envs[] = {nullptr};
  ASSERT_EXIT(execve("/system/bin/run-as", args, envs), testing::ExitedWithCode(1),
              ": usage: run-as");
}

TEST(UNISTD_TEST, fexecve_failure) {
  ExecTestHelper eth;
  errno = 0;
  int fd = open("/", O_RDONLY);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(-1, fexecve(fd, eth.GetArgs(), eth.GetEnv()));
  ASSERT_ERRNO(EACCES);
  close(fd);
}

TEST(UNISTD_TEST, fexecve_bad_fd) {
  ExecTestHelper eth;
  errno = 0;
  ASSERT_EQ(-1, fexecve(-1, eth.GetArgs(), eth.GetEnv()));
  ASSERT_ERRNO(EBADF);
}

TEST(UNISTD_TEST, fexecve_args) {
  // Test basic argument passing.
  int echo_fd = open(BIN_DIR "echo", O_RDONLY | O_CLOEXEC);
  ASSERT_NE(-1, echo_fd);
  ExecTestHelper eth;
  eth.SetArgs({"echo", "hello", "world", nullptr});
  eth.Run([&]() { fexecve(echo_fd, eth.GetArgs(), eth.GetEnv()); }, 0, "hello world\n");
  close(echo_fd);

  // Test environment variable setting too.
  int printenv_fd = open(BIN_DIR "printenv", O_RDONLY | O_CLOEXEC);
  ASSERT_NE(-1, printenv_fd);
  eth.SetArgs({"printenv", nullptr});
  eth.SetEnv({"A=B", nullptr});

  std::string expected_output("A=B\n");
  append_llvm_cov_env_var(expected_output);

  eth.Run([&]() { fexecve(printenv_fd, eth.GetArgs(), eth.GetEnv()); }, 0, expected_output.c_str());
  close(printenv_fd);
}

TEST(UNISTD_TEST, getlogin_r) {
  char buf[LOGIN_NAME_MAX] = {};
  EXPECT_EQ(ERANGE, getlogin_r(buf, 0));
  EXPECT_EQ(0, getlogin_r(buf, sizeof(buf)));
  EXPECT_STREQ(getlogin(), buf);
}

TEST(UNISTD_TEST, swab) {
  // POSIX: "The swab() function shall copy nbytes bytes, which are pointed to by src,
  // to the object pointed to by dest, exchanging adjacent bytes."
  char buf[BUFSIZ];
  memset(buf, 'x', sizeof(buf));
  swab("ehll oowlr\0d", buf, 12);
  ASSERT_STREQ("hello world", buf);
}

TEST(UNISTD_TEST, swab_odd_byte_count) {
  // POSIX: "If nbytes is odd, swab() copies and exchanges nbytes-1 bytes and the disposition
  // of the last byte is unspecified."
  // ...but it seems unreasonable to not just leave the last byte alone.
  char buf[BUFSIZ];
  memset(buf, 'x', sizeof(buf));
  swab("012345", buf, 3);
  ASSERT_EQ('1', buf[0]);
  ASSERT_EQ('0', buf[1]);
  ASSERT_EQ('x', buf[2]);
}

TEST(UNISTD_TEST, swab_overlap) {
  // POSIX: "If copying takes place between objects that overlap, the behavior is undefined."
  // ...but it seems unreasonable to not just do the right thing.
  char buf[] = "012345";
  swab(buf, buf, 4);
  ASSERT_EQ('1', buf[0]);
  ASSERT_EQ('0', buf[1]);
  ASSERT_EQ('3', buf[2]);
  ASSERT_EQ('2', buf[3]);
  ASSERT_EQ('4', buf[4]);
  ASSERT_EQ('5', buf[5]);
  ASSERT_EQ(0, buf[6]);
}

TEST(UNISTD_TEST, swab_negative_byte_count) {
  // POSIX: "If nbytes is negative, swab() does nothing."
  char buf[BUFSIZ];
  memset(buf, 'x', sizeof(buf));
  swab("hello", buf, -1);
  ASSERT_EQ('x', buf[0]);
}

TEST(UNISTD_TEST, usleep) {
  auto t0 = std::chrono::steady_clock::now();
  ASSERT_EQ(0, usleep(5000));
  auto t1 = std::chrono::steady_clock::now();
  ASSERT_GE(t1-t0, 5000us);
}

TEST(UNISTD_TEST, sleep) {
  auto t0 = std::chrono::steady_clock::now();
  ASSERT_EQ(0U, sleep(1));
  auto t1 = std::chrono::steady_clock::now();
  ASSERT_GE(t1-t0, 1s);
}

TEST(UNISTD_TEST, close_range) {
#if defined(__GLIBC__)
  GTEST_SKIP() << "glibc too old";
#elif defined(ANDROID_HOST_MUSL)
  GTEST_SKIP() << "musl does not have close_range";
#else   // __GLIBC__
  int fd = open("/proc/version", O_RDONLY);
  ASSERT_GE(fd, 0);

  int rc = close_range(fd, fd, 0);
  if (rc == -1 && errno == ENOSYS) GTEST_SKIP() << "no close_range() in this kernel";
  ASSERT_EQ(0, rc) << strerror(errno);

  // Check the fd is actually closed.
  ASSERT_EQ(close(fd), -1);
  ASSERT_ERRNO(EBADF);
#endif  // __GLIBC__
}

TEST(UNISTD_TEST, copy_file_range) {
#if defined(__GLIBC__)
  GTEST_SKIP() << "glibc too old";
#else   // __GLIBC__
  TemporaryFile tf;
  ASSERT_TRUE(android::base::WriteStringToFd("hello world", tf.fd));
  ASSERT_EQ(0, lseek(tf.fd, SEEK_SET, 0));
  TemporaryFile tf2;
  ASSERT_EQ(11, copy_file_range(tf.fd, NULL, tf2.fd, NULL, 11, 0));
  ASSERT_EQ(0, lseek(tf2.fd, SEEK_SET, 0));
  std::string content;
  ASSERT_TRUE(android::base::ReadFdToString(tf2.fd, &content));
  ASSERT_EQ("hello world", content);
#endif  // __GLIBC__
}
