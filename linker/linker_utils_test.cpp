/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <gtest/gtest.h>

#include "linker_utils.h"
#include "platform/bionic/page.h"

TEST(linker_utils, format_string) {
  std::vector<std::pair<std::string, std::string>> params = {{ "LIB", "lib32"}, { "SDKVER", "42"}};
  std::string str_smoke = "LIB$LIB${LIB${SDKVER}SDKVER$TEST$";
  format_string(&str_smoke, params);
  ASSERT_EQ("LIBlib32${LIB42SDKVER$TEST$", str_smoke);
}

TEST(linker_utils, normalize_path_smoke) {
  std::string output;
  ASSERT_TRUE(normalize_path("/../root///dir/.///dir2/somedir/../zipfile!/dir/dir9//..///afile", &output));
  ASSERT_EQ("/root/dir/dir2/zipfile!/dir/afile", output);

  ASSERT_TRUE(normalize_path("/../root///dir/.///dir2/somedir/.../zipfile!/.dir/dir9//..///afile", &output));
  ASSERT_EQ("/root/dir/dir2/somedir/.../zipfile!/.dir/afile", output);

  ASSERT_TRUE(normalize_path("/root/..", &output));
  ASSERT_EQ("/", output);

  ASSERT_TRUE(normalize_path("/root/notroot/..", &output));
  ASSERT_EQ("/root/", output);

  ASSERT_TRUE(normalize_path("/a/../../b", &output));
  ASSERT_EQ("/b", output);

  ASSERT_TRUE(normalize_path("/..", &output));
  ASSERT_EQ("/", output);

  output = "unchanged";
  ASSERT_FALSE(normalize_path("root///dir/.///dir2/somedir/../zipfile!/dir/dir9//..///afile", &output));
  ASSERT_EQ("unchanged", output);
}

TEST(linker_utils, file_is_in_dir_smoke) {
  ASSERT_TRUE(file_is_in_dir("/foo/bar/file", "/foo/bar"));
  ASSERT_FALSE(file_is_in_dir("/foo/bar/file", "/foo"));

  ASSERT_FALSE(file_is_in_dir("/foo/bar/file", "/bar/foo"));

  ASSERT_TRUE(file_is_in_dir("/file", ""));
  ASSERT_FALSE(file_is_in_dir("/file", "/"));
}

TEST(linker_utils, file_is_under_dir_smoke) {
  ASSERT_TRUE(file_is_under_dir("/foo/bar/file", "/foo/bar"));
  ASSERT_TRUE(file_is_under_dir("/foo/bar/file", "/foo"));

  ASSERT_FALSE(file_is_under_dir("/foo/bar/file", "/bar/foo"));

  ASSERT_TRUE(file_is_under_dir("/file", ""));
  ASSERT_TRUE(file_is_under_dir("/foo/bar/file", ""));
  ASSERT_FALSE(file_is_under_dir("/file", "/"));
  ASSERT_FALSE(file_is_under_dir("/foo/bar/file", "/"));
}

TEST(linker_utils, parse_zip_path_smoke) {
  std::string zip_path;
  std::string entry_path;

  ASSERT_FALSE(parse_zip_path("/not/a/zip/path/file.zip", &zip_path, &entry_path));
  ASSERT_FALSE(parse_zip_path("/not/a/zip/path/file.zip!path/in/zip", &zip_path, &entry_path));
  ASSERT_TRUE(parse_zip_path("/zip/path/file.zip!/path/in/zip", &zip_path, &entry_path));
  ASSERT_EQ("/zip/path/file.zip", zip_path);
  ASSERT_EQ("path/in/zip", entry_path);

  ASSERT_TRUE(parse_zip_path("/zip/path/file2.zip!/", &zip_path, &entry_path));
  ASSERT_EQ("/zip/path/file2.zip", zip_path);
  ASSERT_EQ("", entry_path);
}

TEST(linker_utils, page_start) {
  const size_t kPageSize = page_size();

  if (kPageSize == 4096) {
    ASSERT_EQ(0x0001000U, page_start(0x0001000));
    ASSERT_EQ(0x3002000U, page_start(0x300222f));
    ASSERT_EQ(0x6001000U, page_start(0x6001fff));
  } else if (kPageSize == 16384) {
    ASSERT_EQ(0x0004000U, page_start(0x0004000));
    ASSERT_EQ(0x3008000U, page_start(0x300822f));
    ASSERT_EQ(0x6004000U, page_start(0x6004fff));
  } else {
    FAIL() << "Page size not supported " << kPageSize;
  }
}

TEST(linker_utils, page_offset) {
  const size_t kPageSize = page_size();

  if (kPageSize == 4096) {
    ASSERT_EQ(0x0U, page_offset(0x0001000));
    ASSERT_EQ(0x22fU, page_offset(0x30222f));
    ASSERT_EQ(0xfffU, page_offset(0x6001fff));
  } else if (kPageSize == 16384) {
    ASSERT_EQ(0x0U, page_offset(0x0004000));
    ASSERT_EQ(0x322fU, page_offset(0x30322f));
    ASSERT_EQ(0x3fffU, page_offset(0x6003fff));
  } else {
    FAIL() << "Page size not supported " << kPageSize;
  }
}

TEST(linker_utils, safe_add) {
  int64_t val = 42;
  ASSERT_FALSE(safe_add(&val, INT64_MAX-20, 21U));
  ASSERT_EQ(42, val);
  ASSERT_TRUE(safe_add(&val, INT64_MAX-42, 42U));
  ASSERT_EQ(INT64_MAX, val);
  ASSERT_TRUE(safe_add(&val, 2000, 42U));
  ASSERT_EQ(2042, val);
}
