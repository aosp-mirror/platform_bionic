/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <thread>
#include <vector>
#include <utility>

#include <gtest/gtest.h>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include <private/bionic_macros.h>
#include <private/bionic_malloc_dispatch.h>

#include "Config.h"
#include "malloc_debug.h"

#include "log_fake.h"
#include "backtrace_fake.h"

__BEGIN_DECLS

bool debug_initialize(const MallocDispatch*, int*, const char*);
void debug_finalize();

void* debug_malloc(size_t);
void debug_free(void*);
void* debug_calloc(size_t, size_t);
void* debug_realloc(void*, size_t);
int debug_posix_memalign(void**, size_t, size_t);
void* debug_memalign(size_t, size_t);
void* debug_aligned_alloc(size_t, size_t);
size_t debug_malloc_usable_size(void*);
void debug_get_malloc_leak_info(uint8_t**, size_t*, size_t*, size_t*, size_t*);
void debug_free_malloc_leak_info(uint8_t*);

struct mallinfo debug_mallinfo();
int debug_mallopt(int, int);

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* debug_pvalloc(size_t);
void* debug_valloc(size_t);
#endif

__END_DECLS

constexpr char DIVIDER[] =
    "6 malloc_debug *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***\n";

static size_t get_tag_offset() {
  return __BIONIC_ALIGN(sizeof(Header), MINIMUM_ALIGNMENT_BYTES);
}

static constexpr const char RECORD_ALLOCS_FILE[] = "/data/local/tmp/record_allocs.txt";

static constexpr const char BACKTRACE_DUMP_PREFIX[] = "/data/local/tmp/backtrace_heap";

class MallocDebugTest : public ::testing::Test {
 protected:
  void SetUp() override {
    initialized = false;
    resetLogs();
    backtrace_fake_clear_all();
    // Delete the record data file if it exists.
    unlink(RECORD_ALLOCS_FILE);
  }

  void TearDown() override {
    if (initialized) {
      debug_finalize();
    }
  }

  void Init(const char* options) {
    zygote = 0;
    ASSERT_TRUE(debug_initialize(&dispatch, &zygote, options));
    initialized = true;
  }

  void BacktraceDumpOnSignal(bool trigger_with_alloc);

  static size_t GetInfoEntrySize(size_t max_frames) {
    return 2 * sizeof(size_t) + max_frames * sizeof(uintptr_t);
  }

  bool initialized;

  int zygote;

  static MallocDispatch dispatch;
};

MallocDispatch MallocDebugTest::dispatch = {
  calloc,
  free,
  mallinfo,
  malloc,
  malloc_usable_size,
  memalign,
  posix_memalign,
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  nullptr,
#endif
  realloc,
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  nullptr,
#endif
  nullptr,
  nullptr,
  nullptr,
  mallopt,
  aligned_alloc,
};

std::string ShowDiffs(uint8_t* a, uint8_t* b, size_t size) {
  std::string diff;
  for (size_t i = 0; i < size; i++) {
    if (a[i] != b[i]) {
      diff += android::base::StringPrintf("Byte %zu: 0x%x 0x%x\n", i, a[i], b[i]);
    }
  }
  return diff;
}

void VerifyAllocCalls(bool backtrace_enabled) {
  size_t alloc_size = 1024;

  // Verify debug_malloc.
  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < debug_malloc_usable_size(pointer); i++) {
    ASSERT_EQ(0xeb, pointer[i]);
  }
  debug_free(pointer);

  // Verify debug_calloc.
  pointer = reinterpret_cast<uint8_t*>(debug_calloc(1, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < debug_malloc_usable_size(pointer); i++) {
    ASSERT_EQ(0, pointer[i]) << "Failed at byte " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_memalign(128, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < debug_malloc_usable_size(pointer); i++) {
    ASSERT_EQ(0xeb, pointer[i]) << "Failed at byte " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < debug_malloc_usable_size(pointer); i++) {
    ASSERT_EQ(0xeb, pointer[i]) << "Failed at byte " << i;
  }
  memset(pointer, 0xff, alloc_size);
  // Increase the size, verify the extra length is initialized to 0xeb,
  // but the rest is 0xff.
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, alloc_size * 2));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < alloc_size; i++) {
    ASSERT_EQ(0xff, pointer[i]) << "Failed at byte " << i;
  }
  for (size_t i = alloc_size; i < debug_malloc_usable_size(pointer); i++) {
    ASSERT_EQ(0xeb, pointer[i]) << "Failed at byte " << i;
  }
  memset(pointer, 0xff, debug_malloc_usable_size(pointer));
  // Shrink the size and verify nothing changes.
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < debug_malloc_usable_size(pointer); i++) {
    ASSERT_EQ(0xff, pointer[i]) << "Failed at byte " << i;
  }
  // This should free the pointer.
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 0));
  ASSERT_TRUE(pointer == nullptr);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log;
  if (backtrace_enabled) {
    expected_log += android::base::StringPrintf(
        "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
        SIGRTMAX - 17, getpid());
  }
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, fill_generic) {
  Init("fill");
  VerifyAllocCalls(false);
}

TEST_F(MallocDebugTest, fill_on_alloc_generic) {
  Init("fill_on_alloc");
  VerifyAllocCalls(false);
}

TEST_F(MallocDebugTest, fill_on_alloc_partial) {
  Init("fill_on_alloc=25");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < 25; i++) {
    ASSERT_EQ(0xeb, pointer[i]) << "Failed at byte " << i;
  }
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, fill_on_free) {
  Init("fill_on_free free_track free_track_backtrace_num_frames=0");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  size_t usable_size = debug_malloc_usable_size(pointer);
  memset(pointer, 0, usable_size);
  debug_free(pointer);

  for (size_t i = 0; i < usable_size; i++) {
    ASSERT_EQ(0xef, pointer[i]) << "Failed at byte " << i;
  }

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, fill_on_free_partial) {
  Init("fill_on_free=30 free_track free_track_backtrace_num_frames=0");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  size_t usable_size = debug_malloc_usable_size(pointer);
  memset(pointer, 0, usable_size);
  debug_free(pointer);

  for (size_t i = 0; i < 30; i++) {
    ASSERT_EQ(0xef, pointer[i]) << "Failed to fill on free at byte " << i;
  }
  for (size_t i = 30; i < usable_size; i++) {
    ASSERT_EQ(0, pointer[i]) << "Filled too much on byte " << i;
  }

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_partial) {
  Init("fill_on_free=30 free_track free_track_backtrace_num_frames=0");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  size_t usable_size = debug_malloc_usable_size(pointer);
  memset(pointer, 0, usable_size);
  debug_free(pointer);

  for (size_t i = 0; i < 30; i++) {
    ASSERT_EQ(0xef, pointer[i]) << "Failed to fill on free at byte " << i;
  }
  for (size_t i = 30; i < usable_size; i++) {
    ASSERT_EQ(0, pointer[i]) << "Filled too much on byte " << i;
  }

  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, all_options) {
  Init("guard backtrace fill expand_alloc free_track leak_track");
  VerifyAllocCalls(true);
}

