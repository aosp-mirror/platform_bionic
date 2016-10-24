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

#pragma once

#include <string>
#include <vector>

#include <llvm/ADT/StringRef.h>

std::string getWorkingDir();
std::vector<std::string> collectFiles(const std::string& directory);

static __attribute__((unused)) std::string to_string(const char* c) {
  return c;
}

static __attribute__((unused)) const std::string& to_string(const std::string& str) {
  return str;
}

template <typename Collection>
static std::string Join(Collection c, const std::string& delimiter = ", ") {
  std::string result;
  for (const auto& item : c) {
    using namespace std;
    result.append(to_string(item));
    result.append(delimiter);
  }
  if (!result.empty()) {
    result.resize(result.length() - delimiter.length());
  }
  return result;
}

llvm::StringRef StripPrefix(llvm::StringRef string, llvm::StringRef prefix);
