/*
 * Copyright (C) 2012 The Android Open Source Project
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

#define __STDINT_LIMITS

#include <gtest/gtest.h>

#include <errno.h>
#include <stdint.h>
#include <unistd.h>

TEST(unistd, sysconf_SC_MONOTONIC_CLOCK) {
  ASSERT_GT(sysconf(_SC_MONOTONIC_CLOCK), 0);
}

static void* get_brk() {
  return sbrk(0);
}

static void* page_align(uintptr_t addr) {
  uintptr_t mask = sysconf(_SC_PAGE_SIZE) - 1;
  return reinterpret_cast<void*>((addr + mask) & ~mask);
}

TEST(unistd, brk) {
  void* initial_break = get_brk();

  // The kernel aligns the break to a page.
  void* new_break = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(initial_break) + 1);
  ASSERT_EQ(0, brk(new_break));
  ASSERT_GE(get_brk(), new_break);

  new_break = page_align(reinterpret_cast<uintptr_t>(initial_break) + sysconf(_SC_PAGE_SIZE));
  ASSERT_EQ(0, brk(new_break));
  ASSERT_EQ(get_brk(), new_break);
}

TEST(unistd, brk_ENOMEM) {
  ASSERT_EQ(-1, brk(reinterpret_cast<void*>(-1)));
  ASSERT_EQ(ENOMEM, errno);
}

#if defined(__GLIBC__)
#define SBRK_MIN (-2147483647-1)
#define SBRK_MAX (2147483647)
#else
#define SBRK_MIN PTRDIFF_MIN
#define SBRK_MAX PTRDIFF_MAX
#endif

TEST(unistd, sbrk_ENOMEM) {
#if defined(__BIONIC__) && !defined(__LP64__)
  // There is no way to guarantee that all overflow conditions can be tested
  // without manipulating the underlying values of the current break.
  extern void* __bionic_brk;

  class ScopedBrk {
  public:
    ScopedBrk() : saved_brk_(__bionic_brk) {}
    virtual ~ScopedBrk() { __bionic_brk = saved_brk_; }

  private:
    void* saved_brk_;
  };

  ScopedBrk scope_brk;

  // Set the current break to a point that will cause an overflow.
  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX) + 2);

  // Can't increase by so much that we'd overflow.
  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MAX));
  ASSERT_EQ(ENOMEM, errno);

  // Set the current break to a point that will cause an overflow.
  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX));

  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MIN));
  ASSERT_EQ(ENOMEM, errno);

  __bionic_brk = reinterpret_cast<void*>(static_cast<uintptr_t>(PTRDIFF_MAX) - 1);

  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MIN + 1));
  ASSERT_EQ(ENOMEM, errno);
#else
  class ScopedBrk {
  public:
    ScopedBrk() : saved_brk_(get_brk()) {}
    virtual ~ScopedBrk() { brk(saved_brk_); }

  private:
    void* saved_brk_;
  };

  ScopedBrk scope_brk;

  uintptr_t cur_brk = reinterpret_cast<uintptr_t>(get_brk());
  if (cur_brk < static_cast<uintptr_t>(-(SBRK_MIN+1))) {
    // Do the overflow test for a max negative increment.
    ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(SBRK_MIN));
#if defined(__BIONIC__)
    // GLIBC does not set errno in overflow case.
    ASSERT_EQ(ENOMEM, errno);
#endif
  }

  uintptr_t overflow_brk = static_cast<uintptr_t>(SBRK_MAX) + 2;
  if (cur_brk < overflow_brk) {
    // Try and move the value to PTRDIFF_MAX + 2.
    cur_brk = reinterpret_cast<uintptr_t>(sbrk(overflow_brk));
  }
  if (cur_brk >= overflow_brk) {
    ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(SBRK_MAX));
#if defined(__BIONIC__)
    // GLIBC does not set errno in overflow case.
    ASSERT_EQ(ENOMEM, errno);
#endif
  }
#endif
}
