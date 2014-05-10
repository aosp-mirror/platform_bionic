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

#include <errno.h>
#include <stdlib.h>

#include "private/bionic_macros.h"
#include "private/ScopedReaddir.h"

// A smart pointer to the scandir dirent**.
class ScandirResult {
 public:
  ScandirResult() : names_(NULL), size_(0), capacity_(0) {
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
    names_ = NULL;
    size_ = capacity_ = 0;
    return result;
  }

  bool Add(dirent* entry) {
    if (size_ >= capacity_) {
      size_t new_capacity = capacity_ + 32;
      dirent** new_names = (dirent**) realloc(names_, new_capacity * sizeof(dirent*));
      if (new_names == NULL) {
        return false;
      }
      names_ = new_names;
      capacity_ = new_capacity;
    }

    dirent* copy = CopyDirent(entry);
    if (copy == NULL) {
      return false;
    }
    names_[size_++] = copy;
    return true;
  }

  void Sort(int (*comparator)(const dirent**, const dirent**)) {
    // If we have entries and a comparator, sort them.
    if (size_ > 0 && comparator != NULL) {
      qsort(names_, size_, sizeof(dirent*), (int (*)(const void*, const void*)) comparator);
    }
  }

 private:
  dirent** names_;
  size_t size_;
  size_t capacity_;

  static dirent* CopyDirent(dirent* original) {
    // Allocate the minimum number of bytes necessary, rounded up to a 4-byte boundary.
    size_t size = ((original->d_reclen + 3) & ~3);
    dirent* copy = (dirent*) malloc(size);
    memcpy(copy, original, original->d_reclen);
    return copy;
  }

  DISALLOW_COPY_AND_ASSIGN(ScandirResult);
};

int scandir(const char* dirname, dirent*** name_list,
            int (*filter)(const dirent*),
            int (*comparator)(const dirent**, const dirent**)) {
  ScopedReaddir reader(dirname);
  if (reader.IsBad()) {
    return -1;
  }

  ScandirResult names;
  dirent* entry;
  while ((entry = reader.ReadEntry()) != NULL) {
    // If we have a filter, skip names that don't match.
    if (filter != NULL && !(*filter)(entry)) {
      continue;
    }
    names.Add(entry);
  }

  names.Sort(comparator);

  size_t size = names.size();
  *name_list = names.release();
  return size;
}
__strong_alias(scandir64, scandir);
