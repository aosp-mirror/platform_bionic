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

#pragma once

#include <stdint.h>

#include <functional>
#include <utility>

#include "Arch.h"

struct CompilationType {
  Arch arch;
  int api_level;
  int file_offset_bits;

 private:
  auto tie() const {
    return std::tie(arch, api_level, file_offset_bits);
  }

 public:
  bool operator<(const CompilationType& other) const {
    return tie() < other.tie();
  }

  bool operator==(const CompilationType& other) const {
    return tie() == other.tie();
  }
};

namespace std {
template <>
struct hash<CompilationType> {
  size_t operator()(CompilationType type) const {
    struct {
      int32_t arch : 3;
      int32_t api_level : 6;
      int32_t file_offset_bits : 1;
      int32_t padding : 22;
    } packed;
    packed.arch = static_cast<int32_t>(type.arch);
    packed.api_level = type.api_level;
    packed.file_offset_bits = (type.file_offset_bits == 64);
    packed.padding = 0;
    int32_t value;
    memcpy(&value, &packed, sizeof(value));
    return std::hash<int32_t>()(value);
  }
};
}

std::string to_string(const CompilationType& type);
