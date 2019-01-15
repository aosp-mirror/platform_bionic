/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "SymbolFileParser.h"

#include "Arch.h"
#include "CompilationType.h"

#include <android-base/strings.h>

#include <fstream>
#include <ios>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <err.h>

namespace {

using TagList = std::vector<std::string>;

struct SymbolEnt {
  std::string name;
  TagList tags;
};

using SymbolList = std::vector<SymbolEnt>;

struct Version {
  std::string name;
  std::string base;
  SymbolList symbols;
  TagList tags;
};

class SymbolFileParser {
 public:
  SymbolFileParser(const std::string& path, const CompilationType& type)
    : file_path(path),
      compilation_type(type),
      api_level_arch_prefix("api-level-" + to_string(type.arch) + "="),
      intro_arch_perfix("introduced-" + to_string(type.arch) + "="),
      file(path, std::ios_base::in),
      curr_line_num(0) {
  }

  // Parse the version script and build a symbol map.
  std::optional<SymbolMap> parse() {
    if (!file) {
      return std::nullopt;
    }

    SymbolMap symbol_map;
    while (hasNextLine()) {
      auto&& version = parseVersion();
      if (!version) {
        return std::nullopt;
      }

      if (isInArch(version->tags) && isInApi(version->tags)) {
        for (auto&& [name, tags] : version->symbols) {
          if (isInArch(tags) && isInApi(tags)) {
            symbol_map[name] = getSymbolType(tags);
          }
        }
      }
    }
    return std::make_optional(std::move(symbol_map));
  }

 private:
  // Read a non-empty line from the input and split at the first '#' character.
  bool hasNextLine() {
    std::string line;
    while (std::getline(file, line)) {
      ++curr_line_num;

      size_t hash_pos = line.find('#');
      curr_line = android::base::Trim(line.substr(0, hash_pos));
      if (!curr_line.empty()) {
        if (hash_pos != std::string::npos) {
          curr_tags = parseTags(line.substr(hash_pos + 1));
        } else {
          curr_tags.clear();
        }
        return true;
      }
    }
    return false;
  }

  // Tokenize the tags after the '#' character.
  static std::vector<std::string> parseTags(const std::string& tags_line) {
    std::vector<std::string> tags = android::base::Split(tags_line, " \t");
    tags.erase(std::remove(tags.begin(), tags.end(), ""), tags.end());
    return tags;
  }

  // Parse a version scope.
  std::optional<Version> parseVersion() {
    size_t start_line_num = curr_line_num;

    std::string::size_type lparen_pos = curr_line.find('{');
    if (lparen_pos == std::string::npos) {
      errx(1, "%s:%zu: error: expected '{' cannot be found in this line",
           file_path.c_str(), curr_line_num);
    }

    // Record the version name and version tags (before hasNextLine()).
    std::string name = android::base::Trim(curr_line.substr(0, lparen_pos));
    TagList tags = std::move(curr_tags);

    // Read symbol lines.
    SymbolList symbols;
    bool global_scope = true;
    bool cpp_scope = false;
    while (hasNextLine()) {
      size_t rparen_pos = curr_line.find('}');
      if (rparen_pos != std::string::npos) {
        size_t semicolon_pos = curr_line.find(';', rparen_pos + 1);
        if (semicolon_pos == std::string::npos) {
          errx(1, "%s:%zu: error: the line that ends a scope must end with ';'",
               file_path.c_str(), curr_line_num);
        }

        if (cpp_scope) {
          cpp_scope = false;
          continue;
        }

        std::string base = android::base::Trim(
          curr_line.substr(rparen_pos + 1, semicolon_pos - 1));

        return std::make_optional(Version{std::move(name), std::move(base),
                                          std::move(symbols), std::move(tags)});
      }

      if (android::base::StartsWith(curr_line, R"(extern "C++" {)")) {
        cpp_scope = true;
        continue;
      }

      if (cpp_scope) {
        continue;
      }

      size_t colon_pos = curr_line.find(':');
      if (colon_pos != std::string::npos) {
        std::string visibility =
          android::base::Trim(curr_line.substr(0, colon_pos));

        if (visibility == "global") {
          global_scope = true;
        } else if (visibility == "local") {
          global_scope = false;
        } else {
          errx(1, "%s:%zu: error: unknown version visibility: %s",
               file_path.c_str(), curr_line_num, visibility.c_str());
        }
        continue;
      }

      if (global_scope) {
        size_t semicolon_pos = curr_line.find(';');
        if (semicolon_pos == std::string::npos) {
          errx(1, "%s:%zu: error: symbol name line must end with ';'",
               file_path.c_str(), curr_line_num);
        }

        std::string symbol_name =
          android::base::Trim(curr_line.substr(0, semicolon_pos));

        size_t asterisk_pos = symbol_name.find('*');
        if (asterisk_pos != std::string::npos) {
          errx(1, "%s:%zu: error: global symbol name must not have wildcards",
               file_path.c_str(), curr_line_num);
        }

        symbols.push_back(SymbolEnt{std::move(symbol_name),
                                    std::move(curr_tags)});
      }
    }

    errx(1, "%s:%zu: error: scope started from %zu must be closed before EOF",
         file_path.c_str(), curr_line_num, start_line_num);
  }

