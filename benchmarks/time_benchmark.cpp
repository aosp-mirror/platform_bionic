/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include "benchmark.h"

#include <sys/syscall.h>
#include <time.h>

static void BM_time_clock_gettime(int iters) {
  StartBenchmarkTiming();

  timespec t;
  for (int i = 0; i < iters; ++i) {
    clock_gettime(CLOCK_MONOTONIC, &t);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_time_clock_gettime);

static void BM_time_clock_gettime_syscall(int iters) {
  StartBenchmarkTiming();

  timespec t;
  for (int i = 0; i < iters; ++i) {
    syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &t);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_time_clock_gettime_syscall);

static void BM_time_gettimeofday(int iters) {
  StartBenchmarkTiming();

  timeval tv;
  for (int i = 0; i < iters; ++i) {
    gettimeofday(&tv, NULL);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_time_gettimeofday);

static void BM_time_gettimeofday_syscall(int iters) {
  StartBenchmarkTiming();

  timeval tv;
  for (int i = 0; i < iters; ++i) {
    syscall(__NR_gettimeofday, &tv, NULL);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_time_gettimeofday_syscall);

static void BM_time_time(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    time(NULL);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_time_time);
