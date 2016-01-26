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
#include "TemporaryFile.h"
#include "utils.h"

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// The random number generator tests all set the seed, get four values, reset the seed and check
// that they get the first two values repeated, and then reset the seed and check two more values
// to rule out the possibility that we're just going round a cycle of four values.
// TODO: factor this out.

TEST(stdlib, drand48) {
  srand48(0x01020304);
  EXPECT_DOUBLE_EQ(0.65619299195623526, drand48());
  EXPECT_DOUBLE_EQ(0.18522597229772941, drand48());
  EXPECT_DOUBLE_EQ(0.42015087072844537, drand48());
  EXPECT_DOUBLE_EQ(0.061637783047395089, drand48());
  srand48(0x01020304);
  EXPECT_DOUBLE_EQ(0.65619299195623526, drand48());
  EXPECT_DOUBLE_EQ(0.18522597229772941, drand48());
  srand48(0x01020304);
  EXPECT_DOUBLE_EQ(0.65619299195623526, drand48());
  EXPECT_DOUBLE_EQ(0.18522597229772941, drand48());
}

TEST(stdlib, erand48) {
  const unsigned short seed[3] = { 0x330e, 0xabcd, 0x1234 };
  unsigned short xsubi[3];
  memcpy(xsubi, seed, sizeof(seed));
  EXPECT_DOUBLE_EQ(0.39646477376027534, erand48(xsubi));
  EXPECT_DOUBLE_EQ(0.84048536941142515, erand48(xsubi));
  EXPECT_DOUBLE_EQ(0.35333609724524351, erand48(xsubi));
  EXPECT_DOUBLE_EQ(0.44658343479654405, erand48(xsubi));
  memcpy(xsubi, seed, sizeof(seed));
  EXPECT_DOUBLE_EQ(0.39646477376027534, erand48(xsubi));
  EXPECT_DOUBLE_EQ(0.84048536941142515, erand48(xsubi));
  memcpy(xsubi, seed, sizeof(seed));
  EXPECT_DOUBLE_EQ(0.39646477376027534, erand48(xsubi));
  EXPECT_DOUBLE_EQ(0.84048536941142515, erand48(xsubi));
}

TEST(stdlib, lcong48) {
  unsigned short p[7] = { 0x0102, 0x0304, 0x0506, 0x0708, 0x090a, 0x0b0c, 0x0d0e };
  lcong48(p);
  EXPECT_EQ(1531389981, lrand48());
  EXPECT_EQ(1598801533, lrand48());
  EXPECT_EQ(2080534853, lrand48());
  EXPECT_EQ(1102488897, lrand48());
  lcong48(p);
  EXPECT_EQ(1531389981, lrand48());
  EXPECT_EQ(1598801533, lrand48());
  lcong48(p);
  EXPECT_EQ(1531389981, lrand48());
  EXPECT_EQ(1598801533, lrand48());
}

TEST(stdlib, lrand48) {
  srand48(0x01020304);
  EXPECT_EQ(1409163720, lrand48());
  EXPECT_EQ(397769746, lrand48());
  EXPECT_EQ(902267124, lrand48());
  EXPECT_EQ(132366131, lrand48());
  srand48(0x01020304);
  EXPECT_EQ(1409163720, lrand48());
  EXPECT_EQ(397769746, lrand48());
  srand48(0x01020304);
  EXPECT_EQ(1409163720, lrand48());
  EXPECT_EQ(397769746, lrand48());
}

TEST(stdlib, random) {
  srandom(0x01020304);
  EXPECT_EQ(55436735, random());
  EXPECT_EQ(1399865117, random());
  EXPECT_EQ(2032643283, random());
  EXPECT_EQ(571329216, random());
  srandom(0x01020304);
  EXPECT_EQ(55436735, random());
  EXPECT_EQ(1399865117, random());
  srandom(0x01020304);
  EXPECT_EQ(55436735, random());
  EXPECT_EQ(1399865117, random());
}

