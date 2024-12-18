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

#pragma once

#include "elf_max_page_size.h"
#include "gtest_globals.h"

#include <android-base/stringprintf.h>

#include <string>

#include <dlfcn.h>
#include <gtest/gtest.h>
#include <unistd.h>

static inline void OpenTestLibrary(std::string lib, bool expect_fail, void** handle) {
  void* _handle = dlopen(lib.c_str(), RTLD_NODELETE);
  const char* dlopen_error = dlerror();

  if (expect_fail) {
    ASSERT_EQ(_handle, nullptr);

    const std::string expected_error = android::base::StringPrintf(
        "dlopen failed: \"%s\" program alignment (%d) cannot be smaller than system page size (%d)",
        lib.c_str(), 4096, getpagesize());

    ASSERT_EQ(expected_error, dlopen_error);
  } else {
    ASSERT_NE(_handle, nullptr) << "Failed to dlopen shared library \"" << lib
                                << "\": " << dlopen_error;
  }

  *handle = _handle;
}

static inline void CallTestFunction(void* handle) {
  loader_test_func_t loader_test_func = (loader_test_func_t)dlsym(handle, "loader_test_func");
  const char* dlsym_error = dlerror();

  ASSERT_EQ(dlsym_error, nullptr) << "Failed to locate symbol \"loader_test_func\": "
                                  << dlsym_error;

  int res = loader_test_func();
  ASSERT_EQ(res, TEST_RESULT_BASE + TEST_RESULT_INCREMENT);

  // Call loader_test_func() twice to ensure we can modify writeable data and bss data
  res = loader_test_func();
  ASSERT_EQ(res, TEST_RESULT_BASE + (2 * TEST_RESULT_INCREMENT));
}
