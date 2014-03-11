/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <stdio.h>

#define KB 1024
#define MB 1024*KB

#define AT_COMMON_SIZES \
    Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(512)-> \
    Arg(1*KB)->Arg(4*KB)->Arg(8*KB)->Arg(16*KB)->Arg(64*KB)

static void BM_stdio_fread(int iters, int chunk_size) {
  StopBenchmarkTiming();
  FILE* fp = fopen("/dev/zero", "rw");
  char* buf = new char[chunk_size];
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    fread(buf, chunk_size, 1, fp);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(chunk_size));
  delete[] buf;
  fclose(fp);
}
BENCHMARK(BM_stdio_fread)->AT_COMMON_SIZES;


static void BM_stdio_fwrite(int iters, int chunk_size) {
  StopBenchmarkTiming();
  FILE* fp = fopen("/dev/zero", "rw");
  char* buf = new char[chunk_size];
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
      fwrite(buf, chunk_size, 1, fp);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(chunk_size));
  delete[] buf;
  fclose(fp);
}
BENCHMARK(BM_stdio_fwrite)->AT_COMMON_SIZES;