TEST_F(MallocDebugTest, expand_alloc) {
  Init("expand_alloc=1024");

  void* pointer = debug_malloc(10);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1034U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  pointer = debug_calloc(1, 20);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1044U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  pointer = debug_memalign(128, 15);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1039U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  pointer = debug_aligned_alloc(128, 15);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1039U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  pointer = debug_realloc(nullptr, 30);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1054U, debug_malloc_usable_size(pointer));
  pointer = debug_realloc(pointer, 100);
  ASSERT_LE(1124U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, front_guard) {
  Init("front_guard=32");

  // Create a buffer for doing comparisons.
  std::vector<uint8_t> buffer(32);
  memset(buffer.data(), 0xaa, buffer.size());

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[-buffer.size()], buffer.size());
  memset(pointer, 0xff, 100);
  debug_free(pointer);

  // Loop through a bunch alignments.
  for (size_t alignment = 1; alignment <= 256; alignment++) {
    pointer = reinterpret_cast<uint8_t*>(debug_memalign(alignment, 100));
    ASSERT_TRUE(pointer != nullptr);
    ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0)
        << ShowDiffs(buffer.data(), &pointer[-buffer.size()], buffer.size());
    size_t alignment_mask = alignment - 1;
    if (!powerof2(alignment)) {
      alignment_mask = BIONIC_ROUND_UP_POWER_OF_2(alignment) - 1;
    }
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(pointer) & alignment_mask);
    memset(pointer, 0xff, 100);
    debug_free(pointer);
  }

  pointer = reinterpret_cast<uint8_t*>(debug_calloc(1, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[-buffer.size()], buffer.size());
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, pointer[i]) << "debug_calloc non-zero byte at " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[-buffer.size()], buffer.size());
  memset(pointer, 0xff, 100);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 200));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[-buffer.size()], buffer.size());
  memset(pointer, 0xff, 200);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 0));
  ASSERT_TRUE(pointer == nullptr);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, realloc_memalign_memory) {
  Init("rear_guard");

  void* pointer = debug_memalign(1024, 100);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 100);

  pointer = debug_realloc(pointer, 1024);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(1024U, debug_malloc_usable_size(pointer));
  memset(pointer, 0, 1024);
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, front_guard_corrupted) {
  Init("front_guard=32");

  backtrace_fake_add(std::vector<uintptr_t> {0x1, 0x2, 0x3});

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  pointer[-32] = 0x00;
  pointer[-15] = 0x02;
  debug_free(pointer);

  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p SIZE 100 HAS A CORRUPTED FRONT GUARD\n", pointer);
  expected_log += "6 malloc_debug   allocation[-32] = 0x00 (expected 0xaa)\n";
  expected_log += "6 malloc_debug   allocation[-15] = 0x02 (expected 0xaa)\n";
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x1\n";
  expected_log += "6 malloc_debug   #01 pc 0x2\n";
  expected_log += "6 malloc_debug   #02 pc 0x3\n";
  expected_log += DIVIDER;
  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, rear_guard) {
  Init("rear_guard=32");

  // Create a buffer for doing comparisons.
  std::vector<uint8_t> buffer(32);
  memset(buffer.data(), 0xbb, buffer.size());

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[100], buffer.size());
  memset(pointer, 0xff, 100);
  debug_free(pointer);

  // Loop through a bunch alignments.
  for (size_t alignment = 1; alignment <= 256; alignment++) {
    pointer = reinterpret_cast<uint8_t*>(debug_memalign(alignment, 100));
    ASSERT_TRUE(pointer != nullptr);
    ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
    ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0)
        << ShowDiffs(buffer.data(), &pointer[100], buffer.size());
    size_t alignment_mask = alignment - 1;
    if (!powerof2(alignment)) {
      alignment_mask = BIONIC_ROUND_UP_POWER_OF_2(alignment) - 1;
    }
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(pointer) & alignment_mask)
        << "Failed at alignment " << alignment << " mask " << alignment_mask;
    memset(pointer, 0xff, 100);
    debug_free(pointer);
  }

  pointer = reinterpret_cast<uint8_t*>(debug_calloc(1, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[100], buffer.size());
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, pointer[i]) << "debug_calloc non-zero byte at " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[100], buffer.size());
  memset(pointer, 0xff, 100);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 200));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[200], buffer.size()) == 0)
      << ShowDiffs(buffer.data(), &pointer[200], buffer.size());
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0xff, pointer[i]) << "debug_realloc not copied byte at " << i;
  }
  memset(pointer, 0xff, 200);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 0));
  ASSERT_TRUE(pointer == nullptr);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, rear_guard_corrupted) {
  Init("rear_guard=32");

  backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200, 0x300});

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  pointer[130] = 0xbf;
  pointer[131] = 0x00;
  debug_free(pointer);

  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p SIZE 100 HAS A CORRUPTED REAR GUARD\n", pointer);
  expected_log += "6 malloc_debug   allocation[130] = 0xbf (expected 0xbb)\n";
  expected_log += "6 malloc_debug   allocation[131] = 0x00 (expected 0xbb)\n";
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x100\n";
  expected_log += "6 malloc_debug   #01 pc 0x200\n";
  expected_log += "6 malloc_debug   #02 pc 0x300\n";
  expected_log += DIVIDER;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, rear_guard_corrupted_after_realloc_shrink) {
  Init("rear_guard=32");

  backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200, 0x300});

  void* pointer = debug_malloc(200);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 200);

  uint8_t* pointer_shrink = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 100));
  pointer_shrink[130] = 0xbf;
  pointer_shrink[131] = 0x00;
  debug_free(pointer);

  // When shrinking sizes, the same pointer should be returned.
  ASSERT_EQ(pointer, pointer_shrink);

  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p SIZE 100 HAS A CORRUPTED REAR GUARD\n", pointer);
  expected_log += "6 malloc_debug   allocation[130] = 0xbf (expected 0xbb)\n";
  expected_log += "6 malloc_debug   allocation[131] = 0x00 (expected 0xbb)\n";
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x100\n";
  expected_log += "6 malloc_debug   #01 pc 0x200\n";
  expected_log += "6 malloc_debug   #02 pc 0x300\n";
  expected_log += DIVIDER;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, tag_corrupted) {
  Init("rear_guard=32");

  backtrace_fake_add(std::vector<uintptr_t> {0xa, 0xb, 0xc});

  backtrace_fake_add(std::vector<uintptr_t> {0xaa, 0xbb, 0xcc});

  backtrace_fake_add(std::vector<uintptr_t> {0xaaa, 0xbbb, 0xccc});

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  uint8_t saved = pointer[-get_tag_offset()];
  pointer[-get_tag_offset()] = 0x00;
  ASSERT_EQ(0U, debug_malloc_usable_size(pointer));
  ASSERT_TRUE(debug_realloc(pointer, 200) == nullptr);
  debug_free(pointer);

  // Fix the pointer and really free it.
  pointer[-get_tag_offset()] = saved;
  debug_free(pointer);

  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p HAS INVALID TAG 1ee7d000 (malloc_usable_size)\n",
      pointer);
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0xa\n";
  expected_log += "6 malloc_debug   #01 pc 0xb\n";
  expected_log += "6 malloc_debug   #02 pc 0xc\n";
  expected_log += DIVIDER;

  expected_log += DIVIDER;
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p HAS INVALID TAG 1ee7d000 (realloc)\n",
      pointer);
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0xaa\n";
  expected_log += "6 malloc_debug   #01 pc 0xbb\n";
  expected_log += "6 malloc_debug   #02 pc 0xcc\n";
  expected_log += DIVIDER;

  expected_log += DIVIDER;
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p HAS INVALID TAG 1ee7d000 (free)\n",
      pointer);
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0xaaa\n";
  expected_log += "6 malloc_debug   #01 pc 0xbbb\n";
  expected_log += "6 malloc_debug   #02 pc 0xccc\n";
  expected_log += DIVIDER;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, leak_track_no_frees) {
  Init("leak_track");

  void* pointer1 = debug_malloc(200);
  ASSERT_TRUE(pointer1 != nullptr);
  memset(pointer1, 0, 200);

  void* pointer2 = debug_malloc(128);
  ASSERT_TRUE(pointer2 != nullptr);
  memset(pointer2, 0, 128);

  void* pointer3 = debug_malloc(1024);
  ASSERT_TRUE(pointer3 != nullptr);
  memset(pointer3, 0, 1024);

  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
        "6 malloc_debug +++ malloc_testing leaked block of size 1024 at %p (leak 1 of 3)\n",
      pointer3);
  expected_log += android::base::StringPrintf(
        "6 malloc_debug +++ malloc_testing leaked block of size 200 at %p (leak 2 of 3)\n",
      pointer1);
  expected_log += android::base::StringPrintf(
        "6 malloc_debug +++ malloc_testing leaked block of size 128 at %p (leak 3 of 3)\n",
      pointer2);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, leak_track_no_frees_with_backtrace) {
  Init("leak_track backtrace");

  backtrace_fake_add(std::vector<uintptr_t> {0x1000, 0x2000, 0x3000});

  void* pointer1 = debug_malloc(100);
  ASSERT_TRUE(pointer1 != nullptr);
  memset(pointer1, 0, 100);

  backtrace_fake_add(std::vector<uintptr_t> {0xa000, 0xb000, 0xc000, 0xd000});

  void* pointer2 = debug_malloc(128);
  ASSERT_TRUE(pointer2 != nullptr);
  memset(pointer2, 0, 128);

  backtrace_fake_add(std::vector<uintptr_t> {0xfe000, 0xde000, 0xce000, 0xbe000, 0xae000});

  void* pointer3 = debug_malloc(1024);
  ASSERT_TRUE(pointer3 != nullptr);
  memset(pointer3, 0, 1024);

  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ malloc_testing leaked block of size 1024 at %p (leak 1 of 3)\n",
      pointer3);
  expected_log += "6 malloc_debug Backtrace at time of allocation:\n";
  expected_log += "6 malloc_debug   #00 pc 0xfe000\n";
  expected_log += "6 malloc_debug   #01 pc 0xde000\n";
  expected_log += "6 malloc_debug   #02 pc 0xce000\n";
  expected_log += "6 malloc_debug   #03 pc 0xbe000\n";
  expected_log += "6 malloc_debug   #04 pc 0xae000\n";

  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ malloc_testing leaked block of size 128 at %p (leak 2 of 3)\n",
      pointer2);
  expected_log += "6 malloc_debug Backtrace at time of allocation:\n";
  expected_log += "6 malloc_debug   #00 pc 0xa000\n";
  expected_log += "6 malloc_debug   #01 pc 0xb000\n";
  expected_log += "6 malloc_debug   #02 pc 0xc000\n";
  expected_log += "6 malloc_debug   #03 pc 0xd000\n";

  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ malloc_testing leaked block of size 100 at %p (leak 3 of 3)\n",
      pointer1);
  expected_log += "6 malloc_debug Backtrace at time of allocation:\n";
  expected_log += "6 malloc_debug   #00 pc 0x1000\n";
  expected_log += "6 malloc_debug   #01 pc 0x2000\n";
  expected_log += "6 malloc_debug   #02 pc 0x3000\n";
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, leak_track_frees) {
  Init("leak_track");

  void* pointer1 = debug_malloc(390);
  ASSERT_TRUE(pointer1 != nullptr);
  memset(pointer1, 0, 390);
  debug_free(pointer1);

  pointer1 = debug_malloc(100);
  ASSERT_TRUE(pointer1 != nullptr);
  memset(pointer1, 0, 100);

  void* pointer2 = debug_malloc(250);
  ASSERT_TRUE(pointer2 != nullptr);
  memset(pointer2, 0, 250);
  debug_free(pointer2);

  pointer2 = debug_malloc(450);
  ASSERT_TRUE(pointer2 != nullptr);
  memset(pointer2, 0, 450);

  void* pointer3 = debug_malloc(999);
  ASSERT_TRUE(pointer3 != nullptr);
  memset(pointer3, 0, 999);
  debug_free(pointer2);

  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "6 malloc_debug +++ malloc_testing leaked block of size 999 at %p (leak 1 of 2)\n",
      pointer3);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ malloc_testing leaked block of size 100 at %p (leak 2 of 2)\n",
      pointer1);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track) {
  Init("free_track=5 free_track_backtrace_num_frames=0");

  void* pointers[10];
  for (size_t i = 0; i < sizeof(pointers) / sizeof(void*); i++) {
    pointers[i] = debug_malloc(100 + i);
    ASSERT_TRUE(pointers[i] != nullptr);
    memset(pointers[i], 0, 100 + i);
    debug_free(pointers[i]);
  }

  // Large allocations (> 4096) to verify large allocation checks.
  void* pointer = debug_malloc(8192);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 8192);
  debug_free(pointer);

  pointer = debug_malloc(9000);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 9000);
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_use_after_free) {
  Init("free_track=5 free_track_backtrace_num_frames=0");

  uint8_t* pointers[5];
  for (size_t i = 0; i < sizeof(pointers) / sizeof(void*); i++) {
    pointers[i] = reinterpret_cast<uint8_t*>(debug_malloc(100 + i));
    ASSERT_TRUE(pointers[i] != nullptr);
    memset(pointers[i], 0, 100 + i);
    debug_free(pointers[i]);
  }

  // Stomp on the data.
  pointers[0][20] = 0xaf;
  pointers[0][99] = 0x12;

  pointers[3][3] = 0x34;

  // Large allocations (> 4096) to verify large allocation checks.
  uint8_t* pointer1_large = reinterpret_cast<uint8_t*>(debug_malloc(8192));
  ASSERT_TRUE(pointer1_large != nullptr);
  memset(pointer1_large, 0, 8192);
  debug_free(pointer1_large);

  pointer1_large[4095] = 0x90;
  pointer1_large[4100] = 0x56;
  pointer1_large[8191] = 0x89;

  uint8_t* pointer2_large = reinterpret_cast<uint8_t*>(debug_malloc(9000));
  ASSERT_TRUE(pointer2_large != nullptr);
  memset(pointer2_large, 0, 9000);
  debug_free(pointer2_large);

  pointer2_large[8200] = 0x78;

  // Do a bunch of alloc and free to verify the above frees are checked.
  for (size_t i = 0; i < 10; i++) {
    void* flush_pointer = debug_malloc(100+i);
    ASSERT_TRUE(flush_pointer != nullptr);
    memset(flush_pointer, 0, 100 + i);
    debug_free(flush_pointer);
  }

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p USED AFTER FREE\n", pointers[0]);
  expected_log += "6 malloc_debug   allocation[20] = 0xaf (expected 0xef)\n";
  expected_log += "6 malloc_debug   allocation[99] = 0x12 (expected 0xef)\n";
  expected_log += DIVIDER;
  expected_log += DIVIDER;
  expected_log += android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p USED AFTER FREE\n", pointers[3]);
  expected_log += "6 malloc_debug   allocation[3] = 0x34 (expected 0xef)\n";
  expected_log += DIVIDER;
  expected_log += DIVIDER;
  expected_log += android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p USED AFTER FREE\n", pointer1_large);
  expected_log += "6 malloc_debug   allocation[4095] = 0x90 (expected 0xef)\n";
  expected_log += "6 malloc_debug   allocation[4100] = 0x56 (expected 0xef)\n";
  expected_log += "6 malloc_debug   allocation[8191] = 0x89 (expected 0xef)\n";
  expected_log += DIVIDER;
  expected_log += DIVIDER;
  expected_log += android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p USED AFTER FREE\n", pointer2_large);
  expected_log += "6 malloc_debug   allocation[8200] = 0x78 (expected 0xef)\n";
  expected_log += DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_use_after_free_finalize) {
  Init("free_track=100 free_track_backtrace_num_frames=0");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 100);
  debug_free(pointer);

  pointer[56] = 0x91;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());

  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p USED AFTER FREE\n", pointer);
  expected_log += "6 malloc_debug   allocation[56] = 0x91 (expected 0xef)\n";
  expected_log += DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_use_after_free_with_backtrace) {
  Init("free_track=100 rear_guard");

  // Free backtrace.
  backtrace_fake_add(std::vector<uintptr_t> {0xfa, 0xeb, 0xdc});

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(200));
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 200);
  debug_free(pointer);

  pointer[101] = 0xab;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());

  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p USED AFTER FREE\n", pointer);
  expected_log += "6 malloc_debug   allocation[101] = 0xab (expected 0xef)\n";
  expected_log += "6 malloc_debug Backtrace at time of free:\n";
  expected_log += "6 malloc_debug   #00 pc 0xfa\n";
  expected_log += "6 malloc_debug   #01 pc 0xeb\n";
  expected_log += "6 malloc_debug   #02 pc 0xdc\n";
  expected_log += DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_use_after_free_call_realloc) {
  Init("free_track=100 rear_guard");

  // Free backtrace.
  backtrace_fake_add(std::vector<uintptr_t> {0xfa, 0xeb, 0xdc});
  // Backtrace at realloc.
  backtrace_fake_add(std::vector<uintptr_t> {0x12, 0x22, 0x32, 0x42});

  void* pointer = debug_malloc(200);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 200);
  debug_free(pointer);

  // Choose a size that should not trigger a realloc to verify tag is
  // verified early.
  ASSERT_TRUE(debug_realloc(pointer, 200) == nullptr);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p USED AFTER FREE (realloc)\n", pointer);
  expected_log += "6 malloc_debug Backtrace of original free:\n";
  expected_log += "6 malloc_debug   #00 pc 0xfa\n";
  expected_log += "6 malloc_debug   #01 pc 0xeb\n";
  expected_log += "6 malloc_debug   #02 pc 0xdc\n";
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x12\n";
  expected_log += "6 malloc_debug   #01 pc 0x22\n";
  expected_log += "6 malloc_debug   #02 pc 0x32\n";
  expected_log += "6 malloc_debug   #03 pc 0x42\n";
  expected_log += DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_use_after_free_call_free) {
  Init("free_track=100 rear_guard");

  // Free backtrace.
  backtrace_fake_add(std::vector<uintptr_t> {0xfa, 0xeb, 0xdc});
  // Backtrace at second free.
  backtrace_fake_add(std::vector<uintptr_t> {0x12, 0x22, 0x32, 0x42});

  void* pointer = debug_malloc(200);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 200);
  debug_free(pointer);

  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p USED AFTER FREE (free)\n", pointer);
  expected_log += "6 malloc_debug Backtrace of original free:\n";
  expected_log += "6 malloc_debug   #00 pc 0xfa\n";
  expected_log += "6 malloc_debug   #01 pc 0xeb\n";
  expected_log += "6 malloc_debug   #02 pc 0xdc\n";
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x12\n";
  expected_log += "6 malloc_debug   #01 pc 0x22\n";
  expected_log += "6 malloc_debug   #02 pc 0x32\n";
  expected_log += "6 malloc_debug   #03 pc 0x42\n";
  expected_log += DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_header_tag_corrupted) {
  Init("free_track=100 free_track_backtrace_num_frames=0 rear_guard");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 100);
  debug_free(pointer);

  pointer[-get_tag_offset()] = 0x00;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());

  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf(
      "6 malloc_debug +++ ALLOCATION %p HAS CORRUPTED HEADER TAG 0x1cc7dc00 AFTER FREE\n",
      pointer);
  expected_log += DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_multiple_thread) {
  Init("free_track=10 free_track_backtrace_num_frames=0");

  std::vector<std::thread*> threads(1000);
  for (size_t i = 0; i < threads.size(); i++) {
    threads[i] = new std::thread([](){
      for (size_t j = 0; j < 100; j++) {
        void* mem = debug_malloc(100);
        write(0, mem, 0);
        debug_free(mem);
      }
    });
  }
  for (size_t i = 0; i < threads.size(); i++) {
    threads[i]->join();
    delete threads[i];
  }

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, free_track_pointer_modified_after_free) {
  Init("free_track=4 fill_on_free=2 free_track_backtrace_num_frames=0");

  void* pointers[5];
  for (size_t i = 0; i < sizeof(pointers) / sizeof(void*); i++) {
    pointers[i] = debug_malloc(100);
    ASSERT_TRUE(pointers[i] != nullptr);
    memset(pointers[i], 0, 100);
  }

  debug_free(pointers[0]);

  // overwrite the whole pointer, only expect errors on the fill bytes we check.
  memset(pointers[0], 0x20, 100);

  for (size_t i = 1; i < sizeof(pointers) / sizeof(void*); i++) {
    debug_free(pointers[i]);
  }

  std::string expected_log(DIVIDER);
  expected_log += android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p USED AFTER FREE\n",
                                              pointers[0]);
  expected_log += "6 malloc_debug   allocation[0] = 0x20 (expected 0xef)\n";
  expected_log += "6 malloc_debug   allocation[1] = 0x20 (expected 0xef)\n";
  expected_log += DIVIDER;
  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_leak_info_invalid) {
  Init("fill");

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  std::string expected_log("6 malloc_debug get_malloc_leak_info: At least one invalid parameter.\n");

  debug_get_malloc_leak_info(nullptr, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  resetLogs();
  debug_get_malloc_leak_info(&info, nullptr, &info_size, &total_memory, &backtrace_size);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  resetLogs();
  debug_get_malloc_leak_info(&info, &overall_size, nullptr, &total_memory, &backtrace_size);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  resetLogs();
  debug_get_malloc_leak_info(&info, &overall_size, &info_size, nullptr, &backtrace_size);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  resetLogs();
  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, nullptr);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_leak_info_not_enabled) {
  Init("fill");

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  std::string expected_log(
      "6 malloc_debug get_malloc_leak_info: Allocations not being tracked, to enable "
      "set the option 'backtrace'.\n");
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

struct InfoEntry {
  size_t size;
  size_t num_allocations;
  uintptr_t frames[0];
} __attribute__((packed));

TEST_F(MallocDebugTest, get_malloc_leak_info_empty) {
  Init("backtrace");

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info == nullptr);
  ASSERT_EQ(0U, overall_size);
  ASSERT_EQ(0U, info_size);
  ASSERT_EQ(0U, total_memory);
  ASSERT_EQ(0U, backtrace_size);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_leak_info_single) {
  Init("backtrace");

  // Create the expected info buffer.
  size_t individual_size = GetInfoEntrySize(16);
  std::vector<uint8_t> expected_info(individual_size);
  memset(expected_info.data(), 0, individual_size);

  InfoEntry* entry = reinterpret_cast<InfoEntry*>(expected_info.data());
  entry->size = 200;
  entry->num_allocations = 1;
  entry->frames[0] = 0xf;
  entry->frames[1] = 0xe;
  entry->frames[2] = 0xd;

  backtrace_fake_add(std::vector<uintptr_t> {0xf, 0xe, 0xd});

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(entry->size));
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, entry->size);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size, overall_size);
  ASSERT_EQ(individual_size, info_size);
  ASSERT_EQ(200U, total_memory);
  ASSERT_EQ(16U, backtrace_size);
  ASSERT_TRUE(memcmp(expected_info.data(), info, overall_size) == 0)
      << ShowDiffs(expected_info.data(), info, overall_size);

  debug_free_malloc_leak_info(info);

  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_leak_info_multi) {
  Init("backtrace=16");

  // Create the expected info buffer.
  size_t individual_size = GetInfoEntrySize(16);
  std::vector<uint8_t> expected_info(individual_size * 3);
  memset(expected_info.data(), 0, individual_size * 3);

  InfoEntry* entry0 = reinterpret_cast<InfoEntry*>(expected_info.data());
  InfoEntry* entry1 = reinterpret_cast<InfoEntry*>(
      reinterpret_cast<uintptr_t>(entry0) + individual_size);
  InfoEntry* entry2 = reinterpret_cast<InfoEntry*>(
      reinterpret_cast<uintptr_t>(entry1) + individual_size);

  // These values will be in the reverse order that we create.
  entry2->size = 500;
  entry2->num_allocations = 1;
  entry2->frames[0] = 0xf;
  entry2->frames[1] = 0xe;
  entry2->frames[2] = 0xd;
  entry2->frames[3] = 0xc;

  backtrace_fake_add(std::vector<uintptr_t> {0xf, 0xe, 0xd, 0xc});

  uint8_t* pointers[3];

  pointers[0] = reinterpret_cast<uint8_t*>(debug_malloc(entry2->size));
  ASSERT_TRUE(pointers[0] != nullptr);
  memset(pointers[0], 0, entry2->size);

  entry1->size = 4100;
  entry1->num_allocations = 1;
  for (size_t i = 0; i < 16; i++) {
    entry1->frames[i] = 0xbc000 + i;
  }

  backtrace_fake_add(
      std::vector<uintptr_t> {0xbc000, 0xbc001, 0xbc002, 0xbc003, 0xbc004, 0xbc005,
                              0xbc006, 0xbc007, 0xbc008, 0xbc009, 0xbc00a, 0xbc00b,
                              0xbc00c, 0xbc00d, 0xbc00e, 0xbc00f, 0xffff});

  pointers[1] = reinterpret_cast<uint8_t*>(debug_malloc(entry1->size));
  ASSERT_TRUE(pointers[1] != nullptr);
  memset(pointers[1], 0, entry1->size);

  entry0->size = 9000;
  entry0->num_allocations = 1;

  entry0->frames[0] = 0x104;
  backtrace_fake_add(std::vector<uintptr_t> {0x104});

  pointers[2] = reinterpret_cast<uint8_t*>(debug_malloc(entry0->size));
  ASSERT_TRUE(pointers[2] != nullptr);
  memset(pointers[2], 0, entry0->size);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size * 3, overall_size);
  ASSERT_EQ(individual_size, info_size);
  ASSERT_EQ(500U + 4100U + 9000U, total_memory);
  ASSERT_EQ(16U, backtrace_size);
  ASSERT_TRUE(memcmp(expected_info.data(), info, overall_size) == 0)
      << ShowDiffs(expected_info.data(), info, overall_size);

  debug_free_malloc_leak_info(info);

  debug_free(pointers[0]);
  debug_free(pointers[1]);
  debug_free(pointers[2]);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_backtrace_with_header) {
  Init("backtrace=16 guard");

  void* pointer = debug_malloc(100);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 100);
  EXPECT_EQ(100U, debug_malloc_usable_size(pointer));

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  EXPECT_TRUE(info != nullptr);
  EXPECT_EQ(GetInfoEntrySize(16), overall_size);
  EXPECT_EQ(GetInfoEntrySize(16), info_size);
  EXPECT_EQ(100U, total_memory);
  EXPECT_EQ(16U, backtrace_size);
  debug_free_malloc_leak_info(info);

  debug_free(pointer);

  // There should be no pointers that have leaked.
  debug_finalize();
  initialized = false;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

