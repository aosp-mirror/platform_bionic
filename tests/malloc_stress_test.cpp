/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <inttypes.h>
#include <malloc.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <thread>
#include <vector>

#if defined(__BIONIC__)
#include <meminfo/procmeminfo.h>
#include <procinfo/process_map.h>

#include <log/log.h>
#include <log/log_read.h>
#endif

#if defined(__BIONIC__)
static void PrintLogStats(uint64_t& last_time) {
  logger_list* logger =
      android_logger_list_open(android_name_to_log_id("main"), ANDROID_LOG_NONBLOCK, 0, getpid());
  if (logger == nullptr) {
    printf("Failed to open log for main\n");
    return;
  }

  uint64_t last_message_time = last_time;
  while (true) {
    log_msg entry;
    ssize_t retval = android_logger_list_read(logger, &entry);
    if (retval == 0) {
      break;
    }
    if (retval < 0) {
      if (retval == -EINTR) {
        continue;
      }
      // EAGAIN means there is nothing left to read when ANDROID_LOG_NONBLOCK is set.
      if (retval != -EAGAIN) {
        printf("Failed to read log entry: %s\n", strerrordesc_np(retval));
      }
      break;
    }
    if (entry.msg() == nullptr) {
      continue;
    }
    // Only print allocator tagged log entries.
    std::string_view tag(entry.msg() + 1);
    if (tag != "scudo" && tag != "jemalloc") {
      continue;
    }
    if (entry.nsec() > last_time) {
      printf("  %s\n", &tag.back() + 2);
      // Only update the last time outside this loop just in case two or more
      // messages have the same timestamp.
      last_message_time = entry.nsec();
    }
  }
  android_logger_list_close(logger);
  last_time = last_message_time;
}
#endif

TEST(malloc_stress, multiple_threads_forever) {
  constexpr size_t kMaxThreads = 256;
  constexpr size_t kAllocSize = 4096;
#if defined(__BIONIC__)
  uint64_t rss_min = UINT64_MAX;
  uint64_t rss_max = 0;
  uint64_t vss_min = UINT64_MAX;
  uint64_t vss_max = 0;
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 1));
#endif
  uint64_t mallinfo_min = UINT64_MAX;
  uint64_t mallinfo_max = 0;

  uint64_t last_message_time = 0;
  for (size_t i = 0; ; i++) {
    printf("Pass %zu\n", i);

    std::vector<std::thread*> threads;
    for (size_t i = 0; i < kMaxThreads; i++) {
      threads.push_back(new std::thread([]() {
        void* buf = malloc(4096);
        if (buf == nullptr) {
          printf("Failed to allocate memory\n");
          _exit(1);
        }
        memset(buf, 0, kAllocSize);
        sleep(1);
        free(buf);
      }));
    }

    for (auto thread : threads) {
      thread->join();
      delete thread;
    }
    threads.clear();

#if defined(__BIONIC__)
    android::meminfo::ProcMemInfo proc_mem(getpid());
    const std::vector<android::meminfo::Vma>& maps = proc_mem.MapsWithoutUsageStats();
    uint64_t rss_bytes = 0;
    uint64_t vss_bytes = 0;
    for (auto& vma : maps) {
      if (vma.name == "[anon:libc_malloc]" || vma.name.starts_with("[anon:scudo:") ||
          vma.name.starts_with("[anon:GWP-ASan")) {
        android::meminfo::Vma update_vma(vma);
        ASSERT_TRUE(proc_mem.FillInVmaStats(update_vma));
        rss_bytes += update_vma.usage.rss;
        vss_bytes += update_vma.usage.vss;
      }
    }
    if (rss_bytes < rss_min) {
      rss_min = rss_bytes;
    }
    if (rss_bytes > rss_max) {
      rss_max = rss_bytes;
    }
    if (vss_bytes < vss_min) {
      vss_min = vss_bytes;
    }
    if (vss_bytes > vss_max) {
      vss_max = vss_bytes;
    }
    printf("RSS %" PRIu64 " %0.2fMB\n", rss_bytes, rss_bytes / (1024.0 * 1024.0));
    printf("  Min %" PRIu64 " %0.2fMB\n", rss_min, rss_min / (1024.0 * 1024.0));
    printf("  Max %" PRIu64 " %0.2fMB\n", rss_max, rss_max / (1024.0 * 1024.0));
    printf("VSS %" PRIu64 " %0.2f MB\n", vss_bytes, vss_bytes / (1024.0 * 1024.0));
    printf("  Min %" PRIu64 " %0.2fMB\n", vss_min, vss_min / (1024.0 * 1024.0));
    printf("  Max %" PRIu64 " %0.2fMB\n", vss_max, vss_max / (1024.0 * 1024.0));
#endif

    size_t mallinfo_bytes = mallinfo().uordblks;
    if (mallinfo_bytes < mallinfo_min) {
      mallinfo_min = mallinfo_bytes;
    }
    if (mallinfo_bytes > mallinfo_max) {
      mallinfo_max = mallinfo_bytes;
    }
    printf("Allocated memory %zu %0.2fMB\n", mallinfo_bytes, mallinfo_bytes / (1024.0 * 1024.0));
    printf("  Min %" PRIu64 " %0.2fMB\n", mallinfo_min, mallinfo_min / (1024.0 * 1024.0));
    printf("  Max %" PRIu64 " %0.2fMB\n", mallinfo_max, mallinfo_max / (1024.0 * 1024.0));

#if defined(__BIONIC__)
    if (((i + 1) % 100) == 0) {
      // Send native allocator stats to the log
      mallopt(M_LOG_STATS, 0);

      printf("Log stats:\n");
      PrintLogStats(last_message_time);
    }
#endif
  }
}
