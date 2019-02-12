/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <gtest/gtest.h>
#include "buffer_tests.h"
#include "utils.h"

// For the comparison buffer tests, the maximum length to test for the
// miscompare checks.
#define MISCMP_MAX_LENGTH 512

#define FENCEPOST_LENGTH 8

static int g_single_aligns[][2] = {
  // Both buffers at same alignment.
  { 1, 0 },
  { 2, 0 },
  { 4, 0 },
  { 8, 0 },
  { 16, 0 },
  { 32, 0 },
  { 64, 0 },
  { 128, 0 },

  // General unaligned cases.
  { 4, 1 },
  { 4, 2 },
  { 4, 3 },

  { 8, 1 },
  { 8, 2 },
  { 8, 3 },
  { 8, 4 },
  { 8, 5 },
  { 8, 6 },
  { 8, 7 },

  { 128, 1 },
  { 128, 4 },
  { 128, 8 },
  { 128, 12 },
  { 128, 16 },
};

static const size_t g_single_aligns_len = sizeof(g_single_aligns)/sizeof(int[2]);

// Set of multiple buffer alignment combinations to be used for string/memory
// testing routines.
static int g_double_aligns[][4] = {
  // Both buffers at same alignment.
  { 1, 0, 1, 0 },
  { 2, 0, 2, 0 },
  { 4, 0, 4, 0 },
  { 8, 0, 8, 0 },
  { 16, 0, 16, 0 },
  { 32, 0, 32, 0 },
  { 64, 0, 64, 0 },
  { 128, 0, 128, 0 },

  // Different word alignments between buffers.
  { 8, 0, 4, 0 },
  { 4, 0, 8, 0 },
  { 16, 0, 4, 0 },
  { 4, 0, 16, 0 },

  // General unaligned cases.
  { 4, 0, 4, 1 },
  { 4, 0, 4, 2 },
  { 4, 0, 4, 3 },

  { 4, 1, 4, 0 },
  { 4, 1, 4, 1 },
  { 4, 1, 4, 2 },
  { 4, 1, 4, 3 },

  { 4, 2, 4, 0 },
  { 4, 2, 4, 1 },
  { 4, 2, 4, 2 },
  { 4, 2, 4, 3 },

  { 4, 3, 4, 0 },
  { 4, 3, 4, 1 },
  { 4, 3, 4, 2 },
  { 4, 3, 4, 3 },

  { 8, 0, 8, 1 },
  { 8, 0, 8, 2 },
  { 8, 0, 8, 3 },
  { 8, 0, 8, 4 },
  { 8, 0, 8, 5 },
  { 8, 0, 8, 6 },
  { 8, 0, 8, 7 },

  { 8, 1, 8, 0 },
  { 8, 1, 8, 1 },
  { 8, 1, 8, 2 },
  { 8, 1, 8, 3 },
  { 8, 1, 8, 4 },
  { 8, 1, 8, 5 },
  { 8, 1, 8, 6 },
  { 8, 1, 8, 7 },

  { 8, 2, 8, 0 },
  { 8, 2, 8, 1 },
  { 8, 2, 8, 2 },
  { 8, 2, 8, 3 },
  { 8, 2, 8, 4 },
  { 8, 2, 8, 5 },
  { 8, 2, 8, 6 },
  { 8, 2, 8, 7 },

  { 8, 3, 8, 0 },
  { 8, 3, 8, 1 },
  { 8, 3, 8, 2 },
  { 8, 3, 8, 3 },
  { 8, 3, 8, 4 },
  { 8, 3, 8, 5 },
  { 8, 3, 8, 6 },
  { 8, 3, 8, 7 },

  { 8, 4, 8, 0 },
  { 8, 4, 8, 1 },
  { 8, 4, 8, 2 },
  { 8, 4, 8, 3 },
  { 8, 4, 8, 4 },
  { 8, 4, 8, 5 },
  { 8, 4, 8, 6 },
  { 8, 4, 8, 7 },

  { 8, 5, 8, 0 },
  { 8, 5, 8, 1 },
  { 8, 5, 8, 2 },
  { 8, 5, 8, 3 },
  { 8, 5, 8, 4 },
  { 8, 5, 8, 5 },
  { 8, 5, 8, 6 },
  { 8, 5, 8, 7 },

  { 8, 6, 8, 0 },
  { 8, 6, 8, 1 },
  { 8, 6, 8, 2 },
  { 8, 6, 8, 3 },
  { 8, 6, 8, 4 },
  { 8, 6, 8, 5 },
  { 8, 6, 8, 6 },
  { 8, 6, 8, 7 },

  { 8, 7, 8, 0 },
  { 8, 7, 8, 1 },
  { 8, 7, 8, 2 },
  { 8, 7, 8, 3 },
  { 8, 7, 8, 4 },
  { 8, 7, 8, 5 },
  { 8, 7, 8, 6 },
  { 8, 7, 8, 7 },

  { 128, 1, 128, 4 },
  { 128, 1, 128, 8 },
  { 128, 1, 128, 12 },
  { 128, 1, 128, 16 },
  { 128, 4, 128, 1 },
  { 128, 8, 128, 1 },
  { 128, 12, 128, 1 },
  { 128, 16, 128, 1 },
};