static std::string SanitizeHeapData(const std::string& data) {
  // Remove the map data since it's not consistent.
  std::string sanitized;
  bool skip_map_data = false;
  bool map_data_found = false;
  for (auto& line : android::base::Split(data, "\n")) {
    if (skip_map_data) {
      if (line == "END") {
        if (map_data_found) {
          sanitized += "MAP_DATA\n";
          map_data_found = false;
        }
        skip_map_data = false;
      } else {
        map_data_found = true;
        continue;
      }
    }

    if (android::base::StartsWith(line, "Build fingerprint:")) {
      sanitized += "Build fingerprint: ''\n";
    } else {
      if (line == "MAPS") {
        skip_map_data = true;
      }
      sanitized += line + '\n';
    }
  }
  return sanitized;
}

void MallocDebugTest::BacktraceDumpOnSignal(bool trigger_with_alloc) {
  Init("backtrace=4");

  backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200});
  backtrace_fake_add(std::vector<uintptr_t> {0x300, 0x400});
  backtrace_fake_add(std::vector<uintptr_t> {0x500, 0x600});

  backtrace_fake_add(std::vector<uintptr_t> {0xa000, 0xb000});
  backtrace_fake_add(std::vector<uintptr_t> {0xa100, 0xb200});
  backtrace_fake_add(std::vector<uintptr_t> {0xa300, 0xb300});

  std::vector<void*> pointers;
  zygote = 1;
  pointers.push_back(debug_malloc(100));
  ASSERT_TRUE(pointers.back() != nullptr);
  pointers.push_back(debug_malloc(40));
  ASSERT_TRUE(pointers.back() != nullptr);
  pointers.push_back(debug_malloc(200));
  ASSERT_TRUE(pointers.back() != nullptr);

  zygote = 0;
  pointers.push_back(debug_malloc(10));
  ASSERT_TRUE(pointers.back() != nullptr);
  pointers.push_back(debug_malloc(50));
  ASSERT_TRUE(pointers.back() != nullptr);
  pointers.push_back(debug_malloc(5));
  ASSERT_TRUE(pointers.back() != nullptr);

  // Dump all of the data accumulated so far.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 17) == 0);
  sleep(1);

  // This triggers the dumping.
  if (trigger_with_alloc) {
    pointers.push_back(debug_malloc(23));
    ASSERT_TRUE(pointers.back() != nullptr);
  } else {
    debug_free(pointers.back());
    pointers.pop_back();
  }

  for (auto* pointer : pointers) {
    debug_free(pointer);
  }

  // Read all of the contents.
  std::string actual;
  std::string name = android::base::StringPrintf("%s.%d.txt", BACKTRACE_DUMP_PREFIX, getpid());
  ASSERT_TRUE(android::base::ReadFileToString(name, &actual));
  ASSERT_EQ(0, unlink(name.c_str()));

  std::string sanitized(SanitizeHeapData(actual));

  std::string expected =
