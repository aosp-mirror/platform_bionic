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

#include <thread>

#include <dlfcn.h>
#include <stdlib.h>

#include <gtest/gtest.h>

#include <android-base/silent_death_test.h>
#include <android-base/test_utils.h>
#include "mte_utils.h"
#include "utils.h"

TEST(MemtagStackDlopenTest, DependentBinaryGetsMemtagStack) {
#if defined(__BIONIC__) && defined(__aarch64__)
  if (!running_with_mte()) GTEST_SKIP() << "Test requires MTE.";
  if (is_stack_mte_on())
    GTEST_SKIP() << "Stack MTE needs to be off for this test. Are you running fullmte?";

  std::string path =
      android::base::GetExecutableDirectory() + "/testbinary_depends_on_simple_memtag_stack";
  ExecTestHelper eth;
  std::string ld_library_path = "LD_LIBRARY_PATH=" + android::base::GetExecutableDirectory();
  eth.SetArgs({path.c_str(), nullptr});
  eth.SetEnv({ld_library_path.c_str(), nullptr});
  eth.Run([&]() { execve(path.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "RAN");
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}

TEST(MemtagStackDlopenTest, DependentBinaryGetsMemtagStack2) {
#if defined(__BIONIC__) && defined(__aarch64__)
  if (!running_with_mte()) GTEST_SKIP() << "Test requires MTE.";
  if (is_stack_mte_on())
    GTEST_SKIP() << "Stack MTE needs to be off for this test. Are you running fullmte?";

  std::string path = android::base::GetExecutableDirectory() +
                     "/testbinary_depends_on_depends_on_simple_memtag_stack";
  ExecTestHelper eth;
  std::string ld_library_path = "LD_LIBRARY_PATH=" + android::base::GetExecutableDirectory();
  eth.SetArgs({path.c_str(), nullptr});
  eth.SetEnv({ld_library_path.c_str(), nullptr});
  eth.Run([&]() { execve(path.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "RAN");
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}

TEST(MemtagStackDlopenTest, DlopenRemapsStack) {
#if defined(__BIONIC__) && defined(__aarch64__)
  // If this test is failing, look at crash logcat for why the test binary died.
  if (!running_with_mte()) GTEST_SKIP() << "Test requires MTE.";
  if (is_stack_mte_on())
    GTEST_SKIP() << "Stack MTE needs to be off for this test. Are you running fullmte?";

  std::string path =
      android::base::GetExecutableDirectory() + "/testbinary_is_stack_mte_after_dlopen";
  std::string lib_path =
      android::base::GetExecutableDirectory() + "/libtest_simple_memtag_stack.so";
  ExecTestHelper eth;
  std::string ld_library_path = "LD_LIBRARY_PATH=" + android::base::GetExecutableDirectory();
  eth.SetArgs({path.c_str(), lib_path.c_str(), nullptr});
  eth.SetEnv({ld_library_path.c_str(), nullptr});
  eth.Run([&]() { execve(path.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "RAN");
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}

TEST(MemtagStackDlopenTest, DlopenRemapsStack2) {
#if defined(__BIONIC__) && defined(__aarch64__)
  // If this test is failing, look at crash logcat for why the test binary died.
  if (!running_with_mte()) GTEST_SKIP() << "Test requires MTE.";
  if (is_stack_mte_on())
    GTEST_SKIP() << "Stack MTE needs to be off for this test. Are you running fullmte?";

  std::string path =
      android::base::GetExecutableDirectory() + "/testbinary_is_stack_mte_after_dlopen";
  std::string lib_path =
      android::base::GetExecutableDirectory() + "/libtest_depends_on_simple_memtag_stack.so";
  ExecTestHelper eth;
  std::string ld_library_path = "LD_LIBRARY_PATH=" + android::base::GetExecutableDirectory();
  eth.SetArgs({path.c_str(), lib_path.c_str(), nullptr});
  eth.SetEnv({ld_library_path.c_str(), nullptr});
  eth.Run([&]() { execve(path.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "RAN");
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}
