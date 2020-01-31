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

#include "Arch.h"

#include <err.h>

#include <string>

std::string to_string(const Arch& arch) {
  switch (arch) {
    case Arch::arm:
      return "arm";

    case Arch::arm64:
      return "arm64";

    case Arch::x86:
      return "x86";

    case Arch::x86_64:
      return "x86_64";
  }

  errx(1, "unknown arch '%zu'", size_t(arch));
}

static const std::unordered_map<std::string, Arch> arch_name_map{
  {"arm", Arch::arm},
  {"arm64", Arch::arm64},
  {"x86", Arch::x86},
  {"x86_64", Arch::x86_64},
};

std::optional<Arch> arch_from_string(const std::string& name) {
  auto it = arch_name_map.find(name);
  if (it == arch_name_map.end()) {
    return std::nullopt;
  }
  return std::make_optional(it->second);
}
