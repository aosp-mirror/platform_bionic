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

#include <stdio.h>

#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include <llvm/ADT/StringRef.h>

#include "Arch.h"
#include "CompilationType.h"
#include "Utils.h"

namespace clang {
class ASTContext;
class Decl;
}

enum class DeclarationType {
  function,
  variable,
  inconsistent,
};

struct AvailabilityValues {
  bool future = false;
  int introduced = 0;
  int deprecated = 0;
  int obsoleted = 0;

  bool empty() const {
    return !(future || introduced || deprecated || obsoleted);
  }

  bool operator==(const AvailabilityValues& rhs) const {
    return std::tie(introduced, deprecated, obsoleted) ==
           std::tie(rhs.introduced, rhs.deprecated, rhs.obsoleted);
  }

  bool operator!=(const AvailabilityValues& rhs) const {
    return !(*this == rhs);
  }
};

std::string to_string(const AvailabilityValues& av);

struct DeclarationAvailability {
  AvailabilityValues global_availability;
  ArchMap<AvailabilityValues> arch_availability;

  bool empty() const {
    if (!global_availability.empty()) {
      return false;
    }

    for (const auto& it : arch_availability) {
      if (!it.second.empty()) {
        return false;
      }
    }

    return true;
  }

  bool operator==(const DeclarationAvailability& rhs) const {
    return std::tie(global_availability, arch_availability) ==
           std::tie(rhs.global_availability, rhs.arch_availability);
  }

  bool operator!=(const DeclarationAvailability& rhs) const {
    return !(*this == rhs);
  }

  // Returns false if the availability declarations conflict.
  bool merge(const DeclarationAvailability& other);
};

std::string to_string(const DeclarationAvailability& decl_av);

struct FileLocation {
  unsigned line;
  unsigned column;

  bool operator<(const FileLocation& rhs) const {
    return std::tie(line, column) < std::tie(rhs.line, rhs.column);
  }

  bool operator==(const FileLocation& rhs) const {
    return std::tie(line, column) == std::tie(rhs.line, rhs.column);
  }
};

struct Location {
  std::string filename;
  FileLocation start;
  FileLocation end;

  bool operator<(const Location& rhs) const {
    return std::tie(filename, start, end) < std::tie(rhs.filename, rhs.start, rhs.end);
  }
};

std::string to_string(const Location& loc);

struct Declaration {
  std::string name;
  Location location;

  bool is_extern;
  bool is_definition;
  bool no_guard;
  std::map<CompilationType, DeclarationAvailability> availability;

  bool calculateAvailability(DeclarationAvailability* output) const;
  bool operator<(const Declaration& rhs) const {
    return location < rhs.location;
  }

  void dump(const std::string& base_path = "", FILE* out = stdout, unsigned indent = 0) const {
    std::string indent_str(indent, ' ');
    fprintf(out, "%s", indent_str.c_str());

    fprintf(out, "%s ", is_extern ? "extern" : "static");
    fprintf(out, "%s ", is_definition ? "definition" : "declaration");
    if (no_guard) {
      fprintf(out, "no_guard ");
    }
    fprintf(out, "@ %s:%u:%u", StripPrefix(location.filename, base_path).str().c_str(),
            location.start.line, location.start.column);

    if (!availability.empty()) {
      DeclarationAvailability avail;

      fprintf(out, "\n%s  ", indent_str.c_str());
      if (!calculateAvailability(&avail)) {
        fprintf(out, "invalid availability\n");
      } else {
        fprintf(out, "%s\n", to_string(avail).c_str());
      }
    }
  }
};

struct Symbol {
  std::string name;
  std::map<Location, Declaration> declarations;

  bool calculateAvailability(DeclarationAvailability* output) const;
  bool hasDeclaration(const CompilationType& type) const;

  bool operator<(const Symbol& rhs) const {
    return name < rhs.name;
  }

  bool operator==(const Symbol& rhs) const {
    return name == rhs.name;
  }

  void dump(const std::string& base_path = "", FILE* out = stdout) const {
    DeclarationAvailability availability;
    bool valid_availability = calculateAvailability(&availability);
    fprintf(out, "  %s: ", name.c_str());

    if (valid_availability) {
      fprintf(out, "%s\n", to_string(availability).c_str());
    } else {
      fprintf(out, "invalid\n");
    }

    for (auto& it : declarations) {
      it.second.dump(base_path, out, 4);
    }
  }
};

class HeaderDatabase {
  std::mutex mutex;

 public:
  std::map<std::string, Symbol> symbols;

  void parseAST(CompilationType type, clang::ASTContext& ast);

  void dump(const std::string& base_path = "", FILE* out = stdout) const {
    fprintf(out, "HeaderDatabase contains %zu symbols:\n", symbols.size());
    for (const auto& pair : symbols) {
      pair.second.dump(base_path, out);
    }
  }
};
