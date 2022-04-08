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

#include "Preprocessor.h"

#include <err.h>
#include <fcntl.h>
#include <fts.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <deque>
#include <fstream>
#include <string>
#include <unordered_map>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>

#include "Arch.h"
#include "DeclarationDatabase.h"
#include "versioner.h"

using namespace std::string_literals;

static DeclarationAvailability calculateRequiredGuard(const Declaration& declaration) {
  // To avoid redundant macro guards, the availability calculated by this function is the set
  // difference of 'targets marked-available' from 'targets the declaration is visible in'.
  // For example, a declaration that is visible always and introduced in 9 would return introduced
  // in 9, but the same declaration, except only visible in 9+ would return an empty
  // DeclarationAvailability.

  // This currently only handles __INTRODUCED_IN.
  // TODO: Do the same for __REMOVED_IN.
  int global_min_api_visible = 0;
  ArchMap<int> arch_visibility;

  for (const auto& it : declaration.availability) {
    const CompilationType& type = it.first;

    if (global_min_api_visible == 0 || global_min_api_visible > type.api_level) {
      global_min_api_visible = type.api_level;
    }

    if (arch_visibility[type.arch] == 0 || arch_visibility[type.arch] > type.api_level) {
      arch_visibility[type.arch] = type.api_level;
    }
  }

  DeclarationAvailability decl_av;
  if (!declaration.calculateAvailability(&decl_av)) {
    fprintf(stderr, "versioner: failed to calculate availability while preprocessing:\n");
    declaration.dump("", stderr, 2);
    exit(1);
  }

  D("Calculating required guard for %s:\n", declaration.name.c_str());
  D("  Declaration availability: %s\n", to_string(decl_av).c_str());

  if (verbose) {
    std::string arch_visibility_str;
    for (Arch arch : supported_archs) {
      if (arch_visibility[arch] != 0) {
        arch_visibility_str += to_string(arch);
        arch_visibility_str += ": ";
        arch_visibility_str += std::to_string(arch_visibility[arch]);
        arch_visibility_str += ", ";
      }
    }
    if (!arch_visibility_str.empty()) {
      arch_visibility_str.resize(arch_visibility_str.size() - 2);
    }
    D("  Declaration visibility: global = %d, arch = %s\n", global_min_api_visible,
      arch_visibility_str.c_str());
  }

  DeclarationAvailability result = decl_av;
  if (result.global_availability.introduced <= global_min_api_visible) {
    result.global_availability.introduced = 0;
  }

  for (Arch arch : supported_archs) {
    if (result.arch_availability[arch].introduced <= arch_visibility[arch]) {
      result.arch_availability[arch].introduced = 0;
    }
  }

  D("  Calculated result: %s\n", to_string(result).c_str());
  D("\n");

  return result;
}

static std::deque<std::string> readFileLines(const std::string& path) {
  std::ifstream is(path.c_str());
  std::deque<std::string> result;
  std::string line;

  while (std::getline(is, line)) {
    result.push_back(std::move(line));
  }

  return result;
}

static void writeFileLines(const std::string& path, const std::deque<std::string>& lines) {
  if (!mkdirs(dirname(path))) {
    err(1, "failed to create directory '%s'", dirname(path).c_str());
  }

  std::ofstream os(path.c_str(), std::ios_base::out | std::ios_base::trunc);

  for (const std::string& line : lines) {
    os << line << "\n";
  }
}

using GuardMap = std::map<Location, DeclarationAvailability>;

