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
#include "util.h"

constexpr auto KB = 1024;

// NOTE: these constants are temporary replacements for AT_COMMON_SIZES until
// the new interface for Bionic benchmarks is implemented.

// Set all four to 0 to test normal alignment.
#define AT_SRC_ALIGN 0
#define AT_DST_ALIGN 0

#define AT_ALIGNED_TWOBUF \
    Args({(8), AT_SRC_ALIGN, AT_DST_ALIGN})->Args({(64), AT_SRC_ALIGN, AT_DST_ALIGN})-> \
    Args({(512), AT_SRC_ALIGN, AT_DST_ALIGN})->Args({(1*KB), AT_SRC_ALIGN, AT_DST_ALIGN})-> \
    Args({(8*KB), AT_SRC_ALIGN, AT_DST_ALIGN})->Args({(16*KB), AT_SRC_ALIGN, AT_DST_ALIGN})-> \
    Args({(32*KB), AT_SRC_ALIGN, AT_DST_ALIGN})->Args({(64*KB), AT_SRC_ALIGN, AT_DST_ALIGN})

#define AT_ALIGNED_ONEBUF \
    Args({(8), AT_SRC_ALIGN})->Args({(64), AT_SRC_ALIGN})->Args({(512), AT_SRC_ALIGN})-> \
    Args({(1*KB), AT_SRC_ALIGN})->Args({(8*KB), AT_SRC_ALIGN})->Args({(16*KB), AT_SRC_ALIGN})-> \
    Args({(32*KB), AT_SRC_ALIGN})->Args({(64*KB), AT_SRC_ALIGN})

