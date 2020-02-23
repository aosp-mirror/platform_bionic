/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <err.h>
#include <malloc.h>
#include <stdint.h>

#include <map>
#include <unordered_map>
#include <vector>

#include <benchmark/benchmark.h>
#include "util.h"

#include <android-base/strings.h>

#if defined(__BIONIC__)

#include <meminfo/procmeminfo.h>
#include <procinfo/process_map.h>

static void Gather(uint64_t* rss_bytes) {
  android::meminfo::ProcMemInfo proc_mem(getpid());
  const std::vector<android::meminfo::Vma>& maps = proc_mem.MapsWithoutUsageStats();
  for (auto& vma : maps) {
    if (vma.name == "[anon:libc_malloc]" || android::base::StartsWith(vma.name, "[anon:scudo:") ||
        android::base::StartsWith(vma.name, "[anon:GWP-ASan")) {
      android::meminfo::Vma update_vma(vma);
      if (!proc_mem.FillInVmaStats(update_vma)) {
        err(1, "FillInVmaStats failed\n");
      }
      *rss_bytes += update_vma.usage.rss;
    }
  }
}
#endif

template <typename MapType>
static void MapBenchmark(benchmark::State& state, size_t num_elements) {
#if defined(__BIONIC__)
  uint64_t rss_bytes = 0;
#endif

  for (auto _ : state) {
#if defined(__BIONIC__)
    state.PauseTiming();
    mallopt(M_PURGE, 0);
    uint64_t rss_bytes_before = 0;
    Gather(&rss_bytes_before);
    state.ResumeTiming();
#endif
    MapType map;
    for (size_t i = 0; i < num_elements; i++) {
      map[i][0] = 0;
    }
#if defined(__BIONIC__)
    state.PauseTiming();
    mallopt(M_PURGE, 0);
    Gather(&rss_bytes);
    // Try and record only the memory used in the map.
    rss_bytes -= rss_bytes_before;
    state.ResumeTiming();
#endif
  }

#if defined(__BIONIC__)
  double rss_mb = (rss_bytes / static_cast<double>(state.iterations())) / 1024.0 / 1024.0;
  state.counters["RSS_MB"] = rss_mb;
#endif
}

static void BM_std_map_8(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[8]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_map_8);

static void BM_std_map_16(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[16]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_map_16);

static void BM_std_map_32(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[32]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_map_32);

static void BM_std_map_64(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[64]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_map_64);

static void BM_std_map_96(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[96]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_map_96);

static void BM_std_map_128(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[128]>>(state, 500000);
}
BIONIC_BENCHMARK(BM_std_map_128);

static void BM_std_map_256(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[256]>>(state, 500000);
}
BIONIC_BENCHMARK(BM_std_map_256);

static void BM_std_map_512(benchmark::State& state) {
  MapBenchmark<std::map<uint64_t, char[512]>>(state, 500000);
}
BIONIC_BENCHMARK(BM_std_map_512);

static void BM_std_unordered_map_8(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[8]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_8);

static void BM_std_unordered_map_16(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[16]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_16);

static void BM_std_unordered_map_32(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[32]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_32);

static void BM_std_unordered_map_64(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[64]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_64);

static void BM_std_unordered_map_96(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[96]>>(state, 1000000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_96);

static void BM_std_unordered_map_128(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[128]>>(state, 500000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_128);

static void BM_std_unordered_map_256(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[256]>>(state, 500000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_256);

static void BM_std_unordered_map_512(benchmark::State& state) {
  MapBenchmark<std::unordered_map<uint64_t, char[512]>>(state, 500000);
}
BIONIC_BENCHMARK(BM_std_unordered_map_512);