static const size_t g_double_aligns_len = sizeof(g_double_aligns)/sizeof(int[4]);

static size_t SetIncrement(size_t len) {
  if (len >= 4096) {
    return 1024;
  } else if (len >= 1024) {
    return 256;
  }
  return 1;
}

// Return a pointer into the current buffer with the specified alignment.
static void *GetAlignedPtr(void *orig_ptr, int alignment, int or_mask) {
  uint64_t ptr = reinterpret_cast<uint64_t>(orig_ptr);
  if (alignment > 0) {
      // When setting the alignment, set it to exactly the alignment chosen.
      // The pointer returned will be guaranteed not to be aligned to anything
      // more than that.
      ptr += alignment - (ptr & (alignment - 1));
      ptr |= alignment | or_mask;
  }

  return reinterpret_cast<void*>(ptr);
}

static void SetFencepost(uint8_t *buffer) {
  for (int i = 0; i < FENCEPOST_LENGTH; i += 2) {
    buffer[i] = 0xde;
    buffer[i+1] = 0xad;
  }
}

static void VerifyFencepost(uint8_t *buffer) {
  for (int i = 0; i < FENCEPOST_LENGTH; i += 2) {
    if (buffer[i] != 0xde || buffer[i+1] != 0xad) {
      uint8_t expected_value;
      if (buffer[i] == 0xde) {
        i++;
        expected_value = 0xad;
      } else {
        expected_value = 0xde;
      }
      ASSERT_EQ(expected_value, buffer[i]);
    }
  }
}

// Malloc can return a tagged pointer, which is not accepted in mm system calls like mprotect
// in the preliminary version of the syscall tagging support in the current Pixel 2 kernel.
// Note: the final version of the kernel patchset may relax this requirement.
static int MprotectHeap(void* addr, size_t len, int prot) {
  return mprotect(untag_address(addr), len, prot);
}

void RunSingleBufferAlignTest(
    size_t max_test_size, void (*test_func)(uint8_t*, size_t),
    size_t (*set_incr)(size_t)) {
  if (!set_incr) {
    set_incr = SetIncrement;
  }

  // Allocate one large buffer with lots of extra space so that we can
  // guarantee that the all possible alignments will fit.
  uint8_t *buf = new uint8_t[3*max_test_size];

  uint8_t *buf_align;
  for (size_t i = 0; i < g_single_aligns_len; i++) {
    size_t incr = 1;
    for (size_t len = 0; len <= max_test_size; len += incr) {
      incr = set_incr(len);

      buf_align = reinterpret_cast<uint8_t*>(GetAlignedPtr(
          buf+FENCEPOST_LENGTH, g_single_aligns[i][0], g_single_aligns[i][1]));

      SetFencepost(&buf_align[-FENCEPOST_LENGTH]);
      SetFencepost(&buf_align[len]);

      test_func(buf_align, len);

      VerifyFencepost(&buf_align[-FENCEPOST_LENGTH]);
      VerifyFencepost(&buf_align[len]);
    }
  }
  delete[] buf;
}

