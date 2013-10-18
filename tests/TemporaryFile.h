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

#include <unistd.h>

class TemporaryFile {
 public:
  TemporaryFile() {
    // Since we might be running on the host or the target, and if we're
    // running on the host we might be running under bionic or glibc,
    // let's just try both possible temporary directories and take the
    // first one that works.
    init("/data/local/tmp");
    if (fd == -1) {
      init("/tmp");
    }
  }

  ~TemporaryFile() {
    close(fd);
    unlink(filename);
  }

  int fd;
  char filename[1024];

 private:
  void init(const char* tmp_dir) {
    snprintf(filename, sizeof(filename), "%s/TemporaryFile-XXXXXX", tmp_dir);
    fd = mkstemp(filename);
  }
};
