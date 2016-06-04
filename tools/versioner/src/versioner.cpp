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

#include <dirent.h>
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/StringRef.h>

#include "DeclarationDatabase.h"
#include "SymbolDatabase.h"
#include "Utils.h"
#include "versioner.h"

using namespace std::string_literals;
using namespace clang;
using namespace clang::tooling;

bool verbose;

class HeaderCompilationDatabase : public CompilationDatabase {
  CompilationType type;
  std::string cwd;
  std::vector<std::string> headers;
  std::vector<std::string> include_dirs;

 public:
  HeaderCompilationDatabase(CompilationType type, std::string cwd, std::vector<std::string> headers,
                            std::vector<std::string> include_dirs)
      : type(type),
        cwd(std::move(cwd)),
        headers(std::move(headers)),
        include_dirs(std::move(include_dirs)) {
  }

  CompileCommand generateCompileCommand(const std::string& filename) const {
    std::vector<std::string> command = { "clang-tool", filename, "-nostdlibinc" };
    for (const auto& dir : include_dirs) {
      command.push_back("-isystem");
      command.push_back(dir);
    }
    command.push_back("-std=c11");
    command.push_back("-DANDROID");
    command.push_back("-D__ANDROID_API__="s + std::to_string(type.api_level));
    command.push_back("-D_FORTIFY_SOURCE=2");
    command.push_back("-D_GNU_SOURCE");
    command.push_back("-Wno-unknown-attributes");
    command.push_back("-target");
    command.push_back(arch_targets[type.arch]);

    return CompileCommand(cwd, filename, command);
  }

  std::vector<CompileCommand> getAllCompileCommands() const override {
    std::vector<CompileCommand> commands;
    for (const std::string& file : headers) {
      commands.push_back(generateCompileCommand(file));
    }
    return commands;
  }

  std::vector<CompileCommand> getCompileCommands(StringRef file) const override {
    std::vector<CompileCommand> commands;
    commands.push_back(generateCompileCommand(file));
    return commands;
  }

  std::vector<std::string> getAllFiles() const override {
    return headers;
  }
};

struct CompilationRequirements {
  std::vector<std::string> headers;
  std::vector<std::string> dependencies;
};

static CompilationRequirements collectRequirements(const std::string& arch,
                                                   const std::string& header_dir,
                                                   const std::string& dependency_dir) {
  std::vector<std::string> headers = collectFiles(header_dir);

  std::vector<std::string> dependencies = { header_dir };
  if (!dependency_dir.empty()) {
    auto collect_children = [&dependencies](const std::string& dir_path) {
      DIR* dir = opendir(dir_path.c_str());
      if (!dir) {
        err(1, "failed to open dependency directory '%s'", dir_path.c_str());
      }

      struct dirent* dent;
      while ((dent = readdir(dir))) {
        if (dent->d_name[0] == '.') {
          continue;
        }

        // TODO: Resolve symlinks.
        std::string dependency = dir_path + "/" + dent->d_name;

        struct stat st;
        if (stat(dependency.c_str(), &st) != 0) {
          err(1, "failed to stat dependency '%s'", dependency.c_str());
        }

        if (!S_ISDIR(st.st_mode)) {
          errx(1, "'%s' is not a directory", dependency.c_str());
        }

        dependencies.push_back(dependency);
      }

      closedir(dir);
    };

    collect_children(dependency_dir + "/common");
    collect_children(dependency_dir + "/" + arch);
  }

  auto new_end = std::remove_if(headers.begin(), headers.end(), [&arch](llvm::StringRef header) {
    for (const auto& it : header_blacklist) {
      if (it.second.find(arch) == it.second.end()) {
        continue;
      }

      if (header.endswith("/" + it.first)) {
        return true;
      }
    }
    return false;
  });

  headers.erase(new_end, headers.end());

  CompilationRequirements result = { .headers = headers, .dependencies = dependencies };
  return result;
}

