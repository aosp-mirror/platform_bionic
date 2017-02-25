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

#include <dlfcn.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>

#include <string>

#include "utils.h"

extern "C" int main_global_default_serial() {
  return 3370318;
}

extern "C" int main_global_protected_serial() {
  return 2716057;
}

// The following functions are defined in DT_NEEDED
// libdl_preempt_test.so library.

// This one calls main_global_default_serial
extern "C" int main_global_default_get_serial();

// This one calls main_global_protected_serial
extern "C" int main_global_protected_get_serial();

// This one calls lib_global_default_serial
extern "C" int lib_global_default_get_serial();

// This one calls lib_global_protected_serial
extern "C" int lib_global_protected_get_serial();

// This test verifies that the global default function
// main_global_default_serial() is preempted by
// the function defined above.
TEST(dl, main_preempts_global_default) {
  ASSERT_EQ(3370318, main_global_default_get_serial());
}

// This one makes sure that the global protected
// symbols do not get preempted
TEST(dl, main_does_not_preempt_global_protected) {
  ASSERT_EQ(3370318, main_global_protected_get_serial());
}

// check same things for lib
TEST(dl, lib_preempts_global_default) {
  ASSERT_EQ(3370318, lib_global_default_get_serial());
}

TEST(dl, lib_does_not_preempt_global_protected) {
  ASSERT_EQ(3370318, lib_global_protected_get_serial());
}

TEST(dl, exec_linker) {
#if defined(__BIONIC__)
#if defined(__LP64__)
  static constexpr const char* kPathToLinker = "/system/bin/linker64";
#else
  static constexpr const char* kPathToLinker = "/system/bin/linker";
#endif
  ExecTestHelper eth;
  std::string expected_output = std::string("This is ") + kPathToLinker +
                                ", the helper program for dynamic executables.\n";
  eth.SetArgs( { kPathToLinker, nullptr });
  eth.Run([&]() { execve(kPathToLinker, eth.GetArgs(), eth.GetEnv()); }, 0, expected_output.c_str());
#endif
}

// TODO: Add tests for LD_PRELOADs
