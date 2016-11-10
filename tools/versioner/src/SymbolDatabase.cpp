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

// The NDK platforms are built by copying the platform directories on top of
// each other to build each successive API version. Thus, we need to walk
// backwards to find each desired file.
static std::string readPlatformFile(const CompilationType& type, llvm::StringRef platform_dir,
                                    const std::string& filename, bool required) {
  int api_level = type.api_level;
  std::ifstream stream;
  while (api_level >= arch_min_api[type.arch]) {
    if (supported_levels.count(api_level) == 0) {
      --api_level;
      continue;
    }

    std::string path = std::string(platform_dir) + "/android-" + std::to_string(api_level) +
                       "/arch-" + to_string(type.arch) + "/symbols/" + filename;

    stream = std::ifstream(path);
    if (stream) {
      return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    }

    --api_level;
  }

  if (required) {
    errx(1, "failed to find platform file '%s' for %s", filename.c_str(), to_string(type).c_str());
  }

  return std::string();
}

static std::map<std::string, NdkSymbolType> parsePlatform(const CompilationType& type,
                                                          const std::string& platform_dir) {
  std::map<std::string, NdkSymbolType> result;
  std::map<std::string, bool /*required*/> wanted_files = {
    { "libc.so.functions.txt", true },
    { "libc.so.variables.txt", false },
    { "libdl.so.functions.txt", false },
    { "libm.so.functions.txt", false },
    { "libm.so.variables.txt", false },
  };

  for (const auto& pair : wanted_files) {
    llvm::StringRef file = pair.first;
    bool required = pair.second;
    NdkSymbolType symbol_type;
    if (file.endswith(".functions.txt")) {
      symbol_type = NdkSymbolType::function;
    } else if (file.endswith(".variables.txt")) {
      symbol_type = NdkSymbolType::variable;
    } else {
      errx(1, "internal error: unexpected platform filename '%s'\n", file.str().c_str());
    }

    std::string platform_file = readPlatformFile(type, platform_dir, file, required);
    if (platform_file.empty()) {
      continue;
    }

    llvm::SmallVector<llvm::StringRef, 0> symbols;
    llvm::StringRef(platform_file).split(symbols, "\n");

    for (llvm::StringRef symbol_name : symbols) {
      if (symbol_name.empty()) {
        continue;
      }

      if (result.count(symbol_name) != 0) {
        if (strict) {
          printf("duplicated symbol '%s' in '%s'\n", symbol_name.str().c_str(), file.str().c_str());
        }
      }

      result[symbol_name] = symbol_type;
    }
  }

  return result;
}

NdkSymbolDatabase parsePlatforms(const std::set<CompilationType>& types,
                                 const std::string& platform_dir) {
  std::map<std::string, std::map<CompilationType, NdkSymbolType>> result;
  for (const CompilationType& type : types) {
    std::map<std::string, NdkSymbolType> symbols = parsePlatform(type, platform_dir);
    for (const auto& it : symbols) {
      result[it.first][type] = it.second;
    }
  }

  return result;
}
