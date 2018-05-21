/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <private/bionic_malloc_dispatch.h>
#include <tests/utils.h>

__BEGIN_DECLS

void get_malloc_leak_info(uint8_t**, size_t*, size_t*, size_t*, size_t*);
void free_malloc_leak_info(uint8_t*);
int malloc_iterate(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*);
void malloc_enable();
void malloc_disable();
ssize_t malloc_backtrace(void*, uintptr_t*, size_t);

__END_DECLS

class MallocHooksTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ASSERT_EQ(0, setenv("LIBC_HOOKS_ENABLE", "1", true));
    initialized_ = false;
  }

  void TearDown() override {
    if (initialized_) {
      Reset();
    }
    ASSERT_EQ(0, unsetenv("LIBC_HOOKS_ENABLE"));
  }

  void Init() {
    initialized_ = true;
    malloc_hook_called_ = false;
    free_hook_called_ = false;
    realloc_hook_called_ = false;
    memalign_hook_called_ = false;

    void_arg_ = nullptr;

    orig_malloc_hook_ = __malloc_hook;
    orig_free_hook_ = __free_hook;
    orig_realloc_hook_ = __realloc_hook;
    orig_memalign_hook_ = __memalign_hook;
  }

  void Reset() {
    __malloc_hook = orig_malloc_hook_;
    __free_hook = orig_free_hook_;
    __realloc_hook = orig_realloc_hook_;
    __memalign_hook = orig_memalign_hook_;
  }

  void Exec(std::vector<const char*> args);
  void RunTest(std::string test_name);

 public:
  bool initialized_;

  int fd_;
  std::string raw_output_;
  pid_t pid_;

  static bool malloc_hook_called_;
  static bool free_hook_called_;
  static bool realloc_hook_called_;
  static bool memalign_hook_called_;
  static const void* void_arg_;

  static void* (*orig_malloc_hook_)(size_t, const void*);
  static void (*orig_free_hook_)(void*, const void*);
  static void* (*orig_realloc_hook_)(void*, size_t, const void*);
  static void* (*orig_memalign_hook_)(size_t, size_t, const void*);

  static void* test_malloc_hook(size_t, const void*);
  static void* test_realloc_hook(void* ptr, size_t, const void*);
  static void* test_memalign_hook(size_t, size_t, const void*);
  static void test_free_hook(void*, const void*);
};

bool MallocHooksTest::malloc_hook_called_;
bool MallocHooksTest::free_hook_called_;
bool MallocHooksTest::realloc_hook_called_;
bool MallocHooksTest::memalign_hook_called_;
const void* MallocHooksTest::void_arg_;

void* (*MallocHooksTest::orig_malloc_hook_)(size_t, const void*);
void (*MallocHooksTest::orig_free_hook_)(void*, const void*);
void* (*MallocHooksTest::orig_realloc_hook_)(void*, size_t, const void*);
void* (*MallocHooksTest::orig_memalign_hook_)(size_t, size_t, const void*);

static void GetExe(std::string* exe_name) {
  char path[PATH_MAX];
  ssize_t path_len = readlink("/proc/self/exe", path, sizeof(path));
  ASSERT_TRUE(path_len >= 0);
  *exe_name = std::string(path, path_len);
}

void MallocHooksTest::RunTest(std::string test_name) {
  std::vector<const char*> args;
  args.push_back("--gtest_also_run_disabled_tests");
  std::string filter_arg("--gtest_filter=" + test_name);
  args.push_back(filter_arg.c_str());

  ExecTestHelper test;
  test.Run(
      [&]() {
        std::string exe_name;
        GetExe(&exe_name);
        args.insert(args.begin(), exe_name.c_str());
        args.push_back(nullptr);
        execv(args[0], reinterpret_cast<char* const*>(const_cast<char**>(args.data())));
        exit(1);
      },
      0, nullptr);
}

void* MallocHooksTest::test_malloc_hook(size_t size, const void* arg) {
  malloc_hook_called_ = true;
  void_arg_ = arg;
  return orig_malloc_hook_(size, arg);
}

void* MallocHooksTest::test_realloc_hook(void* ptr, size_t size, const void* arg) {
  realloc_hook_called_ = true;
  void_arg_ = arg;
  return orig_realloc_hook_(ptr, size, arg);
}

