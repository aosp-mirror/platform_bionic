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
#include <stdlib.h>

#include "utils.h"

TEST(cpu_target_features, has_expected_x86_compiler_values) {
#if defined(__x86_64__) || defined(__i386__)
  ExecTestHelper eth;
  char* const argv[] = {nullptr};
  const auto invocation = [&] { execvp("cpu-target-features", argv); };
  eth.Run(invocation, 0, "(^|\n)__AES__=1($|\n)");
  eth.Run(invocation, 0, "(^|\n)__CRC32__=1($|\n)");
#else
  GTEST_SKIP() << "Not targeting an x86 architecture.";
#endif
}

TEST(cpu_target_features, has_expected_aarch64_compiler_values) {
#if defined(__aarch64__)
  ExecTestHelper eth;
  char* const argv[] = {nullptr};
  const auto invocation = [&] { execvp("cpu-target-features", argv); };
  eth.Run(invocation, 0, "(^|\n)__ARM_FEATURE_AES=1($|\n)");
  eth.Run(invocation, 0, "(^|\n)__ARM_FEATURE_CRC32=1($|\n)");
#else
  GTEST_SKIP() << "Not targeting an aarch64 architecture.";
#endif
}

TEST(cpu_target_features, has_expected_arm_compiler_values) {
#if defined(__arm__)
  ExecTestHelper eth;
  char* const argv[] = {nullptr};
  const auto invocation = [&] { execvp("cpu-target-features", argv); };
  eth.Run(invocation, 0, "(^|\n)__ARM_FEATURE_AES=1($|\n)");
  eth.Run(invocation, 0, "(^|\n)__ARM_FEATURE_CRC32=1($|\n)");
#else
  GTEST_SKIP() << "Not targeting an arm architecture.";
#endif
}