void RunSrcDstBufferAlignTest(
    size_t max_test_size, void (*test_func)(uint8_t*, uint8_t*, size_t),
    size_t (*set_incr)(size_t)) {
  if (!set_incr) {
    set_incr = SetIncrement;
  }

  // Allocate two large buffers for all of the testing.
  uint8_t* src = new uint8_t[3*max_test_size];
  uint8_t* dst = new uint8_t[3*max_test_size];

  uint8_t* src_align;
  uint8_t* dst_align;
  for (size_t i = 0; i < g_double_aligns_len; i++) {
    size_t incr = 1;
    for (size_t len = 0; len <= max_test_size; len += incr) {
      incr = set_incr(len);

      src_align =
          reinterpret_cast<uint8_t*>(GetAlignedPtr(
              src+FENCEPOST_LENGTH, g_double_aligns[i][0], g_double_aligns[i][1]));
      dst_align =
          reinterpret_cast<uint8_t*>(GetAlignedPtr(
              dst+FENCEPOST_LENGTH, g_double_aligns[i][2], g_double_aligns[i][3]));
      SetFencepost(&dst_align[-FENCEPOST_LENGTH]);
      SetFencepost(&dst_align[len]);

      test_func(src_align, dst_align, len);

      VerifyFencepost(&dst_align[-FENCEPOST_LENGTH]);
      VerifyFencepost(&dst_align[len]);
    }
  }
  delete[] src;
  delete[] dst;
}

void RunCmpBufferAlignTest(
    size_t max_test_size, void (*test_cmp_func)(uint8_t*, uint8_t*, size_t),
    void (*test_miscmp_func)(uint8_t*, uint8_t*, size_t, size_t),
    size_t (*set_incr)(size_t)) {
  if (!set_incr) {
    set_incr = SetIncrement;
  }

  // Allocate two large buffers for all of the testing.
  uint8_t* buf1 = new uint8_t[3*max_test_size];
  uint8_t* buf2 = new uint8_t[3*max_test_size];

  uint8_t* buf1_align;
  uint8_t* buf2_align;
  for (size_t i = 0; i < g_double_aligns_len; i++) {
    size_t incr = 1;
    for (size_t len = 0; len <= max_test_size; len += incr) {
      incr = set_incr(len);

      buf1_align =
          reinterpret_cast<uint8_t*>(GetAlignedPtr(
              buf1, g_double_aligns[i][0], g_double_aligns[i][1]));
      buf2_align =
          reinterpret_cast<uint8_t*>(GetAlignedPtr(
              buf2, g_double_aligns[i][2], g_double_aligns[i][3]));

      // Check by putting all zeroes after both buffers.
      memset(buf1_align+len, 0, 32);
      memset(buf2_align+len, 0, 32);
      test_cmp_func(buf1_align, buf2_align, len);

      // Check by putting different values after both buffers.
      for (size_t j = 0; j < 32; j++) {
        buf1_align[len+j] = j;
        buf2_align[len+j] = j+1;
      }
      test_cmp_func(buf1_align, buf2_align, len);

      if (len > 0) {
        // Change the lengths of the buffers and verify that there are
        // miscompares.
        for (size_t len2 = len+1; len2 < len+32; len2++) {
          test_miscmp_func(buf1_align, buf2_align, len, len2);
          test_miscmp_func(buf1_align, buf2_align, len2, len);
        }
      }
    }
  }
  delete[] buf1;
  delete[] buf2;
}

void RunSingleBufferOverreadTest(void (*test_func)(uint8_t*, size_t)) {
  // In order to verify that functions are not reading past the end of the
  // src, create data that ends exactly at an unreadable memory boundary.
  size_t pagesize = static_cast<size_t>(sysconf(_SC_PAGE_SIZE));
  uint8_t* memory;
  ASSERT_TRUE(posix_memalign(reinterpret_cast<void**>(&memory), pagesize,
                             2*pagesize) == 0);
  memset(memory, 0x23, 2*pagesize);

  // Make the second page unreadable and unwritable.
  ASSERT_TRUE(MprotectHeap(&memory[pagesize], pagesize, PROT_NONE) == 0);

  for (size_t i = 0; i < pagesize; i++) {
    uint8_t* buf = &memory[pagesize-i];

    test_func(buf, i);
  }
  ASSERT_TRUE(MprotectHeap(&memory[pagesize], pagesize, PROT_READ | PROT_WRITE) == 0);
  free(memory);
}

