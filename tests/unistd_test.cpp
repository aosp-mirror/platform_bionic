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

TEST(unistd, sbrk_ENOMEM) {
  intptr_t current_brk = reinterpret_cast<intptr_t>(get_brk());

#if !defined(__GLIBC__)
  // Can't increase by so much that we'd overflow.
  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MAX));
  ASSERT_EQ(ENOMEM, errno);
#endif

  // Can't reduce by more than the current break.
  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(-(current_brk + 1)));
  ASSERT_EQ(ENOMEM, errno);

#if !defined(__GLIBC__)
  // The maximum negative value is an interesting special case that glibc gets wrong.
  ASSERT_EQ(reinterpret_cast<void*>(-1), sbrk(PTRDIFF_MIN));
  ASSERT_EQ(ENOMEM, errno);
#endif
}
