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
#include "TemporaryFile.h"

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

TEST(stdlib, drand48) {
  srand48(0x01020304);
  EXPECT_DOUBLE_EQ(0.65619299195623526, drand48());
  EXPECT_DOUBLE_EQ(0.18522597229772941, drand48());
  EXPECT_DOUBLE_EQ(0.42015087072844537, drand48());
  EXPECT_DOUBLE_EQ(0.061637783047395089, drand48());
}

TEST(stdlib, lrand48_random_rand) {
  srand48(0x01020304);
  EXPECT_EQ(1409163720, lrand48());
  EXPECT_EQ(397769746, lrand48());
  EXPECT_EQ(902267124, lrand48());
  EXPECT_EQ(132366131, lrand48());

#if defined(__BIONIC__)
  // On bionic, random(3) is equivalent to lrand48...
  srandom(0x01020304);
  EXPECT_EQ(1409163720, random());
  EXPECT_EQ(397769746, random());
  EXPECT_EQ(902267124, random());
  EXPECT_EQ(132366131, random());

  // ...and rand(3) is the bottom 32 bits.
  srand(0x01020304);
  EXPECT_EQ(static_cast<int>(1409163720), rand());
  EXPECT_EQ(static_cast<int>(397769746), rand());
  EXPECT_EQ(static_cast<int>(902267124), rand());
  EXPECT_EQ(static_cast<int>(132366131), rand());
#endif
}

TEST(stdlib, mrand48) {
  srand48(0x01020304);
  EXPECT_EQ(-1476639856, mrand48());
  EXPECT_EQ(795539493, mrand48());
  EXPECT_EQ(1804534249, mrand48());
  EXPECT_EQ(264732262, mrand48());
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
TEST(stdlib_DeathTest, getenv_after_main_thread_exits) {
  // https://code.google.com/p/android/issues/detail?id=57421
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_EXIT(TestBug57421_main(), ::testing::ExitedWithCode(0), "");
}

TEST(stdlib, mkstemp) {
  TemporaryFile tf;
  struct stat sb;
  ASSERT_EQ(0, fstat(tf.fd, &sb));
}

TEST(stdlib, mkstemp64) {
  GenericTemporaryFile<mkstemp64> tf;
  struct stat64 sb;
  ASSERT_EQ(0, fstat64(tf.fd, &sb));
  ASSERT_EQ(O_LARGEFILE, fcntl(tf.fd, F_GETFL) & O_LARGEFILE);
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

TEST(stdlib, quick_exit) {
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    quick_exit(99);
  }

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(99, WEXITSTATUS(status));
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

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(99, WEXITSTATUS(status));
}

TEST(unistd, _Exit) {
  int pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    _Exit(99);
  }

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(99, WEXITSTATUS(status));
}
