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
