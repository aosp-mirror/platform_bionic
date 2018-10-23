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

#include "SymbolDatabase.h"

#include "SymbolFileParser.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <streambuf>
#include <string>
#include <unordered_set>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Object/Binary.h>
#include <llvm/Object/ELFObjectFile.h>

#include "versioner.h"

using namespace llvm;
using namespace llvm::object;

std::unordered_set<std::string> getSymbols(const std::string& filename) {
  std::unordered_set<std::string> result;
  auto binaryOrError = createBinary(filename);
  if (!binaryOrError) {
    errx(1, "failed to open library at %s: %s\n", filename.c_str(),
         llvm::toString(binaryOrError.takeError()).c_str());
  }

  ELFObjectFileBase* elf = dyn_cast_or_null<ELFObjectFileBase>(binaryOrError.get().getBinary());
  if (!elf) {
    errx(1, "failed to parse %s as ELF", filename.c_str());
  }

  for (const ELFSymbolRef symbol : elf->getDynamicSymbolIterators()) {
    Expected<StringRef> symbolNameOrError = symbol.getName();

    if (!symbolNameOrError) {
      errx(1, "failed to get symbol name for symbol in %s: %s", filename.c_str(),
           llvm::toString(symbolNameOrError.takeError()).c_str());
    }

    result.insert(symbolNameOrError.get().str());
  }

  return result;
}

static std::map<std::string, NdkSymbolType> parsePlatform(const CompilationType& type,
                                                          const std::string& platform_dir) {
  static const std::pair<const char*, bool> wanted_files[] = {
    {"crtbegin.map.txt", false},
    {"libc.map.txt", true},
  };

  std::map<std::string, NdkSymbolType> result;

  for (auto&& [filename, required] : wanted_files) {
    std::string path = platform_dir + "/" + filename;

    std::optional<SymbolMap> symbols = parseSymbolFile(path, type);
    if (!symbols) {
      if (required) {
        errx(1, "error: failed to load: %s", path.c_str());
      }
      continue;
    }

    for (auto&& [symbol_name, symbol_type] : *symbols) {
      if (symbol_name.empty()) {
        continue;
      }

      if (result.count(symbol_name) != 0) {
        if (strict) {
          printf("duplicated symbol '%s' in '%s'\n", symbol_name.c_str(), path.c_str());
        }
      }

      result[symbol_name] = symbol_type;
    }
  }

  return result;
}

std::optional<NdkSymbolDatabase> parsePlatforms(const std::set<CompilationType>& types,
                                                const std::string& platform_dir) {
  NdkSymbolDatabase result;
  for (const CompilationType& type : types) {
    std::map<std::string, NdkSymbolType> symbols = parsePlatform(type, platform_dir);
    for (const auto& it : symbols) {
      result[it.first][type] = it.second;
    }
  }
  return std::make_optional(std::move(result));
}
