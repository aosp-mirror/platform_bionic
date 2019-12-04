/*
 * Copyright (C) 2018 The Android Open Source Project
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
#include <stdlib.h>
#include <unistd.h>

#include <benchmark/benchmark.h>
#include "util.h"

#if defined(__BIONIC__)

enum AllocEnum : uint8_t {
  MALLOC = 0,
  CALLOC,
  MEMALIGN,
  REALLOC,
  FREE,
};

struct MallocEntry {
  AllocEnum type;
  size_t idx;
  size_t size;
  size_t arg2;
};

void BenchmarkMalloc(MallocEntry entries[], size_t total_entries, size_t max_allocs) {
  void* ptrs[max_allocs];

  for (size_t i = 0; i < total_entries; i++) {
    switch (entries[i].type) {
    case MALLOC:
      ptrs[entries[i].idx] = malloc(entries[i].size);
      // Touch at least one byte of the allocation to make sure that
      // PSS for this allocation is counted.
      reinterpret_cast<uint8_t*>(ptrs[entries[i].idx])[0] = 10;
      break;
    case CALLOC:
      ptrs[entries[i].idx] = calloc(entries[i].arg2, entries[i].size);
      // Touch at least one byte of the allocation to make sure that
      // PSS for this allocation is counted.
      reinterpret_cast<uint8_t*>(ptrs[entries[i].idx])[0] = 20;
      break;
    case MEMALIGN:
      ptrs[entries[i].idx] = memalign(entries[i].arg2, entries[i].size);
      // Touch at least one byte of the allocation to make sure that
      // PSS for this allocation is counted.
      reinterpret_cast<uint8_t*>(ptrs[entries[i].idx])[0] = 30;
      break;
    case REALLOC:
      if (entries[i].arg2 == 0) {
        ptrs[entries[i].idx] = realloc(nullptr, entries[i].size);
      } else {
        ptrs[entries[i].idx] = realloc(ptrs[entries[i].arg2 - 1], entries[i].size);
      }
      // Touch at least one byte of the allocation to make sure that
      // PSS for this allocation is counted.
      reinterpret_cast<uint8_t*>(ptrs[entries[i].idx])[0] = 40;
      break;
    case FREE:
      free(ptrs[entries[i].idx]);
      break;
    }
  }
}

// This codifies playing back a single threaded trace of the allocations
// when running the SQLite BenchMark app.
// Instructions for recreating:
//   - Enable malloc debug
//       setprop wrap.com.wtsang02.sqliteutil "LIBC_DEBUG_MALLOC_OPTIONS=record_allocs logwrapper"
//   - Start the SQLite BenchMark app
//   - Dump allocs using the signal to get rid of non sql allocs(kill -47 <SQLITE_PID>)
//   - Run the benchmark.
//   - Dump allocs using the signal again.
//   - Find the thread that has the most allocs and run the helper script
//       bionic/libc/malloc_debug/tools/gen_malloc.pl -i <THREAD_ID> g_sql_entries kMaxSqlAllocSlots < <ALLOC_FILE> > malloc_sql.h
#include "malloc_sql.h"

static void BM_malloc_sql_trace_default(benchmark::State& state) {
  // The default is expected to be a zero decay time.
  mallopt(M_DECAY_TIME, 0);

  for (auto _ : state) {
    BenchmarkMalloc(g_sql_entries, sizeof(g_sql_entries) / sizeof(MallocEntry),
                    kMaxSqlAllocSlots);
  }
}
BIONIC_BENCHMARK(BM_malloc_sql_trace_default);

static void BM_malloc_sql_trace_decay1(benchmark::State& state) {
  mallopt(M_DECAY_TIME, 1);

  for (auto _ : state) {
    BenchmarkMalloc(g_sql_entries, sizeof(g_sql_entries) / sizeof(MallocEntry),
                    kMaxSqlAllocSlots);
  }

  mallopt(M_DECAY_TIME, 0);
}
BIONIC_BENCHMARK(BM_malloc_sql_trace_decay1);

#endif
