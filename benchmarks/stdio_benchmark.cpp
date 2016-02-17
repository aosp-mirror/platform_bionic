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
#include <stdlib.h>

#include <benchmark/benchmark.h>

#define KB 1024
#define MB 1024*KB

#define AT_COMMON_SIZES \
    Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(512)-> \
    Arg(1*KB)->Arg(4*KB)->Arg(8*KB)->Arg(16*KB)->Arg(64*KB)

template <typename Fn>
void ReadWriteTest(benchmark::State& state, Fn f, bool buffered) {
  size_t chunk_size = state.range_x();

  FILE* fp = fopen("/dev/zero", "rw");
  __fsetlocking(fp, FSETLOCKING_BYCALLER);
  char* buf = new char[chunk_size];

  if (!buffered) {
    setvbuf(fp, 0, _IONBF, 0);
  }

  while (state.KeepRunning()) {
    f(buf, chunk_size, 1, fp);
  }

  state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(chunk_size));
  delete[] buf;
  fclose(fp);
}

void BM_stdio_fread(benchmark::State& state) {
  ReadWriteTest(state, fread, true);
}
BENCHMARK(BM_stdio_fread)->AT_COMMON_SIZES;

void BM_stdio_fwrite(benchmark::State& state) {
  ReadWriteTest(state, fwrite, true);
}
BENCHMARK(BM_stdio_fwrite)->AT_COMMON_SIZES;

void BM_stdio_fread_unbuffered(benchmark::State& state) {
  ReadWriteTest(state, fread, false);
}
BENCHMARK(BM_stdio_fread_unbuffered)->AT_COMMON_SIZES;

void BM_stdio_fwrite_unbuffered(benchmark::State& state) {
  ReadWriteTest(state, fwrite, false);
}
BENCHMARK(BM_stdio_fwrite_unbuffered)->AT_COMMON_SIZES;

static void FopenFgetsFclose(benchmark::State& state, bool no_locking) {
  char buf[1024];
  while (state.KeepRunning()) {
    FILE* fp = fopen("/proc/version", "re");
    if (no_locking) __fsetlocking(fp, FSETLOCKING_BYCALLER);
    if (fgets(buf, sizeof(buf), fp) == nullptr) abort();
    fclose(fp);
  }
}

static void BM_stdio_fopen_fgets_fclose_locking(benchmark::State& state) {
  FopenFgetsFclose(state, false);
}
BENCHMARK(BM_stdio_fopen_fgets_fclose_locking);

void BM_stdio_fopen_fgets_fclose_no_locking(benchmark::State& state) {
  FopenFgetsFclose(state, true);
}
BENCHMARK(BM_stdio_fopen_fgets_fclose_no_locking);
