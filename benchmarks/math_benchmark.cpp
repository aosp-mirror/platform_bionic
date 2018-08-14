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
#include "util.h"

static const double values[] = { 1234.0, nan(""), HUGE_VAL, 0.0 };
static const char* names[] = { "1234.0", "nan", "HUGE_VAL", "0.0" };

static void SetLabel(benchmark::State& state) {
  state.SetLabel(names[state.range(0)]);
}

// Avoid optimization.
volatile double d;
volatile double v;
volatile float f;

static float zero = 0.0f;
static double zerod = 0.0f;

static void BM_math_sqrt(benchmark::State& state) {
  d = 0.0;
  v = 2.0;
  while (state.KeepRunning()) {
    d += sqrt(v);
  }
}
BIONIC_BENCHMARK(BM_math_sqrt);

static void BM_math_log10(benchmark::State& state) {
  d = 0.0;
  v = 1234.0;
  while (state.KeepRunning()) {
    d += log10(v);
  }
}
BIONIC_BENCHMARK(BM_math_log10);

static void BM_math_logb(benchmark::State& state) {
  d = 0.0;
  v = 1234.0;
  while (state.KeepRunning()) {
    d += logb(v);
  }
}
BIONIC_BENCHMARK(BM_math_logb);

static void BM_math_isfinite_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isfinite(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isfinite_macro, "MATH_COMMON");

