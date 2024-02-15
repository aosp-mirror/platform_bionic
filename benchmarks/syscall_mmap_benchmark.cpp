/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <benchmark/benchmark.h>

#include "util.h"

enum BenchmarkType : uint8_t {
  kBenchmarkMmapOnly,
  kBenchmarkMunmapOnly,
  kBenchmarkAll,
};

static size_t page_sz = getpagesize();

struct MmapParams {
  int prot;
  int flags;
  int64_t size;
};

template <BenchmarkType type>
void MmapBenchmarkImpl(benchmark::State& state, const struct MmapParams& params, int fd,
                       void* area = nullptr) {
  for (auto _ : state) {
    if (type == kBenchmarkMunmapOnly) state.PauseTiming();
    void* addr = mmap(area, params.size, params.prot, params.flags, fd, 0);
    if (addr == MAP_FAILED) {
      state.SkipWithError(android::base::StringPrintf("mmap failed: %s", strerror(errno)).c_str());
      break;
    }

    if (type == kBenchmarkMmapOnly) state.PauseTiming();

    if (params.prot & PROT_WRITE) {
      MakeAllocationResident(addr, params.size, page_sz);
    }

    if (type == kBenchmarkMunmapOnly) state.ResumeTiming();

    if (munmap(addr, params.size) != 0) {
      state.SkipWithError(
          android::base::StringPrintf("munmap failed: %s", strerror(errno)).c_str());
      break;
    }
    if (type == kBenchmarkMmapOnly) state.ResumeTiming();
  }
}

static void MmapBenchmark(benchmark::State& state, const struct MmapParams& params, int fd,
                          void* area = nullptr) {
  MmapBenchmarkImpl<kBenchmarkAll>(state, params, fd, area);
}

static void MmapFixedBenchmark(benchmark::State& state, const struct MmapParams& params, int fd,
                               size_t area_size, size_t offs) {
  if ((params.flags & MAP_FIXED) == 0) {
    state.SkipWithError("MmapFixedBenchmark called without MAP_FIXED set");
    return;
  }

  // Create the mmap that will be used for the fixed mmaps.
  uint8_t* area = reinterpret_cast<uint8_t*>(
      mmap(nullptr, area_size, params.prot, params.flags & ~MAP_FIXED, fd, 0));
  if (area == MAP_FAILED) {
    state.SkipWithError(android::base::StringPrintf("mmap failed: %s", strerror(errno)).c_str());
    return;
  }

  MmapBenchmark(state, params, fd, area + offs);

  if (munmap(area, area_size) != 0) {
    state.SkipWithError(android::base::StringPrintf("munmap failed: %s", strerror(errno)).c_str());
    return;
  }
}

static void MmapFileBenchmark(benchmark::State& state, const struct MmapParams& params,
                              size_t area_size, size_t offs) {
  TemporaryFile tf;

  if (tf.fd < 0) {
    state.SkipWithError(
        android::base::StringPrintf("failed to create a temporary file: %s", strerror(errno))
            .c_str());
    return;
  }

  if (area_size > 0 && ftruncate(tf.fd, area_size)) {
    state.SkipWithError(
        android::base::StringPrintf("ftruncate failed: %s", strerror(errno)).c_str());
    return;
  }

  if (params.flags & MAP_FIXED) {
    MmapFixedBenchmark(state, params, tf.fd, area_size, offs);
  } else {
    MmapBenchmark(state, params, tf.fd);
  }
}

// anon mmap
static void BM_syscall_mmap_anon_rw(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS,
      .size = state.range(0),
  };

  MmapBenchmark(state, params, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_rw, "AT_All_PAGE_SIZES");

static void BM_syscall_mmap_anon_noreserve(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_NONE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE,
      .size = state.range(0),
  };

  MmapBenchmark(state, params, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_noreserve, "AT_All_PAGE_SIZES");

static void BM_syscall_mmap_anon_none(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_NONE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS,
      .size = state.range(0),
  };

  MmapBenchmark(state, params, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_none, "AT_All_PAGE_SIZES");

// anon fixed mmap
static void BM_syscall_mmap_anon_rw_fixed(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
      .size = state.range(0),
  };

  MmapFixedBenchmark(state, params, -1, params.size, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_rw_fixed, "AT_All_PAGE_SIZES");

