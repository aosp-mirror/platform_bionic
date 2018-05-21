/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "dlfcn_symlink_support.h"

#include <gtest/gtest.h>

#include <dlfcn.h>
#include <libgen.h>
#include <link.h>
#include <unistd.h>

#include <android-base/strings.h>

#include <algorithm>
#include <string>
#include <vector>

static const constexpr char* source_file_name = "libdlext_test.so";
static const constexpr char* symlink_name_prefix = "libdlext_test_";

static int dl_callback(struct dl_phdr_info *info, size_t /* size */, void *data) {
  // The case when path is not absolute and is equal to source_file_name
  // is disregarded intentionally since in bionic dlpi_name should always
  // be realpath to a shared object.
  const std::string suffix = std::string("/") + source_file_name;

  // TODO (dimitry): remove this check once fake libdl.so is gone
  if (info->dlpi_name == nullptr) {
    // This is linker imposing as libdl.so - skip it
    return 0;
  }

  if (android::base::EndsWith(info->dlpi_name, suffix)) {
    std::string* path = reinterpret_cast<std::string*>(data);
    *path = info->dlpi_name;
    return 1; // found
  }

  return 0;
}

void create_dlfcn_test_symlink(const char* suffix, std::string* result) {
  void* handle = dlopen(source_file_name, RTLD_NOW);
  std::string source_file_path;

  ASSERT_TRUE(handle != nullptr) << dlerror();
  ASSERT_TRUE(dl_iterate_phdr(dl_callback, &source_file_path) == 1)
      << "dl_phdr_info for \"" << source_file_name << "\" was not found.";

  dlclose(handle);
  std::vector<char> buf;
  std::copy(source_file_path.begin(), source_file_path.end(), std::back_inserter(buf));
  buf.push_back('\0');

  std::string path_dir = dirname(&buf[0]);
  std::string link_path = path_dir + "/" + symlink_name_prefix + suffix + ".so";

  ASSERT_TRUE(symlink(source_file_path.c_str(), link_path.c_str()) == 0) << strerror(errno);
  *result = link_path;
}

void remove_dlfcn_test_symlink(const std::string& path) {
  ASSERT_TRUE(unlink(path.c_str()) == 0) << strerror(errno);
}
