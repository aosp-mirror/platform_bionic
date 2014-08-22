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
#include "ScopedSignalHandler.h"
#include "TemporaryFile.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>

TEST(unistd, sysconf_SC_MONOTONIC_CLOCK) {
  ASSERT_GT(sysconf(_SC_MONOTONIC_CLOCK), 0);
}

static void* get_brk() {
  return sbrk(0);
}

static void* page_align(uintptr_t addr) {
  uintptr_t mask = sysconf(_SC_PAGE_SIZE) - 1;
  return reinterpret_cast<void*>((addr + mask) & ~mask);
}

TEST(unistd, brk) {
  void* initial_break = get_brk();

  // The kernel aligns the break to a page.
  void* new_break = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(initial_break) + 1);
  ASSERT_EQ(0, brk(new_break));
  ASSERT_GE(get_brk(), new_break);

  new_break = page_align(reinterpret_cast<uintptr_t>(initial_break) + sysconf(_SC_PAGE_SIZE));
  ASSERT_EQ(0, brk(new_break));
  ASSERT_EQ(get_brk(), new_break);
}

TEST(unistd, brk_ENOMEM) {
  ASSERT_EQ(-1, brk(reinterpret_cast<void*>(-1)));
  ASSERT_EQ(ENOMEM, errno);
}

#if defined(__GLIBC__)
#define SBRK_MIN INTPTR_MIN
#define SBRK_MAX INTPTR_MAX
#else
#define SBRK_MIN PTRDIFF_MIN
#define SBRK_MAX PTRDIFF_MAX
#endif

TEST(unistd, sbrk_ENOMEM) {
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
  ASSERT_EQ(ENOMEM, errno);

  // Set the current break to a point that will cause an overflow.
  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX));

  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MIN));
  ASSERT_EQ(ENOMEM, errno);

  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX) - 1);

  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MIN + 1));
  ASSERT_EQ(ENOMEM, errno);
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
    ASSERT_EQ(ENOMEM, errno);
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
    ASSERT_EQ(ENOMEM, errno);
#endif
  }
#endif
}

TEST(unistd, truncate) {
  TemporaryFile tf;
  ASSERT_EQ(0, close(tf.fd));
  ASSERT_EQ(0, truncate(tf.filename, 123));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(unistd, truncate64) {
  TemporaryFile tf;
  ASSERT_EQ(0, close(tf.fd));
  ASSERT_EQ(0, truncate64(tf.filename, 123));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(unistd, ftruncate) {
  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, 123));
  ASSERT_EQ(0, close(tf.fd));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(unistd, ftruncate64) {
  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate64(tf.fd, 123));
  ASSERT_EQ(0, close(tf.fd));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

static bool g_pause_test_flag = false;
static void PauseTestSignalHandler(int) {
  g_pause_test_flag = true;
}

TEST(unistd, pause) {
  ScopedSignalHandler handler(SIGALRM, PauseTestSignalHandler);

  alarm(1);
  ASSERT_FALSE(g_pause_test_flag);
  ASSERT_EQ(-1, pause());
  ASSERT_TRUE(g_pause_test_flag);
}

TEST(unistd, read) {
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

TEST(unistd, read_EBADF) {
  // read returns ssize_t which is 64-bits on LP64, so it's worth explicitly checking that
  // our syscall stubs correctly return a 64-bit -1.
  char buf[1];
  ASSERT_EQ(-1, read(-1, buf, sizeof(buf)));
  ASSERT_EQ(EBADF, errno);
}

TEST(unistd, syscall_long) {
  // Check that syscall(3) correctly returns long results.
  // https://code.google.com/p/android/issues/detail?id=73952
  // We assume that the break is > 4GiB, but this is potentially flaky.
  uintptr_t p = reinterpret_cast<uintptr_t>(sbrk(0));
  ASSERT_EQ(p, static_cast<uintptr_t>(syscall(__NR_brk, 0)));
}

TEST(unistd, alarm) {
  ASSERT_EQ(0U, alarm(0));
}

TEST(unistd, _exit) {
  int pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    _exit(99);
  }

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(99, WEXITSTATUS(status));
}

TEST(unistd, getenv_unsetenv) {
  ASSERT_EQ(0, setenv("test-variable", "hello", 1));
  ASSERT_STREQ("hello", getenv("test-variable"));
  ASSERT_EQ(0, unsetenv("test-variable"));
  ASSERT_TRUE(getenv("test-variable") == NULL);
}

TEST(unistd, unsetenv_EINVAL) {
  EXPECT_EQ(-1, unsetenv(NULL));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, unsetenv(""));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, unsetenv("a=b"));
  EXPECT_EQ(EINVAL, errno);
}

