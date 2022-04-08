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

#include "Utils.h"

#include <err.h>
#include <fts.h>
#include <string.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

#include <android-base/strings.h>

#include "DeclarationDatabase.h"

std::string getWorkingDir() {
  char buf[PATH_MAX];
  if (!getcwd(buf, sizeof(buf))) {
    err(1, "getcwd failed");
  }
  return buf;
}

std::vector<std::string> collectHeaders(const std::string& directory,
                                        const std::unordered_set<std::string>& ignored_directories) {
  std::vector<std::string> headers;

  char* dir_argv[2] = { const_cast<char*>(directory.c_str()), nullptr };
  std::unique_ptr<FTS, decltype(&fts_close)> fts(
      fts_open(dir_argv, FTS_LOGICAL | FTS_NOCHDIR, nullptr), fts_close);

  if (!fts) {
    err(1, "failed to open directory '%s'", directory.c_str());
  }

  FTSENT* skipping = nullptr;
  while (FTSENT* ent = fts_read(fts.get())) {
    if (ent->fts_info & FTS_DP) {
      if (ent == skipping) {
        skipping = nullptr;
      }
      continue;
    }

    if (skipping != nullptr) {
      continue;
    }

    if (ent->fts_info & FTS_D) {
      if (ignored_directories.count(ent->fts_path) != 0) {
        // fts_read guarantees that `ent` is valid and sane to hold on to until
        // after it's returned with FTS_DP set.
        skipping = ent;
      }
      continue;
    }

    std::string path = ent->fts_path;
    if (!android::base::EndsWith(path, ".h")) {
      continue;
    }

    headers.push_back(std::move(path));
  }

  return headers;
}

llvm::StringRef StripPrefix(llvm::StringRef string, llvm::StringRef prefix) {
  if (string.startswith(prefix)) {
    return string.drop_front(prefix.size());
  }
  return string;
}
