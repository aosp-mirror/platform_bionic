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
#include <stdio_ext.h>

#define KB 1024
#define MB 1024*KB

#define AT_COMMON_SIZES \
    Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(512)-> \
    Arg(1*KB)->Arg(4*KB)->Arg(8*KB)->Arg(16*KB)->Arg(64*KB)

template <typename Fn>
static void ReadWriteTest(int iters, int chunk_size, Fn f, bool buffered) {
  StopBenchmarkTiming();
  FILE* fp = fopen("/dev/zero", "rw");
  __fsetlocking(fp, FSETLOCKING_BYCALLER);
  char* buf = new char[chunk_size];
  StartBenchmarkTiming();

  if (!buffered) {
    setvbuf(fp, 0, _IONBF, 0);
  }

  for (int i = 0; i < iters; ++i) {
    f(buf, chunk_size, 1, fp);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(chunk_size));
  delete[] buf;
  fclose(fp);
}

static void BM_stdio_fread(int iters, int chunk_size) {
  ReadWriteTest(iters, chunk_size, fread, true);
}
BENCHMARK(BM_stdio_fread)->AT_COMMON_SIZES;

static void BM_stdio_fwrite(int iters, int chunk_size) {
  ReadWriteTest(iters, chunk_size, fwrite, true);
}
BENCHMARK(BM_stdio_fwrite)->AT_COMMON_SIZES;

static void BM_stdio_fread_unbuffered(int iters, int chunk_size) {
  ReadWriteTest(iters, chunk_size, fread, false);
}
BENCHMARK(BM_stdio_fread_unbuffered)->AT_COMMON_SIZES;

static void BM_stdio_fwrite_unbuffered(int iters, int chunk_size) {
  ReadWriteTest(iters, chunk_size, fwrite, false);
}
BENCHMARK(BM_stdio_fwrite_unbuffered)->AT_COMMON_SIZES;

static void FopenFgetsFclose(int iters, bool no_locking) {
  char buf[1024];
  for (int i = 0; i < iters; ++i) {
    FILE* fp = fopen("/proc/version", "re");
    if (no_locking) __fsetlocking(fp, FSETLOCKING_BYCALLER);
    fgets(buf, sizeof(buf), fp);
    fclose(fp);
  }
}

static void BM_stdio_fopen_fgets_fclose_locking(int iters) {
  FopenFgetsFclose(iters, false);
}
BENCHMARK(BM_stdio_fopen_fgets_fclose_locking);

static void BM_stdio_fopen_fgets_fclose_no_locking(int iters) {
  FopenFgetsFclose(iters, true);
}
BENCHMARK(BM_stdio_fopen_fgets_fclose_no_locking);
