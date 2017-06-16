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

constexpr auto KB = 1024;

#define AT_COMMON_SIZES \
    Arg(8)->Arg(64)->Arg(512)->Arg(1*KB)->Arg(8*KB)->Arg(16*KB)->Arg(32*KB)->Arg(64*KB)

// TODO: test unaligned operation too? (currently everything will be 8-byte aligned by malloc.)

static void BM_string_memcmp(benchmark::State& state) {
  const size_t nbytes = state.range(0);
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
  const size_t nbytes = state.range(0);
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

static void BM_string_memmove_non_overlapping(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> src(nbytes, 'x');
  std::vector<char> dst(nbytes, 'x');

  while (state.KeepRunning()) {
    memmove(dst.data(), src.data(), nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memmove_non_overlapping)->AT_COMMON_SIZES;

static void BM_string_memmove_overlap_dst_before_src(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> buf(nbytes + 1, 'x');

  while (state.KeepRunning()) {
    memmove(buf.data(), buf.data() + 1, nbytes); // Worst-case overlap.
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memmove_overlap_dst_before_src)->AT_COMMON_SIZES;

static void BM_string_memmove_overlap_src_before_dst(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> buf(nbytes + 1, 'x');

  while (state.KeepRunning()) {
    memmove(buf.data() + 1, buf.data(), nbytes); // Worst-case overlap.
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memmove_overlap_src_before_dst)->AT_COMMON_SIZES;

static void BM_string_memset(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* dst = new char[nbytes];

  while (state.KeepRunning()) {
    memset(dst, 0, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] dst;
}
BENCHMARK(BM_string_memset)->AT_COMMON_SIZES;

static void BM_string_strlen(benchmark::State& state) {
  const size_t nbytes = state.range(0);
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

static void BM_string_strcat_copy_only(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> src(nbytes, 'x');
  std::vector<char> dst(nbytes + 2);
  src[nbytes - 1] = '\0';
  dst[0] = 'y';
  dst[1] = 'y';
  dst[2] = '\0';

  while (state.KeepRunning()) {
    strcat(dst.data(), src.data());
    dst[2] = '\0';
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcat_copy_only)->AT_COMMON_SIZES;

static void BM_string_strcat_seek_only(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> src(3, 'x');
  std::vector<char> dst(nbytes + 2, 'y');
  src[2] = '\0';
  dst[nbytes - 1] = '\0';

  while (state.KeepRunning()) {
    strcat(dst.data(), src.data());
    dst[nbytes - 1] = '\0';
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcat_seek_only)->AT_COMMON_SIZES;

static void BM_string_strcat_half_copy_half_seek(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> src(nbytes / 2, 'x');
  std::vector<char> dst(nbytes / 2, 'y');
  src[nbytes / 2 - 1] = '\0';
  dst[nbytes / 2 - 1] = '\0';

  while (state.KeepRunning()) {
    strcat(dst.data(), src.data());
    dst[nbytes / 2 - 1] = '\0';
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcat_half_copy_half_seek)->AT_COMMON_SIZES;

static void BM_string_strcpy(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> src(nbytes, 'x');
  std::vector<char> dst(nbytes);
  src[nbytes - 1] = '\0';

  while (state.KeepRunning()) {
    strcpy(dst.data(), src.data());
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcpy)->AT_COMMON_SIZES;

static void BM_string_strcmp(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  std::vector<char> s1(nbytes, 'x');
  std::vector<char> s2(nbytes, 'x');
  s1[nbytes - 1] = '\0';
  s2[nbytes - 1] = '\0';

  volatile int c __attribute__((unused));
  while (state.KeepRunning()) {
    c = strcmp(s1.data(), s2.data());
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcmp)->AT_COMMON_SIZES;
