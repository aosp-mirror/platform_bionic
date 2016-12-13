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

std::vector<std::string> collectHeaders(const std::string& directory) {
  std::vector<std::string> headers;

  char* dir_argv[2] = { const_cast<char*>(directory.c_str()), nullptr };
  std::unique_ptr<FTS, decltype(&fts_close)> fts(
      fts_open(dir_argv, FTS_LOGICAL | FTS_NOCHDIR, nullptr), fts_close);

  if (!fts) {
    err(1, "failed to open directory '%s'", directory.c_str());
  }

  while (FTSENT* ent = fts_read(fts.get())) {
    if (ent->fts_info & (FTS_D | FTS_DP)) {
      continue;
    }

    if (!android::base::EndsWith(ent->fts_path, ".h")) {
      continue;
    }

    headers.push_back(ent->fts_path);
  }

  return headers;
}

llvm::StringRef StripPrefix(llvm::StringRef string, llvm::StringRef prefix) {
  if (string.startswith(prefix)) {
    return string.drop_front(prefix.size());
  }
  return string;
}
