/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>

#include <android-base/stringprintf.h>
#include <benchmark/benchmark.h>
#include "util.h"

BIONIC_TRIVIAL_BENCHMARK(BM_unistd_getpid, getpid());
BIONIC_TRIVIAL_BENCHMARK(BM_unistd_getpid_syscall, syscall(__NR_getpid));

// TODO: glibc 2.30 added gettid() too.
#if defined(__BIONIC__)
BIONIC_TRIVIAL_BENCHMARK(BM_unistd_gettid, gettid());
#endif
BIONIC_TRIVIAL_BENCHMARK(BM_unistd_gettid_syscall, syscall(__NR_gettid));

// Many native allocators have custom prefork and postfork functions.
// Measure the fork call to make sure nothing takes too long.
void BM_unistd_fork_call(benchmark::State& state) {
  for (auto _ : state) {
    pid_t pid;
    if ((pid = fork()) == 0) {
      // Sleep for a little while so that the parent is not interrupted
      // right away when the process exits.
      usleep(100);
      _exit(1);
    }
    state.PauseTiming();
    if (pid == -1) {
      std::string err = android::base::StringPrintf("Fork failed: %s", strerror(errno));
      state.SkipWithError(err.c_str());
    }
    pid_t wait_pid = waitpid(pid, 0, 0);
    if (wait_pid != pid) {
      if (wait_pid == -1) {
        std::string err = android::base::StringPrintf("waitpid call failed: %s", strerror(errno));
        state.SkipWithError(err.c_str());
      } else {
        std::string err = android::base::StringPrintf(
            "waitpid return an unknown pid, expected %d, actual %d", pid, wait_pid);
        state.SkipWithError(err.c_str());
      }
    }
    state.ResumeTiming();
  }
}
BIONIC_BENCHMARK(BM_unistd_fork_call);
