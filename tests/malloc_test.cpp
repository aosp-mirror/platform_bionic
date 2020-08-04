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

#include <gtest/gtest.h>

#include <elf.h>
#include <limits.h>
#include <malloc.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <atomic>
#include <thread>

#include <tinyxml2.h>

#include <android-base/file.h>

#include "utils.h"

#if defined(__BIONIC__)

#include "SignalUtils.h"

#include "platform/bionic/malloc.h"
#include "platform/bionic/mte_kernel.h"
#include "platform/bionic/reserved_signals.h"
#include "private/bionic_config.h"

#define HAVE_REALLOCARRAY 1

#else

#define HAVE_REALLOCARRAY __GLIBC_PREREQ(2, 26)

#endif

TEST(malloc, malloc_std) {
  // Simple malloc test.
  void *ptr = malloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  free(ptr);
}

TEST(malloc, malloc_overflow) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, malloc(SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, calloc_std) {
  // Simple calloc test.
  size_t alloc_len = 100;
  char *ptr = (char *)calloc(1, alloc_len);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(alloc_len, malloc_usable_size(ptr));
  for (size_t i = 0; i < alloc_len; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_illegal) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, calloc(-1, 100));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, calloc_overflow) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, calloc(1, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(nullptr, calloc(SIZE_MAX, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(nullptr, calloc(2, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(nullptr, calloc(SIZE_MAX, 2));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, memalign_multiple) {
  SKIP_WITH_HWASAN << "hwasan requires power of 2 alignment";
  // Memalign test where the alignment is any value.
  for (size_t i = 0; i <= 12; i++) {
    for (size_t alignment = 1 << i; alignment < (1U << (i+1)); alignment++) {
      char *ptr = reinterpret_cast<char*>(memalign(alignment, 100));
      ASSERT_TRUE(ptr != nullptr) << "Failed at alignment " << alignment;
      ASSERT_LE(100U, malloc_usable_size(ptr)) << "Failed at alignment " << alignment;
      ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % ((1U << i)))
          << "Failed at alignment " << alignment;
      free(ptr);
    }
  }
}

TEST(malloc, memalign_overflow) {
  SKIP_WITH_HWASAN;
  ASSERT_EQ(nullptr, memalign(4096, SIZE_MAX));
}

TEST(malloc, memalign_non_power2) {
  SKIP_WITH_HWASAN;
  void* ptr;
  for (size_t align = 0; align <= 256; align++) {
    ptr = memalign(align, 1024);
    ASSERT_TRUE(ptr != nullptr) << "Failed at align " << align;
    free(ptr);
  }
}

TEST(malloc, memalign_realloc) {
  // Memalign and then realloc the pointer a couple of times.
  for (size_t alignment = 1; alignment <= 4096; alignment <<= 1) {
    char *ptr = (char*)memalign(alignment, 100);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(100U, malloc_usable_size(ptr));
    ASSERT_EQ(0U, (intptr_t)ptr % alignment);
    memset(ptr, 0x23, 100);

    ptr = (char*)realloc(ptr, 200);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(200U, malloc_usable_size(ptr));
    ASSERT_TRUE(ptr != nullptr);
    for (size_t i = 0; i < 100; i++) {
      ASSERT_EQ(0x23, ptr[i]);
    }
    memset(ptr, 0x45, 200);

    ptr = (char*)realloc(ptr, 300);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(300U, malloc_usable_size(ptr));
    for (size_t i = 0; i < 200; i++) {
      ASSERT_EQ(0x45, ptr[i]);
    }
    memset(ptr, 0x67, 300);

    ptr = (char*)realloc(ptr, 250);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(250U, malloc_usable_size(ptr));
    for (size_t i = 0; i < 250; i++) {
      ASSERT_EQ(0x67, ptr[i]);
    }
    free(ptr);
  }
}

TEST(malloc, malloc_realloc_larger) {
  // Realloc to a larger size, malloc is used for the original allocation.
  char *ptr = (char *)malloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  memset(ptr, 67, 100);

  ptr = (char *)realloc(ptr, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(67, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, malloc_realloc_smaller) {
  // Realloc to a smaller size, malloc is used for the original allocation.
  char *ptr = (char *)malloc(200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  memset(ptr, 67, 200);

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(67, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, malloc_multiple_realloc) {
  // Multiple reallocs, malloc is used for the original allocation.
  char *ptr = (char *)malloc(200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  memset(ptr, 0x23, 200);

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 50);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(50U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 150);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(150U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }
  memset(ptr, 0x23, 150);

  ptr = (char*)realloc(ptr, 425);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(425U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 150; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_realloc_larger) {
  // Realloc to a larger size, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_realloc_smaller) {
  // Realloc to a smaller size, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_multiple_realloc) {
  // Multiple reallocs, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 50);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(50U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 150);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(150U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  memset(ptr, 0, 150);

  ptr = (char*)realloc(ptr, 425);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(425U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 150; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, realloc_overflow) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, realloc(nullptr, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  void* ptr = malloc(100);
  ASSERT_TRUE(ptr != nullptr);
  errno = 0;
  ASSERT_EQ(nullptr, realloc(ptr, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  free(ptr);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t);
extern "C" void* valloc(size_t);
#endif

TEST(malloc, pvalloc_std) {
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void* ptr = pvalloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_TRUE((reinterpret_cast<uintptr_t>(ptr) & (pagesize-1)) == 0);
  ASSERT_LE(pagesize, malloc_usable_size(ptr));
  free(ptr);
#else
  GTEST_SKIP() << "pvalloc not supported.";
#endif
}

TEST(malloc, pvalloc_overflow) {
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  ASSERT_EQ(nullptr, pvalloc(SIZE_MAX));
#else
  GTEST_SKIP() << "pvalloc not supported.";
#endif
}

TEST(malloc, valloc_std) {
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void* ptr = valloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_TRUE((reinterpret_cast<uintptr_t>(ptr) & (pagesize-1)) == 0);
  free(ptr);
#else
  GTEST_SKIP() << "valloc not supported.";
#endif
}

TEST(malloc, valloc_overflow) {
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  ASSERT_EQ(nullptr, valloc(SIZE_MAX));
#else
  GTEST_SKIP() << "valloc not supported.";
#endif
}

TEST(malloc, malloc_info) {
#ifdef __BIONIC__
  SKIP_WITH_HWASAN; // hwasan does not implement malloc_info

  TemporaryFile tf;
  ASSERT_TRUE(tf.fd != -1);
  FILE* fp = fdopen(tf.fd, "w+");
  tf.release();
  ASSERT_TRUE(fp != nullptr);
  ASSERT_EQ(0, malloc_info(0, fp));
  ASSERT_EQ(0, fclose(fp));

  std::string contents;
  ASSERT_TRUE(android::base::ReadFileToString(tf.path, &contents));

  tinyxml2::XMLDocument doc;
  ASSERT_EQ(tinyxml2::XML_SUCCESS, doc.Parse(contents.c_str()));

  auto root = doc.FirstChildElement();
  ASSERT_NE(nullptr, root);
  ASSERT_STREQ("malloc", root->Name());
  std::string version(root->Attribute("version"));
  if (version == "jemalloc-1") {
    auto arena = root->FirstChildElement();
    for (; arena != nullptr; arena = arena->NextSiblingElement()) {
      int val;

      ASSERT_STREQ("heap", arena->Name());
      ASSERT_EQ(tinyxml2::XML_SUCCESS, arena->QueryIntAttribute("nr", &val));
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("allocated-large")->QueryIntText(&val));
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("allocated-huge")->QueryIntText(&val));
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("allocated-bins")->QueryIntText(&val));
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("bins-total")->QueryIntText(&val));

      auto bin = arena->FirstChildElement("bin");
      for (; bin != nullptr; bin = bin ->NextSiblingElement()) {
        if (strcmp(bin->Name(), "bin") == 0) {
          ASSERT_EQ(tinyxml2::XML_SUCCESS, bin->QueryIntAttribute("nr", &val));
          ASSERT_EQ(tinyxml2::XML_SUCCESS,
                    bin->FirstChildElement("allocated")->QueryIntText(&val));
          ASSERT_EQ(tinyxml2::XML_SUCCESS,
                    bin->FirstChildElement("nmalloc")->QueryIntText(&val));
          ASSERT_EQ(tinyxml2::XML_SUCCESS,
                    bin->FirstChildElement("ndalloc")->QueryIntText(&val));
        }
      }
    }
  } else if (version == "scudo-1") {
    auto element = root->FirstChildElement();
    for (; element != nullptr; element = element->NextSiblingElement()) {
      int val;

      ASSERT_STREQ("alloc", element->Name());
      ASSERT_EQ(tinyxml2::XML_SUCCESS, element->QueryIntAttribute("size", &val));
      ASSERT_EQ(tinyxml2::XML_SUCCESS, element->QueryIntAttribute("count", &val));
    }
  } else {
    // Do not verify output for debug malloc.
    ASSERT_TRUE(version == "debug-malloc-1") << "Unknown version: " << version;
  }
#endif
}

TEST(malloc, malloc_info_matches_mallinfo) {
#ifdef __BIONIC__
  SKIP_WITH_HWASAN; // hwasan does not implement malloc_info

  TemporaryFile tf;
  ASSERT_TRUE(tf.fd != -1);
  FILE* fp = fdopen(tf.fd, "w+");
  tf.release();
  ASSERT_TRUE(fp != nullptr);
  size_t mallinfo_before_allocated_bytes = mallinfo().uordblks;
  ASSERT_EQ(0, malloc_info(0, fp));
  size_t mallinfo_after_allocated_bytes = mallinfo().uordblks;
  ASSERT_EQ(0, fclose(fp));

  std::string contents;
  ASSERT_TRUE(android::base::ReadFileToString(tf.path, &contents));

  tinyxml2::XMLDocument doc;
  ASSERT_EQ(tinyxml2::XML_SUCCESS, doc.Parse(contents.c_str()));

  size_t total_allocated_bytes = 0;
  auto root = doc.FirstChildElement();
  ASSERT_NE(nullptr, root);
  ASSERT_STREQ("malloc", root->Name());
  std::string version(root->Attribute("version"));
  if (version == "jemalloc-1") {
    auto arena = root->FirstChildElement();
    for (; arena != nullptr; arena = arena->NextSiblingElement()) {
      int val;

      ASSERT_STREQ("heap", arena->Name());
      ASSERT_EQ(tinyxml2::XML_SUCCESS, arena->QueryIntAttribute("nr", &val));
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("allocated-large")->QueryIntText(&val));
      total_allocated_bytes += val;
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("allocated-huge")->QueryIntText(&val));
      total_allocated_bytes += val;
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("allocated-bins")->QueryIntText(&val));
      total_allocated_bytes += val;
      ASSERT_EQ(tinyxml2::XML_SUCCESS,
                arena->FirstChildElement("bins-total")->QueryIntText(&val));
    }
    // The total needs to be between the mallinfo call before and after
    // since malloc_info allocates some memory.
    EXPECT_LE(mallinfo_before_allocated_bytes, total_allocated_bytes);
    EXPECT_GE(mallinfo_after_allocated_bytes, total_allocated_bytes);
  } else if (version == "scudo-1") {
    auto element = root->FirstChildElement();
    for (; element != nullptr; element = element->NextSiblingElement()) {
      ASSERT_STREQ("alloc", element->Name());
      int size;
      ASSERT_EQ(tinyxml2::XML_SUCCESS, element->QueryIntAttribute("size", &size));
      int count;
      ASSERT_EQ(tinyxml2::XML_SUCCESS, element->QueryIntAttribute("count", &count));
      total_allocated_bytes += size * count;
    }
    // Scudo only gives the information on the primary, so simply make
    // sure that the value is non-zero.
    EXPECT_NE(0U, total_allocated_bytes);
  } else {
    // Do not verify output for debug malloc.
    ASSERT_TRUE(version == "debug-malloc-1") << "Unknown version: " << version;
  }
#endif
}

TEST(malloc, calloc_usable_size) {
  for (size_t size = 1; size <= 2048; size++) {
    void* pointer = malloc(size);
    ASSERT_TRUE(pointer != nullptr);
    memset(pointer, 0xeb, malloc_usable_size(pointer));
    free(pointer);

    // We should get a previous pointer that has been set to non-zero.
    // If calloc does not zero out all of the data, this will fail.
    uint8_t* zero_mem = reinterpret_cast<uint8_t*>(calloc(1, size));
    ASSERT_TRUE(pointer != nullptr);
    size_t usable_size = malloc_usable_size(zero_mem);
    for (size_t i = 0; i < usable_size; i++) {
      ASSERT_EQ(0, zero_mem[i]) << "Failed at allocation size " << size << " at byte " << i;
    }
    free(zero_mem);
  }
}

TEST(malloc, malloc_0) {
  void* p = malloc(0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, calloc_0_0) {
  void* p = calloc(0, 0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, calloc_0_1) {
  void* p = calloc(0, 1);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, calloc_1_0) {
  void* p = calloc(1, 0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, realloc_nullptr_0) {
  // realloc(nullptr, size) is actually malloc(size).
  void* p = realloc(nullptr, 0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, realloc_0) {
  void* p = malloc(1024);
  ASSERT_TRUE(p != nullptr);
  // realloc(p, 0) is actually free(p).
  void* p2 = realloc(p, 0);
  ASSERT_TRUE(p2 == nullptr);
}

constexpr size_t MAX_LOOPS = 200;

// Make sure that memory returned by malloc is aligned to allow these data types.
TEST(malloc, verify_alignment) {
  uint32_t** values_32 = new uint32_t*[MAX_LOOPS];
  uint64_t** values_64 = new uint64_t*[MAX_LOOPS];
  long double** values_ldouble = new long double*[MAX_LOOPS];
  // Use filler to attempt to force the allocator to get potentially bad alignments.
  void** filler = new void*[MAX_LOOPS];

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    // Check uint32_t pointers.
    filler[i] = malloc(1);
    ASSERT_TRUE(filler[i] != nullptr);

    values_32[i] = reinterpret_cast<uint32_t*>(malloc(sizeof(uint32_t)));
    ASSERT_TRUE(values_32[i] != nullptr);
    *values_32[i] = i;
    ASSERT_EQ(*values_32[i], i);
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_32[i]) & (sizeof(uint32_t) - 1));

    free(filler[i]);
  }

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    // Check uint64_t pointers.
    filler[i] = malloc(1);
    ASSERT_TRUE(filler[i] != nullptr);

    values_64[i] = reinterpret_cast<uint64_t*>(malloc(sizeof(uint64_t)));
    ASSERT_TRUE(values_64[i] != nullptr);
    *values_64[i] = 0x1000 + i;
    ASSERT_EQ(*values_64[i], 0x1000 + i);
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_64[i]) & (sizeof(uint64_t) - 1));

    free(filler[i]);
  }

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    // Check long double pointers.
    filler[i] = malloc(1);
    ASSERT_TRUE(filler[i] != nullptr);

    values_ldouble[i] = reinterpret_cast<long double*>(malloc(sizeof(long double)));
    ASSERT_TRUE(values_ldouble[i] != nullptr);
    *values_ldouble[i] = 5.5 + i;
    ASSERT_DOUBLE_EQ(*values_ldouble[i], 5.5 + i);
    // 32 bit glibc has a long double size of 12 bytes, so hardcode the
    // required alignment to 0x7.
#if !defined(__BIONIC__) && !defined(__LP64__)
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_ldouble[i]) & 0x7);
#else
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_ldouble[i]) & (sizeof(long double) - 1));
#endif

    free(filler[i]);
  }

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    free(values_32[i]);
    free(values_64[i]);
    free(values_ldouble[i]);
  }

  delete[] filler;
  delete[] values_32;
  delete[] values_64;
  delete[] values_ldouble;
}