R"(Android Native Heap Dump v1.2

Build fingerprint: ''

Total memory: 405
Allocation records: 6
Backtrace size: 4

z 0  sz       50  num    1  bt a100 b200
z 0  sz       10  num    1  bt a000 b000
z 0  sz        5  num    1  bt a300 b300
z 1  sz      200  num    1  bt 500 600
z 1  sz      100  num    1  bt 100 200
z 1  sz       40  num    1  bt 300 400
MAPS
MAP_DATA
END

)";
  ASSERT_STREQ(expected.c_str(), sanitized.c_str()) << "Actual data: \n" << actual;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  expected_log += android::base::StringPrintf(
      "6 malloc_debug Dumping to file: /data/local/tmp/backtrace_heap.%d.txt\n\n", getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_dump_on_signal_by_malloc) {
  BacktraceDumpOnSignal(true);
}

TEST_F(MallocDebugTest, backtrace_dump_on_signal_by_free) {
  BacktraceDumpOnSignal(false);
}

TEST_F(MallocDebugTest, backtrace_dump_on_exit) {
  pid_t pid;
  if ((pid = fork()) == 0) {
    Init("backtrace=4 backtrace_dump_on_exit");
    backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200});
    backtrace_fake_add(std::vector<uintptr_t> {0xa000, 0xb000});
    backtrace_fake_add(std::vector<uintptr_t> {0xa000, 0xb000, 0xc000});

    std::vector<void*> pointers;
    pointers.push_back(debug_malloc(300));
    pointers.push_back(debug_malloc(400));
    pointers.push_back(debug_malloc(500));

    // Call the exit function manually.
    debug_finalize();
    exit(0);
  }
  ASSERT_NE(-1, pid);
  ASSERT_EQ(pid, TEMP_FAILURE_RETRY(waitpid(pid, nullptr, 0)));

  // Read all of the contents.
  std::string actual;
  std::string name = android::base::StringPrintf("%s.%d.exit.txt", BACKTRACE_DUMP_PREFIX, pid);
  ASSERT_TRUE(android::base::ReadFileToString(name, &actual));
  ASSERT_EQ(0, unlink(name.c_str()));

  std::string sanitized(SanitizeHeapData(actual));

  std::string expected =