TEST(unistd, setenv_EINVAL) {
  EXPECT_EQ(-1, setenv(NULL, "value", 0));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, setenv(NULL, "value", 1));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, setenv("", "value", 0));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, setenv("", "value", 1));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, setenv("a=b", "value", 0));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, setenv("a=b", "value", 1));
  EXPECT_EQ(EINVAL, errno);
}

TEST(unistd, setenv) {
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

TEST(unistd, putenv) {
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

TEST(unistd, clearenv) {
  extern char** environ;

  // Guarantee that environ is not initially empty...
  ASSERT_EQ(0, setenv("test-variable", "a", 1));

  // Stash a copy.
  std::vector<char*> old_environ;
  for (size_t i = 0; environ[i] != NULL; ++i) {
    old_environ.push_back(strdup(environ[i]));
  }

  ASSERT_EQ(0, clearenv());

  EXPECT_TRUE(environ == NULL || environ[0] == NULL);
  EXPECT_EQ(NULL, getenv("test-variable"));
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

static void TestFsyncFunction(int (*fn)(int)) {
  int fd;

  // Can't sync an invalid fd.
  errno = 0;
  EXPECT_EQ(-1, fn(-1));
  EXPECT_EQ(EBADF, errno);

  // It doesn't matter whether you've opened a file for write or not.
  TemporaryFile tf;
  ASSERT_NE(-1, tf.fd);

  EXPECT_EQ(0, fn(tf.fd));

  ASSERT_NE(-1, fd = open(tf.filename, O_RDONLY));
  EXPECT_EQ(0, fn(fd));
  close(fd);

  ASSERT_NE(-1, fd = open(tf.filename, O_RDWR));
  EXPECT_EQ(0, fn(fd));
  close(fd);

  // The fd can even be a directory.
  ASSERT_NE(-1, fd = open("/", O_RDONLY));
  EXPECT_EQ(0, fn(fd));
  close(fd);

  // But some file systems may choose to be fussy...
  errno = 0;
  ASSERT_NE(-1, fd = open("/proc/version", O_RDONLY));
  EXPECT_EQ(-1, fn(fd));
  EXPECT_EQ(EINVAL, errno);
  close(fd);
}

TEST(unistd, fdatasync) {
  TestFsyncFunction(fdatasync);
}

TEST(unistd, fsync) {
  TestFsyncFunction(fsync);
}

static void AssertGetPidCorrect() {
  // The loop is just to make manual testing/debugging with strace easier.
  pid_t getpid_syscall_result = syscall(__NR_getpid);
  for (size_t i = 0; i < 128; ++i) {
    ASSERT_EQ(getpid_syscall_result, getpid());
  }
}

TEST(unistd, getpid_caching_and_fork) {
  pid_t parent_pid = getpid();
  ASSERT_EQ(syscall(__NR_getpid), parent_pid);

  pid_t fork_result = fork();
  ASSERT_NE(fork_result, -1);
  if (fork_result == 0) {
    // We're the child.
    AssertGetPidCorrect();
    ASSERT_EQ(parent_pid, getppid());
    _exit(123);
  } else {
    // We're the parent.
    ASSERT_EQ(parent_pid, getpid());

    int status;
    ASSERT_EQ(fork_result, waitpid(fork_result, &status, 0));
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_EQ(123, WEXITSTATUS(status));
  }
}

static int GetPidCachingCloneStartRoutine(void*) {
  AssertGetPidCorrect();
  return 123;
}

TEST(unistd, getpid_caching_and_clone) {
  pid_t parent_pid = getpid();
  ASSERT_EQ(syscall(__NR_getpid), parent_pid);

  void* child_stack[1024];
  int clone_result = clone(GetPidCachingCloneStartRoutine, &child_stack[1024], CLONE_NEWNS | SIGCHLD, NULL);
  if (clone_result == -1 && errno == EPERM && getuid() != 0) {
    GTEST_LOG_(INFO) << "This test only works if you have permission to CLONE_NEWNS; try running as root.\n";
    return;
  }
  ASSERT_NE(clone_result, -1);

  ASSERT_EQ(parent_pid, getpid());

  int status;
  ASSERT_EQ(clone_result, waitpid(clone_result, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(123, WEXITSTATUS(status));
}

static void* GetPidCachingPthreadStartRoutine(void*) {
  AssertGetPidCorrect();
  return NULL;
}

TEST(unistd, getpid_caching_and_pthread_create) {
  pid_t parent_pid = getpid();

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, GetPidCachingPthreadStartRoutine, NULL));

  ASSERT_EQ(parent_pid, getpid());

  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(NULL, result);
}
