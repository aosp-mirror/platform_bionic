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

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <limits>
#include <string>

#include <android-base/file.h>
#include <android-base/macros.h>
#include <android-base/silent_death_test.h>
#include <android-base/test_utils.h>
#include <gtest/gtest.h>

#include "math_data_test.h"
#include "utils.h"

using namespace std::string_literals;

template <typename T = int (*)(char*)>
class GenericTemporaryFile {
 public:
  explicit GenericTemporaryFile(T mk_fn = mkstemp) : mk_fn_(mk_fn) {
    // Since we might be running on the host or the target, and if we're
    // running on the host we might be running under bionic or glibc,
    // let's just try both possible temporary directories and take the
    // first one that works.
    init("/data/local/tmp");
    if (fd == -1) {
      init("/tmp");
    }
  }

  ~GenericTemporaryFile() {
    close(fd);
    unlink(path);
  }

  int fd;
  char path[1024];

 private:
  T mk_fn_;

  void init(const char* tmp_dir) {
    snprintf(path, sizeof(path), "%s/TemporaryFile-XXXXXX", tmp_dir);
    fd = mk_fn_(path);
  }

  DISALLOW_COPY_AND_ASSIGN(GenericTemporaryFile);
};

typedef GenericTemporaryFile<> MyTemporaryFile;

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

TEST(stdlib, jrand48_distribution) {
  const int iterations = 4096;
  const int pivot_low  = 1536;
  const int pivot_high = 2560;

  unsigned short xsubi[3];
  int bits[32] = {};

  for (int iter = 0; iter < iterations; ++iter) {
    long rand_val = jrand48(xsubi);
    for (int bit = 0; bit < 32; ++bit) {
      bits[bit] += (static_cast<unsigned long>(rand_val) >> bit) & 0x01;
    }
  }

  // Check that bit probability is uniform
  for (int bit = 0; bit < 32; ++bit) {
    EXPECT_TRUE((pivot_low <= bits[bit]) && (bits[bit] <= pivot_high));
  }
}

TEST(stdlib, mrand48_distribution) {
  const int iterations = 4096;
  const int pivot_low  = 1536;
  const int pivot_high = 2560;

  int bits[32] = {};

  for (int iter = 0; iter < iterations; ++iter) {
    long rand_val = mrand48();
    for (int bit = 0; bit < 32; ++bit) {
      bits[bit] += (static_cast<unsigned long>(rand_val) >> bit) & 0x01;
    }
  }

  // Check that bit probability is uniform
  for (int bit = 0; bit < 32; ++bit) {
    EXPECT_TRUE((pivot_low <= bits[bit]) && (bits[bit] <= pivot_high));
  }
}

TEST(stdlib, posix_memalign_sweep) {
  SKIP_WITH_HWASAN;
  void* ptr;

  // These should all fail.
  for (size_t align = 0; align < sizeof(long); align++) {
    ASSERT_EQ(EINVAL, posix_memalign(&ptr, align, 256))
        << "Unexpected value at align " << align;
  }

  // Verify powers of 2 up to 2048 allocate, and verify that all other
  // alignment values between the powers of 2 fail.
  size_t last_align = sizeof(long);
  for (size_t align = sizeof(long); align <= 2048; align <<= 1) {
    // Try all of the non power of 2 values from the last until this value.
    for (size_t fail_align = last_align + 1; fail_align < align; fail_align++) {
      ASSERT_EQ(EINVAL, posix_memalign(&ptr, fail_align, 256))
          << "Unexpected success at align " << fail_align;
    }
    ASSERT_EQ(0, posix_memalign(&ptr, align, 256))
        << "Unexpected failure at align " << align;
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) & (align - 1))
        << "Did not return a valid aligned ptr " << ptr << " expected alignment " << align;
    free(ptr);
    last_align = align;
  }
}

