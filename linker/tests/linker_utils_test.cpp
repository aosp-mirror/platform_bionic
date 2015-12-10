/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <gtest/gtest.h>

#include "../linker_utils.h"

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
  ASSERT_EQ(0x0001000, page_start(0x0001000));
  ASSERT_EQ(0x3002000, page_start(0x300222f));
  ASSERT_EQ(0x6001000, page_start(0x6001fff));
}

TEST(linker_utils, page_offset) {
  ASSERT_EQ(0x0U, page_offset(0x0001000));
  ASSERT_EQ(0x22fU, page_offset(0x300222f));
  ASSERT_EQ(0xfffU, page_offset(0x6001fff));
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