R"(Android Native Heap Dump v1.2

Build fingerprint: ''

Total memory: 1200
Allocation records: 3
Backtrace size: 4

z 0  sz      500  num    1  bt a000 b000 c000
z 0  sz      400  num    1  bt a000 b000
z 0  sz      300  num    1  bt 100 200
MAPS
MAP_DATA
END

)";
  ASSERT_STREQ(expected.c_str(), sanitized.c_str()) << "Actual data: \n" << actual;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_dump_on_exit_shared_backtrace) {
  pid_t pid;
  if ((pid = fork()) == 0) {
    Init("backtrace=4 backtrace_dump_on_exit");
    backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200});
    backtrace_fake_add(std::vector<uintptr_t> {0xa000, 0xb000, 0xc000});
    backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200});

    std::vector<void*> pointers;
    pointers.push_back(debug_malloc(300));
    pointers.push_back(debug_malloc(400));
    pointers.push_back(debug_malloc(300));

    // Call the exit function manually.
    debug_finalize();
    exit(0);
  }
  ASSERT_NE(-1, pid);
  ASSERT_EQ(pid, TEMP_FAILURE_RETRY(waitpid(pid, nullptr, 0)));

  // Read all of the contents.
  std::string actual;
  std::string name = android::base::StringPrintf("%s.%d.exit.txt", BACKTRACE_DUMP_PREFIX, pid);
  ASSERT_TRUE(android::base::ReadFileToString(name, &actual));
  ASSERT_EQ(0, unlink(name.c_str()));

  std::string sanitized(SanitizeHeapData(actual));

  std::string expected =
R"(Android Native Heap Dump v1.2

Build fingerprint: ''

Total memory: 1000
Allocation records: 2
Backtrace size: 4

