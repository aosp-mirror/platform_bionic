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
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

#include <benchmark/benchmark.h>
#include "ScopedDecayTimeRestorer.h"
#include "util.h"

static void MallocFree(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  int pagesize = getpagesize();

  for (auto _ : state) {
    void* ptr;
    benchmark::DoNotOptimize(ptr = malloc(nbytes));
    MakeAllocationResident(ptr, nbytes, pagesize);
    free(ptr);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void BM_stdlib_malloc_free_default(benchmark::State& state) {
#if defined(__BIONIC__)
  ScopedDecayTimeRestorer restorer;

  // The default is expected to be a zero decay time.
  mallopt(M_DECAY_TIME, 0);
#endif

  MallocFree(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_malloc_free_default, "AT_COMMON_SIZES");

#if defined(__BIONIC__)
static void BM_stdlib_malloc_free_decay1(benchmark::State& state) {
  ScopedDecayTimeRestorer restorer;

  mallopt(M_DECAY_TIME, 1);

  MallocFree(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_malloc_free_decay1, "AT_COMMON_SIZES");
#endif

static void CallocFree(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  int pagesize = getpagesize();

  for (auto _ : state) {
    void* ptr;
    benchmark::DoNotOptimize(ptr = calloc(1, nbytes));
    MakeAllocationResident(ptr, nbytes, pagesize);
    free(ptr);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void BM_stdlib_calloc_free_default(benchmark::State& state) {
#if defined(__BIONIC__)
  ScopedDecayTimeRestorer restorer;

  // The default is expected to be a zero decay time.
  mallopt(M_DECAY_TIME, 0);
#endif

  CallocFree(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_calloc_free_default, "AT_COMMON_SIZES");

#if defined(__BIONIC__)
static void BM_stdlib_calloc_free_decay1(benchmark::State& state) {
  mallopt(M_DECAY_TIME, 1);

  CallocFree(state);

  mallopt(M_DECAY_TIME, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_calloc_free_decay1, "AT_COMMON_SIZES");
#endif

static void MallocMultiple(benchmark::State& state, size_t nbytes, size_t numAllocs) {
  int pagesize = getpagesize();
  void* ptrs[numAllocs];
  for (auto _ : state) {
    for (size_t i = 0; i < numAllocs; i++) {
      benchmark::DoNotOptimize(ptrs[i] = reinterpret_cast<uint8_t*>(malloc(nbytes)));
      MakeAllocationResident(ptrs[i], nbytes, pagesize);
    }
    state.PauseTiming(); // Stop timers while freeing pointers.
    for (size_t i = 0; i < numAllocs; i++) {
      free(ptrs[i]);
    }
    state.ResumeTiming();
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes) * numAllocs);
}

void BM_stdlib_malloc_forty_default(benchmark::State& state) {
#if defined(__BIONIC__)
  ScopedDecayTimeRestorer restorer;

  // The default is expected to be a zero decay time.
  mallopt(M_DECAY_TIME, 0);
#endif

  MallocMultiple(state, state.range(0), 40);
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_malloc_forty_default, "AT_COMMON_SIZES");

#if defined(__BIONIC__)
void BM_stdlib_malloc_forty_decay1(benchmark::State& state) {
  ScopedDecayTimeRestorer restorer;

  mallopt(M_DECAY_TIME, 1);

  MallocMultiple(state, state.range(0), 40);
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_malloc_forty_decay1, "AT_COMMON_SIZES");
#endif

void BM_stdlib_malloc_multiple_8192_allocs_default(benchmark::State& state) {
#if defined(__BIONIC__)
  ScopedDecayTimeRestorer restorer;

  // The default is expected to be a zero decay time.
  mallopt(M_DECAY_TIME, 0);
#endif

  MallocMultiple(state, 8192, state.range(0));
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_malloc_multiple_8192_allocs_default, "AT_SMALL_SIZES");

#if defined(__BIONIC__)
void BM_stdlib_malloc_multiple_8192_allocs_decay1(benchmark::State& state) {
  ScopedDecayTimeRestorer restorer;

  mallopt(M_DECAY_TIME, 1);

  MallocMultiple(state, 8192, state.range(0));
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_malloc_multiple_8192_allocs_decay1, "AT_SMALL_SIZES");
#endif

static void BM_stdlib_mbstowcs_ascii(benchmark::State& state) {
  // It doesn't really matter what ASCII character we pick.
  // The flow through the fast path is the same regardless.
  const size_t count = 500000;
  std::vector<char> mbs(count, 'e');
  std::vector<wchar_t> wcs(count);

  for (auto _ : state) {
    benchmark::DoNotOptimize(mbstowcs(&wcs[0], &mbs[0], wcs.size()));
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(wcs.size()));
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbstowcs_ascii, "");

static void BM_stdlib_mbstowcs_wide(benchmark::State& state) {
  // It doesn't matter much what wide character we pick.
  // A three-byte character seems pretty representative, and all three byte
  // characters are the same from the code's perspective.
  const size_t count = 500000;
  std::string mbs;
  for (size_t i = 0; i < count; i++) {
    mbs += "\xe5\xb1\xb1";
  }
  std::vector<wchar_t> wcs(count);

  for (auto _ : state) {
    benchmark::DoNotOptimize(mbstowcs(&wcs[0], &mbs[0], wcs.size()));
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(wcs.size()));
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbstowcs_wide, "");

static void BM_stdlib_mbrtowc_1(benchmark::State& state) {
  wchar_t wc;
  for (auto _ : state) {
    benchmark::DoNotOptimize(mbrtowc(&wc, "e", 1, nullptr));
  }
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbrtowc_1, "");

static void BM_stdlib_mbrtowc_2(benchmark::State& state) {
  wchar_t wc;
  for (auto _ : state) {
    benchmark::DoNotOptimize(mbrtowc(&wc, "\xc3\x9f", 3, nullptr));
  }
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbrtowc_2, "");

static void BM_stdlib_mbrtowc_3(benchmark::State& state) {
  wchar_t wc;
  for (auto _ : state) {
    benchmark::DoNotOptimize(mbrtowc(&wc, "\xe5\xb1\xb1", 3, nullptr));
  }
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbrtowc_3, "");

static void BM_stdlib_mbrtowc_4(benchmark::State& state) {
  wchar_t wc;
  for (auto _ : state) {
    benchmark::DoNotOptimize(mbrtowc(&wc, "\xf0\xa4\xad\xa2", 4, nullptr));
  }
}
BIONIC_BENCHMARK_WITH_ARG(BM_stdlib_mbrtowc_4, "");

BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_atoi, atoi(" -123"));
BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_atol, atol(" -123"));
BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_strtol, strtol(" -123", nullptr, 0));
BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_strtoll, strtoll(" -123", nullptr, 0));
BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_strtoul, strtoul(" -123", nullptr, 0));
BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_strtoull, strtoull(" -123", nullptr, 0));

BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_strtol_hex, strtol("0xdeadbeef", nullptr, 0));
BIONIC_TRIVIAL_BENCHMARK(BM_stdlib_strtoul_hex, strtoul("0xdeadbeef", nullptr, 0));