void* MallocHooksTest::test_memalign_hook(size_t alignment, size_t size, const void* arg) {
  memalign_hook_called_ = true;
  void_arg_ = arg;
  return orig_memalign_hook_(alignment, size, arg);
}

void MallocHooksTest::test_free_hook(void* ptr, const void* arg) {
  free_hook_called_ = true;
  void_arg_ = arg;
  return orig_free_hook_(ptr, arg);
}

TEST_F(MallocHooksTest, other_malloc_functions) {
  RunTest("*.DISABLED_other_malloc_functions");
}

// Call other intercepted functions to make sure there are no crashes.
TEST_F(MallocHooksTest, DISABLED_other_malloc_functions) {
  struct mallinfo info = mallinfo();
  EXPECT_NE(0U, info.uordblks);

  EXPECT_EQ(0, mallopt(-1000, -1000));

  void* ptr = malloc(1024);
  EXPECT_LE(1024U, malloc_usable_size(ptr));
  free(ptr);
}

TEST_F(MallocHooksTest, extended_functions) {
  RunTest("*.DISABLED_extended_functions");
}

TEST_F(MallocHooksTest, DISABLED_extended_functions) {
  uint8_t* info = nullptr;
  size_t overall_size = 100;
  size_t info_size = 200;
  size_t total_memory = 300;
  size_t backtrace_size = 400;
  get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  EXPECT_EQ(nullptr, info);
  EXPECT_EQ(0U, overall_size);
  EXPECT_EQ(0U, info_size);
  EXPECT_EQ(0U, total_memory);
  EXPECT_EQ(0U, backtrace_size);

  free_malloc_leak_info(info);

  malloc_enable();
  malloc_disable();

  EXPECT_EQ(0, malloc_iterate(0, 0, nullptr, nullptr));

  // Malloc hooks doesn't support any backtracing.
  EXPECT_EQ(0, malloc_backtrace(nullptr, nullptr, 10));
}

TEST_F(MallocHooksTest, malloc_hook) {
  RunTest("*.DISABLED_malloc_hook");
}

TEST_F(MallocHooksTest, DISABLED_malloc_hook) {
  Init();
  ASSERT_TRUE(__malloc_hook != nullptr);
  __malloc_hook = test_malloc_hook;

  void* ptr = malloc(1024);
  ASSERT_TRUE(ptr != nullptr);
  write(0, ptr, 0);
  free(ptr);

  EXPECT_TRUE(malloc_hook_called_) << "The malloc hook was not called.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The malloc hook was called with a nullptr.";
}

TEST_F(MallocHooksTest, free_hook) {
  RunTest("*.DISABLED_free_hook");
}

TEST_F(MallocHooksTest, DISABLED_free_hook) {
  Init();
  ASSERT_TRUE(__free_hook != nullptr);
  __free_hook = test_free_hook;

  void* ptr = malloc(1024);
  ASSERT_TRUE(ptr != nullptr);
  free(ptr);
  write(0, ptr, 0);

  EXPECT_TRUE(free_hook_called_) << "The free hook was not called.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The free hook was called with a nullptr.";
}

TEST_F(MallocHooksTest, realloc_hook) {
  RunTest("*.DISABLED_realloc_hook");
}

TEST_F(MallocHooksTest, DISABLED_realloc_hook) {
  Init();
  ASSERT_TRUE(__realloc_hook != nullptr);
  __realloc_hook = test_realloc_hook;

  void* ptr = malloc(1024);
  ASSERT_TRUE(ptr != nullptr);
  ptr = realloc(ptr, 2048);
  free(ptr);
  write(0, ptr, 0);

  EXPECT_TRUE(realloc_hook_called_) << "The realloc hook was not called.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The realloc hook was called with a nullptr.";
}

TEST_F(MallocHooksTest, memalign_hook) {
  RunTest("*.DISABLED_memalign_hook");
}

TEST_F(MallocHooksTest, DISABLED_memalign_hook) {
  Init();
  ASSERT_TRUE(__memalign_hook != nullptr);
  __memalign_hook = test_memalign_hook;

  void* ptr = memalign(32, 1024);
  ASSERT_TRUE(ptr != nullptr);
  free(ptr);
  write(0, ptr, 0);

  EXPECT_TRUE(memalign_hook_called_) << "The memalign hook was not called.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The memalign hook was called with a nullptr.";
}

