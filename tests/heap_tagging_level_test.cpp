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

#include <malloc.h>
#include <sys/prctl.h>

#include <android-base/silent_death_test.h>

#if defined(__BIONIC__)
#include "gtest_globals.h"
#include "platform/bionic/mte.h"
#include "utils.h"

#include "SignalUtils.h"

#include <android-base/properties.h>
#include <android-base/test_utils.h>
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
  return mallopt(M_BIONIC_SET_HEAP_TAGGING_LEVEL, level);
}
#endif

using heap_tagging_level_DeathTest = SilentDeathTest;

TEST_F(heap_tagging_level_DeathTest, tagged_pointer_dies) {
#if defined(__BIONIC__)
  if (!KernelSupportsTaggedPointers()) {
    GTEST_SKIP() << "Kernel doesn't support tagged pointers.";
  }

#ifdef __aarch64__
  if (mte_supported()) {
    GTEST_SKIP() << "Tagged pointers are not used on MTE hardware.";
  }
  if (running_with_hwasan()) {
    GTEST_SKIP() << "Tagged heap pointers feature is disabled under HWASan.";
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

namespace {
#if defined(__BIONIC__) && defined(__aarch64__)
void ExitWithSiCode(int, siginfo_t* info, void*) {
  _exit(info->si_code);
}

template <typename Pred>
class Or {
  Pred A, B;

 public:
  Or(Pred A, Pred B) : A(A), B(B) {}
  bool operator()(int exit_status) { return A(exit_status) || B(exit_status); }
};
#endif

TEST(heap_tagging_level, sync_async_bad_accesses_die) {
#if defined(__BIONIC__) && defined(__aarch64__)
  if (!mte_supported() || !running_with_mte()) {
    GTEST_SKIP() << "requires MTE to be enabled";
  }

  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);
  volatile int sink ATTRIBUTE_UNUSED;

  // We assume that scudo is used on all MTE enabled hardware; scudo inserts a header with a
  // mismatching tag before each allocation.
  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, ExitWithSiCode, SA_SIGINFO);
        p[-1] = 42;
      },
      testing::ExitedWithCode(SEGV_MTESERR), "");
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, ExitWithSiCode, SA_SIGINFO);
        sink = p[-1];
      },
      testing::ExitedWithCode(SEGV_MTESERR), "");

  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, ExitWithSiCode, SA_SIGINFO);
        p[-1] = 42;
      },
      Or(testing::ExitedWithCode(SEGV_MTESERR), testing::ExitedWithCode(SEGV_MTEAERR)), "");
  EXPECT_EXIT(
      {
        ScopedSignalHandler ssh(SIGSEGV, ExitWithSiCode, SA_SIGINFO);
        sink = p[-1];
      },
      Or(testing::ExitedWithCode(SEGV_MTESERR), testing::ExitedWithCode(SEGV_MTEAERR)), "");

  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
  sink = p[-1];
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}
}  // namespace

TEST(heap_tagging_level, none_pointers_untagged) {
#if defined(__BIONIC__)
  if (running_with_hwasan()) {
    GTEST_SKIP() << "HWASan is unaffected by heap tagging level.";
  }
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

  if (running_with_hwasan()) {
    // NONE -> ...
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
  } else if (mte_supported() && running_with_mte()) {
    // ASYNC -> ...
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));

    // SYNC -> ...
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
  } else if (!mte_supported()) {
    // TBI -> ...
    EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_TBI));
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_ASYNC));
    EXPECT_FALSE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
  }

  // TBI -> NONE on non-MTE, ASYNC|SYNC|NONE -> NONE on MTE.
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
  if (!mte_supported() || !running_with_mte()) {
    GTEST_SKIP() << "requires MTE to be enabled";
  }

  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_SYNC));
  EXPECT_TRUE(SetHeapTaggingLevel(M_HEAP_TAGGING_LEVEL_NONE));
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}

enum class MemtagNote { NONE, ASYNC, SYNC };
class MemtagNoteTest : public testing::TestWithParam<std::tuple<MemtagNote, bool>> {};

static const char* kNoteSuffix[] = {"disabled", "async", "sync"};

TEST_P(MemtagNoteTest, SEGV) {
#if defined(__BIONIC__) && defined(__aarch64__)
  SKIP_WITH_NATIVE_BRIDGE;  // http://b/242170715
  if (android::base::GetProperty("persist.arm64.memtag.default", "") != "") {
    GTEST_SKIP() << "not supported when overriding memtag mode with property";
  }
  // Note that we do not check running_with_hwasan() - what matters here is whether the test binary
  // itself is built with HWASan.
  bool withMTE = getauxval(AT_HWCAP2) & HWCAP2_MTE;

  // Note that we do not check the exact si_code of the "async" variant, as it may be auto-upgraded
  // to asymm or even sync.
  const char* kExpectedOutputMTE[] = {"normal exit\n", "SEGV_MTE[AS]ERR\n", "SEGV_MTESERR\n"};
  const char* kExpectedOutputNonMTE[] = {"normal exit\n", "normal exit\n", "normal exit\n"};
  const char** kExpectedOutput = withMTE ? kExpectedOutputMTE : kExpectedOutputNonMTE;

  MemtagNote note = std::get<0>(GetParam());
  bool isStatic = std::get<1>(GetParam());
  std::string helper_base = std::string("heap_tagging_") + (isStatic ? "static_" : "") +
                            kNoteSuffix[static_cast<int>(note)] + "_helper";
  std::string helper = GetTestlibRoot() + "/" + helper_base;
  chmod(helper.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({helper.c_str(), nullptr});
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0,
          kExpectedOutput[static_cast<int>(note)]);
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}

INSTANTIATE_TEST_SUITE_P(, MemtagNoteTest,
                         testing::Combine(testing::Values(MemtagNote::NONE, MemtagNote::ASYNC,
                                                          MemtagNote::SYNC),
                                          testing::Bool()),
                         [](const ::testing::TestParamInfo<MemtagNoteTest::ParamType>& info) {
                           MemtagNote note = std::get<0>(info.param);
                           std::string s = kNoteSuffix[static_cast<int>(note)];
                           if (std::get<1>(info.param)) s += "_static";
                           return s;
                         });
