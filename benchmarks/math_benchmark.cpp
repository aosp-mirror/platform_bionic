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

#include <fenv.h>
#include <math.h>

#include <benchmark/benchmark.h>

static const double values[] = { 1234.0, nan(""), HUGE_VAL, 0.0 };
static const char* names[] = { "1234.0", "nan", "HUGE_VAL", "0.0" };

#define BENCHMARK_COMMON_VALS(name) BENCHMARK(name)->Arg(0)->Arg(1)->Arg(2)->Arg(3)

static void SetLabel(benchmark::State& state) {
  state.SetLabel(names[state.range(0)]);
}

// Avoid optimization.
volatile double d;
volatile double v;

static void BM_math_sqrt(benchmark::State& state) {
  d = 0.0;
  v = 2.0;
  while (state.KeepRunning()) {
    d += sqrt(v);
  }
}
BENCHMARK(BM_math_sqrt);

static void BM_math_log10(benchmark::State& state) {
  d = 0.0;
  v = 1234.0;
  while (state.KeepRunning()) {
    d += log10(v);
  }
}
BENCHMARK(BM_math_log10);

static void BM_math_logb(benchmark::State& state) {
  d = 0.0;
  v = 1234.0;
  while (state.KeepRunning()) {
    d += logb(v);
  }
}
BENCHMARK(BM_math_logb);

static void BM_math_isfinite_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isfinite(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isfinite_macro);

#if defined(__BIONIC__)
#define test_isfinite __isfinite
#else
#define test_isfinite __finite
#endif
static void BM_math_isfinite(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += test_isfinite(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isfinite);

static void BM_math_isinf_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isinf(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isinf_macro);

static void BM_math_isinf(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (isinf)(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isinf);

static void BM_math_isnan_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isnan(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isnan_macro);

static void BM_math_isnan(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (isnan)(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isnan);

static void BM_math_isnormal_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isnormal(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isnormal_macro);

#if defined(__BIONIC__)
static void BM_math_isnormal(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (__isnormal)(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_isnormal);
#endif

static void BM_math_sin_fast(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    d += sin(d);
  }
}
BENCHMARK(BM_math_sin_fast);

static void BM_math_sin_feupdateenv(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    feupdateenv(&__libc_save_rm);
  }
}
BENCHMARK(BM_math_sin_feupdateenv);

static void BM_math_sin_fesetenv(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    fesetenv(&__libc_save_rm);
  }
}
BENCHMARK(BM_math_sin_fesetenv);

static void BM_math_fpclassify(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += fpclassify(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_fpclassify);

static void BM_math_signbit_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += signbit(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_signbit_macro);

static void BM_math_signbit(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (__signbit)(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_signbit);

static void BM_math_fabs_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += fabs(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_fabs_macro);

static void BM_math_fabs(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (fabs)(v);
  }
  SetLabel(state);
}
BENCHMARK_COMMON_VALS(BM_math_fabs);
