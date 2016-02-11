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

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#include <tinyxml2.h>

#include "private/bionic_config.h"

TEST(malloc, malloc_std) {
  // Simple malloc test.
  void *ptr = malloc(100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  free(ptr);
}

TEST(malloc, malloc_overflow) {
  errno = 0;
  ASSERT_EQ(NULL, malloc(SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, calloc_std) {
  // Simple calloc test.
  size_t alloc_len = 100;
  char *ptr = (char *)calloc(1, alloc_len);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(alloc_len, malloc_usable_size(ptr));
  for (size_t i = 0; i < alloc_len; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_illegal) {
  errno = 0;
  ASSERT_EQ(NULL, calloc(-1, 100));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, calloc_overflow) {
  errno = 0;
  ASSERT_EQ(NULL, calloc(1, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(NULL, calloc(SIZE_MAX, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(NULL, calloc(2, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(NULL, calloc(SIZE_MAX, 2));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, memalign_multiple) {
  // Memalign test where the alignment is any value.
  for (size_t i = 0; i <= 12; i++) {
    for (size_t alignment = 1 << i; alignment < (1U << (i+1)); alignment++) {
      char *ptr = reinterpret_cast<char*>(memalign(alignment, 100));
      ASSERT_TRUE(ptr != NULL) << "Failed at alignment " << alignment;
      ASSERT_LE(100U, malloc_usable_size(ptr)) << "Failed at alignment " << alignment;
      ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % ((1U << i)))
          << "Failed at alignment " << alignment;
      free(ptr);
    }
  }
}

TEST(malloc, memalign_overflow) {
  ASSERT_EQ(NULL, memalign(4096, SIZE_MAX));
}

TEST(malloc, memalign_non_power2) {
  void* ptr;
  for (size_t align = 0; align <= 256; align++) {
    ptr = memalign(align, 1024);
    ASSERT_TRUE(ptr != NULL) << "Failed at align " << align;
    free(ptr);
  }
}

TEST(malloc, posix_memalign_non_power2) {
  void* ptr;
  ASSERT_EQ(EINVAL, posix_memalign(&ptr, 17, 1024));
}

TEST(malloc, posix_memalign_overflow) {
  void* ptr;
  ASSERT_NE(0, posix_memalign(&ptr, 16, SIZE_MAX));
}

TEST(malloc, memalign_realloc) {
  // Memalign and then realloc the pointer a couple of times.
  for (size_t alignment = 1; alignment <= 4096; alignment <<= 1) {
    char *ptr = (char*)memalign(alignment, 100);
    ASSERT_TRUE(ptr != NULL);
    ASSERT_LE(100U, malloc_usable_size(ptr));
    ASSERT_EQ(0U, (intptr_t)ptr % alignment);
    memset(ptr, 0x23, 100);

    ptr = (char*)realloc(ptr, 200);
    ASSERT_TRUE(ptr != NULL);
    ASSERT_LE(200U, malloc_usable_size(ptr));
    ASSERT_TRUE(ptr != NULL);
    for (size_t i = 0; i < 100; i++) {
      ASSERT_EQ(0x23, ptr[i]);
    }
    memset(ptr, 0x45, 200);

    ptr = (char*)realloc(ptr, 300);
    ASSERT_TRUE(ptr != NULL);
    ASSERT_LE(300U, malloc_usable_size(ptr));
    for (size_t i = 0; i < 200; i++) {
      ASSERT_EQ(0x45, ptr[i]);
    }
    memset(ptr, 0x67, 300);

    ptr = (char*)realloc(ptr, 250);
    ASSERT_TRUE(ptr != NULL);
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
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  memset(ptr, 67, 100);

  ptr = (char *)realloc(ptr, 200);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(67, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, malloc_realloc_smaller) {
  // Realloc to a smaller size, malloc is used for the original allocation.
  char *ptr = (char *)malloc(200);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  memset(ptr, 67, 200);

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(67, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, malloc_multiple_realloc) {
  // Multiple reallocs, malloc is used for the original allocation.
  char *ptr = (char *)malloc(200);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  memset(ptr, 0x23, 200);

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 50);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(50U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 150);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(150U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }
  memset(ptr, 0x23, 150);

  ptr = (char*)realloc(ptr, 425);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(425U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 150; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_realloc_larger) {
  // Realloc to a larger size, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 200);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_realloc_smaller) {
  // Realloc to a smaller size, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 200);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(200U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_multiple_realloc) {
  // Multiple reallocs, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 200);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(200U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 50);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(50U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 150);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(150U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  memset(ptr, 0, 150);

  ptr = (char*)realloc(ptr, 425);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(425U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 150; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, realloc_overflow) {
  errno = 0;
  ASSERT_EQ(NULL, realloc(NULL, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  void* ptr = malloc(100);
  ASSERT_TRUE(ptr != NULL);
  errno = 0;
  ASSERT_EQ(NULL, realloc(ptr, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  free(ptr);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t);
extern "C" void* valloc(size_t);

TEST(malloc, pvalloc_std) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void* ptr = pvalloc(100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_TRUE((reinterpret_cast<uintptr_t>(ptr) & (pagesize-1)) == 0);
  ASSERT_LE(pagesize, malloc_usable_size(ptr));
  free(ptr);
}

TEST(malloc, pvalloc_overflow) {
  ASSERT_EQ(NULL, pvalloc(SIZE_MAX));
}

TEST(malloc, valloc_std) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void* ptr = pvalloc(100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_TRUE((reinterpret_cast<uintptr_t>(ptr) & (pagesize-1)) == 0);
  free(ptr);
}

TEST(malloc, valloc_overflow) {
  ASSERT_EQ(NULL, valloc(SIZE_MAX));
}
#endif

TEST(malloc, malloc_info) {
#ifdef __BIONIC__
  char* buf;
  size_t bufsize;
  FILE* memstream = open_memstream(&buf, &bufsize);
  ASSERT_NE(nullptr, memstream);
  ASSERT_EQ(0, malloc_info(0, memstream));
  ASSERT_EQ(0, fclose(memstream));

  tinyxml2::XMLDocument doc;
  ASSERT_EQ(tinyxml2::XML_SUCCESS, doc.Parse(buf));

  auto root = doc.FirstChildElement();
  ASSERT_NE(nullptr, root);
  ASSERT_STREQ("malloc", root->Name());
  ASSERT_STREQ("jemalloc-1", root->Attribute("version"));

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
