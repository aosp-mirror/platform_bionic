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
size_t debug_malloc_usable_size(void*);
void debug_get_malloc_leak_info(uint8_t**, size_t*, size_t*, size_t*, size_t*);
void debug_free_malloc_leak_info(uint8_t*);

struct mallinfo debug_mallinfo();

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* debug_pvalloc(size_t);
void* debug_valloc(size_t);
#endif

__END_DECLS

constexpr char DIVIDER[] =
    "6 malloc_debug *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***\n";

constexpr uint32_t BACKTRACE_HEADER = 0x1;

static size_t get_tag_offset(uint32_t flags = 0, size_t backtrace_frames = 0) {
  size_t offset = BIONIC_ALIGN(sizeof(Header), MINIMUM_ALIGNMENT_BYTES);
  if (flags & BACKTRACE_HEADER) {
    offset += BIONIC_ALIGN(sizeof(BacktraceHeader) + sizeof(uintptr_t) * backtrace_frames, MINIMUM_ALIGNMENT_BYTES);
  }
  return offset;
}

static constexpr const char RECORD_ALLOCS_FILE[] = "/data/local/tmp/record_allocs.txt";

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
};

void VerifyAllocCalls() {
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
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, fill_generic) {
  Init("fill");
  VerifyAllocCalls();
}

TEST_F(MallocDebugTest, fill_on_alloc_generic) {
  Init("fill_on_alloc");
  VerifyAllocCalls();
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
  VerifyAllocCalls();
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
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  debug_free(pointer);

  // Loop through a bunch alignments.
  for (size_t alignment = 1; alignment <= 256; alignment++) {
    pointer = reinterpret_cast<uint8_t*>(debug_memalign(alignment, 100));
    ASSERT_TRUE(pointer != nullptr);
    ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
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
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, pointer[i]) << "debug_calloc non-zero byte at " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 200));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
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
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  debug_free(pointer);

  // Loop through a bunch alignments.
  for (size_t alignment = 1; alignment <= 256; alignment++) {
    pointer = reinterpret_cast<uint8_t*>(debug_memalign(alignment, 100));
    ASSERT_TRUE(pointer != nullptr);
    ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
    ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
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
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, pointer[i]) << "debug_calloc non-zero byte at " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 200));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[200], buffer.size()) == 0);
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
  Init("free_track=100");

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
  Init("free_track=100");

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
  Init("free_track=100");

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
  Init("free_track=100 free_track_backtrace_num_frames=0");

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
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_leak_info_single) {
  Init("backtrace");

  // Create the expected info buffer.
  size_t individual_size = 2 * sizeof(size_t) + 16 * sizeof(uintptr_t);
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
  ASSERT_TRUE(memcmp(expected_info.data(), info, overall_size) == 0);

  debug_free_malloc_leak_info(info);

  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_leak_info_multi) {
  Init("backtrace=16");

  // Create the expected info buffer.
  size_t individual_size = 2 * sizeof(size_t) + 16 * sizeof(uintptr_t);
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
  ASSERT_TRUE(memcmp(expected_info.data(), info, overall_size) == 0);

  debug_free_malloc_leak_info(info);

  debug_free(pointers[0]);
  debug_free(pointers[1]);
  debug_free(pointers[2]);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
}

TEST_F(MallocDebugTest, get_malloc_leak_info_multi_skip_empty_backtrace) {
  Init("backtrace=16");

  // Create the expected info buffer.
  size_t individual_size = 2 * sizeof(size_t) + 16 * sizeof(uintptr_t);
  std::vector<uint8_t> expected_info(individual_size * 2);
  memset(expected_info.data(), 0, individual_size * 2);

  InfoEntry* entry0 = reinterpret_cast<InfoEntry*>(expected_info.data());
  InfoEntry* entry1 = reinterpret_cast<InfoEntry*>(
      reinterpret_cast<uintptr_t>(entry0) + individual_size);

  // These values will be in the reverse order that we create.
  entry1->size = 500;
  entry1->num_allocations = 1;
  entry1->frames[0] = 0xf;
  entry1->frames[1] = 0xe;
  entry1->frames[2] = 0xd;
  entry1->frames[3] = 0xc;

  backtrace_fake_add(std::vector<uintptr_t> {0xf, 0xe, 0xd, 0xc});

  uint8_t* pointers[3];

  pointers[0] = reinterpret_cast<uint8_t*>(debug_malloc(entry1->size));
  ASSERT_TRUE(pointers[0] != nullptr);
  memset(pointers[0], 0, entry1->size);

  entry0->size = 4100;
  entry0->num_allocations = 1;
  for (size_t i = 0; i < 16; i++) {
    entry0->frames[i] = 0xbc000 + i;
  }

  backtrace_fake_add(
      std::vector<uintptr_t> {0xbc000, 0xbc001, 0xbc002, 0xbc003, 0xbc004, 0xbc005,
                              0xbc006, 0xbc007, 0xbc008, 0xbc009, 0xbc00a, 0xbc00b,
                              0xbc00c, 0xbc00d, 0xbc00e, 0xbc00f, 0xffff});

  pointers[1] = reinterpret_cast<uint8_t*>(debug_malloc(entry0->size));
  ASSERT_TRUE(pointers[1] != nullptr);
  memset(pointers[1], 0, entry0->size);

  pointers[2] = reinterpret_cast<uint8_t*>(debug_malloc(10000));
  ASSERT_TRUE(pointers[2] != nullptr);
  memset(pointers[2], 0, 10000);

  uint8_t* info;
  size_t overall_size;
  size_t info_size;
  size_t total_memory;
  size_t backtrace_size;

  debug_get_malloc_leak_info(&info, &overall_size, &info_size, &total_memory, &backtrace_size);
  ASSERT_TRUE(info != nullptr);
  ASSERT_EQ(individual_size * 2, overall_size);
  ASSERT_EQ(individual_size, info_size);
  ASSERT_EQ(500U + 4100U, total_memory);
  ASSERT_EQ(16U, backtrace_size);
  ASSERT_TRUE(memcmp(expected_info.data(), info, overall_size) == 0);

  debug_free_malloc_leak_info(info);

  debug_free(pointers[0]);
  debug_free(pointers[1]);
  debug_free(pointers[2]);

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

  size_t individual_size = 2 * sizeof(size_t) + 20 * sizeof(uintptr_t);

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
  ASSERT_TRUE(memcmp(info, expected_info.data(), expected_info_size) == 0);

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
  debug_free(pointer);

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_calloc(10, 20);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(200U, debug_malloc_usable_size(pointer));
  VerifyZygoteSet(200);
  debug_free(pointer);

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_memalign(128, 300);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(300U, debug_malloc_usable_size(pointer));
  memset(pointer, 0, 300);
  VerifyZygoteSet(300);
  debug_free(pointer);

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_malloc(500);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(500U, debug_malloc_usable_size(pointer));
  memset(pointer, 0, 500);
  VerifyZygoteSet(500);

  backtrace_fake_add(std::vector<uintptr_t> {0x1});
  pointer = debug_realloc(pointer, 300);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(300U, debug_malloc_usable_size(pointer));
  VerifyZygoteSet(300);
  debug_free(pointer);

  ASSERT_STREQ("", getFakeLogBuf().c_str());
  ASSERT_STREQ("", getFakeLogPrint().c_str());
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
