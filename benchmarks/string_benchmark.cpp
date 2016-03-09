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

#include <stdint.h>
#include <string.h>

#include <benchmark/benchmark.h>

#define KB 1024
#define MB 1024*KB

#define AT_COMMON_SIZES \
    Arg(8)->Arg(64)->Arg(512)->Arg(1*KB)->Arg(8*KB)->Arg(16*KB)->Arg(32*KB)->Arg(64*KB)

// TODO: test unaligned operation too? (currently everything will be 8-byte aligned by malloc.)

static void BM_string_memcmp(benchmark::State& state) {
  const size_t nbytes = state.range_x();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);
  memset(dst, 'x', nbytes);

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {
    c += memcmp(dst, src, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_string_memcmp)->AT_COMMON_SIZES;

static void BM_string_memcpy(benchmark::State& state) {
  const size_t nbytes = state.range_x();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);

  while (state.KeepRunning()) {
    memcpy(dst, src, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_string_memcpy)->AT_COMMON_SIZES;

static void BM_string_memmove(benchmark::State& state) {
  const size_t nbytes = state.range_x();
  char* buf = new char[nbytes + 64];
  memset(buf, 'x', nbytes + 64);

  while (state.KeepRunning()) {
    memmove(buf, buf + 1, nbytes); // Worst-case overlap.
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] buf;
}
BENCHMARK(BM_string_memmove)->AT_COMMON_SIZES;

static void BM_string_memset(benchmark::State& state) {
  const size_t nbytes = state.range_x();
  char* dst = new char[nbytes];

  while (state.KeepRunning()) {
    memset(dst, 0, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] dst;
}
BENCHMARK(BM_string_memset)->AT_COMMON_SIZES;

static void BM_string_strlen(benchmark::State& state) {
  const size_t nbytes = state.range_x();
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {
    c += strlen(s);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] s;
}
BENCHMARK(BM_string_strlen)->AT_COMMON_SIZES;