TEST(malloc, mallopt_smoke) {
  errno = 0;
  ASSERT_EQ(0, mallopt(-1000, 1));
  // mallopt doesn't set errno.
  ASSERT_EQ(0, errno);
}

TEST(malloc, mallopt_decay) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "hwasan does not implement mallopt";
  errno = 0;
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 1));
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 0));
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 1));
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 0));
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(malloc, mallopt_purge) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "hwasan does not implement mallopt";
  errno = 0;
  ASSERT_EQ(1, mallopt(M_PURGE, 0));
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

#if defined(__BIONIC__)
static void GetAllocatorVersion(bool* allocator_scudo) {
  TemporaryFile tf;
  ASSERT_TRUE(tf.fd != -1);
  FILE* fp = fdopen(tf.fd, "w+");
  tf.release();
  ASSERT_TRUE(fp != nullptr);
  ASSERT_EQ(0, malloc_info(0, fp));
  ASSERT_EQ(0, fclose(fp));

  std::string contents;
  ASSERT_TRUE(android::base::ReadFileToString(tf.path, &contents));

  tinyxml2::XMLDocument doc;
  ASSERT_EQ(tinyxml2::XML_SUCCESS, doc.Parse(contents.c_str()));

  auto root = doc.FirstChildElement();
  ASSERT_NE(nullptr, root);
  ASSERT_STREQ("malloc", root->Name());
  std::string version(root->Attribute("version"));
  *allocator_scudo = (version == "scudo-1");
}
#endif

