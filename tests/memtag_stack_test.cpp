/*
 * Copyright (C) 2022 The Android Open Source Project
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

#include <tuple>

#include <gtest/gtest.h>

#if defined(__BIONIC__)
#include "gtest_globals.h"
#include "platform/bionic/mte.h"
#include "utils.h"
#endif

class MemtagStackTest : public testing::TestWithParam<std::tuple<const char*, bool>> {};

TEST_P(MemtagStackTest, test) {
#if defined(__BIONIC__) && defined(__aarch64__)
  if (!mte_supported()) {
    GTEST_SKIP() << "MTE unsupported";
  }
  bool is_static = std::get<1>(GetParam());
  std::string helper =
      GetTestLibRoot() + (is_static ? "/stack_tagging_static_helper" : "/stack_tagging_helper");
  const char* arg = std::get<0>(GetParam());
  ExecTestHelper eth;
  eth.SetArgs({helper.c_str(), arg, nullptr});
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, "");
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}

INSTANTIATE_TEST_SUITE_P(
    , MemtagStackTest,
    testing::Combine(testing::Values("vfork_execve", "vfork_execl", "vfork_exit", "longjmp",
                                     "longjmp_sigaltstack", "android_mallopt", "exception_cleanup"),
                     testing::Bool()),
    [](const ::testing::TestParamInfo<MemtagStackTest::ParamType>& info) {
      std::string s = std::get<0>(info.param);
      if (std::get<1>(info.param)) s += "_static";
      return s;
    });