static void BM_syscall_mmap_anon_none_fixed(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_NONE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
      .size = state.range(0),
  };

  MmapFixedBenchmark(state, params, -1, params.size, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_none_fixed, "AT_All_PAGE_SIZES");

// file mmap
static void BM_syscall_mmap_file_rd_priv(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ,
      .flags = MAP_PRIVATE,
      .size = state.range(0),
  };

  MmapFileBenchmark(state, params, params.size, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rd_priv, "AT_All_PAGE_SIZES");

static void BM_syscall_mmap_file_rw_shared(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_SHARED,
      .size = state.range(0),
  };

  MmapFileBenchmark(state, params, params.size, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rw_shared, "AT_All_PAGE_SIZES");

// file fixed mmap
static void BM_syscall_mmap_file_rw_priv_fixed_start(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_FIXED,
      .size = state.range(0),
  };

  // allocate 3x area and map at the start
  MmapFileBenchmark(state, params, params.size * 3, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rw_priv_fixed_start, "AT_All_PAGE_SIZES");

static void BM_syscall_mmap_file_rw_priv_fixed_mid(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_FIXED,
      .size = state.range(0),
  };

  // allocate 3x area and map at the middle
  MmapFileBenchmark(state, params, params.size * 3, params.size);
}
// mapping at sub-page size offset is not supported, so run only for AT_MULTI_PAGE_SIZES
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rw_priv_fixed_mid, "AT_MULTI_PAGE_SIZES");

static void BM_syscall_mmap_file_rw_priv_fixed_end(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_FIXED,
      .size = state.range(0),
  };

  // allocate 3x area and map at the end
  MmapFileBenchmark(state, params, params.size * 3, params.size * 2);
}
// mapping at sub-page size offset is not supported, so run only for AT_MULTI_PAGE_SIZES
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_file_rw_priv_fixed_end, "AT_MULTI_PAGE_SIZES");

static void BM_syscall_mmap_anon_mmap_only(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS,
      .size = state.range(0),
  };
  MmapBenchmarkImpl<kBenchmarkMmapOnly>(state, params, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_mmap_only, "AT_MULTI_PAGE_SIZES");

static void BM_syscall_mmap_anon_munmap_only(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS,
      .size = state.range(0),
  };
  MmapBenchmarkImpl<kBenchmarkMunmapOnly>(state, params, 0);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_munmap_only, "AT_MULTI_PAGE_SIZES");

void MadviseBenchmark(benchmark::State& state, const struct MmapParams& params, int madvise_flags) {
  void* addr = mmap(nullptr, params.size, params.prot, params.flags, 0, 0);
  if (addr == MAP_FAILED) {
    state.SkipWithError(android::base::StringPrintf("mmap failed: %s", strerror(errno)).c_str());
    return;
  }
  for (auto _ : state) {
    state.PauseTiming();
    if (params.prot & PROT_WRITE) {
      MakeAllocationResident(addr, params.size, page_sz);
    }
    state.ResumeTiming();

    madvise(addr, params.size, madvise_flags);
  }

  if (munmap(addr, params.size) != 0) {
    state.SkipWithError(android::base::StringPrintf("munmap failed: %s", strerror(errno)).c_str());
  }
}

static void BM_syscall_mmap_anon_madvise_dontneed(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS,
      .size = state.range(0),
  };
  MadviseBenchmark(state, params, MADV_DONTNEED);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_madvise_dontneed, "AT_MULTI_PAGE_SIZES");

static void BM_syscall_mmap_anon_madvise_pageout(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS,
      .size = state.range(0),
  };
  MadviseBenchmark(state, params, MADV_PAGEOUT);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_madvise_pageout, "AT_MULTI_PAGE_SIZES");

static void BM_syscall_mmap_anon_madvise_free(benchmark::State& state) {
  struct MmapParams params = {
      .prot = PROT_READ | PROT_WRITE,
      .flags = MAP_PRIVATE | MAP_ANONYMOUS,
      .size = state.range(0),
  };
  MadviseBenchmark(state, params, MADV_FREE);
}
BIONIC_BENCHMARK_WITH_ARG(BM_syscall_mmap_anon_madvise_free, "AT_MULTI_PAGE_SIZES");
