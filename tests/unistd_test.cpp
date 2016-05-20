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

#include "BionicDeathTest.h"
#include "ScopedSignalHandler.h"
#include "TemporaryFile.h"
#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <sys/param.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/strings.h>

#include "private/get_cpu_count_from_string.h"

#if defined(NOFORTIFY)
#define UNISTD_TEST unistd_nofortify
#define UNISTD_DEATHTEST unistd_nofortify_DeathTest
#else
#define UNISTD_TEST unistd
#define UNISTD_DEATHTEST unistd_DeathTest
#endif

static void* get_brk() {
  return sbrk(0);
}

static void* page_align(uintptr_t addr) {
  uintptr_t mask = sysconf(_SC_PAGE_SIZE) - 1;
  return reinterpret_cast<void*>((addr + mask) & ~mask);
}

TEST(UNISTD_TEST, brk) {
  void* initial_break = get_brk();

  // The kernel aligns the break to a page.
  void* new_break = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(initial_break) + 1);
  ASSERT_EQ(0, brk(new_break));
  ASSERT_GE(get_brk(), new_break);

  new_break = page_align(reinterpret_cast<uintptr_t>(initial_break) + sysconf(_SC_PAGE_SIZE));
  ASSERT_EQ(0, brk(new_break));
  ASSERT_EQ(get_brk(), new_break);
}

TEST(UNISTD_TEST, brk_ENOMEM) {
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

TEST(UNISTD_TEST, truncate) {
  TemporaryFile tf;
  ASSERT_EQ(0, close(tf.fd));
  ASSERT_EQ(0, truncate(tf.filename, 123));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, truncate64) {
  TemporaryFile tf;
  ASSERT_EQ(0, close(tf.fd));
  ASSERT_EQ(0, truncate64(tf.filename, 123));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, ftruncate) {
  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate(tf.fd, 123));
  ASSERT_EQ(0, close(tf.fd));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, ftruncate64) {
  TemporaryFile tf;
  ASSERT_EQ(0, ftruncate64(tf.fd, 123));
  ASSERT_EQ(0, close(tf.fd));

  struct stat sb;
  ASSERT_EQ(0, stat(tf.filename, &sb));
  ASSERT_EQ(123, sb.st_size);
}

TEST(UNISTD_TEST, ftruncate_negative) {
  TemporaryFile tf;
  errno = 0;
  ASSERT_EQ(-1, ftruncate(tf.fd, -123));
  ASSERT_EQ(EINVAL, errno);
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
  ASSERT_EQ(EBADF, errno);
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
  ASSERT_TRUE(getenv("test-variable") == NULL);
}

TEST(UNISTD_TEST, unsetenv_EINVAL) {
  EXPECT_EQ(-1, unsetenv(""));
  EXPECT_EQ(EINVAL, errno);
  EXPECT_EQ(-1, unsetenv("a=b"));
  EXPECT_EQ(EINVAL, errno);
}

TEST(UNISTD_TEST, setenv_EINVAL) {
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
  ASSERT_NE(-1, fd = open("/data/local/tmp", O_RDONLY));
  EXPECT_EQ(0, fn(fd));
  close(fd);

  // But some file systems may choose to be fussy...
  errno = 0;
  ASSERT_NE(-1, fd = open("/proc/version", O_RDONLY));
  EXPECT_EQ(-1, fn(fd));
  EXPECT_EQ(EINVAL, errno);
  close(fd);
}

TEST(UNISTD_TEST, fdatasync) {
  TestFsyncFunction(fdatasync);
}

TEST(UNISTD_TEST, fsync) {
  TestFsyncFunction(fsync);
}

static void AssertGetPidCorrect() {
  // The loop is just to make manual testing/debugging with strace easier.
  pid_t getpid_syscall_result = syscall(__NR_getpid);
  for (size_t i = 0; i < 128; ++i) {
    ASSERT_EQ(getpid_syscall_result, getpid());
  }
}

static void TestGetPidCachingWithFork(int (*fork_fn)()) {
  pid_t parent_pid = getpid();
  ASSERT_EQ(syscall(__NR_getpid), parent_pid);

  pid_t fork_result = fork_fn();
  ASSERT_NE(fork_result, -1);
  if (fork_result == 0) {
    // We're the child.
    AssertGetPidCorrect();
    ASSERT_EQ(parent_pid, getppid());
    _exit(123);
  } else {
    // We're the parent.
    ASSERT_EQ(parent_pid, getpid());
    AssertChildExited(fork_result, 123);
  }
}