TEST(malloc, mallopt_scudo_only_options) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "hwasan does not implement mallopt";
  bool allocator_scudo;
  GetAllocatorVersion(&allocator_scudo);
  if (!allocator_scudo) {
    GTEST_SKIP() << "scudo allocator only test";
  }
  ASSERT_EQ(1, mallopt(M_CACHE_COUNT_MAX, 100));
  ASSERT_EQ(1, mallopt(M_CACHE_SIZE_MAX, 1024 * 1024 * 2));
  ASSERT_EQ(1, mallopt(M_TSDS_COUNT_MAX, 8));
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(malloc, reallocarray_overflow) {
#if HAVE_REALLOCARRAY
  // Values that cause overflow to a result small enough (8 on LP64) that malloc would "succeed".
  size_t a = static_cast<size_t>(INTPTR_MIN + 4);
  size_t b = 2;

  errno = 0;
  ASSERT_TRUE(reallocarray(nullptr, a, b) == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  errno = 0;
  ASSERT_TRUE(reallocarray(nullptr, b, a) == nullptr);
  ASSERT_EQ(ENOMEM, errno);
#else
  GTEST_SKIP() << "reallocarray not available";
#endif
}

TEST(malloc, reallocarray) {
#if HAVE_REALLOCARRAY
  void* p = reallocarray(nullptr, 2, 32);
  ASSERT_TRUE(p != nullptr);
  ASSERT_GE(malloc_usable_size(p), 64U);
#else
  GTEST_SKIP() << "reallocarray not available";
#endif
}

TEST(malloc, mallinfo) {
#if defined(__BIONIC__)
  SKIP_WITH_HWASAN << "hwasan does not implement mallinfo";
  static size_t sizes[] = {
    8, 32, 128, 4096, 32768, 131072, 1024000, 10240000, 20480000, 300000000
  };

  constexpr static size_t kMaxAllocs = 50;

  for (size_t size : sizes) {
    // If some of these allocations are stuck in a thread cache, then keep
    // looping until we make an allocation that changes the total size of the
    // memory allocated.
    // jemalloc implementations counts the thread cache allocations against
    // total memory allocated.
    void* ptrs[kMaxAllocs] = {};
    bool pass = false;
    for (size_t i = 0; i < kMaxAllocs; i++) {
      size_t allocated = mallinfo().uordblks;
      ptrs[i] = malloc(size);
      ASSERT_TRUE(ptrs[i] != nullptr);
      size_t new_allocated = mallinfo().uordblks;
      if (allocated != new_allocated) {
        size_t usable_size = malloc_usable_size(ptrs[i]);
        // Only check if the total got bigger by at least allocation size.
        // Sometimes the mallinfo numbers can go backwards due to compaction
        // and/or freeing of cached data.
        if (new_allocated >= allocated + usable_size) {
          pass = true;
          break;
        }
      }
    }
    for (void* ptr : ptrs) {
      free(ptr);
    }
    ASSERT_TRUE(pass)
        << "For size " << size << " allocated bytes did not increase after "
        << kMaxAllocs << " allocations.";
  }
#else
  GTEST_SKIP() << "glibc is broken";
#endif
}

template <typename Type>
void __attribute__((optnone)) VerifyAlignment(Type* floating) {
  size_t expected_alignment = alignof(Type);
  if (expected_alignment != 0) {
    ASSERT_EQ(0U, (expected_alignment - 1) & reinterpret_cast<uintptr_t>(floating))
        << "Expected alignment " << expected_alignment << " ptr value " << floating;
  }
}

template <typename Type>
void __attribute__((optnone)) TestAllocateType() {
  // The number of allocations to do in a row. This is to attempt to
  // expose the worst case alignment for native allocators that use
  // bins.
  static constexpr size_t kMaxConsecutiveAllocs = 100;

  // Verify using new directly.
  Type* types[kMaxConsecutiveAllocs];
  for (size_t i = 0; i < kMaxConsecutiveAllocs; i++) {
    types[i] = new Type;
    VerifyAlignment(types[i]);
    if (::testing::Test::HasFatalFailure()) {
      return;
    }
  }
  for (size_t i = 0; i < kMaxConsecutiveAllocs; i++) {
    delete types[i];
  }

  // Verify using malloc.
  for (size_t i = 0; i < kMaxConsecutiveAllocs; i++) {
    types[i] = reinterpret_cast<Type*>(malloc(sizeof(Type)));
    ASSERT_TRUE(types[i] != nullptr);
    VerifyAlignment(types[i]);
    if (::testing::Test::HasFatalFailure()) {
      return;
    }
  }
  for (size_t i = 0; i < kMaxConsecutiveAllocs; i++) {
    free(types[i]);
  }

  // Verify using a vector.
  std::vector<Type> type_vector(kMaxConsecutiveAllocs);
  for (size_t i = 0; i < type_vector.size(); i++) {
    VerifyAlignment(&type_vector[i]);
    if (::testing::Test::HasFatalFailure()) {
      return;
    }
  }
}

#if defined(__ANDROID__)
static void __attribute__((optnone)) AndroidVerifyAlignment(size_t alloc_size, size_t aligned_bytes) {
  void* ptrs[100];
  uintptr_t mask = aligned_bytes - 1;
  for (size_t i = 0; i < sizeof(ptrs) / sizeof(void*); i++) {
    ptrs[i] = malloc(alloc_size);
    ASSERT_TRUE(ptrs[i] != nullptr);
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptrs[i]) & mask)
        << "Expected at least " << aligned_bytes << " byte alignment: size "
        << alloc_size << " actual ptr " << ptrs[i];
  }
}
#endif

