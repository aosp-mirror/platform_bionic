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

#include <benchmark/Benchmark.h>

#define AT_COMMON_VALS \
    Arg(1234.0)->Arg(nan(""))->Arg(HUGE_VAL)->Arg(0.0)

// Avoid optimization.
volatile double d;
volatile double v;

BENCHMARK_NO_ARG(BM_math_sqrt);
void BM_math_sqrt::Run(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 2.0;
  for (int i = 0; i < iters; ++i) {
    d += sqrt(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_math_log10);
void BM_math_log10::Run(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 1234.0;
  for (int i = 0; i < iters; ++i) {
    d += log10(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_math_logb);
void BM_math_logb::Run(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 1234.0;
  for (int i = 0; i < iters; ++i) {
    d += logb(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_isfinite_macro, double)->AT_COMMON_VALS;
void BM_math_isfinite_macro::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += isfinite(v);
  }

  StopBenchmarkTiming();
}

#if defined(__BIONIC__)
#define test_isfinite __isfinite
#else
#define test_isfinite __finite
#endif
BENCHMARK_WITH_ARG(BM_math_isfinite, double)->AT_COMMON_VALS;
void BM_math_isfinite::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += test_isfinite(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_isinf_macro, double)->AT_COMMON_VALS;
void BM_math_isinf_macro::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += isinf(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_isinf, double)->AT_COMMON_VALS;
void BM_math_isinf::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += (isinf)(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_isnan_macro, double)->AT_COMMON_VALS;
void BM_math_isnan_macro::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += isnan(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_isnan, double)->AT_COMMON_VALS;
void BM_math_isnan::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += (isnan)(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_isnormal_macro, double)->AT_COMMON_VALS;
void BM_math_isnormal_macro::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += isnormal(v);
  }

  StopBenchmarkTiming();
}

#if defined(__BIONIC__)
BENCHMARK_WITH_ARG(BM_math_isnormal, double)->AT_COMMON_VALS;
void BM_math_isnormal::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += (__isnormal)(v);
  }

  StopBenchmarkTiming();
}
#endif

BENCHMARK_NO_ARG(BM_math_sin_fast);
void BM_math_sin_fast::Run(int iters) {
  StartBenchmarkTiming();

  d = 1.0;
  for (int i = 0; i < iters; ++i) {
    d += sin(d);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_math_sin_feupdateenv);
void BM_math_sin_feupdateenv::Run(int iters) {
  StartBenchmarkTiming();

  d = 1.0;
  for (int i = 0; i < iters; ++i) {
    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    feupdateenv(&__libc_save_rm);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_math_sin_fesetenv);
void BM_math_sin_fesetenv::Run(int iters) {
  StartBenchmarkTiming();

  d = 1.0;
  for (int i = 0; i < iters; ++i) {
    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    fesetenv(&__libc_save_rm);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_fpclassify, double)->AT_COMMON_VALS;
void BM_math_fpclassify::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += fpclassify(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_signbit_macro, double)->AT_COMMON_VALS;
void BM_math_signbit_macro::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += signbit(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_signbit, double)->AT_COMMON_VALS;
void BM_math_signbit::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += (__signbit)(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_fabs_macro, double)->AT_COMMON_VALS;
void BM_math_fabs_macro::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += fabs(v);
  }

  StopBenchmarkTiming();
}

BENCHMARK_WITH_ARG(BM_math_fabs, double)->AT_COMMON_VALS;
void BM_math_fabs::Run(int iters, double value) {
  StartBenchmarkTiming();

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
    d += (fabs)(v);
  }

  StopBenchmarkTiming();
}
