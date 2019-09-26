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

#include <benchmark/benchmark.h>
#include "util.h"

static void BM_wctype_towlower_ascii_y(benchmark::State& state) {
  for (auto _ : state) {
    towlower('X');
  }
}
BIONIC_BENCHMARK(BM_wctype_towlower_ascii_y);

static void BM_wctype_towlower_ascii_n(benchmark::State& state) {
  for (auto _ : state) {
    towlower('x');
  }
}
BIONIC_BENCHMARK(BM_wctype_towlower_ascii_n);

static void BM_wctype_towlower_unicode_y(benchmark::State& state) {
  for (auto _ : state) {
    towlower(0x0391);
  }
}
BIONIC_BENCHMARK(BM_wctype_towlower_unicode_y);

static void BM_wctype_towlower_unicode_n(benchmark::State& state) {
  for (auto _ : state) {
    towlower(0x03b1);
  }
}
BIONIC_BENCHMARK(BM_wctype_towlower_unicode_n);

static void BM_wctype_towupper_ascii_y(benchmark::State& state) {
  for (auto _ : state) {
    towupper('x');
  }
}
BIONIC_BENCHMARK(BM_wctype_towupper_ascii_y);

static void BM_wctype_towupper_ascii_n(benchmark::State& state) {
  for (auto _ : state) {
    towupper('X');
  }
}
BIONIC_BENCHMARK(BM_wctype_towupper_ascii_n);

static void BM_wctype_towupper_unicode_y(benchmark::State& state) {
  for (auto _ : state) {
    towupper(0x03b1);
  }
}
BIONIC_BENCHMARK(BM_wctype_towupper_unicode_y);

static void BM_wctype_towupper_unicode_n(benchmark::State& state) {
  for (auto _ : state) {
    towupper(0x0391);
  }
}
BIONIC_BENCHMARK(BM_wctype_towupper_unicode_n);