TEST(stdlib, rand) {
  srand(0x01020304);
  EXPECT_EQ(55436735, rand());
  EXPECT_EQ(1399865117, rand());
  EXPECT_EQ(2032643283, rand());
  EXPECT_EQ(571329216, rand());
  srand(0x01020304);
  EXPECT_EQ(55436735, rand());
  EXPECT_EQ(1399865117, rand());
  srand(0x01020304);
  EXPECT_EQ(55436735, rand());
  EXPECT_EQ(1399865117, rand());
}

TEST(stdlib, mrand48) {
  srand48(0x01020304);
  EXPECT_EQ(-1476639856, mrand48());
  EXPECT_EQ(795539493, mrand48());
  EXPECT_EQ(1804534249, mrand48());
  EXPECT_EQ(264732262, mrand48());
  srand48(0x01020304);
  EXPECT_EQ(-1476639856, mrand48());
  EXPECT_EQ(795539493, mrand48());
  srand48(0x01020304);
  EXPECT_EQ(-1476639856, mrand48());
  EXPECT_EQ(795539493, mrand48());
}

TEST(stdlib, posix_memalign) {
  void* p;

  ASSERT_EQ(0, posix_memalign(&p, 512, 128));
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(p) % 512);
  free(p);

  // Can't align to a non-power of 2.
  ASSERT_EQ(EINVAL, posix_memalign(&p, 81, 128));
}

TEST(stdlib, realpath__NULL_filename) {
  errno = 0;
  char* p = realpath(NULL, NULL);
  ASSERT_TRUE(p == NULL);
  ASSERT_EQ(EINVAL, errno);
}

TEST(stdlib, realpath__empty_filename) {
  errno = 0;
  char* p = realpath("", NULL);
  ASSERT_TRUE(p == NULL);
  ASSERT_EQ(ENOENT, errno);
}

TEST(stdlib, realpath__ENOENT) {
  errno = 0;
  char* p = realpath("/this/directory/path/almost/certainly/does/not/exist", NULL);
  ASSERT_TRUE(p == NULL);
  ASSERT_EQ(ENOENT, errno);
}

TEST(stdlib, realpath__component_after_non_directory) {
  errno = 0;
  char* p = realpath("/dev/null/.", NULL);
  ASSERT_TRUE(p == NULL);
  ASSERT_EQ(ENOTDIR, errno);

  errno = 0;
  p = realpath("/dev/null/..", NULL);
  ASSERT_TRUE(p == NULL);
  ASSERT_EQ(ENOTDIR, errno);
}

TEST(stdlib, realpath) {
  // Get the name of this executable.
  char executable_path[PATH_MAX];
  int rc = readlink("/proc/self/exe", executable_path, sizeof(executable_path));
  ASSERT_NE(rc, -1);
  executable_path[rc] = '\0';

  char buf[PATH_MAX + 1];
  char* p = realpath("/proc/self/exe", buf);
  ASSERT_STREQ(executable_path, p);

  p = realpath("/proc/self/exe", NULL);
  ASSERT_STREQ(executable_path, p);
  free(p);
}

TEST(stdlib, qsort) {
  struct s {
    char name[16];
    static int comparator(const void* lhs, const void* rhs) {
      return strcmp(reinterpret_cast<const s*>(lhs)->name, reinterpret_cast<const s*>(rhs)->name);
    }
  };
  s entries[3];
  strcpy(entries[0].name, "charlie");
  strcpy(entries[1].name, "bravo");
  strcpy(entries[2].name, "alpha");

  qsort(entries, 3, sizeof(s), s::comparator);
  ASSERT_STREQ("alpha", entries[0].name);
  ASSERT_STREQ("bravo", entries[1].name);
  ASSERT_STREQ("charlie", entries[2].name);

  qsort(entries, 3, sizeof(s), s::comparator);
  ASSERT_STREQ("alpha", entries[0].name);
  ASSERT_STREQ("bravo", entries[1].name);
  ASSERT_STREQ("charlie", entries[2].name);
}

static void* TestBug57421_child(void* arg) {
  pthread_t main_thread = reinterpret_cast<pthread_t>(arg);
  pthread_join(main_thread, NULL);
  char* value = getenv("ENVIRONMENT_VARIABLE");
  if (value == NULL) {
    setenv("ENVIRONMENT_VARIABLE", "value", 1);
  }
  return NULL;
}

