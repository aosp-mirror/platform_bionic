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

#include <stdlib.h>

#include <array>
#include <initializer_list>
#include <set>
#include <string>

enum class Arch : size_t {
  arm = 0,
  arm64,
  mips,
  mips64,
  x86,
  x86_64,
};

std::string to_string(const Arch& arch);
Arch arch_from_string(const std::string& name);

template <typename T>
class ArchMapIterator;

template <typename T>
class ArchMap {
 public:
  ArchMap() {
  }

  ArchMap(std::initializer_list<std::pair<Arch, T>> initializer) {
    for (auto& pair : initializer) {
      this->operator[](pair.first) = pair.second;
    }
  }

  T& operator[](Arch arch) {
    return data_[size_t(arch)];
  }

  const T& operator[](Arch arch) const {
    return data_[size_t(arch)];
  }

  bool operator==(const ArchMap& other) const {
    for (size_t i = 0; i < data_.size(); ++i) {
      if (data_[i] != other.data_[i]) {
        return false;
      }
    }
    return true;
  }

  ArchMapIterator<T> begin() const {
    return ArchMapIterator<T>(*this, Arch::arm);
  }

  ArchMapIterator<T> end() const {
    return ArchMapIterator<T>(*this, Arch(size_t(Arch::x86_64) + 1));
  }

 private:
  std::array<T, size_t(Arch::x86_64) + 1> data_ = {};
};

template <typename T>
class ArchMapIterator {
  const ArchMap<T>& map_;
  Arch arch_ = Arch::arm;

 public:
  ArchMapIterator() = delete;

  ArchMapIterator(const ArchMap<T>& map, Arch arch) : map_(map), arch_(arch) {
  }

  bool operator==(const ArchMapIterator<T>& rhs) const {
    return map_ == rhs.map_ && arch_ == rhs.arch_;
  }

  bool operator!=(const ArchMapIterator<T>& rhs) const {
    return !(*this == rhs);
  }

  ArchMapIterator& operator++() {
    arch_ = Arch(size_t(arch_) + 1);
    return *this;
  }

  ArchMapIterator operator++(int) {
    ArchMapIterator result = *this;
    ++*this;
    return result;
  }

  std::pair<const Arch&, const T&> operator*() const {
    return std::tie(arch_, map_[arch_]);
  }

  std::pair<const Arch&, const T&> operator->() const {
    return std::tie(arch_, map_[arch_]);
  }
};

static const std::set<Arch> supported_archs = {
  Arch::arm,
  Arch::arm64,
  Arch::mips,
  Arch::mips64,
  Arch::x86,
  Arch::x86_64,
};

static ArchMap<std::string> arch_targets = {
  { Arch::arm, "arm-linux-androideabi" },
  { Arch::arm64, "aarch64-linux-android" },
  { Arch::mips, "mipsel-linux-android" },
  { Arch::mips64, "mips64el-linux-android" },
  { Arch::x86, "i686-linux-android" },
  { Arch::x86_64, "x86_64-linux-android" },
};

static const std::set<int> supported_levels = { 9, 12, 13, 14, 15, 16, 17, 18, 19, 21, 23, 24 };

static const ArchMap<int> arch_min_api = {
  { Arch::arm, 9 },
  { Arch::arm64, 21 },
  { Arch::mips, 9 },
  { Arch::mips64, 21 },
  { Arch::x86, 9 },
  { Arch::x86_64, 21 },
};
