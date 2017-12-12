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