TEST(stdlib, posix_memalign_various_sizes) {
  std::vector<size_t> sizes{1, 4, 8, 256, 1024, 65000, 128000, 256000, 1000000};
  for (auto size : sizes) {
    void* ptr;
    ASSERT_EQ(0, posix_memalign(&ptr, 16, 1))
        << "posix_memalign failed at size " << size;
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) & 0xf)
        << "Pointer not aligned at size " << size << " ptr " << ptr;
    free(ptr);
  }
}

TEST(stdlib, posix_memalign_overflow) {
  SKIP_WITH_HWASAN;
  void* ptr;
  ASSERT_NE(0, posix_memalign(&ptr, 16, SIZE_MAX));
}

TEST(stdlib, aligned_alloc_sweep) {
  SKIP_WITH_HWASAN;
  // Verify powers of 2 up to 2048 allocate, and verify that all other
  // alignment values between the powers of 2 fail.
  size_t last_align = 1;
  for (size_t align = 1; align <= 2048; align <<= 1) {
    // Try all of the non power of 2 values from the last until this value.
    for (size_t fail_align = last_align + 1; fail_align < align; fail_align++) {
      ASSERT_TRUE(aligned_alloc(fail_align, fail_align) == nullptr)
          << "Unexpected success at align " << fail_align;
      ASSERT_EQ(EINVAL, errno) << "Unexpected errno at align " << fail_align;
    }
    void* ptr = aligned_alloc(align, 2 * align);
    ASSERT_TRUE(ptr != nullptr) << "Unexpected failure at align " << align;
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) & (align - 1))
        << "Did not return a valid aligned ptr " << ptr << " expected alignment " << align;
    free(ptr);
    last_align = align;
  }
}

TEST(stdlib, aligned_alloc_overflow) {
  SKIP_WITH_HWASAN;
  ASSERT_TRUE(aligned_alloc(16, SIZE_MAX) == nullptr);
}

TEST(stdlib, aligned_alloc_size_not_multiple_of_alignment) {
  SKIP_WITH_HWASAN;

  ASSERT_TRUE(aligned_alloc(2048, 1) == nullptr);
  ASSERT_TRUE(aligned_alloc(4, 3) == nullptr);
  ASSERT_TRUE(aligned_alloc(4, 7) == nullptr);
  ASSERT_TRUE(aligned_alloc(16, 8) == nullptr);
}

TEST(stdlib, realpath__NULL_filename) {
  errno = 0;
  // Work around the compile-time error generated by FORTIFY here.
  const char* path = nullptr;
  char* p = realpath(path, nullptr);
  ASSERT_TRUE(p == nullptr);
  ASSERT_EQ(EINVAL, errno);
}

TEST(stdlib, realpath__empty_filename) {
  errno = 0;
  char* p = realpath("", nullptr);
  ASSERT_TRUE(p == nullptr);
  ASSERT_EQ(ENOENT, errno);
}

TEST(stdlib, realpath__ENOENT) {
  errno = 0;
  char* p = realpath("/this/directory/path/almost/certainly/does/not/exist", nullptr);
  ASSERT_TRUE(p == nullptr);
  ASSERT_EQ(ENOENT, errno);
}

TEST(stdlib, realpath__ELOOP) {
  TemporaryDir td;
  std::string link = std::string(td.path) + "/loop";
  ASSERT_EQ(0, symlink(link.c_str(), link.c_str()));

  errno = 0;
  char* p = realpath(link.c_str(), nullptr);
  ASSERT_TRUE(p == nullptr);
  ASSERT_EQ(ELOOP, errno);
}

TEST(stdlib, realpath__component_after_non_directory) {
  errno = 0;
  char* p = realpath("/dev/null/.", nullptr);
  ASSERT_TRUE(p == nullptr);
  ASSERT_EQ(ENOTDIR, errno);

  errno = 0;
  p = realpath("/dev/null/..", nullptr);
  ASSERT_TRUE(p == nullptr);
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

  p = realpath("/proc/self/exe", nullptr);
  ASSERT_STREQ(executable_path, p);
  free(p);
}

TEST(stdlib, realpath__dot) {
  char* p = realpath("/proc/./version", nullptr);
  ASSERT_STREQ("/proc/version", p);
  free(p);
}

