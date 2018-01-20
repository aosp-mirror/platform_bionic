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

#include <fcntl.h>
#include <unistd.h>

#include "private/bionic_macros.h"

template <typename T = int (*)(char*)>
class GenericTemporaryFile {
 public:
  explicit GenericTemporaryFile(T mk_fn = mkstemp) : mk_fn(mk_fn) {
    // Since we might be running on the host or the target, and if we're
    // running on the host we might be running under bionic or glibc,
    // let's just try both possible temporary directories and take the
    // first one that works.
    init("/data/local/tmp");
    if (fd == -1) {
      init("/tmp");
    }
  }

  explicit GenericTemporaryFile(const char* dirpath, T mk_fn = mkstemp) : mk_fn(mk_fn) {
    init(dirpath);
  }

  ~GenericTemporaryFile() {
    close(fd);
    unlink(filename);
  }

  void reopen() {
    close(fd);
    fd = open(filename, O_RDWR);
  }

  int fd;
  char filename[1024];

 private:
  T mk_fn;

  void init(const char* tmp_dir) {
    snprintf(filename, sizeof(filename), "%s/TemporaryFile-XXXXXX", tmp_dir);
    fd = mk_fn(filename);
  }

  DISALLOW_COPY_AND_ASSIGN(GenericTemporaryFile);
};

typedef GenericTemporaryFile<> TemporaryFile;

class TemporaryDir {
 public:
  TemporaryDir() {
    if (!init("/data/local/tmp")) {
      init("/tmp");
    }
  }

  ~TemporaryDir() {
    rmdir(dirname);
  }

  char dirname[1024];

 private:
  bool init(const char* tmp_dir) {
    snprintf(dirname, sizeof(dirname), "%s/TemporaryDir-XXXXXX", tmp_dir);
    return (mkdtemp(dirname) != NULL);
  }

  DISALLOW_COPY_AND_ASSIGN(TemporaryDir);
};
