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

#include <sys/cdefs.h>

#if defined(__BIONIC__)

#include <gtest/gtest.h>

#include <android-base/macros.h>
#include <bionic/mte.h>
#include "utils.h"

__attribute__((no_sanitize("hwaddress")))
static void test_tag_mismatch() {
  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);
  p[0] = 1;
  int* mistagged_p ATTRIBUTE_UNUSED =
      reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(p.get()) + (1ULL << 56));
  {
    ScopedDisableMTE x;
    // Test that nested ScopedDisableMTE does not reset MTE state.
    { ScopedDisableMTE y; }
#if defined(__aarch64__)
    volatile int load ATTRIBUTE_UNUSED = *mistagged_p;
#endif
  }
#if defined(__aarch64__)
  if (mte_supported() && running_with_mte()) {
    EXPECT_DEATH(
        {
          volatile int load ATTRIBUTE_UNUSED = *mistagged_p;
        },
        "");
  }
#endif
}

TEST(mte_test, ScopedDisableMTE) {
  // With native_bridge, native and emulated parts exchange data, including pointers.
  // This implies tagging on native and emulated architectures should match, which is
  // not the case at the moment.
  SKIP_WITH_NATIVE_BRIDGE;

  test_tag_mismatch();
}

#endif  // __BIONIC__