TEST(stdlib, realpath__dot_dot) {
  char* p = realpath("/dev/../proc/version", nullptr);
  ASSERT_STREQ("/proc/version", p);
  free(p);
}

TEST(stdlib, realpath__deleted) {
  TemporaryDir td;

  // Create a file "A".
  std::string A_path = td.path + "/A"s;
  ASSERT_TRUE(android::base::WriteStringToFile("test\n", A_path));

  // Get an O_PATH fd for it.
  android::base::unique_fd fd(open(A_path.c_str(), O_PATH));
  ASSERT_NE(fd, -1);

  // Create a file "A (deleted)".
  android::base::unique_fd fd2(open((td.path + "/A (deleted)"s).c_str(),
                                    O_CREAT | O_TRUNC | O_WRONLY, 0644));
  ASSERT_NE(fd2, -1);

  // Delete "A".
  ASSERT_EQ(0, unlink(A_path.c_str()));

  // Now realpath() on the O_PATH fd, and check we *don't* get "A (deleted)".
  std::string path = android::base::StringPrintf("/proc/%d/fd/%d", static_cast<int>(getpid()),
                                                 fd.get());
  errno = 0;
  char* result = realpath(path.c_str(), nullptr);
  ASSERT_EQ(nullptr, result) << result;
  ASSERT_EQ(ENOENT, errno);
  free(result);
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
  pthread_join(main_thread, nullptr);
  char* value = getenv("ENVIRONMENT_VARIABLE");
  if (value == nullptr) {
    setenv("ENVIRONMENT_VARIABLE", "value", 1);
  }
  return nullptr;
}

static void TestBug57421_main() {
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, TestBug57421_child, reinterpret_cast<void*>(pthread_self())));
  pthread_exit(nullptr);
}

// Even though this isn't really a death test, we have to say "DeathTest" here so gtest knows to
// run this test (which exits normally) in its own process.

using stdlib_DeathTest = SilentDeathTest;

TEST_F(stdlib_DeathTest, getenv_after_main_thread_exits) {
  // https://code.google.com/p/android/issues/detail?id=57421
  ASSERT_EXIT(TestBug57421_main(), ::testing::ExitedWithCode(0), "");
}

TEST(stdlib, mkostemp64_smoke) {
  MyTemporaryFile tf([](char* path) { return mkostemp64(path, O_CLOEXEC); });
  ASSERT_TRUE(CloseOnExec(tf.fd));
}

TEST(stdlib, mkostemp) {
  MyTemporaryFile tf([](char* path) { return mkostemp(path, O_CLOEXEC); });
  ASSERT_TRUE(CloseOnExec(tf.fd));
}

TEST(stdlib, mkstemp64_smoke) {
  MyTemporaryFile tf(mkstemp64);
  struct stat64 sb;
  ASSERT_EQ(0, fstat64(tf.fd, &sb));
  ASSERT_EQ(O_LARGEFILE, fcntl(tf.fd, F_GETFL) & O_LARGEFILE);
}

