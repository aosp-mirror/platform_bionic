/*
 * Copyright (C) 2020 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <benchmark/benchmark.h>
#include <property_info_parser/property_info_parser.h>
#include <property_info_serializer/property_info_serializer.h>
#include <system_properties/contexts_split.h>

#include "context_lookup_benchmark_data.h"

using android::base::Split;
using android::base::WriteStringToFd;
using android::properties::BuildTrie;
using android::properties::ParsePropertyInfoFile;
using android::properties::PropertyInfoArea;
using android::properties::PropertyInfoEntry;

BENCHMARK_MAIN();

class LegacyPropertyMapping : public ContextsSplit {
 public:
  LegacyPropertyMapping(const char* property_contexts) {
    TemporaryFile file;
    if (!WriteStringToFd(property_contexts, file.fd)) {
      PLOG(FATAL) << "Could not write to temporary file";
    }

    if (!InitializePropertiesFromFile(file.path)) {
      LOG(FATAL) << "Could not initialize properties";
    }
  }
};

static std::vector<std::string> PropertiesToLookup() {
  std::vector<std::string> properties;
  auto property_lines = Split(aosp_s_property_contexts, "\n");
  for (const auto& line : property_lines) {
    if (line.empty() || line[0] == '#') {
      continue;
    }

    auto property = Split(line, " ")[0];
    properties.push_back(property);
    properties.push_back(property + "0");
    properties.push_back(property + "A");
  }
  return properties;
}

static void LegacyLookupOreo(benchmark::State& state) {
  LegacyPropertyMapping mapping(oreo_property_contexts);
  auto properties = PropertiesToLookup();
  for (auto _ : state) {
    for (const auto& property : properties) {
      benchmark::DoNotOptimize(mapping.GetPrefixNodeForName(property.c_str()));
    }
  }
}
BENCHMARK(LegacyLookupOreo);

static void LegacyLookupS(benchmark::State& state) {
  LegacyPropertyMapping mapping(aosp_s_property_contexts);
  auto properties = PropertiesToLookup();
  for (auto _ : state) {
    for (const auto& property : properties) {
      benchmark::DoNotOptimize(mapping.GetPrefixNodeForName(property.c_str()));
    }
  }
}
BENCHMARK(LegacyLookupS);

static std::string CreateSerializedTrie(const char* input_file) {
  std::vector<std::string> errors;
  std::vector<PropertyInfoEntry> property_infos;
  ParsePropertyInfoFile(input_file, false, &property_infos, &errors);

  std::string serialized_trie;
  std::string error;
  if (!BuildTrie(property_infos, "u:object_r:default_prop:s0", "string", &serialized_trie,
                 &error)) {
    LOG(FATAL) << "Could not build trie: " << error;
  }
  return serialized_trie;
}

static void TrieLookupOreo(benchmark::State& state) {
  std::string serialized_trie = CreateSerializedTrie(oreo_property_contexts);
  PropertyInfoArea* trie = reinterpret_cast<PropertyInfoArea*>(serialized_trie.data());
  auto properties = PropertiesToLookup();
  for (auto _ : state) {
    for (const auto& property : properties) {
      trie->GetPropertyInfo(property.c_str(), nullptr, nullptr);
    }
  }
}
BENCHMARK(TrieLookupOreo);

static void TrieLookupS(benchmark::State& state) {
  std::string serialized_trie = CreateSerializedTrie(aosp_s_property_contexts);
  PropertyInfoArea* trie = reinterpret_cast<PropertyInfoArea*>(serialized_trie.data());
  auto properties = PropertiesToLookup();
  for (auto _ : state) {
    for (const auto& property : properties) {
      trie->GetPropertyInfo(property.c_str(), nullptr, nullptr);
    }
  }
}
BENCHMARK(TrieLookupS);
