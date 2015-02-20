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

#include <sys/syscall.h>
#include <unistd.h>

#include <benchmark/Benchmark.h>

BENCHMARK_NO_ARG(BM_unistd_getpid);
void BM_unistd_getpid::Run(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    getpid();
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_unistd_getpid_syscall);
void BM_unistd_getpid_syscall::Run(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    syscall(__NR_getpid);
  }

  StopBenchmarkTiming();
}

#if defined(__BIONIC__)

// Stop GCC optimizing out our pure function.
/* Must not be static! */ pid_t (*gettid_fp)() = gettid;

BENCHMARK_NO_ARG(BM_unistd_gettid);
void BM_unistd_gettid::Run(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    gettid_fp();
  }

  StopBenchmarkTiming();
}

#endif

BENCHMARK_NO_ARG(BM_unistd_gettid_syscall);
void BM_unistd_gettid_syscall::Run(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    syscall(__NR_gettid);
  }

  StopBenchmarkTiming();
}