TEST(stdlib, mkstemp) {
  MyTemporaryFile tf(mkstemp);
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

TEST(stdlib, system_NULL) {
  // "The system() function shall always return non-zero when command is NULL."
  // http://pubs.opengroup.org/onlinepubs/9699919799/functions/system.html
  ASSERT_NE(0, system(nullptr));
}

// https://austingroupbugs.net/view.php?id=1440
TEST(stdlib, system_minus) {
  // Create a script with a name that starts with a '-'.
  TemporaryDir td;
  std::string script = std::string(td.path) + "/-minus";
  ASSERT_TRUE(android::base::WriteStringToFile("#!" BIN_DIR "sh\nexit 66\n", script));

  // Set $PATH so we can find it.
  setenv("PATH", td.path, 1);
  // Make it executable so we can run it.
  ASSERT_EQ(0, chmod(script.c_str(), 0555));

  int status = system("-minus");
  EXPECT_TRUE(WIFEXITED(status));
  EXPECT_EQ(66, WEXITSTATUS(status));

  // While we're here and have all the setup, let's test popen(3) too...
  FILE* fp = popen("-minus", "r");
  ASSERT_TRUE(fp != nullptr);
  status = pclose(fp);
  EXPECT_TRUE(WIFEXITED(status));
  EXPECT_EQ(66, WEXITSTATUS(status));
}

TEST(stdlib, atof) {
  ASSERT_DOUBLE_EQ(1.23, atof("1.23"));
}

template <typename T>
static void CheckStrToFloat(T fn(const char* s, char** end)) {
  FpUlpEq<0, T> pred;

  EXPECT_PRED_FORMAT2(pred, 9.0, fn("9.0", nullptr));
  EXPECT_PRED_FORMAT2(pred, 9.0, fn("0.9e1", nullptr));
  EXPECT_PRED_FORMAT2(pred, 9.0, fn("0x1.2p3", nullptr));

  const char* s = " \t\v\f\r\n9.0";
  char* p;
  EXPECT_PRED_FORMAT2(pred, 9.0, fn(s, &p));
  EXPECT_EQ(s + strlen(s), p);

  EXPECT_TRUE(isnan(fn("+nan", nullptr)));
  EXPECT_TRUE(isnan(fn("nan", nullptr)));
  EXPECT_TRUE(isnan(fn("-nan", nullptr)));

  EXPECT_TRUE(isnan(fn("+nan(0xff)", nullptr)));
  EXPECT_TRUE(isnan(fn("nan(0xff)", nullptr)));
  EXPECT_TRUE(isnan(fn("-nan(0xff)", nullptr)));

  EXPECT_TRUE(isnan(fn("+nanny", &p)));
  EXPECT_STREQ("ny", p);
  EXPECT_TRUE(isnan(fn("nanny", &p)));
  EXPECT_STREQ("ny", p);
  EXPECT_TRUE(isnan(fn("-nanny", &p)));
  EXPECT_STREQ("ny", p);

  EXPECT_EQ(0, fn("muppet", &p));
  EXPECT_STREQ("muppet", p);
  EXPECT_EQ(0, fn("  muppet", &p));
  EXPECT_STREQ("  muppet", p);

  EXPECT_EQ(std::numeric_limits<T>::infinity(), fn("+inf", nullptr));
  EXPECT_EQ(std::numeric_limits<T>::infinity(), fn("inf", nullptr));
  EXPECT_EQ(-std::numeric_limits<T>::infinity(), fn("-inf", nullptr));

  EXPECT_EQ(std::numeric_limits<T>::infinity(), fn("+infinity", nullptr));
  EXPECT_EQ(std::numeric_limits<T>::infinity(), fn("infinity", nullptr));
  EXPECT_EQ(-std::numeric_limits<T>::infinity(), fn("-infinity", nullptr));

  EXPECT_EQ(std::numeric_limits<T>::infinity(), fn("+infinitude", &p));
  EXPECT_STREQ("initude", p);
  EXPECT_EQ(std::numeric_limits<T>::infinity(), fn("infinitude", &p));
  EXPECT_STREQ("initude", p);
  EXPECT_EQ(-std::numeric_limits<T>::infinity(), fn("-infinitude", &p));
  EXPECT_STREQ("initude", p);

  // Check case-insensitivity.
  EXPECT_EQ(std::numeric_limits<T>::infinity(), fn("InFiNiTy", nullptr));
  EXPECT_TRUE(isnan(fn("NaN", nullptr)));
}

TEST(stdlib, strtod) {
  CheckStrToFloat(strtod);
}

TEST(stdlib, strtof) {
  CheckStrToFloat(strtof);
}

TEST(stdlib, strtold) {
  CheckStrToFloat(strtold);
}

TEST(stdlib, strtof_2206701) {
  ASSERT_EQ(0.0f, strtof("7.0064923216240853546186479164495e-46", nullptr));
  ASSERT_EQ(1.4e-45f, strtof("7.0064923216240853546186479164496e-46", nullptr));
}

TEST(stdlib, strtod_largest_subnormal) {
  // This value has been known to cause javac and java to infinite loop.
  // http://www.exploringbinary.com/java-hangs-when-converting-2-2250738585072012e-308/
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.2250738585072012e-308", nullptr));
  ASSERT_EQ(2.2250738585072014e-308, strtod("0.00022250738585072012e-304", nullptr));
  ASSERT_EQ(2.2250738585072014e-308, strtod("00000002.2250738585072012e-308", nullptr));
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.225073858507201200000e-308", nullptr));
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.2250738585072012e-00308", nullptr));
  ASSERT_EQ(2.2250738585072014e-308, strtod("2.22507385850720129978001e-308", nullptr));
  ASSERT_EQ(-2.2250738585072014e-308, strtod("-2.2250738585072012e-308", nullptr));
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

#if defined(ANDROID_HOST_MUSL)
// musl doesn't have getpt
int getpt() {
  return posix_openpt(O_RDWR|O_NOCTTY);
}
#endif

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
  char* buf = nullptr;
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

TEST(stdlib, ttyname) {
  int fd = getpt();
  ASSERT_NE(-1, fd);

  // ttyname returns "/dev/ptmx" for a pty.
  ASSERT_STREQ("/dev/ptmx", ttyname(fd));

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
  char* buf = nullptr;
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

TEST(stdlib, getsubopt) {
  char* const tokens[] = {
    const_cast<char*>("a"),
    const_cast<char*>("b"),
    const_cast<char*>("foo"),
    nullptr
  };
  std::string input = "a,b,foo=bar,a,unknown";
  char* subopts = &input[0];
  char* value = nullptr;

  ASSERT_EQ(0, getsubopt(&subopts, tokens, &value));
  ASSERT_EQ(nullptr, value);
  ASSERT_EQ(1, getsubopt(&subopts, tokens, &value));
  ASSERT_EQ(nullptr, value);
  ASSERT_EQ(2, getsubopt(&subopts, tokens, &value));
  ASSERT_STREQ("bar", value);
  ASSERT_EQ(0, getsubopt(&subopts, tokens, &value));
  ASSERT_EQ(nullptr, value);

  ASSERT_EQ(-1, getsubopt(&subopts, tokens, &value));
}

TEST(stdlib, mblen) {
  // "If s is a null pointer, mblen() shall return a non-zero or 0 value, if character encodings,
  // respectively, do or do not have state-dependent encodings." We're always UTF-8.
  EXPECT_EQ(0, mblen(nullptr, 1));

  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  // 1-byte UTF-8.
  EXPECT_EQ(1, mblen("abcdef", 6));
  // 2-byte UTF-8.
  EXPECT_EQ(2, mblen("\xc2\xa2" "cdef", 6));
  // 3-byte UTF-8.
  EXPECT_EQ(3, mblen("\xe2\x82\xac" "def", 6));
  // 4-byte UTF-8.
  EXPECT_EQ(4, mblen("\xf0\xa4\xad\xa2" "ef", 6));

  // Illegal over-long sequence.
  ASSERT_EQ(-1, mblen("\xf0\x82\x82\xac" "ef", 6));

  // "mblen() shall ... return 0 (if s points to the null byte)".
  EXPECT_EQ(0, mblen("", 1));
}

template <typename T>
static void CheckStrToInt(T fn(const char* s, char** end, int base)) {
  char* end_p;

  // Negative base => invalid.
  errno = 0;
  ASSERT_EQ(T(0), fn("123", &end_p, -1));
  ASSERT_EQ(EINVAL, errno);

  // Base 1 => invalid (base 0 means "please guess").
  errno = 0;
  ASSERT_EQ(T(0), fn("123", &end_p, 1));
  ASSERT_EQ(EINVAL, errno);

  // Base > 36 => invalid.
  errno = 0;
  ASSERT_EQ(T(0), fn("123", &end_p, 37));
  ASSERT_EQ(EINVAL, errno);

  // Both leading + or - are always allowed (even for the strtou* family).
  ASSERT_EQ(T(-123), fn("-123", &end_p, 10));
  ASSERT_EQ(T(123), fn("+123", &end_p, 10));

  // If we see "0b" *not* followed by a binary digit, we shouldn't swallow the 'b'.
  ASSERT_EQ(T(0), fn("0b", &end_p, 2));
  ASSERT_EQ('b', *end_p);

  // Binary (the "0b" prefix) is case-insensitive.
  ASSERT_EQ(T(0b101), fn("0b101", &end_p, 0));
  ASSERT_EQ(T(0b101), fn("0B101", &end_p, 0));

  // If we see "0x" *not* followed by a hex digit, we shouldn't swallow the 'x'.
  ASSERT_EQ(T(0), fn("0xy", &end_p, 16));
  ASSERT_EQ('x', *end_p);

  // Hexadecimal (both the "0x" prefix and the digits) is case-insensitive.
  ASSERT_EQ(T(0xab), fn("0xab", &end_p, 0));
  ASSERT_EQ(T(0xab), fn("0Xab", &end_p, 0));
  ASSERT_EQ(T(0xab), fn("0xAB", &end_p, 0));
  ASSERT_EQ(T(0xab), fn("0XAB", &end_p, 0));
  ASSERT_EQ(T(0xab), fn("0xAb", &end_p, 0));
  ASSERT_EQ(T(0xab), fn("0XAb", &end_p, 0));

  // Octal lives! (Sadly.)
  ASSERT_EQ(T(0666), fn("0666", &end_p, 0));

  if (std::numeric_limits<T>::is_signed) {
    // Minimum (such as -128).
    std::string min{std::to_string(std::numeric_limits<T>::min())};
    end_p = nullptr;
    errno = 0;
    ASSERT_EQ(std::numeric_limits<T>::min(), fn(min.c_str(), &end_p, 0));
    ASSERT_EQ(0, errno);
    ASSERT_EQ('\0', *end_p);
    // Too negative (such as -129).
    min.back() = (min.back() + 1);
    end_p = nullptr;
    errno = 0;
    ASSERT_EQ(std::numeric_limits<T>::min(), fn(min.c_str(), &end_p, 0));
    ASSERT_EQ(ERANGE, errno);
    ASSERT_EQ('\0', *end_p);
  }

  // Maximum (such as 127).
  std::string max{std::to_string(std::numeric_limits<T>::max())};
  end_p = nullptr;
  errno = 0;
  ASSERT_EQ(std::numeric_limits<T>::max(), fn(max.c_str(), &end_p, 0));
  ASSERT_EQ(0, errno);
  ASSERT_EQ('\0', *end_p);
  // Too positive (such as 128).
  max.back() = (max.back() + 1);
  end_p = nullptr;
  errno = 0;
  ASSERT_EQ(std::numeric_limits<T>::max(), fn(max.c_str(), &end_p, 0));
  ASSERT_EQ(ERANGE, errno);
  ASSERT_EQ('\0', *end_p);

  // In case of overflow, strto* leaves us pointing past the end of the number,
  // not at the digit that overflowed.
  end_p = nullptr;
  errno = 0;
  ASSERT_EQ(std::numeric_limits<T>::max(),
            fn("99999999999999999999999999999999999999999999999999999abc", &end_p, 0));
  ASSERT_EQ(ERANGE, errno);
  ASSERT_STREQ("abc", end_p);
  if (std::numeric_limits<T>::is_signed) {
      end_p = nullptr;
      errno = 0;
      ASSERT_EQ(std::numeric_limits<T>::min(),
                fn("-99999999999999999999999999999999999999999999999999999abc", &end_p, 0));
      ASSERT_EQ(ERANGE, errno);
      ASSERT_STREQ("abc", end_p);
  }
}

TEST(stdlib, strtol_smoke) {
  CheckStrToInt(strtol);
}

TEST(stdlib, strtoll_smoke) {
  CheckStrToInt(strtoll);
}

TEST(stdlib, strtoul_smoke) {
  CheckStrToInt(strtoul);
}

TEST(stdlib, strtoull_smoke) {
  CheckStrToInt(strtoull);
}

TEST(stdlib, strtoimax_smoke) {
  CheckStrToInt(strtoimax);
}

TEST(stdlib, strtoumax_smoke) {
  CheckStrToInt(strtoumax);
}

TEST(stdlib, atoi) {
  // Implemented using strtol in bionic, so extensive testing unnecessary.
  ASSERT_EQ(123, atoi("123four"));
  ASSERT_EQ(0, atoi("hello"));
}

TEST(stdlib, atol) {
  // Implemented using strtol in bionic, so extensive testing unnecessary.
  ASSERT_EQ(123L, atol("123four"));
  ASSERT_EQ(0L, atol("hello"));
}

TEST(stdlib, abs) {
  ASSERT_EQ(INT_MAX, abs(-INT_MAX));
  ASSERT_EQ(INT_MAX, abs(INT_MAX));
}

TEST(stdlib, labs) {
  ASSERT_EQ(LONG_MAX, labs(-LONG_MAX));
  ASSERT_EQ(LONG_MAX, labs(LONG_MAX));
}

TEST(stdlib, llabs) {
  ASSERT_EQ(LLONG_MAX, llabs(-LLONG_MAX));
  ASSERT_EQ(LLONG_MAX, llabs(LLONG_MAX));
}

TEST(stdlib, getloadavg) {
  double load[3];

  // The second argument should have been size_t.
  ASSERT_EQ(-1, getloadavg(load, -1));
  ASSERT_EQ(-1, getloadavg(load, INT_MIN));

  // Zero is a no-op.
  ASSERT_EQ(0, getloadavg(load, 0));

  // The Linux kernel doesn't support more than 3 (but you can ask for fewer).
  ASSERT_EQ(1, getloadavg(load, 1));
  ASSERT_EQ(2, getloadavg(load, 2));
  ASSERT_EQ(3, getloadavg(load, 3));
  ASSERT_EQ(3, getloadavg(load, 4));
  ASSERT_EQ(3, getloadavg(load, INT_MAX));

  // Read /proc/loadavg and check that it's "close enough".
  double expected[3];
  std::unique_ptr<FILE, decltype(&fclose)> fp{fopen("/proc/loadavg", "re"), fclose};
  ASSERT_EQ(3, fscanf(fp.get(), "%lf %lf %lf", &expected[0], &expected[1], &expected[2]));
  load[0] = load[1] = load[2] = nan("");
  ASSERT_EQ(3, getloadavg(load, 3));

  // Check that getloadavg(3) at least overwrote the NaNs.
  ASSERT_FALSE(isnan(load[0]));
  ASSERT_FALSE(isnan(load[1]));
  ASSERT_FALSE(isnan(load[2]));
  // And that the difference between /proc/loadavg and getloadavg(3) is "small".
  ASSERT_TRUE(fabs(expected[0] - load[0]) < 0.5) << expected[0] << ' ' << load[0];
  ASSERT_TRUE(fabs(expected[1] - load[1]) < 0.5) << expected[1] << ' ' << load[1];
  ASSERT_TRUE(fabs(expected[2] - load[2]) < 0.5) << expected[2] << ' ' << load[2];
}

TEST(stdlib, getprogname) {
#if defined(__GLIBC__) || defined(ANDROID_HOST_MUSL)
  GTEST_SKIP() << "glibc and musl don't have getprogname()";
#else
  // You should always have a name.
  ASSERT_TRUE(getprogname() != nullptr);
  // The name should never have a slash in it.
  ASSERT_TRUE(strchr(getprogname(), '/') == nullptr);
#endif
}

TEST(stdlib, setprogname) {
#if defined(__GLIBC__) || defined(ANDROID_HOST_MUSL)
  GTEST_SKIP() << "glibc and musl don't have setprogname()";
#else
  // setprogname() only takes the basename of what you give it.
  setprogname("/usr/bin/muppet");
  ASSERT_STREQ("muppet", getprogname());
#endif
}
