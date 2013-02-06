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

#include "benchmark.h"

#include <string.h>

#define KB 1024
#define MB 1024*KB

#define AT_COMMON_SIZES \
    Arg(8)->Arg(64)->Arg(512)->Arg(1*KB)->Arg(8*KB)->Arg(16*KB)->Arg(32*KB)->Arg(64*KB)

// TODO: test unaligned operation too? (currently everything will be 8-byte aligned by malloc.)

static void BM_string_memcmp(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);
  memset(dst, 'x', nbytes);
  StartBenchmarkTiming();

  volatile int c __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c += memcmp(dst, src, nbytes);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_string_memcmp)->AT_COMMON_SIZES;

static void BM_string_memcpy(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    memcpy(dst, src, nbytes);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_string_memcpy)->AT_COMMON_SIZES;

static void BM_string_memmove(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* buf = new char[nbytes + 64];
  memset(buf, 'x', nbytes + 64);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    memmove(buf, buf + 1, nbytes); // Worst-case overlap.
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] buf;
}
BENCHMARK(BM_string_memmove)->AT_COMMON_SIZES;

static void BM_string_memset(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* dst = new char[nbytes];
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    memset(dst, 0, nbytes);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] dst;
}
BENCHMARK(BM_string_memset)->AT_COMMON_SIZES;

static void BM_string_strlen(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;
  StartBenchmarkTiming();

  volatile int c __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c += strlen(s);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] s;
}
BENCHMARK(BM_string_strlen)->AT_COMMON_SIZES;
