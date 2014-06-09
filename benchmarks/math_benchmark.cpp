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

#include "benchmark.h"

#include <fenv.h>
#include <math.h>

// Avoid optimization.
double d;
double v;

static void BM_math_sqrt(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 2.0;
  for (int i = 0; i < iters; ++i) {
    d += sqrt(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_sqrt);

static void BM_math_log10(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 1234.0;
  for (int i = 0; i < iters; ++i) {
    d += log10(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_log10);

static void BM_math_logb(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 1234.0;
  for (int i = 0; i < iters; ++i) {
    d += logb(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_logb);

static void BM_math_isinf_NORMAL(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 1234.0; // FP_NORMAL
  for (int i = 0; i < iters; ++i) {
    d += (isinf)(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_isinf_NORMAL);

static void BM_math_isinf_NAN(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = nan(""); // FP_NAN
  for (int i = 0; i < iters; ++i) {
    d += (isinf)(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_isinf_NAN);

static void BM_math_isinf_INFINITE(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = HUGE_VAL; // FP_INFINITE
  for (int i = 0; i < iters; ++i) {
    d += (isinf)(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_isinf_INFINITE);

static void BM_math_isinf_ZERO(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 0.0; // FP_ZERO
  for (int i = 0; i < iters; ++i) {
    d += (isinf)(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_isinf_ZERO);

static void BM_math_sin_fast(int iters) {
  StartBenchmarkTiming();

  d = 1.0;
  for (int i = 0; i < iters; ++i) {
    d += sin(d);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_sin_fast);

static void BM_math_sin_feupdateenv(int iters) {
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
BENCHMARK(BM_math_sin_feupdateenv);

static void BM_math_sin_fesetenv(int iters) {
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
BENCHMARK(BM_math_sin_fesetenv);

static void BM_math_fpclassify_NORMAL(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 1234.0; // FP_NORMAL
  for (int i = 0; i < iters; ++i) {
    d += fpclassify(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_fpclassify_NORMAL);

static void BM_math_fpclassify_NAN(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = nan(""); // FP_NAN
  for (int i = 0; i < iters; ++i) {
    d += fpclassify(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_fpclassify_NAN);

static void BM_math_fpclassify_INFINITE(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = HUGE_VAL; // FP_INFINITE
  for (int i = 0; i < iters; ++i) {
    d += fpclassify(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_fpclassify_INFINITE);

static void BM_math_fpclassify_ZERO(int iters) {
  StartBenchmarkTiming();

  d = 0.0;
  v = 0.0; // FP_ZERO
  for (int i = 0; i < iters; ++i) {
    d += fpclassify(v);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_math_fpclassify_ZERO);