static std::string generateGuardCondition(const DeclarationAvailability& avail) {
  // Logically orred expressions that constitute the macro guard.
  std::vector<std::string> expressions;
  static const std::vector<std::pair<std::string, std::set<Arch>>> arch_sets = {
    { "", supported_archs },
    { "!defined(__LP64__)", { Arch::arm, Arch::x86 } },
    { "defined(__LP64__)", { Arch::arm64, Arch::x86_64 } },
  };
  std::map<Arch, std::string> individual_archs = {
    { Arch::arm, "defined(__arm__)" },
    { Arch::arm64, "defined(__aarch64__)" },
    { Arch::x86, "defined(__i386__)" },
    { Arch::x86_64, "defined(__x86_64__)" },
  };

  auto generate_guard = [](const std::string& arch_expr, int min_version) {
    if (min_version == 0) {
      return arch_expr;
    }
    return arch_expr + " && __ANDROID_API__ >= " + std::to_string(min_version);
  };

  D("Generating guard for availability: %s\n", to_string(avail).c_str());
  if (!avail.global_availability.empty()) {
    for (Arch arch : supported_archs) {
      if (!avail.arch_availability[arch].empty()) {
        errx(1, "attempted to generate guard with global and per-arch values: %s",
             to_string(avail).c_str());
      }
    }

    if (avail.global_availability.introduced == 0) {
      fprintf(stderr, "warning: attempted to generate guard with empty availability: %s\n",
              to_string(avail).c_str());
      return "";
    }

    if (avail.global_availability.introduced <= 9) {
      return "";
    }

    return "__ANDROID_API__ >= "s + std::to_string(avail.global_availability.introduced);
  }

  for (const auto& it : arch_sets) {
    const std::string& arch_expr = it.first;
    const std::set<Arch>& archs = it.second;

    D("  Checking arch set '%s'\n", arch_expr.c_str());

    int version = avail.arch_availability[*it.second.begin()].introduced;

    // The maximum min_version of the set.
    int max_min_version = 0;
    for (Arch arch : archs) {
      if (arch_min_api[arch] > max_min_version) {
        max_min_version = arch_min_api[arch];
      }

      if (avail.arch_availability[arch].introduced != version) {
        D("    Skipping arch set, availability for %s doesn't match %s\n",
          to_string(*it.second.begin()).c_str(), to_string(arch).c_str());
        goto skip;
      }
    }

    // If all of the archs in the set have a min_api that satifies version, elide the check.
    if (max_min_version >= version) {
      version = 0;
    }

    expressions.emplace_back(generate_guard(arch_expr, version));

    D("    Generated expression '%s'\n", expressions.rbegin()->c_str());

    for (Arch arch : archs) {
      individual_archs.erase(arch);
    }

  skip:
    continue;
  }

  for (const auto& it : individual_archs) {
    const std::string& arch_expr = it.second;
    int introduced = avail.arch_availability[it.first].introduced;
    if (introduced == 0) {
      expressions.emplace_back(arch_expr);
    } else {
      expressions.emplace_back(generate_guard(arch_expr, introduced));
    }
  }

  if (expressions.size() == 0) {
    errx(1, "generated empty guard for availability %s", to_string(avail).c_str());
  } else if (expressions.size() == 1) {
    return expressions[0];
  }

  return "("s + Join(expressions, ") || (") + ")";
}

// Assumes that nothing crazy is happening (e.g. having the semicolon be in a macro)
static FileLocation findNextSemicolon(const std::deque<std::string>& lines, FileLocation start) {
  unsigned current_line = start.line;
  unsigned current_column = start.column;
  while (current_line <= lines.size()) {
    size_t result = lines[current_line - 1].find_first_of(';', current_column - 1);

    if (result != std::string::npos) {
      FileLocation loc = {
        .line = current_line,
        .column = unsigned(result) + 1,
      };

      return loc;
    }

    ++current_line;
    current_column = 0;
  }

  errx(1, "failed to find semicolon starting from %u:%u", start.line, start.column);
}

// Merge adjacent blocks with identical guards.
static void mergeGuards(std::deque<std::string>& file_lines, GuardMap& guard_map) {
  if (guard_map.size() < 2) {
    return;
  }

  auto current = guard_map.begin();
  auto next = current;
  ++next;

  while (next != guard_map.end()) {
    if (current->second != next->second) {
      ++current;
      ++next;
      continue;
    }

    // Scan from the end of current to the beginning of next.
    bool in_block_comment = false;
    bool valid = true;

    FileLocation current_location = current->first.end;
    FileLocation end_location = next->first.start;

    auto nextLine = [&current_location]() {
      ++current_location.line;
      current_location.column = 1;
    };

    auto nextCol = [&file_lines, &current_location, &nextLine]() {
      if (current_location.column == file_lines[current_location.line - 1].length()) {
        nextLine();
      } else {
        ++current_location.column;
      }
    };

    // The end location will point to the semicolon, which we don't want to read, so skip it.
    nextCol();

    while (current_location < end_location) {
      const std::string& line = file_lines[current_location.line - 1];
      size_t line_index = current_location.column - 1;

      if (in_block_comment) {
        size_t pos = line.find("*/", line_index);
        if (pos == std::string::npos) {
          D("Didn't find block comment terminator, skipping line\n");
          nextLine();
          continue;
        } else {
          D("Found block comment terminator\n");
          in_block_comment = false;
          current_location.column = pos + 2;
          nextCol();
          continue;
        }
      } else {
        size_t pos = line.find_first_not_of(" \t", line_index);
        if (pos == std::string::npos) {
          nextLine();
          continue;
        }

        current_location.column = pos + 1;
        if (line[pos] != '/') {
          valid = false;
          break;
        }

        nextCol();
        if (line.length() <= pos + 1) {
          // Trailing slash at the end of a line?
          D("Trailing slash at end of line\n");
          valid = false;
          break;
        }

        if (line[pos + 1] == '/') {
          // C++ style comment
          nextLine();
        } else if (line[pos + 1] == '*') {
          // Block comment
          nextCol();
          in_block_comment = true;
          D("In a block comment\n");
        } else {
          // Garbage?
          D("Unexpected output after /: %s\n", line.substr(pos).c_str());
          valid = false;
          break;
        }
      }
    }

    if (!valid) {
      D("Not merging blocks %s and %s\n", to_string(current->first).c_str(),
        to_string(next->first).c_str());
      ++current;
      ++next;
      continue;
    }

    D("Merging blocks %s and %s\n", to_string(current->first).c_str(),
      to_string(next->first).c_str());

    Location merged = current->first;
    merged.end = next->first.end;

    DeclarationAvailability avail = current->second;

    guard_map.erase(current);
    guard_map.erase(next);
    bool dummy;
    std::tie(current, dummy) = guard_map.insert(std::make_pair(merged, avail));
    next = current;
    ++next;
  }
}