TEST(malloc, align_check) {
  // See http://www.open-std.org/jtc1/sc22/wg14/www/docs/summary.htm#dr_445
  // for a discussion of type alignment.
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<float>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<double>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<long double>());

  ASSERT_NO_FATAL_FAILURE(TestAllocateType<char>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<char16_t>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<char32_t>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<wchar_t>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<signed char>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<short int>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<int>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<long int>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<long long int>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<unsigned char>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<unsigned short int>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<unsigned int>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<unsigned long int>());
  ASSERT_NO_FATAL_FAILURE(TestAllocateType<unsigned long long int>());

#if defined(__ANDROID__)
  // On Android, there is a lot of code that expects certain alignments:
  // - Allocations of a size that rounds up to a multiple of 16 bytes
  //   must have at least 16 byte alignment.
  // - Allocations of a size that rounds up to a multiple of 8 bytes and
  //   not 16 bytes, are only required to have at least 8 byte alignment.
  // This is regardless of whether it is in a 32 bit or 64 bit environment.

  // See http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2293.htm for
  // a discussion of this alignment mess. The code below is enforcing
  // strong-alignment, since who knows what code depends on this behavior now.
  for (size_t i = 1; i <= 128; i++) {
    size_t rounded = (i + 7) & ~7;
    if ((rounded % 16) == 0) {
      AndroidVerifyAlignment(i, 16);
    } else {
      AndroidVerifyAlignment(i, 8);
    }
    if (::testing::Test::HasFatalFailure()) {
      return;
    }
  }
