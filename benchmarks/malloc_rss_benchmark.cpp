/*
 * Copyright (C) 2022 The Android Open Source Project
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
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#include <android-base/strings.h>
#if defined(__BIONIC__)
#include <malloc.h>
#include <meminfo/procmeminfo.h>
#include <procinfo/process_map.h>
#endif

constexpr size_t kMaxThreads = 8;
// The max number of bytes that can be allocated by a thread. Note that each
// allocator may have its own limitation on each size allocation. For example,
// Scudo has a 256 MB limit for each size-class in the primary allocator. The
// amount of memory allocated should not exceed the limit in each allocator.
constexpr size_t kMaxBytes = 1 << 24;
constexpr size_t kMaxLen = kMaxBytes;
void* MemPool[kMaxThreads][kMaxLen];

void dirtyMem(void* ptr, size_t bytes) {
  memset(ptr, 1U, bytes);
}

void ThreadTask(int id, size_t allocSize) {
  // In the following, we will first allocate blocks with kMaxBytes of memory
  // and release all of them in random order. In the end, we will do another
  // round of allocations until it reaches 1/10 kMaxBytes.

  // Total number of blocks
  const size_t maxCounts = kMaxBytes / allocSize;
  // The number of blocks in the end
  const size_t finalCounts = maxCounts / 10;

  for (size_t i = 0; i < maxCounts; ++i) {
    MemPool[id][i] = malloc(allocSize);
    if (MemPool[id][i] == 0) {
      std::cout << "Allocation failure."
                   "Please consider reducing the number of threads"
                << std::endl;
      exit(1);
    }
    dirtyMem(MemPool[id][i], allocSize);
  }

  // Each allocator may apply different strategies to manage the free blocks and
  // each strategy may have different impacts on future memory usage. For
  // example, managing free blocks in simple FIFO list may have its memory usage
  // highly correlated with the blocks releasing pattern. Therefore, release the
  // blocks in random order to observe the impact of free blocks handling.
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(MemPool[id], MemPool[id] + maxCounts, std::default_random_engine(seed));
  for (size_t i = 0; i < maxCounts; ++i) {
    free(MemPool[id][i]);
    MemPool[id][i] = nullptr;
  }

  for (size_t i = 0; i < finalCounts; ++i) {
    MemPool[id][i] = malloc(allocSize);
    dirtyMem(MemPool[id][i], allocSize);
  }
}

void StressSizeClass(size_t numThreads, size_t allocSize) {
  // We would like to see the minimum memory usage under aggressive page
  // releasing.
  mallopt(M_DECAY_TIME, 0);

  std::thread* threads[kMaxThreads];
  for (size_t i = 0; i < numThreads; ++i) threads[i] = new std::thread(ThreadTask, i, allocSize);

  for (size_t i = 0; i < numThreads; ++i) {
    threads[i]->join();
    delete threads[i];
  }

  // Do an explicit purge to ensure we will be more likely to get the actual
  // in-use memory.
  mallopt(M_PURGE_ALL, 0);

  android::meminfo::ProcMemInfo proc_mem(getpid());
  const std::vector<android::meminfo::Vma>& maps = proc_mem.MapsWithoutUsageStats();
  uint64_t rss_bytes = 0;
  uint64_t vss_bytes = 0;

  for (auto& vma : maps) {
    if (vma.name == "[anon:libc_malloc]" || android::base::StartsWith(vma.name, "[anon:scudo:") ||
        android::base::StartsWith(vma.name, "[anon:GWP-ASan")) {
      android::meminfo::Vma update_vma(vma);
      if (!proc_mem.FillInVmaStats(update_vma)) {
        std::cout << "Failed to parse VMA" << std::endl;
        exit(1);
      }
      rss_bytes += update_vma.usage.rss;
      vss_bytes += update_vma.usage.vss;
    }
  }

  std::cout << "RSS: " << rss_bytes / (1024.0 * 1024.0) << " MB" << std::endl;
  std::cout << "VSS: " << vss_bytes / (1024.0 * 1024.0) << " MB" << std::endl;

  for (size_t i = 0; i < numThreads; ++i) {
    for (size_t j = 0; j < kMaxLen; ++j) free(MemPool[i][j]);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " $NUM_THREADS $ALLOC_SIZE" << std::endl;
    return 1;
  }

  size_t numThreads = atoi(argv[1]);
  size_t allocSize = atoi(argv[2]);

  if (numThreads == 0 || allocSize == 0) {
    std::cerr << "Please provide valid $NUM_THREADS and $ALLOC_SIZE" << std::endl;
    return 1;
  }

  if (numThreads > kMaxThreads) {
    std::cerr << "The max number of threads is " << kMaxThreads << std::endl;
    return 1;
  }

  StressSizeClass(numThreads, allocSize);

  return 0;
}
