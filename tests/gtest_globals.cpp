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

#include <android-base/file.h>

#include <string>

std::string GetTestlibRoot() {
  // Calculate ANDROID_DATA assuming the binary is in "$ANDROID_DATA/somedir/binary-dir/binary"
  std::string path = android::base::Dirname(android::base::GetExecutablePath()) + "/..";

  std::string out_path;
  if (!android::base::Realpath(path.c_str(), &out_path)) {
    printf("Failed to get realpath for \"%s\"", path.c_str());
    abort();
  }

  out_path += "/bionic-loader-test-libs";

  std::string real_path;
  if (!android::base::Realpath(out_path, &real_path)) {
    printf("\"%s\": does not exists", out_path.c_str());
    abort();
  }

  return real_path;
}