void RunSrcDstBufferOverreadTest(void (*test_func)(uint8_t*, uint8_t*, size_t)) {
  // In order to verify that functions are not reading past the end of the
  // src, create data that ends exactly at an unreadable memory boundary.
  size_t pagesize = static_cast<size_t>(sysconf(_SC_PAGE_SIZE));
  uint8_t* memory;
  ASSERT_TRUE(posix_memalign(reinterpret_cast<void**>(&memory), pagesize,
                             2*pagesize) == 0);
  memset(memory, 0x23, 2*pagesize);

  // Make the second page unreadable and unwritable.
  ASSERT_TRUE(MprotectHeap(&memory[pagesize], pagesize, PROT_NONE) == 0);

  uint8_t* dst_buffer = new uint8_t[2*pagesize];
  // Change the dst alignment as we change the source.
  for (size_t i = 0; i < 16; i++) {
    uint8_t* dst = &dst_buffer[i];
    for (size_t j = 0; j < pagesize; j++) {
      uint8_t* src = &memory[pagesize-j];

      test_func(src, dst, j);
    }
  }
  ASSERT_TRUE(MprotectHeap(&memory[pagesize], pagesize, PROT_READ | PROT_WRITE) == 0);
  free(memory);
  delete[] dst_buffer;
}

void RunCmpBufferOverreadTest(
    void (*test_cmp_func)(uint8_t*, uint8_t*, size_t),
    void (*test_miscmp_func)(uint8_t*, uint8_t*, size_t, size_t)) {
  // In order to verify that functions are not reading past the end of either
  // of the bufs, create both buffers that end exactly at an unreadable memory
  // boundary.
  size_t pagesize = static_cast<size_t>(sysconf(_SC_PAGE_SIZE));
  uint8_t* memory1;
  ASSERT_TRUE(posix_memalign(reinterpret_cast<void**>(&memory1), pagesize,
                             2*pagesize) == 0);
  memset(memory1, 0x23, 2*pagesize);

  // Make the second page unreadable and unwritable.
  ASSERT_TRUE(MprotectHeap(&memory1[pagesize], pagesize, PROT_NONE) == 0);

  uint8_t* memory2;
  ASSERT_TRUE(posix_memalign(reinterpret_cast<void**>(&memory2), pagesize,
                             2*pagesize) == 0);
  memset(memory2, 0x23, 2*pagesize);

  // Make the second page unreadable and unwritable.
  ASSERT_TRUE(MprotectHeap(&memory2[pagesize], pagesize, PROT_NONE) == 0);

  for (size_t i = 0; i < pagesize; i++) {
    uint8_t* buf1 = &memory1[pagesize-i];
    uint8_t* buf2 = &memory2[pagesize-i];

    test_cmp_func(buf1, buf2, i);
  }

  // Don't cycle through pagesize, MISCMP_MAX_LENGTH bytes should be good.
  size_t miscmp_len;
  if (pagesize > MISCMP_MAX_LENGTH) {
    miscmp_len = MISCMP_MAX_LENGTH;
  } else {
    miscmp_len = pagesize;
  }
  for (size_t i = 1; i < miscmp_len; i++) {
    uint8_t* buf1 = &memory1[pagesize-i];
    for (size_t j = 1; j < miscmp_len; j++) {
      if (j == i)
        continue;

      uint8_t* buf2 = &memory2[pagesize-j];

      test_miscmp_func(buf1, buf2, i, j);
    }
  }

  ASSERT_TRUE(MprotectHeap(&memory1[pagesize], pagesize, PROT_READ | PROT_WRITE) == 0);
  ASSERT_TRUE(MprotectHeap(&memory2[pagesize], pagesize, PROT_READ | PROT_WRITE) == 0);
  free(memory1);
  free(memory2);
}
