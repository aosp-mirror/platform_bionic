/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <wctype.h>

#include <numeric>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>
#include "util.h"

static std::vector<wint_t> RandomAscii() {
  std::vector<wint_t> result(128);
  std::iota(result.begin(), result.end(), 0);
  std::shuffle(result.begin(), result.end(), std::mt19937{std::random_device{}()});
  return result;
}

static std::vector<wint_t> RandomNonAscii() {
  std::vector<wint_t> result;
  std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<> d(0x80, 0xffff);
  for (size_t i = 0; i < 128; i++) result.push_back(d(rng));
  return result;
}

#define WCTYPE_BENCHMARK(__benchmark, fn, random_fn)            \
  static void __benchmark##_##fn(benchmark::State& state) {     \
    auto chars = random_fn();                                   \
    for (auto _ : state) {                                      \
      for (char ch : chars) {                                   \
        benchmark::DoNotOptimize(fn(ch));                       \
      }                                                         \
    }                                                           \
    state.SetBytesProcessed(state.iterations() * chars.size()); \
  }                                                             \
  BIONIC_BENCHMARK(__benchmark##_##fn)

#define WCTYPE_BENCHMARK_ASCII(__benchmark, fn) WCTYPE_BENCHMARK(__benchmark, fn, RandomAscii)

#define WCTYPE_BENCHMARK_NON_ASCII(__benchmark, fn) \
  WCTYPE_BENCHMARK(__benchmark, fn, RandomNonAscii)

WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswalnum);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswalpha);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswblank);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswcntrl);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswdigit);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswgraph);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswlower);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswprint);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswpunct);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswspace);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswupper);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii, iswxdigit);

WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii_transform, towlower);
WCTYPE_BENCHMARK_ASCII(BM_wctype_ascii_transform, towupper);

WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswalnum);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswalpha);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswblank);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswcntrl);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswdigit);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswgraph);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswlower);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswprint);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswpunct);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswspace);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswupper);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii, iswxdigit);

WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii_transform, towlower);
WCTYPE_BENCHMARK_NON_ASCII(BM_wctype_non_ascii_transform, towupper);
