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
#include <unordered_set>

#include "DeclarationDatabase.h"

using LibrarySymbolDatabase = std::unordered_set<std::string>;
std::unordered_set<std::string> getSymbols(const std::string& filename);

enum class NdkSymbolType {
  function,
  variable,
};

using NdkSymbolDatabase = std::map<std::string, std::map<CompilationType, NdkSymbolType>>;
NdkSymbolDatabase parsePlatforms(const std::set<CompilationType>& types,
                                 const std::string& platform_dir);
