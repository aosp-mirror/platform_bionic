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

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

extern bool strict;
extern bool verbose;
extern bool add_include;

#define D(...)             \
  do {                     \
    if (verbose) {         \
      printf(__VA_ARGS__); \
    }                      \
  } while (0)

static const std::unordered_map<std::string, std::set<Arch>> header_blacklist = {
  // Internal header.
  { "sys/_system_properties.h", supported_archs },

  // time64.h #errors when included on LP64 archs.
  { "time64.h", { Arch::arm64, Arch::x86_64 } },
};

static const std::unordered_set<std::string> missing_symbol_whitelist = {
  // atexit comes from crtbegin.
  "atexit",
};
