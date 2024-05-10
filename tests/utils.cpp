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

#include <string.h>
#include <syscall.h>

#include <string>

#include <android-base/properties.h>

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

bool IsLowRamDevice() {
  return android::base::GetBoolProperty("ro.config.low_ram", false) ||
         (android::base::GetBoolProperty("ro.debuggable", false) &&
          android::base::GetBoolProperty("debug.force_low_ram", false));
}

#if defined(__GLIBC__) && __GLIBC_MINOR__ < 30
pid_t gettid() {
  return syscall(__NR_gettid);
}
#endif

void PrintTo(const Errno& e, std::ostream* os) {
  // Prefer EINVAL or whatever, but fall back to strerror() to print
  // "Unknown error 666" for bogus values. Not that I've ever seen one,
  // but we shouldn't be looking at an assertion failure unless something
  // weird has happened!
#if defined(__BIONIC__)
  const char* errno_name = strerrorname_np(e.errno_);
  if (errno_name != nullptr) {
    *os << errno_name;
  } else
#endif
  {
    *os << strerror(e.errno_);
  }
}

int64_t NanoTime() {
  auto t = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now());
  return t.time_since_epoch().count();
}

bool operator==(const Errno& lhs, const Errno& rhs) {
  return lhs.errno_ == rhs.errno_;
}
