/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>
#include "private/bionic_prctl.h"

// http://b/20017123.
TEST(sys_prctl, bug_20017123) {
#if defined(__ANDROID__)
  size_t page_size = static_cast<size_t>(sysconf(_SC_PAGESIZE));
  void* p = mmap(NULL, page_size * 3, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  ASSERT_NE(MAP_FAILED, p);
  ASSERT_EQ(0, mprotect(p, page_size, PROT_NONE));
  ASSERT_NE(-1, prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, p, page_size * 3, "anonymous map space"));
  volatile char* vp = reinterpret_cast<volatile char*>(p);
  // Below memory access causes SEGV if the memory map is screwed up.
  *(vp + page_size) = 0;
  ASSERT_EQ(0, munmap(p, page_size * 3));
#else
  GTEST_LOG_(INFO) << "This test does nothing as it tests an Android specific kernel feature.";
#endif
}