TEST_F(MallocHooksTest, posix_memalign_hook) {
  RunTest("*.DISABLED_posix_memalign_hook");
}

TEST_F(MallocHooksTest, DISABLED_posix_memalign_hook) {
  Init();
  ASSERT_TRUE(__memalign_hook != nullptr);
  __memalign_hook = test_memalign_hook;

  void* ptr;
  ASSERT_EQ(0, posix_memalign(&ptr, 32, 1024));
  ASSERT_TRUE(ptr != nullptr);
  free(ptr);
  write(0, ptr, 0);

  EXPECT_TRUE(memalign_hook_called_) << "The memalign hook was not called when running posix_memalign.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The memalign hook was called with a nullptr.";
}

TEST_F(MallocHooksTest, posix_memalign_hook_error) {
  RunTest("*.DISABLED_posix_memalign_hook_error");
}

TEST_F(MallocHooksTest, DISABLED_posix_memalign_hook_error) {
  Init();
  ASSERT_TRUE(__memalign_hook != nullptr);
  __memalign_hook = test_memalign_hook;

  void* ptr;
  ASSERT_EQ(EINVAL, posix_memalign(&ptr, 11, 1024));
  write(0, ptr, 0);

  EXPECT_FALSE(memalign_hook_called_)
      << "The memalign hook was called when running posix_memalign with an error.";
  EXPECT_FALSE(void_arg_ != nullptr)
      << "The memalign hook was called with a nullptr with an error.";
}

TEST_F(MallocHooksTest, aligned_alloc_hook) {
  RunTest("*.DISABLED_aligned_alloc_hook");
}

TEST_F(MallocHooksTest, DISABLED_aligned_alloc_hook) {
  Init();
  ASSERT_TRUE(__memalign_hook != nullptr);
  __memalign_hook = test_memalign_hook;

  void* ptr = aligned_alloc(32, 1024);
  ASSERT_TRUE(ptr != nullptr);
  free(ptr);
  write(0, ptr, 0);

  EXPECT_TRUE(memalign_hook_called_) << "The memalign hook was not called when running aligned_alloc.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The memalign hook was called with a nullptr.";
}

TEST_F(MallocHooksTest, aligned_alloc_hook_error) {
  RunTest("*.DISABLED_aligned_alloc_hook_error");
}

TEST_F(MallocHooksTest, DISABLED_aligned_alloc_hook_error) {
  Init();
  ASSERT_TRUE(__memalign_hook != nullptr);
  __memalign_hook = test_memalign_hook;

  void* ptr = aligned_alloc(11, 1024);
  ASSERT_TRUE(ptr == nullptr);
  EXPECT_EQ(EINVAL, errno);
  write(0, ptr, 0);

  EXPECT_FALSE(memalign_hook_called_)
      << "The memalign hook was called when running aligned_alloc with an error.";
  EXPECT_FALSE(void_arg_ != nullptr)
      << "The memalign hook was called with a nullptr with an error.";
}

#if !defined(__LP64__)
TEST_F(MallocHooksTest, pvalloc_hook) {
  RunTest("*.DISABLED_pvalloc_hook");
}

extern "C" void* pvalloc(size_t);

TEST_F(MallocHooksTest, DISABLED_pvalloc_hook) {
  Init();
  ASSERT_TRUE(__memalign_hook != nullptr);
  __memalign_hook = test_memalign_hook;

  void* ptr = pvalloc(1024);
  ASSERT_TRUE(ptr != nullptr);
  write(0, ptr, 0);
  free(ptr);

  EXPECT_TRUE(memalign_hook_called_) << "The memalign hook was not called for pvalloc.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The memalign hook was called with a nullptr.";
}

TEST_F(MallocHooksTest, valloc_hook) {
  RunTest("*.DISABLED_valloc_hook");
}

extern "C" void* valloc(size_t);

TEST_F(MallocHooksTest, DISABLED_valloc_hook) {
  Init();
  ASSERT_TRUE(__memalign_hook != nullptr);
  __memalign_hook = test_memalign_hook;

  void* ptr = valloc(1024);
  ASSERT_TRUE(ptr != nullptr);
  write(0, ptr, 0);
  free(ptr);

  EXPECT_TRUE(memalign_hook_called_) << "The memalign hook was not called for valloc.";
  EXPECT_TRUE(void_arg_ != nullptr) << "The memalign hook was called with a nullptr.";
}
#endif