TEST(UNISTD_TEST, getpid_caching_and_fork) {
  TestGetPidCachingWithFork(fork);
}

TEST(UNISTD_TEST, getpid_caching_and_vfork) {
  TestGetPidCachingWithFork(vfork);
}

static int GetPidCachingCloneStartRoutine(void*) {
  AssertGetPidCorrect();
  return 123;
}

TEST(UNISTD_TEST, getpid_caching_and_clone) {
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

  AssertChildExited(clone_result, 123);
}

static void* GetPidCachingPthreadStartRoutine(void*) {
  AssertGetPidCorrect();
  return NULL;
}

TEST(UNISTD_TEST, getpid_caching_and_pthread_create) {
  pid_t parent_pid = getpid();

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, GetPidCachingPthreadStartRoutine, NULL));

  ASSERT_EQ(parent_pid, getpid());

  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(NULL, result);
}

class UNISTD_DEATHTEST : public BionicDeathTest {};

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
  errno = 0;
  ASSERT_EQ(0, gethostname(hostname, strlen(hostname) + 1));

  // Does uname(2) agree?
  utsname buf;
  ASSERT_EQ(0, uname(&buf));
  ASSERT_EQ(0, strncmp(hostname, buf.nodename, SYS_NMLN));
  ASSERT_GT(strlen(hostname), 0U);

  // Do we correctly detect truncation?
  errno = 0;
  ASSERT_EQ(-1, gethostname(hostname, strlen(hostname)));
  ASSERT_EQ(ENAMETOOLONG, errno);
}

