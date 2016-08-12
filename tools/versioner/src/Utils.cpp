/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
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

#include "DeclarationDatabase.h"

std::string getWorkingDir() {
  char buf[PATH_MAX];
  if (!getcwd(buf, sizeof(buf))) {
    err(1, "getcwd failed");
  }
  return buf;
}

std::vector<std::string> collectFiles(const std::string& directory) {
  std::vector<std::string> files;

  char* dir_argv[2] = { const_cast<char*>(directory.c_str()), nullptr };
  FTS* fts = fts_open(dir_argv, FTS_LOGICAL | FTS_NOCHDIR, nullptr);

  if (!fts) {
    err(1, "failed to open directory '%s'", directory.c_str());
  }

  FTSENT* ent;
  while ((ent = fts_read(fts))) {
    if (ent->fts_info & (FTS_D | FTS_DP)) {
      continue;
    }

    files.push_back(ent->fts_path);
  }

  fts_close(fts);
  return files;
}

llvm::StringRef StripPrefix(llvm::StringRef string, llvm::StringRef prefix) {
  if (string.startswith(prefix)) {
    return string.drop_front(prefix.size());
  }
  return string;
}