z 0  sz      400  num    1  bt a000 b000 c000
z 0  sz      300  num    2  bt 100 200
MAPS
MAP_DATA
END

)";
  ASSERT_STREQ(expected.c_str(), sanitized.c_str()) << "Actual data: \n" << actual;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_full_dump_on_exit) {
  pid_t pid;
  if ((pid = fork()) == 0) {
    Init("backtrace=4 backtrace_full backtrace_dump_on_exit");
    BacktraceUnwindFake(
      std::vector<unwindstack::LocalFrameData>{{nullptr, 0x1100, 0x100, "fake1", 10},
                                               {nullptr, 0x1200, 0x200, "fake2", 20}});
    unwindstack::MapInfo map_info{nullptr, 0x10000, 0x20000, 0, PROT_READ | PROT_EXEC, "/data/fake.so"};
    BacktraceUnwindFake(
      std::vector<unwindstack::LocalFrameData>{{&map_info, 0x1a000, 0xa000, "level1", 0},
                                               {&map_info, 0x1b000, 0xb000, "level2", 10}});
    BacktraceUnwindFake(
      std::vector<unwindstack::LocalFrameData>{{nullptr, 0x1a000, 0xa000, "func1", 0},
                                               {nullptr, 0x1b000, 0xb000, "func2", 10},
                                               {nullptr, 0x1c000, 0xc000, "", 30}});

    std::vector<void*> pointers;
    pointers.push_back(debug_malloc(300));
    pointers.push_back(debug_malloc(400));
    pointers.push_back(debug_malloc(500));

    // Call the exit function manually.
    debug_finalize();
    exit(0);
  }
  ASSERT_NE(-1, pid);
  ASSERT_EQ(pid, TEMP_FAILURE_RETRY(waitpid(pid, nullptr, 0)));

  // Read all of the contents.
  std::string actual;
  std::string name = android::base::StringPrintf("%s.%d.exit.txt", BACKTRACE_DUMP_PREFIX, pid);
  ASSERT_TRUE(android::base::ReadFileToString(name, &actual));
  ASSERT_EQ(0, unlink(name.c_str()));

  std::string sanitized(SanitizeHeapData(actual));

  std::string expected =
R"(Android Native Heap Dump v1.2

Build fingerprint: ''

Total memory: 1200
Allocation records: 3
Backtrace size: 4

z 0  sz      500  num    1  bt 1a000 1b000 1c000
  bt_info {"" a000 "func1" 0} {"" b000 "func2" a} {"" c000 "" 0}
z 0  sz      400  num    1  bt 1a000 1b000
  bt_info {"/data/fake.so" a000 "level1" 0} {"/data/fake.so" b000 "level2" a}
z 0  sz      300  num    1  bt 1100 1200
  bt_info {"" 100 "fake1" a} {"" 200 "fake2" 14}
MAPS
MAP_DATA
END

)";
  ASSERT_STREQ(expected.c_str(), sanitized.c_str()) << "Actual data: \n" << actual;

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, realloc_usable_size) {
  Init("front_guard");

  // Verify that if the usable size > size of alloc, that realloc
  // copies the bytes in the usable size not just the size.
  // This assumes that an allocation of size 1 returns usable size > 1.
  // If this isn't true, this test is not going to do anything.
  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(1));
  ASSERT_TRUE(pointer != nullptr);
  size_t usable_size = debug_malloc_usable_size(pointer);
  memset(pointer, 0xaa, usable_size);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, usable_size + 10));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(usable_size + 10, debug_malloc_usable_size(pointer));
  for (size_t i = 0; i < usable_size; i++) {
    ASSERT_EQ(0xaa, pointer[i]) << "Failed compare at byte " << i;
  }
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_enable_on_signal) {
  Init("backtrace_enable_on_signal=20");

  size_t individual_size = GetInfoEntrySize(20);

  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200, 0x300, 0x400});
  backtrace_fake_add(std::vector<uintptr_t> {0x500, 0xa00, 0xb00});

  // First allocation should not actually attempt to get the backtrace.
  void* pointer = debug_malloc(10);
  ASSERT_TRUE(pointer != nullptr);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info == nullptr);
  ASSERT_EQ(0U, overall_size);
  ASSERT_EQ(0U, info_size);
  ASSERT_EQ(0U, total_memory);
  ASSERT_EQ(0U, backtrace_size);
  debug_free(pointer);

  debug_free_malloc_leak_info(info);

  // Send the signal to enable.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 19) == 0);
  sleep(1);

  pointer = debug_malloc(100);
  ASSERT_TRUE(pointer != nullptr);

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size, overall_size);
  ASSERT_EQ(individual_size, info_size);
  ASSERT_EQ(100U, total_memory);
  ASSERT_EQ(20U, backtrace_size);
  uintptr_t* ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t)]);
  ASSERT_EQ(0xbc000U, ips[0]);
  ASSERT_EQ(0xecd00U, ips[1]);
  ASSERT_EQ(0x12000U, ips[2]);
  for (size_t i = 3; i < 20; i++) {
    ASSERT_EQ(0U, ips[i]);
  }

  debug_free(pointer);

  debug_free_malloc_leak_info(info);

  // Send the signal to disable.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 19) == 0);
  sleep(1);

  pointer = debug_malloc(200);
  ASSERT_TRUE(pointer != nullptr);

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info == nullptr);
  ASSERT_EQ(0U, overall_size);
  ASSERT_EQ(0U, info_size);
  ASSERT_EQ(0U, total_memory);
  ASSERT_EQ(0U, backtrace_size);

  debug_free(pointer);

  debug_free_malloc_leak_info(info);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to enable backtracing.\n",
      SIGRTMAX - 19, getpid());
  expected_log += android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_same_stack) {
  Init("backtrace=4");

  size_t individual_size = GetInfoEntrySize(4);

  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});

  void* pointers[4];
  pointers[0] = debug_malloc(10);
  ASSERT_TRUE(pointers[0] != nullptr);
  pointers[1] = debug_malloc(10);
  ASSERT_TRUE(pointers[1] != nullptr);
  pointers[2] = debug_malloc(10);
  ASSERT_TRUE(pointers[2] != nullptr);
  pointers[3] = debug_malloc(100);
  ASSERT_TRUE(pointers[3] != nullptr);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size * 2, overall_size);
  ASSERT_EQ(individual_size, info_size);
  EXPECT_EQ(130U, total_memory);
  EXPECT_EQ(4U, backtrace_size);
  EXPECT_EQ(100U, *reinterpret_cast<size_t*>(&info[0]));
  EXPECT_EQ(1U, *reinterpret_cast<size_t*>(&info[sizeof(size_t)]));
  uintptr_t* ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t)]);
  EXPECT_EQ(0xbc000U, ips[0]);
  EXPECT_EQ(0xecd00U, ips[1]);
  EXPECT_EQ(0x12000U, ips[2]);

  EXPECT_EQ(10U, *reinterpret_cast<size_t*>(&info[individual_size]));
  EXPECT_EQ(3U, *reinterpret_cast<size_t*>(&info[sizeof(size_t) + individual_size]));
  ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t) + individual_size]);
  EXPECT_EQ(0xbc000U, ips[0]);
  EXPECT_EQ(0xecd00U, ips[1]);
  EXPECT_EQ(0x12000U, ips[2]);

  debug_free_malloc_leak_info(info);

  debug_free(pointers[0]);
  debug_free(pointers[1]);
  debug_free(pointers[2]);
  debug_free(pointers[3]);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_same_stack_zygote) {
  Init("backtrace=4");

  size_t individual_size = GetInfoEntrySize(4);

  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000});

  zygote = 1;

  void* pointers[4];
  pointers[0] = debug_malloc(100);
  ASSERT_TRUE(pointers[0] != nullptr);
  pointers[1] = debug_malloc(100);
  ASSERT_TRUE(pointers[1] != nullptr);
  pointers[2] = debug_malloc(100);
  ASSERT_TRUE(pointers[2] != nullptr);
  pointers[3] = debug_malloc(100);
  ASSERT_TRUE(pointers[3] != nullptr);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size * 2, overall_size);
  EXPECT_EQ(individual_size, info_size);
  EXPECT_EQ(400U, total_memory);
  EXPECT_EQ(4U, backtrace_size);

  EXPECT_EQ(0x80000064U, *reinterpret_cast<size_t*>(&info[0]));
  EXPECT_EQ(3U, *reinterpret_cast<size_t*>(&info[sizeof(size_t)]));
  uintptr_t* ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t)]);
  EXPECT_EQ(0xbc000U, ips[0]);
  EXPECT_EQ(0xecd00U, ips[1]);
  EXPECT_EQ(0x12000U, ips[2]);

  EXPECT_EQ(0x80000064U, *reinterpret_cast<size_t*>(&info[individual_size]));
  EXPECT_EQ(1U, *reinterpret_cast<size_t*>(&info[sizeof(size_t) + individual_size]));
  ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t) + individual_size]);
  EXPECT_EQ(0xbc000U, ips[0]);
  EXPECT_EQ(0U, ips[1]);

  debug_free_malloc_leak_info(info);

  debug_free(pointers[0]);
  debug_free(pointers[1]);
  debug_free(pointers[2]);
  debug_free(pointers[3]);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_same_stack_mix_zygote) {
  Init("backtrace=4");

  size_t individual_size = GetInfoEntrySize(4);

  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000, 0xecd00, 0x12000});
  backtrace_fake_add(std::vector<uintptr_t> {0xbc000});

  zygote = 1;
  void* pointers[4];
  pointers[0] = debug_malloc(40);
  ASSERT_TRUE(pointers[0] != nullptr);
  pointers[1] = debug_malloc(40);
  ASSERT_TRUE(pointers[1] != nullptr);

  zygote = 0;
  pointers[2] = debug_malloc(40);
  ASSERT_TRUE(pointers[2] != nullptr);
  pointers[3] = debug_malloc(100);
  ASSERT_TRUE(pointers[3] != nullptr);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size * 3, overall_size);
  ASSERT_EQ(individual_size, info_size);
  EXPECT_EQ(220U, total_memory);
  EXPECT_EQ(4U, backtrace_size);

  EXPECT_EQ(100U, *reinterpret_cast<size_t*>(&info[0]));
  EXPECT_EQ(1U, *reinterpret_cast<size_t*>(&info[sizeof(size_t)]));
  uintptr_t* ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t)]);
  EXPECT_EQ(0xbc000U, ips[0]);
  EXPECT_EQ(0U, ips[1]);

  EXPECT_EQ(40U, *reinterpret_cast<size_t*>(&info[individual_size]));
  EXPECT_EQ(1U, *reinterpret_cast<size_t*>(&info[sizeof(size_t) + individual_size]));
  ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t) + individual_size]);
  EXPECT_EQ(0xbc000U, ips[0]);
  EXPECT_EQ(0xecd00U, ips[1]);
  EXPECT_EQ(0x12000U, ips[2]);

  EXPECT_EQ(0x80000028U, *reinterpret_cast<size_t*>(&info[2 * individual_size]));
  EXPECT_EQ(2U, *reinterpret_cast<size_t*>(&info[sizeof(size_t) + 2 * individual_size]));
  ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t) + 2 * individual_size]);
  EXPECT_EQ(0xbc000U, ips[0]);
  EXPECT_EQ(0xecd00U, ips[1]);
  EXPECT_EQ(0x12000U, ips[2]);

  debug_free_malloc_leak_info(info);

  debug_free(pointers[0]);
  debug_free(pointers[1]);
  debug_free(pointers[2]);
  debug_free(pointers[3]);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, backtrace_frame_data_nullptr_same_size) {
  Init("backtrace=4");

  size_t individual_size = GetInfoEntrySize(4);

  void* pointers[4];
  pointers[0] = debug_malloc(100);
  ASSERT_TRUE(pointers[0] != nullptr);
  pointers[1] = debug_malloc(100);
  ASSERT_TRUE(pointers[1] != nullptr);
  pointers[2] = debug_malloc(100);
  ASSERT_TRUE(pointers[2] != nullptr);
  pointers[3] = debug_malloc(100);
  ASSERT_TRUE(pointers[3] != nullptr);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size, overall_size);
  EXPECT_EQ(individual_size, info_size);
  EXPECT_EQ(400U, total_memory);
  EXPECT_EQ(4U, backtrace_size);

  EXPECT_EQ(100U, *reinterpret_cast<size_t*>(&info[0]));
  EXPECT_EQ(4U, *reinterpret_cast<size_t*>(&info[sizeof(size_t)]));
  uintptr_t* ips = reinterpret_cast<uintptr_t*>(&info[2 * sizeof(size_t)]);
  EXPECT_EQ(0U, ips[0]);

  debug_free_malloc_leak_info(info);

  debug_free(pointers[0]);
  debug_free(pointers[1]);
  debug_free(pointers[2]);
  debug_free(pointers[3]);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, overflow) {
  Init("guard fill_on_free");

  void* pointer = debug_malloc(SIZE_MAX);
  ASSERT_TRUE(pointer == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  pointer = debug_calloc(1, SIZE_MAX);
  ASSERT_TRUE(pointer == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  pointer = debug_calloc(SIZE_MAX, 1);
  ASSERT_TRUE(pointer == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  pointer = debug_calloc(SIZE_MAX/100, 100);
  ASSERT_TRUE(pointer == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  pointer = debug_calloc(100, SIZE_MAX/100);
  ASSERT_TRUE(pointer == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  const size_t size_t_bits = sizeof(size_t) * 8;
  const size_t sqrt_size_t = 1ULL << (size_t_bits/2);
  pointer = debug_calloc(sqrt_size_t + 1, sqrt_size_t);
  ASSERT_TRUE(pointer == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  pointer = debug_realloc(nullptr, SIZE_MAX);
  ASSERT_TRUE(pointer == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  pointer = debug_malloc(100);
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0xd0, 100);

  void* realloc_pointer = debug_realloc(pointer, SIZE_MAX);
  ASSERT_TRUE(realloc_pointer == nullptr);
  // Verify the pointer was not freed.
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0xd0, reinterpret_cast<uint8_t*>(pointer)[i]) << "Failed checking byte " << i;
  }
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

static void VerifyZygoteSet(size_t memory_bytes) {
  size_t expected_info_size = 2 * sizeof(size_t) + 16 * sizeof(uintptr_t);
  std::vector<uint8_t> expected_info(expected_info_size);
  memset(expected_info.data(), 0, expected_info_size);
  InfoEntry* entry = reinterpret_cast<InfoEntry*>(expected_info.data());
  entry->size = memory_bytes | (1U << 31);
  entry->num_allocations = 1;
  entry->frames[0] = 0x1;

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_EQ(expected_info_size, overall_size);
  ASSERT_EQ(expected_info_size, info_size);
  ASSERT_EQ(memory_bytes, total_memory);
  ASSERT_EQ(16U, backtrace_size);
  ASSERT_TRUE(memcmp(info, expected_info.data(), expected_info_size) == 0)
      << ShowDiffs(info, expected_info.data(), expected_info_size);

  debug_free_malloc_leak_info(info);
}

TEST_F(MallocDebugTest, zygote_set) {
  // Set all of the options.
  Init("guard fill backtrace leak_track free_track=2");

  zygote = 1;

  backtrace_fake_add(std::vector<uintptr_t> {0x1});

  void* pointer = debug_malloc(100);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
  memset(pointer, 0, 100);
  VerifyZygoteSet(100);
  ASSERT_FALSE(HasFatalFailure());
  debug_free(pointer);

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_calloc(10, 20);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(200U, debug_malloc_usable_size(pointer));
  VerifyZygoteSet(200);
  ASSERT_FALSE(HasFatalFailure());
  debug_free(pointer);

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_memalign(128, 300);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(300U, debug_malloc_usable_size(pointer));
  memset(pointer, 0, 300);
  VerifyZygoteSet(300);
  ASSERT_FALSE(HasFatalFailure());
  debug_free(pointer);

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_malloc(500);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(500U, debug_malloc_usable_size(pointer));
  memset(pointer, 0, 500);
  VerifyZygoteSet(500);
  ASSERT_FALSE(HasFatalFailure());

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_realloc(pointer, 300);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(300U, debug_malloc_usable_size(pointer));
  VerifyZygoteSet(300);
  ASSERT_FALSE(HasFatalFailure());
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the backtrace.\n",
      SIGRTMAX - 17, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, max_size) {
  Init("guard");

  void* pointer = debug_malloc(1U << 31);
  ASSERT_TRUE(pointer == nullptr);

  pointer = debug_calloc(1, 1U << 31);
  ASSERT_TRUE(pointer == nullptr);

  pointer = debug_calloc(1U << 31, 1);
  ASSERT_TRUE(pointer == nullptr);

  pointer = debug_memalign(16, 1U << 31);
  ASSERT_TRUE(pointer == nullptr);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, debug_mallinfo) {
  Init("guard");

  void* pointer = debug_malloc(150);
  ASSERT_TRUE(pointer != nullptr);

  struct mallinfo mi = debug_mallinfo();
  EXPECT_NE(0U, mi.uordblks);

  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, debug_mallopt) {
  Init("guard");

  void* pointer = debug_malloc(150);
  ASSERT_TRUE(pointer != nullptr);

  EXPECT_EQ(0, debug_mallopt(-1000, 1));

  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, debug_posix_memalign) {
  Init("guard");

  void* pointer;
  ASSERT_EQ(0, debug_posix_memalign(&pointer, 32, 300));
  ASSERT_TRUE(pointer != nullptr);
  debug_free(pointer);

  ASSERT_EQ(EINVAL, debug_posix_memalign(&pointer, 11, 300));

  ASSERT_EQ(ENOMEM, debug_posix_memalign(&pointer, 16, SIZE_MAX));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
TEST_F(MallocDebugTest, debug_pvalloc) {
  Init("guard");

  size_t pagesize = getpagesize();
  void* pointer = debug_pvalloc(1);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(pagesize, debug_malloc_usable_size(pointer));
  uintptr_t value = reinterpret_cast<uintptr_t>(pointer) & (pagesize - 1);
  ASSERT_EQ(0U, value);
  debug_free(pointer);
}

TEST_F(MallocDebugTest, debug_valloc) {
  Init("guard");

  size_t pagesize = getpagesize();
  void* pointer = debug_valloc(100);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
  uintptr_t value = reinterpret_cast<uintptr_t>(pointer) & (pagesize - 1);
  ASSERT_EQ(0U, value);
  debug_free(pointer);
}
#endif

void VerifyRecordAllocs() {
  std::string expected;

  void* pointer = debug_malloc(10);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 10\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  pointer = debug_calloc(1, 20);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: calloc %p 20 1\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  pointer = debug_realloc(nullptr, 30);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: realloc %p 0x0 30\n", getpid(), pointer);
  void* old_pointer = pointer;
  pointer = debug_realloc(pointer, 2048);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: realloc %p %p 2048\n", getpid(),
                                          pointer, old_pointer);
  debug_realloc(pointer, 0);
  expected += android::base::StringPrintf("%d: realloc 0x0 %p 0\n", getpid(), pointer);

  pointer = debug_memalign(16, 40);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: memalign %p 16 40\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  pointer = debug_aligned_alloc(32, 50);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: memalign %p 32 50\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  ASSERT_EQ(0, debug_posix_memalign(&pointer, 32, 50));
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: memalign %p 32 50\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  pointer = debug_pvalloc(60);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: memalign %p 4096 4096\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  pointer = debug_valloc(70);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: memalign %p 4096 70\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);
#endif

  // Dump all of the data accumulated so far.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 18) == 0);
  sleep(1);

  // This triggers the dumping.
  pointer = debug_malloc(110);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 110\n", getpid(), pointer);

  // Read all of the contents.
  std::string actual;
  ASSERT_TRUE(android::base::ReadFileToString(RECORD_ALLOCS_FILE, &actual));
  ASSERT_EQ(0, unlink(RECORD_ALLOCS_FILE));

  ASSERT_STREQ(expected.c_str(), actual.c_str());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the allocation records.\n",
      SIGRTMAX - 18, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  debug_free(pointer);
}

TEST_F(MallocDebugTest, record_allocs_no_header) {
  Init("record_allocs");

  VerifyRecordAllocs();
}

TEST_F(MallocDebugTest, record_allocs_with_header) {
  Init("record_allocs front_guard");

  VerifyRecordAllocs();
}

TEST_F(MallocDebugTest, record_allocs_max) {
  Init("record_allocs=5");

  std::string expected;

  void* pointer = debug_malloc(10);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 10\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  pointer = debug_malloc(20);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 20\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  pointer = debug_malloc(1024);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 1024\n", getpid(), pointer);
  debug_free(pointer);

  // Dump all of the data accumulated so far.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 18) == 0);
  sleep(1);

  // This triggers the dumping.
  pointer = debug_malloc(110);
  ASSERT_TRUE(pointer != nullptr);

  // Read all of the contents.
  std::string actual;
  ASSERT_TRUE(android::base::ReadFileToString(RECORD_ALLOCS_FILE, &actual));
  ASSERT_EQ(0, unlink(RECORD_ALLOCS_FILE));

  ASSERT_STREQ(expected.c_str(), actual.c_str());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the allocation records.\n",
      SIGRTMAX - 18, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  debug_free(pointer);
}

TEST_F(MallocDebugTest, record_allocs_thread_done) {
  Init("record_allocs=5");

  static pid_t tid = 0;
  static void* pointer = nullptr;
  std::thread thread([](){
    tid = gettid();
    pointer = debug_malloc(100);
    write(0, pointer, 0);
    debug_free(pointer);
  });
  thread.join();

  std::string expected = android::base::StringPrintf("%d: malloc %p 100\n", tid, pointer);
  expected += android::base::StringPrintf("%d: free %p\n", tid, pointer);
  expected += android::base::StringPrintf("%d: thread_done 0x0\n", tid);

  // Dump all of the data accumulated so far.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 18) == 0);
  sleep(1);

  // This triggers the dumping.
  pointer = debug_malloc(23);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 23\n", getpid(), pointer);

  // Read all of the contents.
  std::string actual;
  ASSERT_TRUE(android::base::ReadFileToString(RECORD_ALLOCS_FILE, &actual));
  ASSERT_EQ(0, unlink(RECORD_ALLOCS_FILE));

  ASSERT_STREQ(expected.c_str(), actual.c_str());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the allocation records.\n",
      SIGRTMAX - 18, getpid());
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  debug_free(pointer);
}

TEST_F(MallocDebugTest, record_allocs_file_name_fail) {
  Init("record_allocs=5");

  // Delete the special.txt file and create a symbolic link there to
  // make sure the create file will fail.
  unlink(RECORD_ALLOCS_FILE);

  ASSERT_EQ(0, symlink("/data/local/tmp/does_not_exist", RECORD_ALLOCS_FILE));

  std::string expected;

  void* pointer = debug_malloc(10);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 10\n", getpid(), pointer);
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  // Dump all of the data accumulated so far.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 18) == 0);
  sleep(1);

  // This triggers the dumping.
  pointer = debug_malloc(110);
  ASSERT_TRUE(pointer != nullptr);
  expected += android::base::StringPrintf("%d: malloc %p 110\n", getpid(), pointer);

  // Read all of the contents.
  std::string actual;
  ASSERT_FALSE(android::base::ReadFileToString(RECORD_ALLOCS_FILE, &actual));

  // Unlink the file so the next dump passes.
  ASSERT_EQ(0, unlink(RECORD_ALLOCS_FILE));

  // Dump all of the data accumulated so far.
  ASSERT_TRUE(kill(getpid(), SIGRTMAX - 18) == 0);
  sleep(1);

  // This triggers the dumping.
  debug_free(pointer);
  expected += android::base::StringPrintf("%d: free %p\n", getpid(), pointer);

  ASSERT_TRUE(android::base::ReadFileToString(RECORD_ALLOCS_FILE, &actual));
  ASSERT_EQ(0, unlink(RECORD_ALLOCS_FILE));
  ASSERT_STREQ(expected.c_str(), actual.c_str());

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string expected_log = android::base::StringPrintf(
      "4 malloc_debug malloc_testing: Run: 'kill -%d %d' to dump the allocation records.\n",
      SIGRTMAX - 18, getpid());
  expected_log += android::base::StringPrintf(
      "6 malloc_debug Cannot create record alloc file %s: Too many symbolic links encountered\n",
      RECORD_ALLOCS_FILE);
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, verify_pointers) {
  Init("verify_pointers");

  void* pointer = debug_malloc(10);
  memset(pointer, 0, 10);
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());

  debug_free(pointer);
  ASSERT_EQ(0U, debug_malloc_usable_size(pointer));
  ASSERT_EQ(nullptr, debug_realloc(pointer, 1000));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  std::string free_pointer_str(
      android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p UNKNOWN POINTER (free)\n",
                                  pointer));
  std::string usable_pointer_str(
      android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p UNKNOWN POINTER (malloc_usable_size)\n",
                                  pointer));
  std::string realloc_pointer_str(
      android::base::StringPrintf("6 malloc_debug +++ ALLOCATION %p UNKNOWN POINTER (realloc)\n",
                                  pointer));
  std::string backtrace_str("6 malloc_debug Backtrace at time of failure:\n");
  backtrace_str += "6 malloc_debug   Backtrace failed to get any frames.\n";

  std::string expected_log(DIVIDER + free_pointer_str + backtrace_str + DIVIDER);
  expected_log += DIVIDER + usable_pointer_str + backtrace_str + DIVIDER;
  expected_log += DIVIDER + realloc_pointer_str + backtrace_str + DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());

  resetLogs();

  backtrace_fake_add(std::vector<uintptr_t> {0x100, 0x200});
  backtrace_fake_add(std::vector<uintptr_t> {0x300, 0x400});
  backtrace_fake_add(std::vector<uintptr_t> {0x500, 0x600});
  debug_free(pointer);
  ASSERT_EQ(0U, debug_malloc_usable_size(pointer));
  ASSERT_EQ(nullptr, debug_realloc(pointer, 1000));

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  expected_log = DIVIDER + free_pointer_str;
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x100\n";
  expected_log += "6 malloc_debug   #01 pc 0x200\n";
  expected_log += DIVIDER;
  expected_log += DIVIDER + usable_pointer_str;
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x300\n";
  expected_log += "6 malloc_debug   #01 pc 0x400\n";
  expected_log += DIVIDER;
  expected_log += DIVIDER + realloc_pointer_str;
  expected_log += "6 malloc_debug Backtrace at time of failure:\n";
  expected_log += "6 malloc_debug   #00 pc 0x500\n";
  expected_log += "6 malloc_debug   #01 pc 0x600\n";
  expected_log += DIVIDER;
  ASSERT_STREQ(expected_log.c_str(), getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, abort_on_error_log_error) {
  Init("abort_on_error verify_pointers");

  void* pointer = debug_malloc(10);
  memset(pointer, 0, 10);
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());

  EXPECT_DEATH(debug_free(pointer), "");
}

TEST_F(MallocDebugTest, abort_on_error_guard_corrupted) {
  Init("abort_on_error front_guard=32");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  pointer[-16] = 0x00;
  EXPECT_DEATH(debug_free(pointer), "");
  pointer[-16] = 0xaa;
  debug_free(pointer);
}

TEST_F(MallocDebugTest, abort_on_error_use_after_free) {
  Init("abort_on_error free_track=100 free_track_backtrace_num_frames=0");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 100);
  debug_free(pointer);

  pointer[56] = 0x91;

  EXPECT_DEATH(debug_finalize(), "");

  pointer[56] = 0xef;
}

TEST_F(MallocDebugTest, abort_on_error_header_tag_corrupted) {
  Init("abort_on_error free_track=100 free_track_backtrace_num_frames=0 rear_guard");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  memset(pointer, 0, 100);
  debug_free(pointer);

  uint8_t tag_value = pointer[-get_tag_offset()];
  pointer[-get_tag_offset()] = 0x00;

  EXPECT_DEATH(debug_finalize(), "");

  pointer[-get_tag_offset()] = tag_value;
}

