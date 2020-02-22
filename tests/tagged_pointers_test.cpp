/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <sys/prctl.h>

#include "platform/bionic/malloc.h"
#include "platform/bionic/mte.h"
#include "utils.h"

#include <bionic/malloc_tagged_pointers.h>

static bool KernelSupportsTaggedPointers() {
#ifdef __aarch64__
#define PR_SET_TAGGED_ADDR_CTRL 55
#define PR_TAGGED_ADDR_ENABLE (1UL << 0)
  int res = prctl(PR_GET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
  return res >= 0 && res & PR_TAGGED_ADDR_ENABLE;
#else
  return false;
#endif
}

TEST(tagged_pointers, check_tagged_pointer_dies) {
  if (!KernelSupportsTaggedPointers()) {
    GTEST_SKIP() << "Kernel doesn't support tagged pointers.";
  }

#ifdef __aarch64__
  if (mte_supported()) {
    GTEST_SKIP() << "Tagged pointers are not used on MTE hardware.";
  }

  void *x = malloc(1);

  // Ensure that `x` has a pointer tag.
  EXPECT_NE(reinterpret_cast<uintptr_t>(x) >> 56, 0u);

  x = untag_address(x);
  EXPECT_DEATH(free(x), "Pointer tag for 0x[a-zA-Z0-9]* was truncated");

  HeapTaggingLevel tag_level = M_HEAP_TAGGING_LEVEL_TBI;
  EXPECT_TRUE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));
  EXPECT_DEATH(free(untag_address(malloc(1))), "Pointer tag for 0x[a-zA-Z0-9]* was truncated");

  tag_level = M_HEAP_TAGGING_LEVEL_ASYNC;
  EXPECT_FALSE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));

  x = malloc(1);
  void *y = malloc(1);
  // Disable heap tagging.
  tag_level = M_HEAP_TAGGING_LEVEL_NONE;
  EXPECT_TRUE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));
  // Ensure an older tagged pointer can still be freed.
  free(x);
  // Tag mismatch is not detected on old pointers.
  free(untag_address(y));
  // New pointers are not tagged.
  x = malloc(1);
  EXPECT_EQ(untag_address(x), x);
  free(x);

  // Switching back to checked mode is not possible.
  tag_level = M_HEAP_TAGGING_LEVEL_TBI;
  EXPECT_FALSE(android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &tag_level, sizeof(tag_level)));
  // We remain in the unchecked mode.
  x = malloc(1);
  EXPECT_EQ(untag_address(x), x);
  free(x);
#endif // defined(__aarch64__)
}
