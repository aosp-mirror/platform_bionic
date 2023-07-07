/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <gtest/gtest.h>

extern "C" {
int __cxa_atexit(void (*func)(void*), void* arg, void* dso);

// TODO(b/175635923). __cxa_finalize's return type should actually be "void",
// but it is declared "int" here instead to be compatible with the declaration
// in an old version of cxxabi.h, which is included indirectly. The declarations
// of __cxa_atexit and __cxa_finalize are removed from newer versions of
// cxxabi.h, so once libc++ is updated, this return type should be changed to
// "void".
int __cxa_finalize(void* dso);
}

TEST(__cxa_atexit, simple) {
  int counter = 0;

  __cxa_atexit([](void* arg) { ++*static_cast<int*>(arg); }, &counter, &counter);

  __cxa_finalize(&counter);
  ASSERT_EQ(counter, 1);

  // The handler won't be called twice.
  __cxa_finalize(&counter);
  ASSERT_EQ(counter, 1);
}

TEST(__cxa_atexit, order) {
  static std::vector<int> actual;

  char handles[2];

  auto append_to_actual = [](void* arg) {
    int* idx = static_cast<int*>(arg);
    actual.push_back(*idx);
    delete idx;
  };

  for (int i = 0; i < 500; ++i) {
    __cxa_atexit(append_to_actual, new int{i}, &handles[i % 2]);
  }

  __cxa_finalize(&handles[0]);

  for (int i = 500; i < 750; ++i) {
    __cxa_atexit(append_to_actual, new int{i}, &handles[1]);
  }

  __cxa_finalize(&handles[1]);

  std::vector<int> expected;
  for (int i = 498; i >= 0; i -= 2) expected.push_back(i);
  for (int i = 749; i >= 500; --i) expected.push_back(i);
  for (int i = 499; i >= 1; i -= 2) expected.push_back(i);

  ASSERT_EQ(expected.size(), actual.size());
  for (size_t i = 0; i < expected.size(); ++i) {
    ASSERT_EQ(expected[i], actual[i]) << "index=" << i;
  }
}
