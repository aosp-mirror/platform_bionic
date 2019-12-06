/*
 * Copyright (C) 2019 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <malloc.h>
#include <unistd.h>

#include <vector>

#include <benchmark/benchmark.h>
#include "util.h"

#if defined(__BIONIC__)

static void BM_mallopt_purge(benchmark::State& state) {
  static size_t sizes[] = {8, 16, 32, 64, 128, 1024, 4096, 16384, 65536, 131072, 1048576};
  static int pagesize = getpagesize();
  mallopt(M_DECAY_TIME, 1);
  mallopt(M_PURGE, 0);
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<void*> ptrs;
    for (auto size : sizes) {
      // Allocate at least two pages worth of the allocations.
      for (size_t allocated = 0; allocated < 2 * static_cast<size_t>(pagesize); allocated += size) {
        void* ptr = malloc(size);
        if (ptr == nullptr) {
          state.SkipWithError("Failed to allocate memory");
        }
        MakeAllocationResident(ptr, size, pagesize);
        ptrs.push_back(ptr);
      }
    }
    // Free the memory, which should leave many of the pages resident until
    // the purge call.
    for (auto ptr : ptrs) {
      free(ptr);
    }
    ptrs.clear();
    state.ResumeTiming();

    mallopt(M_PURGE, 0);
  }
  mallopt(M_DECAY_TIME, 0);
}
BIONIC_BENCHMARK(BM_mallopt_purge);

#endif