static void BM_math_isfinite(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isfinite(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isfinite, "MATH_COMMON");

static void BM_math_isinf_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isinf(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isinf_macro, "MATH_COMMON");

static void BM_math_isinf(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (isinf)(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isinf, "MATH_COMMON");

static void BM_math_isnan_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isnan(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isnan_macro, "MATH_COMMON");

static void BM_math_isnan(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (isnan)(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isnan, "MATH_COMMON");

static void BM_math_isnormal_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isnormal(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isnormal_macro, "MATH_COMMON");

static void BM_math_isnormal(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isnormal(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_isnormal, "MATH_COMMON");

static void BM_math_sin_fast(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    d += sin(d);
  }
}
BIONIC_BENCHMARK(BM_math_sin_fast);

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
BIONIC_BENCHMARK(BM_math_sin_feupdateenv);

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
BIONIC_BENCHMARK(BM_math_sin_fesetenv);

static void BM_math_fpclassify(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += fpclassify(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_fpclassify, "MATH_COMMON");

static void BM_math_signbit_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += signbit(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_signbit_macro, "MATH_COMMON");

static void BM_math_signbit(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += signbit(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_signbit, "MATH_COMMON");

static void BM_math_fabs_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += fabs(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_fabs_macro, "MATH_COMMON");

static void BM_math_fabs(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (fabs)(v);
  }
  SetLabel(state);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_fabs, "MATH_COMMON");

static void BM_math_sincos(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    double s, c;
    sincos(d, &s, &c);
    d += s + c;
  }
}
BIONIC_BENCHMARK(BM_math_sincos);

#include "expf_input.cpp"

static void BM_math_expf_speccpu2017(benchmark::State& state) {
  f = 0.0;
  auto cin = expf_input.cbegin();
  for (auto _ : state) {
    f = expf(*cin);
    if (++cin == expf_input.cend())
      cin = expf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_expf_speccpu2017);

static void BM_math_expf_speccpu2017_latency(benchmark::State& state) {
  f = 0.0;
  auto cin = expf_input.cbegin();
  for (auto _ : state) {
    f = expf(f * zero + *cin);
    if (++cin == expf_input.cend())
      cin = expf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_expf_speccpu2017_latency);

// Create a double version of expf_input to avoid overhead of float to
// double conversion.
static const std::vector<double> exp_input (expf_input.begin(),
                                            expf_input.end());

static void BM_math_exp_speccpu2017(benchmark::State& state) {
  d = 0.0;
  auto cin = exp_input.cbegin();
  for (auto _ : state) {
    d = exp(*cin);
    if (++cin == exp_input.cend())
      cin = exp_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_exp_speccpu2017);

static void BM_math_exp_speccpu2017_latency(benchmark::State& state) {
  d = 0.0;
  auto cin = exp_input.cbegin();
  for (auto _ : state) {
    d = exp(d * zerod + *cin);
    if (++cin == exp_input.cend())
      cin = exp_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_exp_speccpu2017_latency);

static void BM_math_exp2f_speccpu2017(benchmark::State& state) {
  f = 0.0;
  auto cin = expf_input.cbegin();
  for (auto _ : state) {
    f = exp2f(*cin);
    if (++cin == expf_input.cend())
      cin = expf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_exp2f_speccpu2017);

static void BM_math_exp2f_speccpu2017_latency(benchmark::State& state) {
  f = 0.0;
  auto cin = expf_input.cbegin();
  for (auto _ : state) {
    f = exp2f(f * zero + *cin);
    if (++cin == expf_input.cend())
      cin = expf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_exp2f_speccpu2017_latency);

static void BM_math_exp2_speccpu2017(benchmark::State& state) {
  d = 0.0;
  auto cin = exp_input.cbegin();
  for (auto _ : state) {
    f = exp2(*cin);
    if (++cin == exp_input.cend())
      cin = exp_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_exp2_speccpu2017);

static void BM_math_exp2_speccpu2017_latency(benchmark::State& state) {
  d = 0.0;
  auto cin = exp_input.cbegin();
  for (auto _ : state) {
    f = exp2(d * zero + *cin);
    if (++cin == exp_input.cend())
      cin = exp_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_exp2_speccpu2017_latency);

#include "powf_input.cpp"

static const std::vector<std::pair<double, double>> pow_input
  (powf_input.begin(), powf_input.end());

static void BM_math_powf_speccpu2006(benchmark::State& state) {
  f = 0.0;
  auto cin = powf_input.cbegin();
  for (auto _ : state) {
    f = powf(cin->first, cin->second);
    if (++cin == powf_input.cend())
      cin = powf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_powf_speccpu2006);

static void BM_math_powf_speccpu2017_latency(benchmark::State& state) {
  f = 0.0;
  auto cin = powf_input.cbegin();
  for (auto _ : state) {
    f = powf(f * zero + cin->first, cin->second);
    if (++cin == powf_input.cend())
      cin = powf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_powf_speccpu2017_latency);

static void BM_math_pow_speccpu2006(benchmark::State& state) {
  d = 0.0;
  auto cin = pow_input.cbegin();
  for (auto _ : state) {
    f = pow(cin->first, cin->second);
    if (++cin == pow_input.cend())
      cin = pow_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_pow_speccpu2006);

static void BM_math_pow_speccpu2017_latency(benchmark::State& state) {
  d = 0.0;
  auto cin = pow_input.cbegin();
  for (auto _ : state) {
    d = powf(d * zero + cin->first, cin->second);
    if (++cin == pow_input.cend())
      cin = pow_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_pow_speccpu2017_latency);

#include "logf_input.cpp"

static const std::vector<double> log_input (logf_input.begin(),
                                            logf_input.end());

static void BM_math_logf_speccpu2017(benchmark::State& state) {
  f = 0.0;
  auto cin = logf_input.cbegin();
  for (auto _ : state) {
    f = logf(*cin);
    if (++cin == logf_input.cend())
      cin = logf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_logf_speccpu2017);

static void BM_math_logf_speccpu2017_latency(benchmark::State& state) {
  f = 0.0;
  auto cin = logf_input.cbegin();
  for (auto _ : state) {
    f = logf(f * zero + *cin);
    if (++cin == logf_input.cend())
      cin = logf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_logf_speccpu2017_latency);

static void BM_math_log_speccpu2017(benchmark::State& state) {
  d = 0.0;
  auto cin = log_input.cbegin();
  for (auto _ : state) {
    d = log(*cin);
    if (++cin == log_input.cend())
      cin = log_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_log_speccpu2017);

static void BM_math_log_speccpu2017_latency(benchmark::State& state) {
  d = 0.0;
  auto cin = log_input.cbegin();
  for (auto _ : state) {
    d = log(d * zerod + *cin);
    if (++cin == log_input.cend())
      cin = log_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_log_speccpu2017_latency);

static void BM_math_log2f_speccpu2017(benchmark::State& state) {
  f = 0.0;
  auto cin = logf_input.cbegin();
  for (auto _ : state) {
    f = log2f(*cin);
    if (++cin == logf_input.cend())
      cin = logf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_log2f_speccpu2017);

static void BM_math_log2_speccpu2017_latency(benchmark::State& state) {
  d = 0.0;
  auto cin = log_input.cbegin();
  for (auto _ : state) {
    d = log2(d * zerod + *cin);
    if (++cin == log_input.cend())
      cin = log_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_log2_speccpu2017_latency);

static void BM_math_log2_speccpu2017(benchmark::State& state) {
  d = 0.0;
  auto cin = log_input.cbegin();
  for (auto _ : state) {
    d = log2(*cin);
    if (++cin == log_input.cend())
      cin = log_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_log2_speccpu2017);

static void BM_math_log2f_speccpu2017_latency(benchmark::State& state) {
  f = 0.0;
  auto cin = logf_input.cbegin();
  for (auto _ : state) {
    f = log2f(f * zero + *cin);
    if (++cin == logf_input.cend())
      cin = logf_input.cbegin();
  }
}
BIONIC_BENCHMARK(BM_math_log2f_speccpu2017_latency);

// Four ranges of values are checked:
// * 0.0 <= x < 0.1
// * 0.1 <= x < 0.7
// * 0.7 <= x < 3.1
// * -3.1 <= x < 3.1
// * 3.3 <= x < 33.3
// * 100.0 <= x < 1000.0
// * 1e6 <= x < 1e32
// * 1e32 < x < FLT_MAX

#include "sincosf_input.cpp"

static void BM_math_sinf(benchmark::State& state) {
  auto range = sincosf_input[state.range(0)];
  auto cin = range.values.cbegin();
  f = 0.0;
  for (auto _ : state) {
    f = sinf(*cin);
    if (++cin == range.values.cend())
      cin = range.values.cbegin();
  }
  state.SetLabel(range.label);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_sinf, "MATH_SINCOS_COMMON");

static void BM_math_sinf_latency(benchmark::State& state) {
  auto range = sincosf_input[state.range(0)];
  auto cin = range.values.cbegin();
  f = 0.0;
  for (auto _ : state) {
    f = sinf(f * zero + *cin);
    if (++cin == range.values.cend())
      cin = range.values.cbegin();
  }
  state.SetLabel(range.label);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_sinf_latency, "MATH_SINCOS_COMMON");

static void BM_math_cosf(benchmark::State& state) {
  auto range = sincosf_input[state.range(0)];
  auto cin = range.values.cbegin();
  f = 0.0;
  for (auto _ : state) {
    f = cosf(*cin);
    if (++cin == range.values.cend())
      cin = range.values.cbegin();
  }
  state.SetLabel(range.label);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_cosf, "MATH_SINCOS_COMMON");

static void BM_math_cosf_latency(benchmark::State& state) {
  auto range = sincosf_input[state.range(0)];
  auto cin = range.values.cbegin();
  f = 0.0;
  for (auto _ : state) {
    f = cosf(f * zero + *cin);
    if (++cin == range.values.cend())
      cin = range.values.cbegin();
  }
  state.SetLabel(range.label);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_cosf_latency, "MATH_SINCOS_COMMON");

static void BM_math_sincosf(benchmark::State& state) {
  auto range = sincosf_input[state.range(0)];
  auto cin = range.values.cbegin();
  f = 0.0;
  for (auto _ : state) {
    float s, c;
    sincosf(*cin, &s, &c);
    f += s;
    if (++cin == range.values.cend())
      cin = range.values.cbegin();
  }
  state.SetLabel(range.label);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_sincosf, "MATH_SINCOS_COMMON");

static void BM_math_sincosf_latency(benchmark::State& state) {
  auto range = sincosf_input[state.range(0)];
  auto cin = range.values.cbegin();
  f = 0.0;
  for (auto _ : state) {
    float s, c;
    sincosf(f * zero + *cin, &s, &c);
    f += s;
    if (++cin == range.values.cend())
      cin = range.values.cbegin();
  }
  state.SetLabel(range.label);
}
BIONIC_BENCHMARK_WITH_ARG(BM_math_sincosf_latency, "MATH_SINCOS_COMMON");