static void TestBug57421_main() {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, TestBug57421_child, reinterpret_cast<void*>(pthread_self())));
  pthread_exit(NULL);
}

// Even though this isn't really a death test, we have to say "DeathTest" here so gtest knows to
// run this test (which exits normally) in its own process.

class stdlib_DeathTest : public BionicDeathTest {};

TEST_F(stdlib_DeathTest, getenv_after_main_thread_exits) {
  // https://code.google.com/p/android/issues/detail?id=57421
  ASSERT_EXIT(TestBug57421_main(), ::testing::ExitedWithCode(0), "");
}

TEST(stdlib, mkostemp64) {
  TemporaryFile tf([](char* path) { return mkostemp64(path, O_CLOEXEC); });
  int flags = fcntl(tf.fd, F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(FD_CLOEXEC, flags & FD_CLOEXEC);
}

TEST(stdlib, mkostemp) {
  TemporaryFile tf([](char* path) { return mkostemp(path, O_CLOEXEC); });
  int flags = fcntl(tf.fd, F_GETFD);
  ASSERT_TRUE(flags != -1);
  ASSERT_EQ(FD_CLOEXEC, flags & FD_CLOEXEC);
}

TEST(stdlib, mkstemp64) {
  TemporaryFile tf(mkstemp64);
  struct stat64 sb;
  ASSERT_EQ(0, fstat64(tf.fd, &sb));
  ASSERT_EQ(O_LARGEFILE, fcntl(tf.fd, F_GETFL) & O_LARGEFILE);
}

TEST(stdlib, mkstemp) {
  TemporaryFile tf;
  struct stat sb;
  ASSERT_EQ(0, fstat(tf.fd, &sb));
}

TEST(stdlib, system) {
  int status;

  status = system("exit 0");
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(0, WEXITSTATUS(status));

  status = system("exit 1");
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(1, WEXITSTATUS(status));
}

TEST(stdlib, atof) {
  ASSERT_DOUBLE_EQ(1.23, atof("1.23"));
}

TEST(stdlib, strtod) {
  ASSERT_DOUBLE_EQ(1.23, strtod("1.23", NULL));
}

TEST(stdlib, strtof) {
  ASSERT_FLOAT_EQ(1.23, strtof("1.23", NULL));
}

TEST(stdlib, strtold) {
  ASSERT_DOUBLE_EQ(1.23, strtold("1.23", NULL));
}

TEST(stdlib, strtof_2206701) {
  ASSERT_EQ(0.0f, strtof("7.0064923216240853546186479164495e-46", NULL));
  ASSERT_EQ(1.4e-45f, strtof("7.0064923216240853546186479164496e-46", NULL));
}

TEST(stdlib, strtod_largest_subnormal) {
  // This value has been known to cause javac and java to infinite loop.
  // http://www.exploringbinary.com/java-hangs-when-converting-2-2250738585072012e-308/
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.2250738585072012e-308", NULL));
  ASSERT_EQ(2.2250738585072014e-308, strtod("0.00022250738585072012e-304", NULL));
  ASSERT_EQ(2.2250738585072014e-308, strtod("00000002.2250738585072012e-308", NULL));
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.225073858507201200000e-308", NULL));
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.2250738585072012e-00308", NULL));
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.22507385850720129978001e-308", NULL));
  ASSERT_EQ(-2.2250738585072014e-308, strtod("-2.2250738585072012e-308", NULL));
}

TEST(stdlib, quick_exit) {
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    quick_exit(99);
  }

  AssertChildExited(pid, 99);
}

static int quick_exit_status = 0;

static void quick_exit_1(void) {
  ASSERT_EQ(quick_exit_status, 0);
  quick_exit_status = 1;
}

static void quick_exit_2(void) {
  ASSERT_EQ(quick_exit_status, 1);
}

static void not_run(void) {
  FAIL();
}

TEST(stdlib, at_quick_exit) {
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    ASSERT_EQ(at_quick_exit(quick_exit_2), 0);
    ASSERT_EQ(at_quick_exit(quick_exit_1), 0);
    atexit(not_run);
    quick_exit(99);
  }

  AssertChildExited(pid, 99);
}

