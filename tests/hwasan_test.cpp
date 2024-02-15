/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <dlfcn.h>
#include <stdlib.h>

#include <gtest/gtest.h>

#include <android-base/silent_death_test.h>
#include <android-base/test_utils.h>

using HwasanDeathTest = SilentDeathTest;

TEST_F(HwasanDeathTest, UseAfterFree) {
  EXPECT_DEATH(
      {
        void* m = malloc(1);
        volatile char* x = const_cast<volatile char*>(reinterpret_cast<char*>(m));
        *x = 1;
        free(m);
        *x = 2;
      },
      "use-after-free");
}

TEST_F(HwasanDeathTest, OutOfBounds) {
  EXPECT_DEATH(
      {
        void* m = malloc(1);
        volatile char* x = const_cast<volatile char*>(reinterpret_cast<char*>(m));
        x[1] = 1;
      },
      "buffer-overflow");
}

// Check whether dlopen of /foo/bar.so checks /foo/hwasan/bar.so first.
TEST(HwasanTest, DlopenAbsolutePath) {
  std::string path = android::base::GetExecutableDirectory() + "/libtest_simple_hwasan.so";
  ASSERT_EQ(0, access(path.c_str(), F_OK));  // Verify test setup.
  std::string hwasan_path =
      android::base::GetExecutableDirectory() + "/hwasan/libtest_simple_hwasan.so";
  ASSERT_EQ(0, access(hwasan_path.c_str(), F_OK));  // Verify test setup.

  void* handle = dlopen(path.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle != nullptr);
  uint32_t* compiled_with_hwasan =
      reinterpret_cast<uint32_t*>(dlsym(handle, "dlopen_testlib_compiled_with_hwasan"));
  EXPECT_TRUE(*compiled_with_hwasan);
  dlclose(handle);
}

TEST(HwasanTest, IsRunningWithHWasan) {
  EXPECT_TRUE(running_with_hwasan());
}