static std::set<CompilationType> generateCompilationTypes(
    const std::set<std::string> selected_architectures, const std::set<int>& selected_levels) {
  std::set<CompilationType> result;
  for (const std::string& arch : selected_architectures) {
    int min_api = arch_min_api[arch];
    for (int api_level : selected_levels) {
      if (api_level < min_api) {
        continue;
      }
      CompilationType type = { .arch = arch, .api_level = api_level };
      result.insert(type);
    }
  }
  return result;
}

using DeclarationDatabase = std::map<std::string, std::map<CompilationType, Declaration>>;

static DeclarationDatabase transposeHeaderDatabases(
    const std::map<CompilationType, HeaderDatabase>& original) {
  DeclarationDatabase result;
  for (const auto& outer : original) {
    const CompilationType& type = outer.first;
    for (const auto& inner : outer.second.declarations) {
      const std::string& symbol_name = inner.first;
      result[symbol_name][type] = inner.second;
    }
  }
  return result;
}

static DeclarationDatabase compileHeaders(const std::set<CompilationType>& types,
                                          const std::string& header_dir,
                                          const std::string& dependency_dir, bool* failed) {
  constexpr size_t thread_count = 8;
  size_t threads_created = 0;
  std::mutex mutex;
  std::vector<std::thread> threads(thread_count);

  std::map<CompilationType, HeaderDatabase> header_databases;
  std::unordered_map<std::string, CompilationRequirements> requirements;

  std::string cwd = getWorkingDir();
  bool errors = false;

  for (const auto& type : types) {
    if (requirements.count(type.arch) == 0) {
      requirements[type.arch] = collectRequirements(type.arch, header_dir, dependency_dir);
    }
  }

  for (const auto& type : types) {
    size_t thread_id = threads_created++;
    if (thread_id >= thread_count) {
      thread_id = thread_id % thread_count;
      threads[thread_id].join();
    }

    threads[thread_id] = std::thread(
        [&](CompilationType type) {
          const auto& req = requirements[type.arch];

          HeaderDatabase database;
          HeaderCompilationDatabase compilation_database(type, cwd, req.headers, req.dependencies);

          ClangTool tool(compilation_database, req.headers);

          clang::DiagnosticOptions diagnostic_options;
          std::vector<std::unique_ptr<ASTUnit>> asts;
          tool.buildASTs(asts);
          for (const auto& ast : asts) {
            clang::DiagnosticsEngine& diagnostics_engine = ast->getDiagnostics();
            if (diagnostics_engine.getNumWarnings() || diagnostics_engine.hasErrorOccurred()) {
              std::unique_lock<std::mutex> l(mutex);
              errors = true;
              printf("versioner: compilation failure for %s in %s\n", type.describe().c_str(),
                     ast->getOriginalSourceFileName().str().c_str());
            }

            database.parseAST(ast.get());
          }

          std::unique_lock<std::mutex> l(mutex);
          header_databases[type] = database;
        },
        type);
  }

  if (threads_created < thread_count) {
    threads.resize(threads_created);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  if (errors) {
    printf("versioner: compilation generated warnings or errors\n");
    *failed = errors;
  }

  return transposeHeaderDatabases(header_databases);
}

static bool sanityCheck(const std::set<CompilationType>& types,
                        const DeclarationDatabase& database) {
  bool error = false;
  std::string cwd = getWorkingDir() + "/";

  for (auto outer : database) {
    const std::string& symbol_name = outer.first;
    CompilationType last_type;
    DeclarationAvailability last_availability;

    // Rely on std::set being sorted to loop through the types by architecture.
    for (const CompilationType& type : types) {
      auto inner = outer.second.find(type);
      if (inner == outer.second.end()) {
        // TODO: Check for holes.
        continue;
      }

      const Declaration& declaration = inner->second;
      bool found_availability = false;
      bool availability_mismatch = false;
      DeclarationAvailability current_availability;

      // Ensure that all of the availability declarations for this symbol match.
      for (const DeclarationLocation& location : declaration.locations) {
        if (!found_availability) {
          found_availability = true;
          current_availability = location.availability;
          continue;
        }

        if (current_availability != location.availability) {
          availability_mismatch = true;
          error = true;
        }
      }

      if (availability_mismatch) {
        printf("%s: availability mismatch for %s\n", symbol_name.c_str(), type.describe().c_str());
        declaration.dump(cwd);
      }

      if (type.arch != last_type.arch) {
        last_type = type;
        last_availability = current_availability;
        continue;
      }

      // Ensure that availability declarations are consistent across API levels for a given arch.
      if (last_availability != current_availability) {
        error = true;
        printf("%s: availability mismatch between %s and %s: [%s] before, [%s] after\n",
               symbol_name.c_str(), last_type.describe().c_str(), type.describe().c_str(),
               last_availability.describe().c_str(), current_availability.describe().c_str());
      }

      // Ensure that at most one inline definition of a function exists.
      std::set<DeclarationLocation> inline_definitions;

      for (const DeclarationLocation& location : declaration.locations) {
        if (location.is_definition) {
          inline_definitions.insert(location);
        }
      }

      if (inline_definitions.size() > 1) {
        error = true;
        printf("%s: multiple inline definitions found:\n", symbol_name.c_str());
        for (const DeclarationLocation& location : declaration.locations) {
          location.dump(cwd);
        }
      }

      last_type = type;
    }
  }
  return !error;
}

// Check that our symbol availability declarations match the actual NDK
// platform symbol availability.
static bool checkVersions(const std::set<CompilationType>& types,
                          const DeclarationDatabase& declaration_database,
                          const NdkSymbolDatabase& symbol_database) {
  bool failed = false;

  std::map<std::string, std::set<CompilationType>> arch_types;
  for (const CompilationType& type : types) {
    arch_types[type.arch].insert(type);
  }

  std::set<std::string> completely_unavailable;

  for (const auto& outer : declaration_database) {
    const std::string& symbol_name = outer.first;
    const auto& compilations = outer.second;

    auto platform_availability_it = symbol_database.find(symbol_name);
    if (platform_availability_it == symbol_database.end()) {
      completely_unavailable.insert(symbol_name);
      continue;
    }

    const auto& platform_availability = platform_availability_it->second;
    std::set<CompilationType> missing_symbol;
    std::set<CompilationType> missing_decl;

    for (const CompilationType& type : types) {
      auto it = compilations.find(type);
      if (it == compilations.end()) {
        missing_decl.insert(type);
        continue;
      }

      const Declaration& declaration = it->second;

      // sanityCheck ensured that the availability declarations for a given arch match.
      DeclarationAvailability availability = declaration.locations.begin()->availability;
      int api_level = type.api_level;

      int introduced = std::max(0, availability.introduced);
      int obsoleted = availability.obsoleted == 0 ? INT_MAX : availability.obsoleted;
      bool decl_available = api_level >= introduced && api_level < obsoleted;

      auto symbol_availability_it = platform_availability.find(type);
      bool symbol_available = symbol_availability_it != platform_availability.end();
      if (decl_available) {
        if (!symbol_available) {
          // Ensure that either it exists in the platform, or an inline definition is visible.
          if (!declaration.hasDefinition()) {
            missing_symbol.insert(type);
            continue;
          }
        } else {
          // Ensure that symbols declared as functions/variables actually are.
          switch (declaration.type()) {
            case DeclarationType::inconsistent:
              printf("%s: inconsistent declaration type\n", symbol_name.c_str());
              declaration.dump();
              exit(1);

            case DeclarationType::variable:
              if (symbol_availability_it->second != NdkSymbolType::variable) {
                printf("%s: declared as variable, exists in platform as function\n",
                       symbol_name.c_str());
                failed = true;
              }
              break;

            case DeclarationType::function:
              if (symbol_availability_it->second != NdkSymbolType::function) {
                printf("%s: declared as function, exists in platform as variable\n",
                       symbol_name.c_str());
                failed = true;
              }
              break;
          }
        }
      } else {
        // Ensure that it's not available in the platform.
        if (symbol_availability_it != platform_availability.end()) {
          printf("%s: symbol should be unavailable in %s (declared with availability %s)\n",
                 symbol_name.c_str(), type.describe().c_str(), availability.describe().c_str());
          failed = true;
        }
      }
    }

    // Allow declarations to be missing from an entire architecture.
    for (const auto& arch_type : arch_types) {
      const std::string& arch = arch_type.first;
      bool found_all = true;
      for (const auto& type : arch_type.second) {
        if (missing_decl.find(type) == missing_decl.end()) {
          found_all = false;
          break;
        }
      }

      if (!found_all) {
        continue;
      }

      for (auto it = missing_decl.begin(); it != missing_decl.end();) {
        if (it->arch == arch) {
          it = missing_decl.erase(it);
        } else {
          ++it;
        }
      }
    }

    auto types_to_string = [](const std::set<CompilationType>& types) {
      std::string result;
      for (const CompilationType& type : types) {
        result += type.describe();
        result += ", ";
      }
      result.resize(result.length() - 2);
      return result;
    };

    if (!missing_decl.empty()) {
      printf("%s: declaration missing in %s\n", symbol_name.c_str(),
             types_to_string(missing_decl).c_str());
      failed = true;
    }

    if (!missing_symbol.empty()) {
      printf("%s: declaration marked available but symbol missing in [%s]\n", symbol_name.c_str(),
             types_to_string(missing_symbol).c_str());
      failed = true;
    }
  }

  for (const std::string& symbol_name : completely_unavailable) {
    bool found_inline_definition = false;
    bool future = false;

    auto symbol_it = declaration_database.find(symbol_name);

    // Ignore inline functions and functions that are tagged as __INTRODUCED_IN_FUTURE.
    // Ensure that all of the declarations of that function satisfy that.
    for (const auto& declaration_pair : symbol_it->second) {
      const Declaration& declaration = declaration_pair.second;
      DeclarationAvailability availability = declaration.locations.begin()->availability;

      if (availability.introduced >= 10000) {
        future = true;
      }

      if (declaration.hasDefinition()) {
        found_inline_definition = true;
      }
    }

    if (future || found_inline_definition) {
      continue;
    }

    if (missing_symbol_whitelist.count(symbol_name) != 0) {
      continue;
    }

    printf("%s: not available in any platform\n", symbol_name.c_str());
    failed = true;
  }

  return !failed;
}

static void usage(bool help = false) {
  fprintf(stderr, "Usage: versioner [OPTION]... [HEADER_PATH] [DEPS_PATH]\n");
  if (!help) {
    printf("Try 'versioner -h' for more information.\n");
    exit(1);
  } else {
    fprintf(stderr, "Version headers at HEADER_PATH, with DEPS_PATH/ARCH/* on the include path\n");
    fprintf(stderr, "Autodetects paths if HEADER_PATH and DEPS_PATH are not specified\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Target specification (defaults to all):\n");
    fprintf(stderr, "  -a API_LEVEL\tbuild with specified API level (can be repeated)\n");
    fprintf(stderr, "    \t\tvalid levels are %s\n", Join(supported_levels).c_str());
    fprintf(stderr, "  -r ARCH\tbuild with specified architecture (can be repeated)\n");
    fprintf(stderr, "    \t\tvalid architectures are %s\n", Join(supported_archs).c_str());
    fprintf(stderr, "\n");
    fprintf(stderr, "Validation:\n");
    fprintf(stderr, "  -p PATH\tcompare against NDK platform at PATH\n");
    fprintf(stderr, "  -v\t\tenable verbose warnings\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Miscellaneous:\n");
    fprintf(stderr, "  -h\t\tdisplay this message\n");
    exit(0);
  }
}

int main(int argc, char** argv) {
  std::string cwd = getWorkingDir() + "/";
  bool default_args = true;
  std::string platform_dir;
  std::set<std::string> selected_architectures;
  std::set<int> selected_levels;

  int c;
  while ((c = getopt(argc, argv, "a:r:p:vh")) != -1) {
    default_args = false;
    switch (c) {
      case 'a': {
        char* end;
        int api_level = strtol(optarg, &end, 10);
        if (end == optarg || strlen(end) > 0) {
          usage();
        }

        if (supported_levels.count(api_level) == 0) {
          errx(1, "unsupported API level %d", api_level);
        }

        selected_levels.insert(api_level);
        break;
      }

      case 'r': {
        if (supported_archs.count(optarg) == 0) {
          errx(1, "unsupported architecture: %s", optarg);
        }
        selected_architectures.insert(optarg);
        break;
      }

      case 'p': {
        if (!platform_dir.empty()) {
          usage();
        }

        platform_dir = optarg;

        struct stat st;
        if (stat(platform_dir.c_str(), &st) != 0) {
          err(1, "failed to stat platform directory '%s'", platform_dir.c_str());
        }
        if (!S_ISDIR(st.st_mode)) {
          errx(1, "'%s' is not a directory", optarg);
        }
        break;
      }

      case 'v':
        verbose = true;
        break;

      case 'h':
        usage(true);
        break;

      default:
        usage();
        break;
    }
  }

  if (argc - optind > 2 || optind > argc) {
    usage();
  }

  std::string header_dir;
  std::string dependency_dir;

  if (optind == argc) {
    // Neither HEADER_PATH nor DEPS_PATH were specified, so try to figure them out.
    const char* top = getenv("ANDROID_BUILD_TOP");
    if (!top) {
      fprintf(stderr, "versioner: failed to autodetect bionic paths. Is ANDROID_BUILD_TOP set?\n");
      usage();
    }

    std::string versioner_dir = std::to_string(top) + "/bionic/tools/versioner";
    header_dir = versioner_dir + "/current";
    dependency_dir = versioner_dir + "/dependencies";
    if (platform_dir.empty()) {
      platform_dir = versioner_dir + "/platforms";
    }
  } else {
    header_dir = argv[optind];

    if (argc - optind == 2) {
      dependency_dir = argv[optind + 1];
    }
  }

  if (selected_levels.empty()) {
    selected_levels = supported_levels;
  }

  if (selected_architectures.empty()) {
    selected_architectures = supported_archs;
  }


  struct stat st;
  if (stat(header_dir.c_str(), &st) != 0) {
    err(1, "failed to stat '%s'", header_dir.c_str());
  } else if (!S_ISDIR(st.st_mode)) {
    errx(1, "'%s' is not a directory", header_dir.c_str());
  }

  std::set<CompilationType> compilation_types;
  DeclarationDatabase declaration_database;
  NdkSymbolDatabase symbol_database;

  compilation_types = generateCompilationTypes(selected_architectures, selected_levels);

  // Do this before compiling so that we can early exit if the platforms don't match what we
  // expect.
  if (!platform_dir.empty()) {
    symbol_database = parsePlatforms(compilation_types, platform_dir);
  }

  bool failed = false;
  declaration_database = compileHeaders(compilation_types, header_dir, dependency_dir, &failed);

  if (!sanityCheck(compilation_types, declaration_database)) {
    printf("versioner: sanity check failed\n");
    failed = true;
  }

  if (!platform_dir.empty()) {
    if (!checkVersions(compilation_types, declaration_database, symbol_database)) {
      printf("versioner: version check failed\n");
      failed = true;
    }
  }

  return failed;
}
