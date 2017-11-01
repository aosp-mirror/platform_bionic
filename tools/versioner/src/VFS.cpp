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

#include <err.h>
#include <fcntl.h>
#include <fts.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <memory>
#include <string>

#include <android-base/unique_fd.h>
#include <clang/Basic/VirtualFileSystem.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/Support/MemoryBuffer.h>

#include "Utils.h"

using android::base::unique_fd;
using namespace clang::vfs;

static void addDirectoryToVFS(InMemoryFileSystem* vfs, const std::string& path) {
  char* paths[] = { const_cast<char*>(path.c_str()), nullptr };
  std::unique_ptr<FTS, decltype(&fts_close)> fts(
      fts_open(paths, FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR, nullptr), fts_close);

  if (!fts) {
    err(1, "failed to open directory %s", path.c_str());
  }

  while (FTSENT* ent = fts_read(fts.get())) {
    if ((ent->fts_info & FTS_F) == 0) {
      continue;
    }

    const char* file_path = ent->fts_accpath;
    unique_fd fd(open(file_path, O_RDONLY | O_CLOEXEC));
    if (fd == -1) {
      err(1, "failed to open header '%s'", file_path);
    }

    auto buffer_opt = llvm::MemoryBuffer::getOpenFile(fd, file_path, -1, false, false);
    if (!buffer_opt) {
      errx(1, "failed to map header '%s'", file_path);
    }

    if (!vfs->addFile(file_path, ent->fts_statp->st_mtime, std::move(buffer_opt.get()))) {
      errx(1, "failed to add file '%s'", file_path);
    }
  }
}

llvm::IntrusiveRefCntPtr<FileSystem> createCommonVFS(const std::string& header_dir,
                                                     const std::string& dependency_dir,
                                                     bool add_versioning_header) {
  auto vfs = std::make_unique<InMemoryFileSystem>();
  addDirectoryToVFS(vfs.get(), header_dir);
  if (!dependency_dir.empty()) {
    addDirectoryToVFS(vfs.get(), dependency_dir);
  }

  if (add_versioning_header) {
    const char* top = getenv("ANDROID_BUILD_TOP");
    if (!top) {
      errx(1, "-i passed, but ANDROID_BUILD_TOP is unset");
    }

    std::string header_path = std::string(top) + "/bionic/libc/include/android/versioning.h";
    auto buffer_opt = llvm::MemoryBuffer::getFile(header_path);
    if (!buffer_opt) {
      err(1, "failed to open %s", header_path.c_str());
    }
    vfs->addFile("android/versioning.h", 0, std::move(buffer_opt.get()));
  }

  return llvm::IntrusiveRefCntPtr<FileSystem>(vfs.release());
}
