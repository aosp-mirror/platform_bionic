/*
 * Copyright (C) 2018 The Android Open Source Project
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

#if defined(__BIONIC__)

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <vector>

#include <async_safe/log.h>
#include <procinfo/process_map.h>

#include "utils.h"

extern "C" void malloc_disable();
extern "C" void malloc_enable();
extern "C" int malloc_iterate(uintptr_t base, size_t size, void (*callback)(uintptr_t base,
                              size_t size, void* arg), void* arg);

struct AllocDataType {
  void* ptr;
  size_t size;
  size_t size_reported;
  size_t count;
};

struct TestDataType {
  size_t total_allocated_bytes;
  std::vector<AllocDataType> allocs;
};

static void AllocPtr(TestDataType* test_data, size_t size) {
  test_data->allocs.resize(test_data->allocs.size() + 1);
  AllocDataType* alloc = &test_data->allocs.back();
  void* ptr = malloc(size);
  ASSERT_TRUE(ptr != nullptr);
  alloc->ptr = ptr;
  alloc->size = malloc_usable_size(ptr);
  alloc->size_reported = 0;
  alloc->count = 0;
}

static void FreePtrs(TestDataType* test_data) {
  for (size_t i = 0; i < test_data->allocs.size(); i++) {
    free(test_data->allocs[i].ptr);
  }
}

static void SavePointers(uintptr_t base, size_t size, void* data) {
  TestDataType* test_data = reinterpret_cast<TestDataType*>(data);

  test_data->total_allocated_bytes += size;

  uintptr_t end;
  if (__builtin_add_overflow(base, size, &end)) {
    // Skip this entry.
    return;
  }

  for (size_t i = 0; i < test_data->allocs.size(); i++) {
    uintptr_t ptr = reinterpret_cast<uintptr_t>(test_data->allocs[i].ptr);
    if (ptr >= base && ptr < end) {
      test_data->allocs[i].count++;

      uintptr_t max_size = end - ptr;
      if (max_size > test_data->allocs[i].size) {
        test_data->allocs[i].size_reported = test_data->allocs[i].size;
      } else {
        test_data->allocs[i].size_reported = max_size;
      }
    }
  }
}

static void VerifyPtrs(TestDataType* test_data) {
  test_data->total_allocated_bytes = 0;

  // Find all of the maps that are from the native allocator.
  auto callback = [&](uint64_t start, uint64_t end, uint16_t, uint64_t, ino_t, const char* name) {
    if (strcmp(name, "[anon:libc_malloc]") == 0 || strncmp(name, "[anon:scudo:", 12) == 0 ||
        strncmp(name, "[anon:GWP-ASan", 14) == 0) {
      malloc_iterate(start, end - start, SavePointers, test_data);
    }
  };

  std::vector<char> buffer(64 * 1024);

  // Avoid doing allocations so that the maps don't change while looking
  // for the pointers.
  malloc_disable();
  bool parsed = android::procinfo::ReadMapFileAsyncSafe("/proc/self/maps", buffer.data(),
                                                        buffer.size(), callback);
  malloc_enable();

  ASSERT_TRUE(parsed) << "Failed to parse /proc/self/maps";

  for (size_t i = 0; i < test_data->allocs.size(); i++) {
    EXPECT_EQ(1UL, test_data->allocs[i].count) << "Failed on size " << test_data->allocs[i].size;
    if (test_data->allocs[i].count == 1) {
      EXPECT_EQ(test_data->allocs[i].size, test_data->allocs[i].size_reported);
    }
  }
}

static void AllocateSizes(TestDataType* test_data, const std::vector<size_t>& sizes) {
  static constexpr size_t kInitialAllocations = 40;
  static constexpr size_t kNumAllocs = 50;
  for (size_t size : sizes) {
    // Verify that if the tcache is enabled, that tcache pointers
    // are found by allocating and freeing 20 pointers (should be larger
    // than the total number of cache entries).
    for (size_t i = 0; i < kInitialAllocations; i++) {
      void* ptr = malloc(size);
      ASSERT_TRUE(ptr != nullptr);
      memset(ptr, 0, size);
      free(ptr);
    }
    for (size_t i = 0; i < kNumAllocs; i++) {
      AllocPtr(test_data, size);
    }
  }
}
#endif

// Verify that small allocs can be found properly.
TEST(malloc_iterate, small_allocs) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN;
  TestDataType test_data;

  // Try to cycle through all of the different small bins.
  // This is specific to the implementation of jemalloc and should be
  // adjusted if a different native memory allocator is used.
  std::vector<size_t> sizes{8,    16,   32,   48,    64,    80,    96,    112,   128,  160,
                            192,  224,  256,  320,   384,   448,   512,   640,   768,  896,
                            1024, 1280, 1536, 1792,  2048,  2560,  3072,  3584,  4096, 5120,
                            6144, 7168, 8192, 10240, 12288, 14336, 16384, 32768, 65536};
  AllocateSizes(&test_data, sizes);

  SCOPED_TRACE("");
  VerifyPtrs(&test_data);

  FreePtrs(&test_data);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

// Verify that large allocs can be found properly.
TEST(malloc_iterate, large_allocs) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN;
  TestDataType test_data;

  // Try some larger sizes.
  std::vector<size_t> sizes{131072, 262144, 524288, 1048576, 2097152};
  AllocateSizes(&test_data, sizes);

  SCOPED_TRACE("");
  VerifyPtrs(&test_data);

  FreePtrs(&test_data);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

// Verify that there are no crashes attempting to get pointers from
// non-allocated pointers.
TEST(malloc_iterate, invalid_pointers) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN;
  TestDataType test_data = {};

  // Only attempt to get memory data for maps that are not from the native allocator.
  auto callback = [&](uint64_t start, uint64_t end, uint16_t, uint64_t, ino_t, const char* name) {
    if (strcmp(name, "[anon:libc_malloc]") != 0 && strncmp(name, "[anon:scudo:", 12) != 0 &&
        strncmp(name, "[anon:GWP-ASan", 14) != 0) {
      size_t total = test_data.total_allocated_bytes;
      malloc_iterate(start, end - start, SavePointers, &test_data);
      total = test_data.total_allocated_bytes - total;
      if (total > 0) {
        char buffer[256];
        int len = 0;
        if (name[0] != '\0') {
          len = async_safe_format_buffer(buffer, sizeof(buffer), "Failed on map %s: %zu\n", name,
                                         total);
        } else {
          len = async_safe_format_buffer(buffer, sizeof(buffer),
                                         "Failed on map anon:<%" PRIx64 "-%" PRIx64 ">: %zu\n",
                                         start, end, total);
        }
        if (len > 0) {
          write(STDOUT_FILENO, buffer, len);
        }
      }
    }
  };

  std::vector<char> buffer(64 * 1024);

  // Need to make sure that there are no allocations while reading the
  // maps. Otherwise, it might create a new map during this check and
  // incorrectly think a map is empty while it actually includes real
  // allocations.
  malloc_disable();
  bool parsed = android::procinfo::ReadMapFileAsyncSafe("/proc/self/maps", buffer.data(),
                                                        buffer.size(), callback);
  malloc_enable();

  ASSERT_TRUE(parsed) << "Failed to parse /proc/self/maps";

  ASSERT_EQ(0UL, test_data.total_allocated_bytes);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(malloc_iterate, malloc_disable_prevents_allocs) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN;
  pid_t pid;
  if ((pid = fork()) == 0) {
    malloc_disable();
    void* ptr = malloc(1024);
    if (ptr == nullptr) {
      exit(1);
    }
    memset(ptr, 0, 1024);
    exit(0);
  }
  ASSERT_NE(-1, pid);

  // Expect that the malloc will hang forever, and that if the process
  // does not return for two seconds, it is hung.
  sleep(2);
  pid_t wait_pid = TEMP_FAILURE_RETRY(waitpid(pid, nullptr, WNOHANG));
  if (wait_pid <= 0) {
    kill(pid, SIGKILL);
  }
  ASSERT_NE(-1, wait_pid) << "Unknown failure in waitpid.";
  ASSERT_EQ(0, wait_pid) << "malloc_disable did not prevent allocation calls.";
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}
