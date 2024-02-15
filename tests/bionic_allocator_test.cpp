/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <gtest/gtest.h>

#include "private/bionic_allocator.h"

#include <unistd.h>

namespace {

/*
 * this one has size below allocator cap which is 2*sizeof(void*)
 */
struct test_struct_small {
  char str[5];
};

struct test_struct_large {
  char str[1009];
};

struct test_struct_huge {
  char str[73939];
};

struct test_struct_512 {
  char str[503];
};

};

static size_t kPageSize = sysconf(_SC_PAGE_SIZE);

TEST(bionic_allocator, test_alloc_0) {
  BionicAllocator allocator;
  void* ptr = allocator.alloc(0);
  ASSERT_TRUE(ptr != nullptr);
  allocator.free(ptr);
}

TEST(bionic_allocator, test_free_nullptr) {
  BionicAllocator allocator;
  allocator.free(nullptr);
}

TEST(bionic_allocator, test_realloc) {
  BionicAllocator allocator;
  uint32_t* array = reinterpret_cast<uint32_t*>(allocator.alloc(512));
  const size_t array_size = 512 / sizeof(uint32_t);

  uint32_t model[1000];

  model[0] = 1;
  model[1] = 1;

  for (size_t i = 2; i < 1000; ++i) {
    model[i] = model[i - 1] + model[i - 2];
  }

  memcpy(array, model, array_size);

  uint32_t* reallocated_ptr = reinterpret_cast<uint32_t*>(allocator.realloc(array, 1024));

  ASSERT_TRUE(reallocated_ptr != nullptr);
  ASSERT_TRUE(reallocated_ptr != array);

  ASSERT_TRUE(memcmp(reallocated_ptr, model, array_size) == 0);

  array = reallocated_ptr;

  memcpy(array, model, 2*array_size);

  reallocated_ptr = reinterpret_cast<uint32_t*>(allocator.realloc(array, 62));

  ASSERT_TRUE(reallocated_ptr == array);

  reallocated_ptr = reinterpret_cast<uint32_t*>(allocator.realloc(array, 4000));

  ASSERT_TRUE(reallocated_ptr != nullptr);
  ASSERT_TRUE(reallocated_ptr != array);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(reallocated_ptr) % 16);

  ASSERT_TRUE(memcmp(reallocated_ptr, model, array_size * 2) == 0);

  array = reallocated_ptr;

  memcpy(array, model, 4000);

  reallocated_ptr = reinterpret_cast<uint32_t*>(allocator.realloc(array, 64000));

  ASSERT_TRUE(reallocated_ptr != nullptr);
  ASSERT_TRUE(reallocated_ptr != array);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(reallocated_ptr) % 16);

  ASSERT_TRUE(memcmp(reallocated_ptr, model, 4000) == 0);

  ASSERT_EQ(nullptr, allocator.realloc(reallocated_ptr, 0));
}

TEST(bionic_allocator, test_small_smoke) {
  BionicAllocator allocator;

  uint8_t zeros[16];
  memset(zeros, 0, sizeof(zeros));

  test_struct_small* ptr1 =
      reinterpret_cast<test_struct_small*>(allocator.alloc(sizeof(test_struct_small)));
  test_struct_small* ptr2 =
      reinterpret_cast<test_struct_small*>(allocator.alloc(sizeof(test_struct_small)));

  ASSERT_TRUE(ptr1 != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr1) % 16);
  ASSERT_TRUE(ptr2 != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr2) % 16);

  ASSERT_EQ(reinterpret_cast<uintptr_t>(ptr1)+16, reinterpret_cast<uintptr_t>(ptr2));
  ASSERT_TRUE(memcmp(ptr1, zeros, 16) == 0);

  allocator.free(ptr1);
  allocator.free(ptr2);
}

TEST(bionic_allocator, test_huge_smoke) {
  BionicAllocator allocator;

  // this should trigger proxy-to-mmap
  test_struct_huge* ptr1 =
      reinterpret_cast<test_struct_huge*>(allocator.alloc(sizeof(test_struct_huge)));
  test_struct_huge* ptr2 =
      reinterpret_cast<test_struct_huge*>(allocator.alloc(sizeof(test_struct_huge)));

  ASSERT_TRUE(ptr1 != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr1) % 16);
  ASSERT_TRUE(ptr2 != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr2) % 16);

  ASSERT_TRUE(
      reinterpret_cast<uintptr_t>(ptr1)/kPageSize != reinterpret_cast<uintptr_t>(ptr2)/kPageSize);
  allocator.free(ptr2);
  allocator.free(ptr1);
}

TEST(bionic_allocator, test_large) {
  BionicAllocator allocator;

  test_struct_large* ptr1 =
      reinterpret_cast<test_struct_large*>(allocator.alloc(sizeof(test_struct_large)));
  test_struct_large* ptr2 =
      reinterpret_cast<test_struct_large*>(allocator.alloc(1024));

  ASSERT_TRUE(ptr1 != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr1) % 16);
  ASSERT_TRUE(ptr2 != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr2) % 16);

  ASSERT_EQ(reinterpret_cast<uintptr_t>(ptr1) + 1024, reinterpret_cast<uintptr_t>(ptr2));

  // let's allocate until we reach the next page.
  size_t n = kPageSize / sizeof(test_struct_large) + 1 - 2;
  test_struct_large* objects[n];

  for (size_t i = 0; i < n; ++i) {
    test_struct_large* obj_ptr =
        reinterpret_cast<test_struct_large*>(allocator.alloc(sizeof(test_struct_large)));
    ASSERT_TRUE(obj_ptr != nullptr);
    objects[i] = obj_ptr;
  }

  test_struct_large* ptr_to_free =
      reinterpret_cast<test_struct_large*>(allocator.alloc(sizeof(test_struct_large)));

  ASSERT_TRUE(ptr_to_free != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr_to_free) % 16);

  allocator.free(ptr1);

  for (size_t i=0; i<n; ++i) {
    allocator.free(objects[i]);
  }

  allocator.free(ptr2);
  allocator.free(ptr_to_free);
}

TEST(bionic_allocator, test_memalign_small) {
  BionicAllocator allocator;
  void* ptr;

  // simple case
  ptr = allocator.memalign(0x100, 0x100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % 0x100);
  allocator.free(ptr);

  // small objects are automatically aligned to their size.
  ptr = allocator.alloc(0x200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % 0x200);
  allocator.free(ptr);

  // the size (0x10) is bumped up to the alignment (0x100)
  ptr = allocator.memalign(0x100, 0x10);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % 0x100);
  allocator.free(ptr);
}

TEST(bionic_allocator, test_memalign_large) {
  BionicAllocator allocator;
  void* ptr;
  size_t alignment;

  // a large object with alignment < kPageSize
  alignment = kPageSize >> 1;
  ptr = allocator.memalign(alignment, 0x2000);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % alignment);
  allocator.free(ptr);

  // a large object with alignment == kPageSize
  alignment = kPageSize;
  ptr = allocator.memalign(alignment, 0x2000);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % alignment);
  allocator.free(ptr);

  // A large object with alignment > kPageSize is only guaranteed to have page
  // alignment.
  alignment = kPageSize << 1;
  ptr = allocator.memalign(alignment, 0x4000);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % kPageSize);
  allocator.free(ptr);
}
