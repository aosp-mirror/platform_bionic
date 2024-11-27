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

#include "page_size_compat_helpers.h"

#include <android-base/properties.h>

extern "C" void android_set_16kb_appcompat_mode(bool enable_app_compat);

TEST(PageSize16KiBCompatTest, ElfAlignment4KiB_LoadElf) {
  if (getpagesize() != 0x4000) {
    GTEST_SKIP() << "This test is only applicable to 16kB page-size devices";
  }

  bool app_compat_enabled =
      android::base::GetBoolProperty("bionic.linker.16kb.app_compat.enabled", false);
  std::string lib = GetTestLibRoot() + "/libtest_elf_max_page_size_4kib.so";
  void* handle = nullptr;

  OpenTestLibrary(lib, !app_compat_enabled, &handle);

  if (app_compat_enabled) CallTestFunction(handle);
}

TEST(PageSize16KiBCompatTest, ElfAlignment4KiB_LoadElf_perAppOption) {
  if (getpagesize() != 0x4000) {
    GTEST_SKIP() << "This test is only applicable to 16kB page-size devices";
  }

  android_set_16kb_appcompat_mode(true);
  std::string lib = GetTestLibRoot() + "/libtest_elf_max_page_size_4kib.so";
  void* handle = nullptr;

  OpenTestLibrary(lib, false /*should_fail*/, &handle);
  CallTestFunction(handle);
  android_set_16kb_appcompat_mode(false);
}