TEST(UNISTD_TEST, pathconf_fpathconf) {
  TemporaryFile tf;
  long rc = 0L;
  // As a file system's block size is always power of 2, the configure values
  // for ALLOC and XFER should be power of 2 as well.
  rc = pathconf(tf.filename, _PC_ALLOC_SIZE_MIN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = pathconf(tf.filename, _PC_REC_MIN_XFER_SIZE);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = pathconf(tf.filename, _PC_REC_XFER_ALIGN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));

  rc = fpathconf(tf.fd, _PC_ALLOC_SIZE_MIN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = fpathconf(tf.fd, _PC_REC_MIN_XFER_SIZE);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
  rc = fpathconf(tf.fd, _PC_REC_XFER_ALIGN);
  ASSERT_TRUE(rc > 0 && powerof2(rc));
}


TEST(UNISTD_TEST, _POSIX_macros_smoke) {
  // Make a tight verification of _POSIX_* / _POSIX2_* / _XOPEN_* macros, to prevent change by mistake.
  // Verify according to POSIX.1-2008.
  EXPECT_EQ(200809L, _POSIX_VERSION);

  EXPECT_EQ(_POSIX_VERSION, _POSIX_ADVISORY_INFO);
  EXPECT_GT(_POSIX_AIO_LISTIO_MAX, 0);
  EXPECT_GT(_POSIX_AIO_MAX, 0);
  EXPECT_GT(_POSIX_ARG_MAX, 0);
  EXPECT_GT(_POSIX_CHILD_MAX, 0);
  EXPECT_NE(_POSIX_CHOWN_RESTRICTED, -1);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_CLOCK_SELECTION);
  EXPECT_EQ(0, _POSIX_CPUTIME);             // Use sysconf to detect support at runtime.
  EXPECT_GT(_POSIX_DELAYTIMER_MAX, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_FSYNC);
  EXPECT_GT(_POSIX_HOST_NAME_MAX, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_IPV6);
  EXPECT_GT(_POSIX_JOB_CONTROL, 0);
  EXPECT_GT(_POSIX_LINK_MAX, 0);
  EXPECT_GT(_POSIX_LOGIN_NAME_MAX, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MAPPED_FILES);
  EXPECT_GT(_POSIX_MAX_CANON, 0);
  EXPECT_GT(_POSIX_MAX_INPUT, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MEMLOCK);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MEMLOCK_RANGE);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_MEMORY_PROTECTION);
  EXPECT_EQ(0, _POSIX_MONOTONIC_CLOCK);
  EXPECT_GT(_POSIX_MQ_OPEN_MAX, 0);
  EXPECT_GT(_POSIX_MQ_PRIO_MAX, 0);
  EXPECT_GT(_POSIX_NAME_MAX, 0);
  EXPECT_GT(_POSIX_NGROUPS_MAX, 0);
  EXPECT_GT(_POSIX_NO_TRUNC, 0);
  EXPECT_GT(_POSIX_OPEN_MAX, 0);
  EXPECT_GT(_POSIX_PATH_MAX, 0);
  EXPECT_GT(_POSIX_PIPE_BUF, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_PRIORITY_SCHEDULING);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_RAW_SOCKETS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_READER_WRITER_LOCKS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_REALTIME_SIGNALS);
  EXPECT_GT(_POSIX_REGEXP, 0);
  EXPECT_GT(_POSIX_RE_DUP_MAX, 0);
  EXPECT_GT(_POSIX_SAVED_IDS, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_SEMAPHORES);
  EXPECT_GT(_POSIX_SEM_NSEMS_MAX, 0);
  EXPECT_GT(_POSIX_SEM_VALUE_MAX, 0);
  EXPECT_GT(_POSIX_SHELL, 0);
  EXPECT_GT(_POSIX_SIGQUEUE_MAX, 0);
  EXPECT_EQ(-1, _POSIX_SPORADIC_SERVER);
  EXPECT_GT(_POSIX_SSIZE_MAX, 0);
  EXPECT_GT(_POSIX_STREAM_MAX, 0);
  EXPECT_GT(_POSIX_SYMLINK_MAX, 0);
  EXPECT_GT(_POSIX_SYMLOOP_MAX, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_SYNCHRONIZED_IO);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREADS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_ATTR_STACKADDR);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_ATTR_STACKSIZE);
  EXPECT_EQ(0, _POSIX_THREAD_CPUTIME);       // Use sysconf to detect support at runtime.
  EXPECT_GT(_POSIX_THREAD_DESTRUCTOR_ITERATIONS, 0);
  EXPECT_EQ(_POSIX_THREAD_KEYS_MAX, 128);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_PRIORITY_SCHEDULING);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_PRIO_INHERIT);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_PRIO_PROTECT);
  EXPECT_EQ(-1, _POSIX_THREAD_ROBUST_PRIO_PROTECT);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_THREAD_SAFE_FUNCTIONS);
  EXPECT_EQ(-1, _POSIX_THREAD_SPORADIC_SERVER);
  EXPECT_GT(_POSIX_THREAD_THREADS_MAX, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_TIMEOUTS);
  EXPECT_EQ(_POSIX_VERSION, _POSIX_TIMERS);
  EXPECT_GT(_POSIX_TIMER_MAX, 0);
  EXPECT_EQ(-1, _POSIX_TRACE);
  EXPECT_EQ(-1, _POSIX_TRACE_EVENT_FILTER);
  EXPECT_EQ(-1, _POSIX_TRACE_INHERIT);
  EXPECT_EQ(-1, _POSIX_TRACE_LOG);
  EXPECT_GT(_POSIX_TTY_NAME_MAX, 0);
  EXPECT_EQ(-1, _POSIX_TYPED_MEMORY_OBJECTS);
  EXPECT_GT(_POSIX_TZNAME_MAX, 0);
  EXPECT_NE(-1, _POSIX_VDISABLE);

  EXPECT_GT(_POSIX2_BC_BASE_MAX, 0);
  EXPECT_GT(_POSIX2_BC_DIM_MAX, 0);
  EXPECT_GT(_POSIX2_BC_SCALE_MAX, 0);
  EXPECT_GT(_POSIX2_BC_STRING_MAX, 0);
  EXPECT_GT(_POSIX2_CHARCLASS_NAME_MAX, 0);
  EXPECT_GT(_POSIX2_COLL_WEIGHTS_MAX, 0);
  EXPECT_EQ(_POSIX_VERSION, _POSIX2_C_BIND);
  EXPECT_GT(_POSIX2_EXPR_NEST_MAX, 0);
  EXPECT_GT(_POSIX2_LINE_MAX, 0);
  EXPECT_GT(_POSIX2_RE_DUP_MAX, 0);

  EXPECT_EQ(700, _XOPEN_VERSION);
  EXPECT_GT(_XOPEN_IOV_MAX, 0);
  EXPECT_GT(_XOPEN_UNIX, 0);