static void BM_string_memcmp(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t src_alignment = state.range(1);
  const size_t dst_alignment = state.range(2);

  std::vector<char> src;
  std::vector<char> dst;
  char* src_aligned = GetAlignedPtrFilled(&src, src_alignment, nbytes, 'x');
  char* dst_aligned = GetAlignedPtrFilled(&dst, dst_alignment, nbytes, 'x');

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {
    c += memcmp(dst_aligned, src_aligned, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memcmp)->AT_ALIGNED_TWOBUF;

static void BM_string_memcpy(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t src_alignment = state.range(1);
  const size_t dst_alignment = state.range(2);

  std::vector<char> src;
  std::vector<char> dst;
  char* src_aligned = GetAlignedPtrFilled(&src, src_alignment, nbytes, 'x');
  char* dst_aligned = GetAlignedPtr(&dst, dst_alignment, nbytes);

  while (state.KeepRunning()) {
    memcpy(dst_aligned, src_aligned, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memcpy)->AT_ALIGNED_TWOBUF;

static void BM_string_memmove_non_overlapping(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t src_alignment = state.range(1);
  const size_t dst_alignment = state.range(2);

  std::vector<char> src;
  std::vector<char> dst;
  char* src_aligned = GetAlignedPtrFilled(&src, src_alignment, nbytes, 'x');
  char* dst_aligned = GetAlignedPtrFilled(&dst, dst_alignment, nbytes, 'y');

  while (state.KeepRunning()) {
    memmove(dst_aligned, src_aligned, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memmove_non_overlapping)->AT_ALIGNED_TWOBUF;

static void BM_string_memmove_overlap_dst_before_src(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t alignment = state.range(1);

  std::vector<char> buf(3 * alignment + nbytes + 1, 'x');
  char* buf_aligned = GetAlignedPtrFilled(&buf, alignment, nbytes + 1, 'x');

  while (state.KeepRunning()) {
    memmove(buf_aligned, buf_aligned + 1, nbytes);  // Worst-case overlap.
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memmove_overlap_dst_before_src)->AT_ALIGNED_ONEBUF;

static void BM_string_memmove_overlap_src_before_dst(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t alignment = state.range(1);

  std::vector<char> buf;
  char* buf_aligned = GetAlignedPtrFilled(&buf, alignment, nbytes + 1, 'x');

  while (state.KeepRunning()) {
    memmove(buf_aligned + 1, buf_aligned, nbytes);  // Worst-case overlap.
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memmove_overlap_src_before_dst)->AT_ALIGNED_ONEBUF;

static void BM_string_memset(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t alignment = state.range(1);

  std::vector<char> buf;
  char* buf_aligned = GetAlignedPtr(&buf, alignment, nbytes + 1);

  while (state.KeepRunning()) {
    memset(buf_aligned, 0, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_memset)->AT_ALIGNED_ONEBUF;

static void BM_string_strlen(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t alignment = state.range(1);

  std::vector<char> buf;
  char* buf_aligned = GetAlignedPtrFilled(&buf, alignment, nbytes + 1, 'x');
  buf_aligned[nbytes - 1] = '\0';

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {
    c += strlen(buf_aligned);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strlen)->AT_ALIGNED_ONEBUF;

static void BM_string_strcat_copy_only(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t src_alignment = state.range(1);
  const size_t dst_alignment = state.range(2);

  std::vector<char> src;
  std::vector<char> dst;
  char* src_aligned = GetAlignedPtrFilled(&src, src_alignment, nbytes, 'x');
  char* dst_aligned = GetAlignedPtr(&dst, dst_alignment, nbytes + 2);
  src_aligned[nbytes - 1] = '\0';
  dst_aligned[0] = 'y';
  dst_aligned[1] = 'y';
  dst_aligned[2] = '\0';

  while (state.KeepRunning()) {
    strcat(dst_aligned, src_aligned);
    dst_aligned[2] = '\0';
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcat_copy_only)->AT_ALIGNED_TWOBUF;

static void BM_string_strcat_seek_only(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t src_alignment = state.range(1);
  const size_t dst_alignment = state.range(2);

  std::vector<char> src;
  std::vector<char> dst;
  char* src_aligned = GetAlignedPtrFilled(&src, src_alignment, 3, 'x');
  char* dst_aligned = GetAlignedPtrFilled(&dst, dst_alignment, nbytes + 2, 'y');
  src_aligned[2] = '\0';
  dst_aligned[nbytes - 1] = '\0';

  while (state.KeepRunning()) {
    strcat(dst_aligned, src_aligned);
    dst_aligned[nbytes - 1] = '\0';
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcat_seek_only)->AT_ALIGNED_TWOBUF;

static void BM_string_strcat_half_copy_half_seek(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t src_alignment = state.range(1);
  const size_t dst_alignment = state.range(2);

  std::vector<char> src;
  std::vector<char> dst;
  char* src_aligned = GetAlignedPtrFilled(&src, src_alignment, nbytes / 2, 'x');
  char* dst_aligned = GetAlignedPtrFilled(&dst, dst_alignment, nbytes, 'y');
  src_aligned[nbytes / 2 - 1] = '\0';
  dst_aligned[nbytes / 2 - 1] = '\0';

  while (state.KeepRunning()) {
    strcat(dst_aligned, src_aligned);
    dst_aligned[nbytes / 2 - 1] = '\0';
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcat_half_copy_half_seek)->AT_ALIGNED_TWOBUF;

static void BM_string_strcpy(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t src_alignment = state.range(1);
  const size_t dst_alignment = state.range(2);

  std::vector<char> src;
  std::vector<char> dst;
  char* src_aligned = GetAlignedPtrFilled(&src, src_alignment, nbytes, 'x');
  char* dst_aligned = GetAlignedPtr(&dst, dst_alignment, nbytes);
  src_aligned[nbytes - 1] = '\0';

  while (state.KeepRunning()) {
    strcpy(dst_aligned, src_aligned);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcpy)->AT_ALIGNED_TWOBUF;

static void BM_string_strcmp(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  const size_t s1_alignment = state.range(1);
  const size_t s2_alignment = state.range(2);

  std::vector<char> s1;
  std::vector<char> s2;
  char* s1_aligned = GetAlignedPtrFilled(&s1, s1_alignment, nbytes, 'x');
  char* s2_aligned = GetAlignedPtrFilled(&s2, s2_alignment, nbytes, 'x');
  s1_aligned[nbytes - 1] = '\0';
  s2_aligned[nbytes - 1] = '\0';

  volatile int c __attribute__((unused));
  while (state.KeepRunning()) {
    c = strcmp(s1_aligned, s2_aligned);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BENCHMARK(BM_string_strcmp)->AT_ALIGNED_TWOBUF;