TEST(unistd, _Exit) {
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    _Exit(99);
  }

  AssertChildExited(pid, 99);
}

TEST(stdlib, pty_smoke) {
  // getpt returns a pty with O_RDWR|O_NOCTTY.
  int fd = getpt();
  ASSERT_NE(-1, fd);

  // grantpt is a no-op.
  ASSERT_EQ(0, grantpt(fd));

  // ptsname_r should start "/dev/pts/".
  char name_r[128];
  ASSERT_EQ(0, ptsname_r(fd, name_r, sizeof(name_r)));
  name_r[9] = 0;
  ASSERT_STREQ("/dev/pts/", name_r);

  close(fd);
}

TEST(stdlib, posix_openpt) {
  int fd = posix_openpt(O_RDWR|O_NOCTTY|O_CLOEXEC);
  ASSERT_NE(-1, fd);
  close(fd);
}

TEST(stdlib, ptsname_r_ENOTTY) {
  errno = 0;
  char buf[128];
  ASSERT_EQ(ENOTTY, ptsname_r(STDOUT_FILENO, buf, sizeof(buf)));
  ASSERT_EQ(ENOTTY, errno);
}

TEST(stdlib, ptsname_r_EINVAL) {
  int fd = getpt();
  ASSERT_NE(-1, fd);
  errno = 0;
  char* buf = NULL;
  ASSERT_EQ(EINVAL, ptsname_r(fd, buf, 128));
  ASSERT_EQ(EINVAL, errno);
  close(fd);
}

TEST(stdlib, ptsname_r_ERANGE) {
  int fd = getpt();
  ASSERT_NE(-1, fd);
  errno = 0;
  char buf[1];
  ASSERT_EQ(ERANGE, ptsname_r(fd, buf, sizeof(buf)));
  ASSERT_EQ(ERANGE, errno);
  close(fd);
}

TEST(stdlib, ttyname_r) {
  int fd = getpt();
  ASSERT_NE(-1, fd);

  // ttyname_r returns "/dev/ptmx" for a pty.
  char name_r[128];
  ASSERT_EQ(0, ttyname_r(fd, name_r, sizeof(name_r)));
  ASSERT_STREQ("/dev/ptmx", name_r);

  close(fd);
}

TEST(stdlib, ttyname_r_ENOTTY) {
  int fd = open("/dev/null", O_WRONLY);
  errno = 0;
  char buf[128];
  ASSERT_EQ(ENOTTY, ttyname_r(fd, buf, sizeof(buf)));
  ASSERT_EQ(ENOTTY, errno);
  close(fd);
}

TEST(stdlib, ttyname_r_EINVAL) {
  int fd = getpt();
  ASSERT_NE(-1, fd);
  errno = 0;
  char* buf = NULL;
  ASSERT_EQ(EINVAL, ttyname_r(fd, buf, 128));
  ASSERT_EQ(EINVAL, errno);
  close(fd);
}

TEST(stdlib, ttyname_r_ERANGE) {
  int fd = getpt();
  ASSERT_NE(-1, fd);
  errno = 0;
  char buf[1];
  ASSERT_EQ(ERANGE, ttyname_r(fd, buf, sizeof(buf)));
  ASSERT_EQ(ERANGE, errno);
  close(fd);
}

TEST(stdlib, unlockpt_ENOTTY) {
  int fd = open("/dev/null", O_WRONLY);
  errno = 0;
  ASSERT_EQ(-1, unlockpt(fd));
  ASSERT_EQ(ENOTTY, errno);
  close(fd);
}

TEST(stdlib, strtol_EINVAL) {
  errno = 0;
  strtol("123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtol("123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtol("123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}

TEST(stdlib, strtoll_EINVAL) {
  errno = 0;
  strtoll("123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoll("123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoll("123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}

TEST(stdlib, strtoul_EINVAL) {
  errno = 0;
  strtoul("123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoul("123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoul("123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}

TEST(stdlib, strtoull_EINVAL) {
  errno = 0;
  strtoull("123", NULL, -1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoull("123", NULL, 1);
  ASSERT_EQ(EINVAL, errno);
  errno = 0;
  strtoull("123", NULL, 37);
  ASSERT_EQ(EINVAL, errno);
}