#endif
}

// Jemalloc doesn't pass this test right now, so leave it as disabled.
TEST(malloc, DISABLED_alloc_after_fork) {
  // Both of these need to be a power of 2.
  static constexpr size_t kMinAllocationSize = 8;
  static constexpr size_t kMaxAllocationSize = 2097152;

  static constexpr size_t kNumAllocatingThreads = 5;
  static constexpr size_t kNumForkLoops = 100;

  std::atomic_bool stop;

  // Create threads that simply allocate and free different sizes.
  std::vector<std::thread*> threads;
  for (size_t i = 0; i < kNumAllocatingThreads; i++) {
    std::thread* t = new std::thread([&stop] {
      while (!stop) {
        for (size_t size = kMinAllocationSize; size <= kMaxAllocationSize; size <<= 1) {
          void* ptr = malloc(size);
          if (ptr == nullptr) {
            return;
          }
          // Make sure this value is not optimized away.
          asm volatile("" : : "r,m"(ptr) : "memory");
          free(ptr);
        }
      }
    });
    threads.push_back(t);
  }

  // Create a thread to fork and allocate.
  for (size_t i = 0; i < kNumForkLoops; i++) {
    pid_t pid;
    if ((pid = fork()) == 0) {
      for (size_t size = kMinAllocationSize; size <= kMaxAllocationSize; size <<= 1) {
        void* ptr = malloc(size);
        ASSERT_TRUE(ptr != nullptr);
        // Make sure this value is not optimized away.
        asm volatile("" : : "r,m"(ptr) : "memory");
        // Make sure we can touch all of the allocation.
        memset(ptr, 0x1, size);
        ASSERT_LE(size, malloc_usable_size(ptr));
        free(ptr);
      }
      _exit(10);
    }
    ASSERT_NE(-1, pid);
    AssertChildExited(pid, 10);
  }

  stop = true;
  for (auto thread : threads) {
    thread->join();
    delete thread;
  }
}

