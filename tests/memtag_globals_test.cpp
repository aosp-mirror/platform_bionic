/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <gtest/gtest.h>

#if defined(__BIONIC__)
#include "gtest_globals.h"
#include "utils.h"
#endif  // defined(__BIONIC__)

#include <android-base/test_utils.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <tuple>

#include "platform/bionic/mte.h"

class MemtagGlobalsTest : public testing::TestWithParam<bool> {};

TEST_P(MemtagGlobalsTest, test) {
  SKIP_WITH_HWASAN << "MTE globals tests are incompatible with HWASan";
#if defined(__BIONIC__) && defined(__aarch64__)
  std::string binary = GetTestLibRoot() + "/memtag_globals_binary";
  bool is_static = MemtagGlobalsTest::GetParam();
  if (is_static) {
    binary += "_static";
  }

  chmod(binary.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({binary.c_str(), nullptr});
  eth.Run(
      [&]() {
        execve(binary.c_str(), eth.GetArgs(), eth.GetEnv());
        GTEST_FAIL() << "Failed to execve: " << strerror(errno) << " " << binary.c_str();
      },
      // We catch the global-buffer-overflow and crash only when MTE globals is
      // supported. Note that MTE globals is unsupported for fully static
      // executables, but we should still make sure the binary passes its
      // assertions, just that global variables won't be tagged.
      (mte_supported() && !is_static) ? -SIGSEGV : 0, "Assertions were passed");
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}

INSTANTIATE_TEST_SUITE_P(MemtagGlobalsTest, MemtagGlobalsTest, testing::Bool(),
                         [](const ::testing::TestParamInfo<MemtagGlobalsTest::ParamType>& info) {
                           if (info.param) return "MemtagGlobalsTest_static";
                           return "MemtagGlobalsTest";
                         });

TEST(MemtagGlobalsTest, RelrRegressionTestForb314038442) {
  SKIP_WITH_HWASAN << "MTE globals tests are incompatible with HWASan";
#if defined(__BIONIC__) && defined(__aarch64__)
  std::string binary = GetTestLibRoot() + "/mte_globals_relr_regression_test_b_314038442";
  chmod(binary.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({binary.c_str(), nullptr});
  eth.Run(
      [&]() {
        execve(binary.c_str(), eth.GetArgs(), eth.GetEnv());
        GTEST_FAIL() << "Failed to execve: " << strerror(errno) << " " << binary.c_str();
      },
      /* exit code */ 0, "Program loaded successfully.*Tags are zero!");
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}

TEST(MemtagGlobalsTest, RelrRegressionTestForb314038442WithMteGlobals) {
  if (!mte_supported()) GTEST_SKIP() << "Must have MTE support.";
#if defined(__BIONIC__) && defined(__aarch64__)
  std::string binary = GetTestLibRoot() + "/mte_globals_relr_regression_test_b_314038442_mte";
  chmod(binary.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({binary.c_str(), nullptr});
  eth.Run(
      [&]() {
        execve(binary.c_str(), eth.GetArgs(), eth.GetEnv());
        GTEST_FAIL() << "Failed to execve: " << strerror(errno) << " " << binary.c_str();
      },
      /* exit code */ 0, "Program loaded successfully.*Tags are non-zero");
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}
