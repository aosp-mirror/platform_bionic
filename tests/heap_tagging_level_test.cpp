/*
 * Copyright (C) 2020 The Android Open Source Project
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

#if defined(__BIONIC__)
#include "platform/bionic/malloc.h"
#include "platform/bionic/mte.h"
#include "utils.h"

#include "SignalUtils.h"

#include <bionic/malloc_tagged_pointers.h>

static bool KernelSupportsTaggedPointers() {
#ifdef __aarch64__
  int res = prctl(PR_GET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
  return res >= 0 && res & PR_TAGGED_ADDR_ENABLE;
#else
  return false;
#endif
}

static bool SetHeapTaggingLevel(HeapTaggingLevel level) {
  return android_mallopt(M_SET_HEAP_TAGGING_LEVEL, &level, sizeof(level));
}
#endif

TEST(heap_tagging_level, tagged_pointer_dies) {
#if defined(__BIONIC__)
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

  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
  EXPECT_DEATH(free(untag_address(malloc(1))), "Pointer tag for 0x[a-zA-Z0-9]* was truncated");

  x = malloc(1);
  void *y = malloc(1);
  // Disable heap tagging.
  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
  // Ensure an older tagged pointer can still be freed.
  free(x);
  // Tag mismatch is not detected on old pointers.
  free(untag_address(y));
#endif // defined(__aarch64__)
#else
  GTEST_SKIP() << "bionic-only test";
#endif // defined(__BIONIC__)
}

#if defined(__BIONIC__) && defined(__aarch64__)
void ExitWithSiCode(int, siginfo_t* info, void*) {
  _exit(info->si_code);
}
#endif

TEST(heap_tagging_level, sync_async_bad_accesses_die) {
#if defined(__BIONIC__) && defined(__aarch64__)
  if (!(getauxval(AT_HWCAP2) & HWCAP2_MTE)) {
    GTEST_SKIP() << "requires MTE support";
  }

  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);

  // First, check that memory tagging is enabled and the default tag checking level is async.
  // We assume that scudo is used on all MTE enabled hardware; scudo inserts a header with a
  // mismatching tag before each allocation.
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, ExitWithSiCode, SA_SIGINFO);
        p[-1] = 42;
      },
      testing::ExitedWithCode(SEGV_MTEAERR), "");

  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, ExitWithSiCode, SA_SIGINFO);
        p[-1] = 42;
      },
      testing::ExitedWithCode(SEGV_MTESERR), "");

  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
  volatile int oob ATTRIBUTE_UNUSED = p[-1];
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}

TEST(heap_tagging_level, none_pointers_untagged) {
#if defined(__BIONIC__)
  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);
  EXPECT_EQ(untag_address(p.get()), p.get());
#else
  GTEST_SKIP() << "bionic-only test";
#endif
}

TEST(heap_tagging_level, tagging_level_transitions) {
#if defined(__BIONIC__) && defined(__aarch64__)
  if (!KernelSupportsTaggedPointers()) {
    GTEST_SKIP() << "Kernel doesn't support tagged pointers.";
  }

  EXPECT_FALSE(SetHeapTaggingLevel(static_cast<HeapTaggingLevel>(12345)));

  if (mte_supported()) {
    // ASYNC -> ...
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));

    // SYNC -> ...
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
  } else {
    // TBI -> ...
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
  }

  // TBI -> NONE on non-MTE, ASYNC -> NONE on MTE.
  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));

  // NONE -> ...
  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
  EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
  EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
  EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}

TEST(heap_tagging_level, tagging_level_transition_sync_none) {
#if defined(__BIONIC__) && defined(__aarch64__)
  // We can't test SYNC -> NONE in tagging_level_transitions because we can only make one transition
  // to NONE (which we use to test ASYNC -> NONE), so we test it here separately.
  if (!mte_supported()) {
    GTEST_SKIP() << "requires MTE support";
  }

  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}
