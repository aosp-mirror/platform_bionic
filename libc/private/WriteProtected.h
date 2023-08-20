/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <errno.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/user.h>

#include <async_safe/log.h>

#include "platform/bionic/macros.h"
#include "platform/bionic/page.h"

template <typename T>
union WriteProtectedContents {
  T value;
  char padding[max_page_size()];

  WriteProtectedContents() = default;
  BIONIC_DISALLOW_COPY_AND_ASSIGN(WriteProtectedContents);
} __attribute__((aligned(max_page_size())));

// Write protected wrapper class that aligns its contents to a page boundary,
// and sets the memory protection to be non-writable, except when being modified
// explicitly.
template <typename T>
class WriteProtected {
 public:
  static_assert(sizeof(T) < max_page_size(),
                "WriteProtected only supports contents up to max_page_size()");
  static_assert(__is_pod(T), "WriteProtected only supports POD contents");

  WriteProtected() = default;
  BIONIC_DISALLOW_COPY_AND_ASSIGN(WriteProtected);

  void initialize() {
    // Not strictly necessary, but this will hopefully segfault if we initialize
    // multiple times by accident.
    memset(&contents, 0, sizeof(contents));
    set_protection(PROT_READ);
  }

  const T* operator->() {
    return &contents.value;
  }

  const T& operator*() {
    return contents.value;
  }

  template <typename Mutator>
  void mutate(Mutator mutator) {
    set_protection(PROT_READ | PROT_WRITE);
    mutator(&contents.value);
    set_protection(PROT_READ);
  }

 private:
  WriteProtectedContents<T> contents;

  void set_protection(int prot) {
    auto addr = &contents;
#if __has_feature(hwaddress_sanitizer)
    // The mprotect system call does not currently untag pointers, so do it
    // ourselves.
    addr = untag_address(addr);
#endif
    if (mprotect(reinterpret_cast<void*>(addr), max_page_size(), prot) == -1) {
      async_safe_fatal("WriteProtected mprotect %x failed: %s", prot, strerror(errno));
    }
  }
};
