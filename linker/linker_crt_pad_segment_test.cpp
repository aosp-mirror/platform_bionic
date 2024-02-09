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

#include <android-base/file.h>
#include <android-base/unique_fd.h>

#include "linker_phdr.h"

#include <gtest/gtest.h>

#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

using ::android::base::GetExecutableDirectory;
using ::android::base::unique_fd;

namespace {

static std::string GetTestElfPath(const std::string& filename) {
  static std::string test_data_dir = GetExecutableDirectory();

  return test_data_dir + "/" + filename;
}

bool GetPadSegment(const std::string& elf_path) {
  std::string path = GetTestElfPath(elf_path);

  unique_fd fd{TEMP_FAILURE_RETRY(open(path.c_str(), O_CLOEXEC | O_RDWR))};
  EXPECT_GE(fd.get(), 0) << "Failed to open " << path << ": " << strerror(errno);

  struct stat file_stat;
  EXPECT_NE(TEMP_FAILURE_RETRY(fstat(fd.get(), &file_stat)), -1)
        << "Failed to stat " << path << ": " << strerror(errno);

  ElfReader elf_reader;
  EXPECT_TRUE(elf_reader.Read(path.c_str(), fd.get(), 0, file_stat.st_size))
        << "Failed to read ELF file";

  return elf_reader.should_pad_segments();
}

};  // anonymous namespace

TEST(crt_pad_segment, note_absent) {
  ASSERT_FALSE(GetPadSegment("no_crt_pad_segment.so"));
}

TEST(crt_pad_segment, note_present_and_enabled) {
  ASSERT_TRUE(GetPadSegment("crt_pad_segment_enabled.so"));
}

TEST(crt_pad_segment, note_present_and_disabled) {
  ASSERT_FALSE(GetPadSegment("crt_pad_segment_disabled.so"));
}
