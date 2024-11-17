/*
 * Copyright (C) 2024 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <thread>

#include <dlfcn.h>
#include <stdlib.h>

#include <android-base/logging.h>
#include <gtest/gtest.h>

static size_t NumberBuffers() {
  size_t bufs = 0;
  std::ifstream file("/proc/self/maps");
  CHECK(file.is_open());
  std::string line;
  while (std::getline(file, line)) {
    if (line.find("stack_mte_ring") != std::string::npos) {
      ++bufs;
    }
  }
  return bufs;
}

static size_t NumberThreads() {
  std::filesystem::directory_iterator di("/proc/self/task");
  return std::distance(begin(di), end(di));
}

TEST(MemtagStackAbiTest, MainThread) {
#if defined(__BIONIC__) && defined(__aarch64__)
  ASSERT_EQ(NumberBuffers(), 1U);
  ASSERT_EQ(NumberBuffers(), NumberThreads());
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}

TEST(MemtagStackAbiTest, JoinableThread) {
#if defined(__BIONIC__) && defined(__aarch64__)
  ASSERT_EQ(NumberBuffers(), 1U);
  ASSERT_EQ(NumberBuffers(), NumberThreads());
  std::thread th([] {
    ASSERT_EQ(NumberBuffers(), 2U);
    ASSERT_EQ(NumberBuffers(), NumberThreads());
  });
  th.join();
  ASSERT_EQ(NumberBuffers(), 1U);
  ASSERT_EQ(NumberBuffers(), NumberThreads());
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}

TEST(MemtagStackAbiTest, DetachedThread) {
#if defined(__BIONIC__) && defined(__aarch64__)
  ASSERT_EQ(NumberBuffers(), 1U);
  ASSERT_EQ(NumberBuffers(), NumberThreads());
  std::thread th([] {
    ASSERT_EQ(NumberBuffers(), 2U);
    ASSERT_EQ(NumberBuffers(), NumberThreads());
  });
  th.detach();
  // Leave the thread some time to exit.
  for (int i = 0; NumberBuffers() != 1 && i < 3; ++i) {
    sleep(1);
  }
  ASSERT_EQ(NumberBuffers(), 1U);
  ASSERT_EQ(NumberBuffers(), NumberThreads());
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}
