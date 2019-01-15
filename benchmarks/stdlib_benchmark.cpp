/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <err.h>
#include <langinfo.h>
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>

#include <benchmark/benchmark.h>
#include "util.h"

static void BM_stdlib_malloc_free(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  int pagesize = getpagesize();

  void* ptr;
  for (auto _ : state) {
    ptr = malloc(nbytes);
    // Make the entire allocation resident.
    uint8_t* data = reinterpret_cast<uint8_t*>(ptr);
    for (size_t i = 0; i < nbytes; i += pagesize) {
      data[i] = 1;
    }
    free(ptr);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_malloc_free, "AT_COMMON_SIZES");

static void BM_stdlib_mbstowcs(benchmark::State& state) {
  const size_t buf_alignment = state.range(0);
  const size_t widebuf_alignment = state.range(1);

  std::vector<char> buf;
  std::vector<wchar_t> widebuf;

  setlocale(LC_CTYPE, "C.UTF-8")
  || setlocale(LC_CTYPE, "en_US.UTF-8")
  || setlocale(LC_CTYPE, "en_GB.UTF-8")
  || setlocale(LC_CTYPE, "en.UTF-8")
  || setlocale(LC_CTYPE, "de_DE-8")
  || setlocale(LC_CTYPE, "fr_FR-8");
  if (strcmp(nl_langinfo(CODESET), "UTF-8")) {
    errx(1, "ERROR: unable to set locale in BM_stdlib_mbstowcs");
  }

  char* buf_aligned = GetAlignedPtr(&buf, buf_alignment, 500000);
  wchar_t* widebuf_aligned = GetAlignedPtr(&widebuf, widebuf_alignment, 500000);
  size_t i, j, k, l;
  l = 0;
  for (i=0xc3; i<0xe0; i++)
    for (j=0x80; j<0xc0; j++)
      buf[l++] = i, buf[l++] = j;
  for (i=0xe1; i<0xed; i++)
    for (j=0x80; j<0xc0; j++)
      for (k=0x80; k<0xc0; k++)
        buf[l++] = i, buf[l++] = j, buf[l++] = k;
  for (i=0xf1; i<0xf4; i++)
    for (j=0x80; j<0xc0; j++)
      for (k=0x80; k<0xc0; k++)
        buf[l++] = i, buf[l++] = j, buf[l++] = 0x80, buf[l++] = k;
  buf[l++] = 0;

  volatile size_t c __attribute__((unused)) = 0;
  for (auto _ : state) {
    c = mbstowcs(widebuf_aligned, buf_aligned, 500000);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(500000));
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbstowcs, "0 0");

static void BM_stdlib_mbrtowc(benchmark::State& state) {
  const size_t buf_alignment = state.range(0);

  std::vector<char> buf;

  setlocale(LC_CTYPE, "C.UTF-8")
  || setlocale(LC_CTYPE, "en_US.UTF-8")
  || setlocale(LC_CTYPE, "en_GB.UTF-8")
  || setlocale(LC_CTYPE, "en.UTF-8")
  || setlocale(LC_CTYPE, "de_DE-8")
  || setlocale(LC_CTYPE, "fr_FR-8");
  if (strcmp(nl_langinfo(CODESET), "UTF-8")) {
    errx(1, "ERROR: unable to set locale in BM_stdlib_mbrtowc");
  }

  char* buf_aligned = GetAlignedPtr(&buf, buf_alignment, 500000);
  size_t i, j, k, l;
  l = 0;
  for (i=0xc3; i<0xe0; i++)
    for (j=0x80; j<0xc0; j++)
      buf[l++] = i, buf[l++] = j;
  for (i=0xe1; i<0xed; i++)
    for (j=0x80; j<0xc0; j++)
      for (k=0x80; k<0xc0; k++)
        buf[l++] = i, buf[l++] = j, buf[l++] = k;
  for (i=0xf1; i<0xf4; i++)
    for (j=0x80; j<0xc0; j++)
      for (k=0x80; k<0xc0; k++)
        buf[l++] = i, buf[l++] = j, buf[l++] = 0x80, buf[l++] = k;
  buf[l++] = 0;

  wchar_t wc = 0;
  for (auto _ : state) {
    for (j = 0; buf_aligned[j]; j+=mbrtowc(&wc, buf_aligned + j, 4, nullptr)) {
    }
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(500000));
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbrtowc, "0");

void BM_stdlib_atoi(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(atoi(" -123"));
  }
}
BIONIC_BENCHMARK(BM_stdlib_atoi);

void BM_stdlib_atol(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(atol(" -123"));
  }
}
BIONIC_BENCHMARK(BM_stdlib_atol);

void BM_stdlib_strtol(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(strtol(" -123", nullptr, 0));
  }
}
BIONIC_BENCHMARK(BM_stdlib_strtol);

void BM_stdlib_strtoll(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(strtoll(" -123", nullptr, 0));
  }
}
BIONIC_BENCHMARK(BM_stdlib_strtoll);

void BM_stdlib_strtoul(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(strtoul(" -123", nullptr, 0));
  }
}
BIONIC_BENCHMARK(BM_stdlib_strtoul);

void BM_stdlib_strtoull(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(strtoull(" -123", nullptr, 0));
  }
}
BIONIC_BENCHMARK(BM_stdlib_strtoull);
