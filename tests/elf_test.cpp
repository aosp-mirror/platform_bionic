/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <elf.h>

#include <gtest/gtest.h>

// https://github.com/android-ndk/ndk/issues/377
TEST(elf, have_ELF_ST_INFO_macros) {
  uint8_t info;

  // 0x0f
  info = ELF32_ST_INFO(STB_LOCAL, STT_HIPROC);
  ASSERT_EQ(STB_LOCAL, ELF32_ST_BIND(info));
  ASSERT_EQ(STT_HIPROC, ELF32_ST_TYPE(info));

  // 0x0f
  info = ELF64_ST_INFO(STB_LOCAL, STT_HIPROC);
  ASSERT_EQ(STB_LOCAL, ELF64_ST_BIND(info));
  ASSERT_EQ(STT_HIPROC, ELF64_ST_TYPE(info));

  // 0xf0
  info = ELF32_ST_INFO(STB_LOCAL, STT_HIPROC);
  ASSERT_EQ(STB_LOCAL, ELF32_ST_BIND(info));
  ASSERT_EQ(STT_HIPROC, ELF32_ST_TYPE(info));

  // 0xf0
  info = ELF64_ST_INFO(STB_LOCAL, STT_HIPROC);
  ASSERT_EQ(STB_LOCAL, ELF64_ST_BIND(info));
  ASSERT_EQ(STT_HIPROC, ELF64_ST_TYPE(info));
}