static void rewriteFile(const std::string& output_path, std::deque<std::string>& file_lines,
                        const GuardMap& guard_map) {
  for (auto it = guard_map.rbegin(); it != guard_map.rend(); ++it) {
    const Location& loc = it->first;
    const DeclarationAvailability& avail = it->second;

    std::string condition = generateGuardCondition(avail);
    if (condition.empty()) {
      continue;
    }

    std::string prologue = "\n#if "s + condition + "\n";
    std::string epilogue = "\n#endif /* " + condition + " */\n";

    file_lines[loc.end.line - 1].insert(loc.end.column, epilogue);
    file_lines[loc.start.line - 1].insert(loc.start.column - 1, prologue);
  }

  if (verbose) {
    printf("Preprocessing %s...\n", output_path.c_str());
  }
  writeFileLines(output_path, file_lines);
}

bool preprocessHeaders(const std::string& dst_dir, const std::string& src_dir,
                       HeaderDatabase* database) {
  std::unordered_map<std::string, GuardMap> guards;
  std::unordered_map<std::string, std::deque<std::string>> file_lines;

  for (const auto& symbol_it : database->symbols) {
    const Symbol& symbol = symbol_it.second;

    for (const auto& decl_it : symbol.declarations) {
      const Location& location = decl_it.first;
      const Declaration& decl = decl_it.second;

      if (decl.no_guard) {
        // No guard required.
        continue;
      }

      DeclarationAvailability macro_guard = calculateRequiredGuard(decl);
      if (!macro_guard.empty()) {
        guards[location.filename][location] = macro_guard;
      }
    }
  }

  // Copy over the original headers before preprocessing.
  char* fts_paths[2] = { const_cast<char*>(src_dir.c_str()), nullptr };
  std::unique_ptr<FTS, decltype(&fts_close)> fts(fts_open(fts_paths, FTS_LOGICAL, nullptr),
                                                 fts_close);
  if (!fts) {
    err(1, "failed to open directory %s", src_dir.c_str());
  }

  while (FTSENT* ent = fts_read(fts.get())) {
    llvm::StringRef path = ent->fts_path;
    if (!path.startswith(src_dir)) {
      err(1, "path '%s' doesn't start with source dir '%s'", ent->fts_path, src_dir.c_str());
    }

    if (ent->fts_info != FTS_F) {
      continue;
    }

    std::string rel_path = path.substr(src_dir.length() + 1);
    std::string dst_path = dst_dir + "/" + rel_path;
    llvm::StringRef parent_path = llvm::sys::path::parent_path(dst_path);
    if (llvm::sys::fs::create_directories(parent_path)) {
      errx(1, "failed to ensure existence of directory '%s'", parent_path.str().c_str());
    }
    if (llvm::sys::fs::copy_file(path, dst_path)) {
      errx(1, "failed to copy '%s/%s' to '%s'", src_dir.c_str(), path.str().c_str(),
           dst_path.c_str());
    }
  }

  for (const auto& file_it : guards) {
    file_lines[file_it.first] = readFileLines(file_it.first);
  }

  for (auto& file_it : guards) {
    llvm::StringRef file_path = file_it.first;
    GuardMap& orig_guard_map = file_it.second;

    // The end positions given to us are the end of the declaration, which is some point before the
    // semicolon. Fix up the end positions by scanning for the next semicolon.
    GuardMap guard_map;
    for (const auto& it : orig_guard_map) {
      Location loc = it.first;
      loc.end = findNextSemicolon(file_lines[file_path], loc.end);
      guard_map[loc] = it.second;
    }

    // TODO: Make sure that the Locations don't overlap.
    // TODO: Merge adjacent non-identical guards.
    mergeGuards(file_lines[file_path], guard_map);

    if (!file_path.startswith(src_dir)) {
      errx(1, "input file %s is not in %s\n", file_path.str().c_str(), src_dir.c_str());
    }

    // rel_path has a leading slash.
    llvm::StringRef rel_path = file_path.substr(src_dir.size(), file_path.size() - src_dir.size());
    std::string output_path = (llvm::Twine(dst_dir) + rel_path).str();

    rewriteFile(output_path, file_lines[file_path], guard_map);
  }

  return true;
}