  static NdkSymbolType getSymbolType(const TagList& tags) {
    for (auto&& tag : tags) {
      if (tag == "var") {
        return NdkSymbolType::variable;
      }
    }
    return NdkSymbolType::function;
  }

  // isInArch() returns true if there is a matching arch-specific tag or there
  // are no arch-specific tags.
  bool isInArch(const TagList& tags) const {
    bool has_arch_tags = false;
    for (auto&& tag : tags) {
      std::optional<Arch> arch = arch_from_string(tag);
      if (!arch) {
        continue;
      }
      if (*arch == compilation_type.arch) {
        return true;
      }
      has_arch_tags = true;
    }
    return !has_arch_tags;
  }

  // isInApi() returns true if the specified API level is equal to the
  // api-level tag, or the specified API level is greater than or equal to the
  // introduced tag, or there are no api-level or introduced tags.
  bool isInApi(const TagList& tags) const {
    bool api_level_arch = false;
    bool intro_arch = false;
    std::string api_level;
    std::string intro;

    for (const std::string& tag : tags) {
      // Check api-level tags.
      if (android::base::StartsWith(tag, "api-level=") && !api_level_arch) {
        api_level = tag;
        continue;
      }
      if (android::base::StartsWith(tag, api_level_arch_prefix)) {
        api_level = tag;
        api_level_arch = true;
        continue;
      }

      // Check introduced tags.
      if (android::base::StartsWith(tag, "introduced=") && !intro_arch) {
        intro = tag;
        continue;
      }
      if (android::base::StartsWith(tag, intro_arch_perfix)) {
        intro = tag;
        intro_arch = true;
        continue;
      }

      if (tag == "future") {
        return compilation_type.api_level == future_api;
      }
    }

    if (intro.empty() && api_level.empty()) {
      return true;
    }

    if (!api_level.empty()) {
      // If an api-level tag is specified, it must be an exact match (mainly
      // for versioner unit tests).
      return compilation_type.api_level == decodeApiLevelValue(api_level);
    }

    return compilation_type.api_level >= decodeApiLevelValue(intro);
  }

  // Extract and decode the integer API level from api-level or introduced tags.
  static int decodeApiLevelValue(const std::string& tag) {
    std::string api_level = tag.substr(tag.find('=') + 1);
    auto it = api_codename_map.find(api_level);
    if (it != api_codename_map.end()) {
      return it->second;
    }
    return std::stoi(api_level);
  }

 private:
  const std::string& file_path;
  const CompilationType& compilation_type;
  const std::string api_level_arch_prefix;
  const std::string intro_arch_perfix;

  std::ifstream file;
  std::string curr_line;
  std::vector<std::string> curr_tags;
  size_t curr_line_num;
};

}  // anonymous namespace


std::optional<SymbolMap> parseSymbolFile(const std::string& file_path,
                                         const CompilationType& type) {
  SymbolFileParser parser(file_path, type);
  return parser.parse();
}
