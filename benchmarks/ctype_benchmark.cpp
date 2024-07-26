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

#include <ctype.h>

#include <array>
#include <numeric>
#include <random>

#include <benchmark/benchmark.h>
#include "util.h"

static std::array<int, 128> RandomAscii() {
  std::array<int, 128> result;
  std::iota(result.begin(), result.end(), 0);
  std::shuffle(result.begin(), result.end(), std::mt19937{std::random_device{}()});
  return result;
}

#define CTYPE_BENCHMARK(__benchmark, fn)                        \
  static void __benchmark##_##fn(benchmark::State& state) {     \
    auto chars = RandomAscii();                                 \
    for (auto _ : state) {                                      \
      for (char ch : chars) {                                   \
        benchmark::DoNotOptimize(fn(ch));                       \
      }                                                         \
    }                                                           \
    state.SetBytesProcessed(state.iterations() * chars.size()); \
  }                                                             \
  BIONIC_BENCHMARK(__benchmark##_##fn)

CTYPE_BENCHMARK(BM_ctype, isalpha);
CTYPE_BENCHMARK(BM_ctype, isalnum);
CTYPE_BENCHMARK(BM_ctype, isascii);
CTYPE_BENCHMARK(BM_ctype, isblank);
CTYPE_BENCHMARK(BM_ctype, iscntrl);
CTYPE_BENCHMARK(BM_ctype, isgraph);
CTYPE_BENCHMARK(BM_ctype, islower);
CTYPE_BENCHMARK(BM_ctype, isprint);
CTYPE_BENCHMARK(BM_ctype, ispunct);
CTYPE_BENCHMARK(BM_ctype, isspace);
CTYPE_BENCHMARK(BM_ctype, isupper);
CTYPE_BENCHMARK(BM_ctype, isxdigit);

CTYPE_BENCHMARK(BM_ctype, toascii);
CTYPE_BENCHMARK(BM_ctype, tolower);
CTYPE_BENCHMARK(BM_ctype, _tolower);
CTYPE_BENCHMARK(BM_ctype, toupper);
CTYPE_BENCHMARK(BM_ctype, _toupper);