#if defined(__BIONIC__)
  // These tests only pass on bionic, as bionic and glibc has different support on these macros.
  // Macros like _POSIX_ASYNCHRONOUS_IO are not supported on bionic yet.
  EXPECT_EQ(-1, _POSIX_ASYNCHRONOUS_IO);
  EXPECT_EQ(-1, _POSIX_BARRIERS);
  EXPECT_EQ(-1, _POSIX_MESSAGE_PASSING);
  EXPECT_EQ(-1, _POSIX_PRIORITIZED_IO);
  EXPECT_EQ(-1, _POSIX_SHARED_MEMORY_OBJECTS);
  EXPECT_EQ(-1, _POSIX_SPAWN);
  EXPECT_EQ(-1, _POSIX_SPIN_LOCKS);
  EXPECT_EQ(-1, _POSIX_THREAD_PROCESS_SHARED);
  EXPECT_EQ(-1, _POSIX_THREAD_ROBUST_PRIO_INHERIT);

  EXPECT_EQ(-1, _POSIX2_VERSION);
  EXPECT_EQ(-1, _POSIX2_CHAR_TERM);
  EXPECT_EQ(-1, _POSIX2_C_DEV);
  EXPECT_EQ(-1, _POSIX2_LOCALEDEF);
  EXPECT_EQ(-1, _POSIX2_SW_DEV);
  EXPECT_EQ(-1, _POSIX2_UPE);

  EXPECT_EQ(-1, _XOPEN_ENH_I18N);
  EXPECT_EQ(-1, _XOPEN_CRYPT);
  EXPECT_EQ(-1, _XOPEN_LEGACY);
  EXPECT_EQ(-1, _XOPEN_REALTIME);
  EXPECT_EQ(-1, _XOPEN_REALTIME_THREADS);
  EXPECT_EQ(-1, _XOPEN_SHM);

#endif // defined(__BIONIC__)
}

#define VERIFY_SYSCONF_NOT_SUPPORT(name) VerifySysconf(name, #name, [](long v){return v == -1;})

// sysconf() means unlimited when it returns -1 with errno unchanged.
#define VERIFY_SYSCONF_POSITIVE(name) \
  VerifySysconf(name, #name, [](long v){return (v > 0 || v == -1);})

#define VERIFY_SYSCONF_POSIX_VERSION(name) \
  VerifySysconf(name, #name, [](long v){return v == _POSIX_VERSION;})

static void VerifySysconf(int option, const char *option_name, bool (*verify)(long)) {
  errno = 0;
  long ret = sysconf(option);
  EXPECT_TRUE(0 == errno && verify(ret)) << "name = " << option_name << ", ret = "
      << ret <<", Error Message: " << strerror(errno);
}

TEST(UNISTD_TEST, sysconf) {
  VERIFY_SYSCONF_POSIX_VERSION(_SC_ADVISORY_INFO);
  VERIFY_SYSCONF_POSITIVE(_SC_ARG_MAX);
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
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_FORT_DEV);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_FORT_RUN);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_UPE);
  VERIFY_SYSCONF_POSITIVE(_SC_JOB_CONTROL);
  VERIFY_SYSCONF_POSITIVE(_SC_SAVED_IDS);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_VERSION);
  VERIFY_SYSCONF_POSITIVE(_SC_RE_DUP_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_STREAM_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_TZNAME_MAX);
  VerifySysconf(_SC_XOPEN_VERSION, "_SC_XOPEN_VERSION", [](long v){return v == _XOPEN_VERSION;});
  VERIFY_SYSCONF_POSITIVE(_SC_ATEXIT_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_IOV_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_PAGESIZE);
  VERIFY_SYSCONF_POSITIVE(_SC_PAGE_SIZE);
  VerifySysconf(_SC_PAGE_SIZE, "_SC_PAGE_SIZE",
                [](long v){return v == sysconf(_SC_PAGESIZE) && v == getpagesize();});
  VERIFY_SYSCONF_POSITIVE(_SC_XOPEN_UNIX);
  VERIFY_SYSCONF_POSITIVE(_SC_AIO_LISTIO_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_AIO_MAX);
  VerifySysconf(_SC_AIO_PRIO_DELTA_MAX, "_SC_AIO_PRIO_DELTA_MAX", [](long v){return v >= 0;});
  VERIFY_SYSCONF_POSITIVE(_SC_DELAYTIMER_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_MQ_OPEN_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_MQ_PRIO_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_RTSIG_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_SEM_NSEMS_MAX);
  VERIFY_SYSCONF_POSITIVE(_SC_SEM_VALUE_MAX);
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
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_PRIO_INHERIT);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_PRIO_PROTECT);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_SAFE_FUNCTIONS);
  VERIFY_SYSCONF_POSITIVE(_SC_NPROCESSORS_CONF);
  VERIFY_SYSCONF_POSITIVE(_SC_NPROCESSORS_ONLN);
  VERIFY_SYSCONF_POSITIVE(_SC_PHYS_PAGES);
  VERIFY_SYSCONF_POSITIVE(_SC_AVPHYS_PAGES);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_MONOTONIC_CLOCK);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_PBS);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_PBS_ACCOUNTING);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_PBS_CHECKPOINT);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_PBS_LOCATE);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_PBS_MESSAGE);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_PBS_TRACK);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_CLOCK_SELECTION);
  VERIFY_SYSCONF_POSITIVE(_SC_HOST_NAME_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_IPV6);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_RAW_SOCKETS);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_READER_WRITER_LOCKS);
  VERIFY_SYSCONF_POSITIVE(_SC_REGEXP);
  VERIFY_SYSCONF_POSITIVE(_SC_SHELL);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_SPORADIC_SERVER);
  VERIFY_SYSCONF_POSITIVE(_SC_SYMLOOP_MAX);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_THREAD_CPUTIME);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_THREAD_SPORADIC_SERVER);
  VERIFY_SYSCONF_POSIX_VERSION(_SC_TIMEOUTS);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE_EVENT_FILTER);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE_EVENT_NAME_MAX);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE_INHERIT);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE_LOG);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE_NAME_MAX);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE_SYS_MAX);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TRACE_USER_EVENT_MAX);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_TYPED_MEMORY_OBJECTS);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_STREAMS);

