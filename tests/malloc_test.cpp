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

#include <stdlib.h>
#include <malloc.h>

TEST(malloc, malloc_std) {
  // Simple malloc test.
  void *ptr = malloc(100);
  ASSERT_TRUE(ptr != NULL);
  ASSERT_LE(100U, malloc_usable_size(ptr));

  free(ptr);
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

TEST(malloc, memalign_multiple) {
  // Memalign test where the alignment is any value.
  for (size_t i = 0; i <= 12; i++) {
    for (size_t alignment = 1 << i; alignment < (1U << (i+1)); alignment++) {
      char *ptr = (char*)memalign(alignment, 100);
      ASSERT_TRUE(ptr != NULL);
      ASSERT_LE(100U, malloc_usable_size(ptr));
      ASSERT_EQ(0, (intptr_t)ptr % (1 << i));

      free(ptr);
    }
  }
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
