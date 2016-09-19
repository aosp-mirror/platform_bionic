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

#ifndef __DLFCN_SYMLINK_SUPPORT_H__
#define __DLFCN_SYMLINK_SUPPORT_H__

#include <string>

void create_dlfcn_test_symlink(const char* suffix, std::string* result);
void remove_dlfcn_test_symlink(const std::string& path);

class DlfcnSymlink {
 public:
  explicit DlfcnSymlink(const char* test_name) {
    create_dlfcn_test_symlink(test_name, &symlink_path_);
  }

  ~DlfcnSymlink() {
    remove_dlfcn_test_symlink(symlink_path_);
  }

  const std::string& get_symlink_path() const {
    return symlink_path_;
  }

 private:
  std::string symlink_path_;
};

#endif /* __DLFCN_SYMLINK_SUPPORT_H__ */
