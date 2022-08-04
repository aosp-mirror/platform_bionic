/*
 * Copyright (C) 2022 The Android Open Source Project
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

#include "utils.h"

void RunGwpAsanTest(const char* test_name) {
  ExecTestHelper eh;
  eh.SetEnv({"GWP_ASAN_SAMPLE_RATE=1", "GWP_ASAN_PROCESS_SAMPLING=1", "GWP_ASAN_MAX_ALLOCS=40000",
             nullptr});
  std::string filter_arg = "--gtest_filter=";
  filter_arg += test_name;
  std::string exec(testing::internal::GetArgvs()[0]);
  eh.SetArgs({exec.c_str(), "--gtest_also_run_disabled_tests", filter_arg.c_str(), nullptr});
  eh.Run([&]() { execve(exec.c_str(), eh.GetArgs(), eh.GetEnv()); },
         /* expected_exit_status */ 0,
         // |expected_output_regex|, ensure at least one test ran:
         R"(\[  PASSED  \] [1-9][0-9]* test)");
}

void RunSubtestNoEnv(const char* test_name) {
  ExecTestHelper eh;
  std::string filter_arg = "--gtest_filter=";
  filter_arg += test_name;
  std::string exec(testing::internal::GetArgvs()[0]);
  eh.SetArgs({exec.c_str(), "--gtest_also_run_disabled_tests", filter_arg.c_str(), nullptr});
  eh.Run([&]() { execve(exec.c_str(), eh.GetArgs(), eh.GetEnv()); },
         /* expected_exit_status */ 0,
         // |expected_output_regex|, ensure at least one test ran:
         R"(\[  PASSED  \] [1-9]+0? test)");
}