#if defined(__LP64__)
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_V7_ILP32_OFF32);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_V7_ILP32_OFFBIG);
  VERIFY_SYSCONF_POSITIVE(_SC_V7_LP64_OFF64);
  VERIFY_SYSCONF_POSITIVE(_SC_V7_LPBIG_OFFBIG);
#else
  VERIFY_SYSCONF_POSITIVE(_SC_V7_ILP32_OFF32);
#if defined(__BIONIC__)
  // bionic does not support 64 bits off_t type on 32bit machine.
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_V7_ILP32_OFFBIG);
#endif
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_V7_LP64_OFF64);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_V7_LPBIG_OFFBIG);
#endif

#if defined(__BIONIC__)
  // Tests can only run on bionic, as bionic and glibc have different support for these options.
  // Below options are not supported on bionic yet.
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_ASYNCHRONOUS_IO);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_BARRIERS);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_MESSAGE_PASSING);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_PRIORITIZED_IO);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_SHARED_MEMORY_OBJECTS);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_SPAWN);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_SPIN_LOCKS);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_THREAD_PROCESS_SHARED);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_THREAD_ROBUST_PRIO_INHERIT);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_THREAD_ROBUST_PRIO_PROTECT);

  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_C_DEV);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_CHAR_TERM);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_LOCALEDEF);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_SW_DEV);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_2_VERSION);

  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_CRYPT);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_ENH_I18N);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_LEGACY);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_REALTIME);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_REALTIME_THREADS);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_SHM);
  VERIFY_SYSCONF_NOT_SUPPORT(_SC_XOPEN_UUCP);
#endif // defined(__BIONIC__)
}

TEST(UNISTD_TEST, get_cpu_count_from_string) {
  ASSERT_EQ(0, GetCpuCountFromString(" "));
  ASSERT_EQ(1, GetCpuCountFromString("0"));
  ASSERT_EQ(40, GetCpuCountFromString("0-39"));
  ASSERT_EQ(4, GetCpuCountFromString("0, 1-2, 4\n"));
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
  ASSERT_EQ(EBADF, errno);
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
    ASSERT_EQ(EAGAIN, errno);
    // Check also that it reports itself as locked.
    ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
    ASSERT_EQ(-1, lockf64(tf.fd, F_TEST, file_size));
    ASSERT_EQ(EACCES, errno);
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
    ASSERT_EQ(EACCES, errno);
    // Check also that it reports itself as locked.
    ASSERT_EQ(0, lseek64(tf.fd, 0, SEEK_SET));
    ASSERT_EQ(-1, lockf64(tf.fd, F_TEST, file_size/2));
    ASSERT_EQ(EACCES, errno);
    _exit(0);
  }
  AssertChildExited(pid, 0);

  // The second half was locked by the child, but the lock disappeared
  // when the process exited, so check it can be locked now.
  ASSERT_EQ(file_size/2, lseek64(tf.fd, file_size/2, SEEK_SET));
  ASSERT_EQ(0, lockf64(tf.fd, F_TLOCK, file_size/2));
}