TEST(android_mallopt, error_on_unexpected_option) {
#if defined(__BIONIC__)
  const int unrecognized_option = -1;
  errno = 0;
  EXPECT_EQ(false, android_mallopt(unrecognized_option, nullptr, 0));
  EXPECT_EQ(ENOTSUP, errno);
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

bool IsDynamic() {
#if defined(__LP64__)
  Elf64_Ehdr ehdr;
#else
  Elf32_Ehdr ehdr;
#endif
  std::string path(android::base::GetExecutablePath());

  int fd = open(path.c_str(), O_RDONLY | O_CLOEXEC);
  if (fd == -1) {
    // Assume dynamic on error.
    return true;
  }
  bool read_completed = android::base::ReadFully(fd, &ehdr, sizeof(ehdr));
  close(fd);
  // Assume dynamic in error cases.
  return !read_completed || ehdr.e_type == ET_DYN;
}

TEST(android_mallopt, init_zygote_child_profiling) {
#if defined(__BIONIC__)
  // Successful call.
  errno = 0;
  if (IsDynamic()) {
    EXPECT_EQ(true, android_mallopt(M_INIT_ZYGOTE_CHILD_PROFILING, nullptr, 0));
    EXPECT_EQ(0, errno);
  } else {
    // Not supported in static executables.
    EXPECT_EQ(false, android_mallopt(M_INIT_ZYGOTE_CHILD_PROFILING, nullptr, 0));
    EXPECT_EQ(ENOTSUP, errno);
  }

  // Unexpected arguments rejected.
  errno = 0;
  char unexpected = 0;
  EXPECT_EQ(false, android_mallopt(M_INIT_ZYGOTE_CHILD_PROFILING, &unexpected, 1));
  if (IsDynamic()) {
    EXPECT_EQ(EINVAL, errno);
  } else {
    EXPECT_EQ(ENOTSUP, errno);
  }
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

#if defined(__BIONIC__)
template <typename FuncType>
void CheckAllocationFunction(FuncType func) {
  // Assumes that no more than 108MB of memory is allocated before this.
  size_t limit = 128 * 1024 * 1024;
  ASSERT_TRUE(android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit)));
  if (!func(20 * 1024 * 1024))
    exit(1);
  if (func(128 * 1024 * 1024))
    exit(1);
  exit(0);
}
#endif

