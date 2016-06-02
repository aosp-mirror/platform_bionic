/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
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

extern bool verbose;

static const std::set<std::string> supported_archs = {
  "arm", "arm64", "mips", "mips64", "x86", "x86_64",
};

static std::unordered_map<std::string, std::string> arch_targets = {
  { "arm", "arm-linux-androideabi" },
  { "arm64", "aarch64-linux-android" },
  { "mips", "mipsel-linux-android" },
  { "mips64", "mips64el-linux-android" },
  { "x86", "i686-linux-android" },
  { "x86_64", "x86_64-linux-android" },
};

static const std::set<int> supported_levels = { 9, 12, 13, 14, 15, 16, 17, 18, 19, 21, 23, 24 };

// Non-const for the convenience of being able to index with operator[].
static std::map<std::string, int> arch_min_api = {
  { "arm", 9 },
  { "arm64", 21 },
  { "mips", 9 },
  { "mips64", 21 },
  { "x86", 9 },
  { "x86_64", 21 },
};

static const std::unordered_map<std::string, std::set<std::string>> header_blacklist = {
  // Internal header.
  { "sys/_system_properties.h", supported_archs },

  // time64.h #errors when included on LP64 archs.
  { "time64.h", { "arm64", "mips64", "x86_64" } },
};
