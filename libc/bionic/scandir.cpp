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

#include <dirent.h>

#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "platform/bionic/macros.h"
#include "private/ScopedReaddir.h"

// A smart pointer to the scandir dirent**.
class ScandirResult {
 public:
  ScandirResult() : names_(nullptr), size_(0), capacity_(0) {
  }

  ~ScandirResult() {
    while (size_ > 0) {
      free(names_[--size_]);
    }
    free(names_);
  }

  size_t size() {
    return size_;
  }

  dirent** release() {
    dirent** result = names_;
    names_ = nullptr;
    size_ = capacity_ = 0;
    return result;
  }

  bool Add(dirent* entry) {
    if (size_ >= capacity_) {
      size_t new_capacity = capacity_ + 32;
      dirent** new_names =
          reinterpret_cast<dirent**>(realloc(names_, new_capacity * sizeof(dirent*)));
      if (new_names == nullptr) {
        return false;
      }
      names_ = new_names;
      capacity_ = new_capacity;
    }

    dirent* copy = CopyDirent(entry);
    if (copy == nullptr) {
      return false;
    }
    names_[size_++] = copy;
    return true;
  }

  void Sort(int (*comparator)(const dirent**, const dirent**)) {
    // If we have entries and a comparator, sort them.
    if (size_ > 0 && comparator != nullptr) {
      qsort(names_, size_, sizeof(dirent*),
            reinterpret_cast<int (*)(const void*, const void*)>(comparator));
    }
  }

 private:
  dirent** names_;
  size_t size_;
  size_t capacity_;

  static dirent* CopyDirent(dirent* original) {
    // Allocate the minimum number of bytes necessary, rounded up to a 4-byte boundary.
    size_t size = ((original->d_reclen + 3) & ~3);
    dirent* copy = reinterpret_cast<dirent*>(malloc(size));
    memcpy(copy, original, original->d_reclen);
    return copy;
  }

  BIONIC_DISALLOW_COPY_AND_ASSIGN(ScandirResult);
};

int scandirat(int parent_fd, const char* dir_name, dirent*** name_list,
              int (*filter)(const dirent*),
              int (*comparator)(const dirent**, const dirent**)) {
  DIR* dir = nullptr;
  if (parent_fd == AT_FDCWD) {
    dir = opendir(dir_name);
  } else {
    int dir_fd = openat(parent_fd, dir_name, O_CLOEXEC | O_DIRECTORY | O_RDONLY);
    if (dir_fd != -1) {
      dir = fdopendir(dir_fd);
    }
  }

  ScopedReaddir reader(dir);
  if (reader.IsBad()) {
    return -1;
  }

  ScandirResult names;
  dirent* entry;
  while ((entry = reader.ReadEntry()) != nullptr) {
    // If we have a filter, skip names that don't match.
    if (filter != nullptr && !(*filter)(entry)) {
      continue;
    }
    names.Add(entry);
  }

  names.Sort(comparator);

  size_t size = names.size();
  *name_list = names.release();
  return size;
}
__strong_alias(scandirat64, scandirat);

int scandir(const char* dir_path, dirent*** name_list,
            int (*filter)(const dirent*),
            int (*comparator)(const dirent**, const dirent**)) {
  return scandirat(AT_FDCWD, dir_path, name_list, filter, comparator);
}
__strong_alias(scandir64, scandir);