TEST(android_mallopt, set_allocation_limit) {
#if defined(__BIONIC__)
  EXPECT_EXIT(CheckAllocationFunction([](size_t bytes) { return calloc(bytes, 1) != nullptr; }),
              testing::ExitedWithCode(0), "");
  EXPECT_EXIT(CheckAllocationFunction([](size_t bytes) { return calloc(1, bytes) != nullptr; }),
              testing::ExitedWithCode(0), "");
  EXPECT_EXIT(CheckAllocationFunction([](size_t bytes) { return malloc(bytes) != nullptr; }),
              testing::ExitedWithCode(0), "");
  EXPECT_EXIT(CheckAllocationFunction(
                  [](size_t bytes) { return memalign(sizeof(void*), bytes) != nullptr; }),
              testing::ExitedWithCode(0), "");
  EXPECT_EXIT(CheckAllocationFunction([](size_t bytes) {
                void* ptr;
                return posix_memalign(&ptr, sizeof(void *), bytes) == 0;
              }),
              testing::ExitedWithCode(0), "");
  EXPECT_EXIT(CheckAllocationFunction(
                  [](size_t bytes) { return aligned_alloc(sizeof(void*), bytes) != nullptr; }),
              testing::ExitedWithCode(0), "");
  EXPECT_EXIT(CheckAllocationFunction([](size_t bytes) {
                void* p = malloc(1024 * 1024);
                return realloc(p, bytes) != nullptr;
              }),
              testing::ExitedWithCode(0), "");
#if !defined(__LP64__)
  EXPECT_EXIT(CheckAllocationFunction([](size_t bytes) { return pvalloc(bytes) != nullptr; }),
              testing::ExitedWithCode(0), "");
  EXPECT_EXIT(CheckAllocationFunction([](size_t bytes) { return valloc(bytes) != nullptr; }),
              testing::ExitedWithCode(0), "");
#endif
#else
  GTEST_SKIP() << "bionic extension";
#endif
}

TEST(android_mallopt, set_allocation_limit_multiple) {
#if defined(__BIONIC__)
  // Only the first set should work.
  size_t limit = 256 * 1024 * 1024;
  ASSERT_TRUE(android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit)));
  limit = 32 * 1024 * 1024;
  ASSERT_FALSE(android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit)));
#else
  GTEST_SKIP() << "bionic extension";
#endif
}

#if defined(__BIONIC__)
static constexpr size_t kAllocationSize = 8 * 1024 * 1024;

static size_t GetMaxAllocations() {
  size_t max_pointers = 0;
  void* ptrs[20];
  for (size_t i = 0; i < sizeof(ptrs) / sizeof(void*); i++) {
    ptrs[i] = malloc(kAllocationSize);
    if (ptrs[i] == nullptr) {
      max_pointers = i;
      break;
    }
  }
  for (size_t i = 0; i < max_pointers; i++) {
    free(ptrs[i]);
  }
  return max_pointers;
}

static void VerifyMaxPointers(size_t max_pointers) {
  // Now verify that we can allocate the same number as before.
  void* ptrs[20];
  for (size_t i = 0; i < max_pointers; i++) {
    ptrs[i] = malloc(kAllocationSize);
    ASSERT_TRUE(ptrs[i] != nullptr) << "Failed to allocate on iteration " << i;
  }

  // Make sure the next allocation still fails.
  ASSERT_TRUE(malloc(kAllocationSize) == nullptr);
  for (size_t i = 0; i < max_pointers; i++) {
    free(ptrs[i]);
  }
}
#endif

