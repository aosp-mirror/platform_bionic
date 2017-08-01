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

#include <errno.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>
#include <unordered_set>
#include <vector>

#include <llvm/ADT/StringRef.h>

std::string getWorkingDir();
std::vector<std::string> collectHeaders(const std::string& directory,
                                        const std::unordered_set<std::string>& ignored_directories);

static inline std::string dirname(const std::string& path) {
  std::unique_ptr<char, decltype(&free)> path_copy(strdup(path.c_str()), free);
  return dirname(path_copy.get());
}

static inline bool is_directory(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
    return true;
  }
  return false;
}

static inline bool mkdirs(const std::string& path) {
  if (is_directory(path)) {
    return true;
  }

  std::string parent = dirname(path);
  if (parent == path) {
    return false;
  }

  if (!mkdirs(parent)) {
    return false;
  }

  if (mkdir(path.c_str(), 0700) != 0) {
    if (errno != EEXIST) {
      return false;
    }
    return is_directory(path);
  }

  return true;
}

static inline std::string to_string(const char* c) {
  return c;
}

static inline const std::string& to_string(const std::string& str) {
  return str;
}

template <typename Collection>
static inline std::string Join(Collection c, const std::string& delimiter = ", ") {
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
