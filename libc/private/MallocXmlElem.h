/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include <platform/bionic/macros.h>

class MallocXmlElem {
 public:
  // Name must be valid throughout lifetime of the object.
  explicit MallocXmlElem(int fd, const char* name,
                         const char* attr_fmt = nullptr, ...) : fd_(fd), name_(name) {
    dprintf(fd_, "<%s", name_);
    if (attr_fmt != nullptr) {
      va_list args;
      va_start(args, attr_fmt);
      write(fd_, " ", 1);
      vdprintf(fd_, attr_fmt, args);
      va_end(args);
    }
    write(fd_, ">", 1);
  }

  ~MallocXmlElem() noexcept {
    dprintf(fd_, "</%s>", name_);
  }

  void Contents(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vdprintf(fd_, fmt, args);
    va_end(args);
  }

private:
  int fd_;
  const char* name_;

  BIONIC_DISALLOW_IMPLICIT_CONSTRUCTORS(MallocXmlElem);
};