TEST(android_mallopt, set_allocation_limit_realloc_increase) {
#if defined(__BIONIC__)
  size_t limit = 128 * 1024 * 1024;
  ASSERT_TRUE(android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit)));

  size_t max_pointers = GetMaxAllocations();
  ASSERT_TRUE(max_pointers != 0) << "Limit never reached.";

  void* memory = malloc(10 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);

  // Increase size.
  memory = realloc(memory, 20 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  memory = realloc(memory, 40 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  memory = realloc(memory, 60 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  memory = realloc(memory, 80 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  // Now push past limit.
  memory = realloc(memory, 130 * 1024 * 1024);
  ASSERT_TRUE(memory == nullptr);

  VerifyMaxPointers(max_pointers);
#else
  GTEST_SKIP() << "bionic extension";
#endif
}

TEST(android_mallopt, set_allocation_limit_realloc_decrease) {
#if defined(__BIONIC__)
  size_t limit = 100 * 1024 * 1024;
  ASSERT_TRUE(android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit)));

  size_t max_pointers = GetMaxAllocations();
  ASSERT_TRUE(max_pointers != 0) << "Limit never reached.";

  void* memory = malloc(80 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);

  // Decrease size.
  memory = realloc(memory, 60 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  memory = realloc(memory, 40 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  memory = realloc(memory, 20 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  memory = realloc(memory, 10 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);
  free(memory);

  VerifyMaxPointers(max_pointers);
#else
  GTEST_SKIP() << "bionic extension";
#endif
}

TEST(android_mallopt, set_allocation_limit_realloc_free) {
#if defined(__BIONIC__)
  size_t limit = 100 * 1024 * 1024;
  ASSERT_TRUE(android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit)));

  size_t max_pointers = GetMaxAllocations();
  ASSERT_TRUE(max_pointers != 0) << "Limit never reached.";

  void* memory = malloc(60 * 1024 * 1024);
  ASSERT_TRUE(memory != nullptr);

  memory = realloc(memory, 0);
  ASSERT_TRUE(memory == nullptr);

  VerifyMaxPointers(max_pointers);
#else
  GTEST_SKIP() << "bionic extension";
#endif
}

#if defined(__BIONIC__)
static void* SetAllocationLimit(void* data) {
  std::atomic_bool* go = reinterpret_cast<std::atomic_bool*>(data);
  while (!go->load()) {
  }
  size_t limit = 500 * 1024 * 1024;
  if (android_mallopt(M_SET_ALLOCATION_LIMIT_BYTES, &limit, sizeof(limit))) {
    return reinterpret_cast<void*>(-1);
  }
  return nullptr;
}

static void SetAllocationLimitMultipleThreads() {
  std::atomic_bool go;
  go = false;

  static constexpr size_t kNumThreads = 4;
  pthread_t threads[kNumThreads];
  for (size_t i = 0; i < kNumThreads; i++) {
    ASSERT_EQ(0, pthread_create(&threads[i], nullptr, SetAllocationLimit, &go));
  }

  // Let them go all at once.
  go = true;
  // Send hardcoded signal (BIONIC_SIGNAL_PROFILER with value 0) to trigger
  // heapprofd handler.
  union sigval signal_value;
  signal_value.sival_int = 0;
  ASSERT_EQ(0, sigqueue(getpid(), BIONIC_SIGNAL_PROFILER, signal_value));

  size_t num_successful = 0;
  for (size_t i = 0; i < kNumThreads; i++) {
    void* result;
    ASSERT_EQ(0, pthread_join(threads[i], &result));
    if (result != nullptr) {
      num_successful++;
    }
  }
  ASSERT_EQ(1U, num_successful);
  exit(0);
}
#endif

TEST(android_mallopt, set_allocation_limit_multiple_threads) {
#if defined(__BIONIC__)
  if (IsDynamic()) {
    ASSERT_TRUE(android_mallopt(M_INIT_ZYGOTE_CHILD_PROFILING, nullptr, 0));
  }

  // Run this a number of times as a stress test.
  for (size_t i = 0; i < 100; i++) {
    // Not using ASSERT_EXIT because errors messages are not displayed.
    pid_t pid;
    if ((pid = fork()) == 0) {
      ASSERT_NO_FATAL_FAILURE(SetAllocationLimitMultipleThreads());
    }
    ASSERT_NE(-1, pid);
    int status;
    ASSERT_EQ(pid, wait(&status));
    ASSERT_EQ(0, WEXITSTATUS(status));
  }
#else
  GTEST_SKIP() << "bionic extension";
#endif
}

#if defined(__BIONIC__) && defined(__aarch64__) && defined(ANDROID_EXPERIMENTAL_MTE)
template <int SiCode> void CheckSiCode(int, siginfo_t* info, void*) {
  if (info->si_code != SiCode) {
    _exit(2);
  }
  _exit(1);
}

static bool SetTagCheckingLevel(int level) {
  int tagged_addr_ctrl = prctl(PR_GET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
  if (tagged_addr_ctrl < 0) {
    return false;
  }

  tagged_addr_ctrl = (tagged_addr_ctrl & ~PR_MTE_TCF_MASK) | level;
  return prctl(PR_SET_TAGGED_ADDR_CTRL, tagged_addr_ctrl, 0, 0, 0) == 0;
}
#endif

TEST(android_mallopt, tag_level) {
#if defined(__BIONIC__) && defined(__aarch64__) && defined(ANDROID_EXPERIMENTAL_MTE)
  if (!(getauxval(AT_HWCAP2) & HWCAP2_MTE)) {
    GTEST_SKIP() << "requires MTE support";
    return;
  }

  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);

  // First, check that memory tagging is enabled and the default tag checking level is async.
  // We assume that scudo is used on all MTE enabled hardware; scudo inserts a header with a
  // mismatching tag before each allocation.
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, CheckSiCode<SEGV_MTEAERR>, SA_SIGINFO);
        p[-1] = 42;
      },
      testing::ExitedWithCode(1), "");

  EXPECT_TRUE(SetTagCheckingLevel(PR_MTE_TCF_SYNC));
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, CheckSiCode<SEGV_MTESERR>, SA_SIGINFO);
        p[-1] = 42;
      },
      testing::ExitedWithCode(1), "");

  EXPECT_TRUE(SetTagCheckingLevel(PR_MTE_TCF_NONE));
  volatile int oob ATTRIBUTE_UNUSED = p[-1];

  HeapTaggingLevel tag_level = M_HEAP_TAGGING_LEVEL_TBI;
  EXPECT_FALSE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));

  tag_level = M_HEAP_TAGGING_LEVEL_NONE;
  EXPECT_TRUE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));
  std::unique_ptr<int[]> p2 = std::make_unique<int[]>(4);
  EXPECT_EQ(0u, reinterpret_cast<uintptr_t>(p2.get()) >> 56);

  tag_level = M_HEAP_TAGGING_LEVEL_ASYNC;
  EXPECT_FALSE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));

  tag_level = M_HEAP_TAGGING_LEVEL_NONE;
  EXPECT_TRUE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));
#else
  GTEST_SKIP() << "arm64 only";
#endif
}
