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

#include <stdio.h>
#include <stdio_ext.h>

#include <benchmark/Benchmark.h>

#define KB 1024
#define MB 1024*KB

#define AT_COMMON_SIZES \
    Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(512)-> \
    Arg(1*KB)->Arg(4*KB)->Arg(8*KB)->Arg(16*KB)->Arg(64*KB)

template <typename Fn>
void ReadWriteTest(::testing::Benchmark* benchmark, int iters, int chunk_size, Fn f, bool buffered) {
  benchmark->StopBenchmarkTiming();
  FILE* fp = fopen("/dev/zero", "rw");
  __fsetlocking(fp, FSETLOCKING_BYCALLER);
  char* buf = new char[chunk_size];
  benchmark->StartBenchmarkTiming();

  if (!buffered) {
    setvbuf(fp, 0, _IONBF, 0);
  }

  for (int i = 0; i < iters; ++i) {
    f(buf, chunk_size, 1, fp);
  }

  benchmark->StopBenchmarkTiming();
  benchmark->SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(chunk_size));
  delete[] buf;
  fclose(fp);
}

BENCHMARK_WITH_ARG(BM_stdio_fread, int)->AT_COMMON_SIZES;
void BM_stdio_fread::Run(int iters, int chunk_size) {
  ReadWriteTest(this, iters, chunk_size, fread, true);
}

BENCHMARK_WITH_ARG(BM_stdio_fwrite, int)->AT_COMMON_SIZES;
void BM_stdio_fwrite::Run(int iters, int chunk_size) {
  ReadWriteTest(this, iters, chunk_size, fwrite, true);
}

BENCHMARK_WITH_ARG(BM_stdio_fread_unbuffered, int)->AT_COMMON_SIZES;
void BM_stdio_fread_unbuffered::Run(int iters, int chunk_size) {
  ReadWriteTest(this, iters, chunk_size, fread, false);
}

BENCHMARK_WITH_ARG(BM_stdio_fwrite_unbuffered, int)->AT_COMMON_SIZES;
void BM_stdio_fwrite_unbuffered::Run(int iters, int chunk_size) {
  ReadWriteTest(this, iters, chunk_size, fwrite, false);
}

static void FopenFgetsFclose(int iters, bool no_locking) {
  char buf[1024];
  for (int i = 0; i < iters; ++i) {
    FILE* fp = fopen("/proc/version", "re");
    if (no_locking) __fsetlocking(fp, FSETLOCKING_BYCALLER);
    fgets(buf, sizeof(buf), fp);
    fclose(fp);
  }
}

BENCHMARK_NO_ARG(BM_stdio_fopen_fgets_fclose_locking);
void BM_stdio_fopen_fgets_fclose_locking::Run(int iters) {
  StartBenchmarkTiming();
  FopenFgetsFclose(iters, false);
  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_stdio_fopen_fgets_fclose_no_locking);
void BM_stdio_fopen_fgets_fclose_no_locking::Run(int iters) {
  StartBenchmarkTiming();
  FopenFgetsFclose(iters, true);
  StopBenchmarkTiming();
}
