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

#include "gtest_globals.h"

#include <gtest/gtest.h>
#include "utils.h"

#include <string>

static std::string get_testlib_root() {
  std::string out_path;
  const char* data_dir = getenv("ANDROID_DATA");
  if (data_dir == nullptr) {
    out_path = "/data";
  } else {
    out_path = data_dir;
  }

  out_path = out_path + "/nativetest";
#if defined(__LP64__)
  out_path += "64";
#endif
  out_path += "/bionic-loader-test-libs";
  std::string real_path;
  if (!get_realpath(out_path, &real_path)) {
    abort();
  }

  return real_path;
}

const std::string g_testlib_root = get_testlib_root();
