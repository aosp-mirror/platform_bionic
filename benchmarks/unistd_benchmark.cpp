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

#include <benchmark/benchmark.h>

static void BM_unistd_getpid(benchmark::State& state) {
  while (state.KeepRunning()) {
    getpid();
  }
}
BENCHMARK(BM_unistd_getpid);

static void BM_unistd_getpid_syscall(benchmark::State& state) {
  while (state.KeepRunning()) {
    syscall(__NR_getpid);
  }
}
BENCHMARK(BM_unistd_getpid_syscall);

#if defined(__BIONIC__)

// Stop GCC optimizing out our pure function.
/* Must not be static! */ pid_t (*gettid_fp)() = gettid;

static void BM_unistd_gettid(benchmark::State& state) {
  while (state.KeepRunning()) {
    gettid_fp();
  }
}
BENCHMARK(BM_unistd_gettid);

#endif

void BM_unistd_gettid_syscall(benchmark::State& state) {
  while (state.KeepRunning()) {
    syscall(__NR_gettid);
  }
}
BENCHMARK(BM_unistd_gettid_syscall);

BENCHMARK_MAIN()
