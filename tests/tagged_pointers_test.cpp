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
#include <sys/utsname.h>

#include "utils.h"

#include <bionic/malloc_tagged_pointers.h>

static bool KernelSupportsTaggedPointers() {
#ifdef __aarch64__
  utsname buf;
  utsname* tagged_buf =
      reinterpret_cast<utsname*>(reinterpret_cast<uintptr_t>(&buf) |
      (static_cast<uintptr_t>(0xAA) << TAG_SHIFT));
  // We use `uname()` here as a system call to determine if the kernel supports
  // tagged pointers. If ARM TBI is enabled, the kernel should truncate the tag
  // before populating `buf`, and `uname()` should return zero (indicating no
  // error). If ARM TBI isn't enabled, the kernel should return an error code
  // that indicates that the tagged memory couldn't be accessed. The exact
  // system call that we use here isn't important, it's just a convenient system
  // call that validates a pointer.
  return uname(tagged_buf) == 0;
#else
  return false;
#endif
}

TEST(tagged_pointers, check_tagged_pointer_dies) {
  if (!KernelSupportsTaggedPointers()) {
    GTEST_SKIP() << "Kernel doesn't support tagged pointers.";
  }

#ifdef __aarch64__
  void *x = malloc(1);

  // Ensure that `x` has a pointer tag.
  EXPECT_NE(reinterpret_cast<uintptr_t>(x) >> 56, 0u);

  x = untag_address(x);
  EXPECT_DEATH(free(x), "Pointer tag for 0x[a-zA-Z0-9]* was truncated");
#endif // defined(__aarch64__)
}
