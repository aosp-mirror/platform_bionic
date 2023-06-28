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

#include <dirent.h>
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__linux__)
#include <sched.h>
#endif

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

#include <llvm/ADT/StringRef.h>

#include <android-base/file.h>
#include <android-base/macros.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>

#include "Arch.h"
#include "DeclarationDatabase.h"
#include "Driver.h"
#include "Preprocessor.h"
#include "SymbolDatabase.h"
#include "Utils.h"
#include "VFS.h"

#include "versioner.h"

using namespace std::chrono_literals;
using namespace std::string_literals;

bool strict;
bool verbose;
bool add_include;

static int getCpuCount();
static int max_thread_count = getCpuCount();

static int getCpuCount() {
#if defined(__linux__)
  cpu_set_t cpu_set;
  int rc = sched_getaffinity(getpid(), sizeof(cpu_set), &cpu_set);
  if (rc != 0) {
    err(1, "sched_getaffinity failed");
  }
  return CPU_COUNT(&cpu_set);
#else
  return 1;
#endif
}

namespace {
struct HeaderLocationInformation {
  std::string header_path;
  std::string dependency_dir;
  // Absolute paths to ignore all children -- including subdirectories -- of.
  std::unordered_set<std::string> ignored_directories;
};
}

static bool is_dir(const std::string& path) {
  struct stat st;
  return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

static CompilationRequirements collectRequirements(const Arch& arch,
                                                   const HeaderLocationInformation& location) {
  std::vector<std::string> headers =
      collectHeaders(location.header_path, location.ignored_directories);
  std::vector<std::string> dependencies;

  if (is_dir(location.header_path)) {
    dependencies.emplace_back(location.header_path);
  }

  if (!location.dependency_dir.empty()) {
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

    collect_children(location.dependency_dir + "/common");
    collect_children(location.dependency_dir + "/" + to_string(arch));
  }

  auto new_end = std::remove_if(headers.begin(), headers.end(), [&arch](llvm::StringRef header) {
    for (const auto& it : ignored_headers) {
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

static std::set<CompilationType> generateCompilationTypes(const std::set<Arch> selected_architectures,
                                                          const std::set<int>& selected_levels) {
  std::set<CompilationType> result;
  for (const auto& arch : selected_architectures) {
    int min_api = arch_min_api[arch];
    for (int api_level : selected_levels) {
      if (api_level < min_api) {
        continue;
      }

      for (int file_offset_bits : { 32, 64 }) {
        for (bool cpp : { true, false }) {
          CompilationType type = {
            .arch = arch, .cpp = cpp, .api_level = api_level, .file_offset_bits = file_offset_bits
          };
          result.insert(type);
        }
      }
    }
  }
  return result;
}

static std::unique_ptr<HeaderDatabase> compileHeaders(const std::set<CompilationType>& types,
                                                      const HeaderLocationInformation& location) {
  if (types.empty()) {
    errx(1, "compileHeaders received no CompilationTypes");
  }

  auto vfs = createCommonVFS(location.header_path, location.dependency_dir, add_include);

  size_t thread_count = max_thread_count;
  std::vector<std::thread> threads;

  std::map<CompilationType, HeaderDatabase> header_databases;
  std::unordered_map<Arch, CompilationRequirements> requirements;

  auto result = std::make_unique<HeaderDatabase>();
  for (const auto& type : types) {
    if (requirements.count(type.arch) == 0) {
      requirements[type.arch] = collectRequirements(type.arch, location);
    }
  }

  initializeTargetCC1FlagCache(vfs, types, requirements);

  std::vector<std::pair<CompilationType, const std::string&>> jobs;
  std::atomic<size_t> job_index(0);
  for (CompilationType type : types) {
    CompilationRequirements& req = requirements[type.arch];
    for (const std::string& header : req.headers) {
      jobs.emplace_back(type, header);
    }
  }

  // Dup an empty file to stdin, so that we can use `clang -include a.h -` instead of `clang a.h`,
  // since some warnings don't get generated in files that are compiled directly.
  FILE* empty_file = tmpfile();
  if (!empty_file) {
    err(1, "failed to create temporary file");
  }

  int empty_file_fd = fileno(empty_file);
  if (empty_file_fd == -1) {
    errx(1, "fileno failed on tmpfile");
  }

  dup2(empty_file_fd, STDIN_FILENO);
  fclose(empty_file);

  thread_count = std::min(thread_count, jobs.size());

  if (thread_count == 1) {
    for (const auto& job : jobs) {
      compileHeader(vfs, result.get(), job.first, job.second);
    }
  } else {
    // Spawn threads.
    for (size_t i = 0; i < thread_count; ++i) {
      threads.emplace_back([&jobs, &job_index, &result, vfs]() {
        while (true) {
          size_t idx = job_index++;
          if (idx >= jobs.size()) {
            return;
          }

          const auto& job = jobs[idx];
          compileHeader(vfs, result.get(), job.first, job.second);
        }
      });
    }

    // Reap them.
    for (auto& thread : threads) {
      thread.join();
    }
    threads.clear();
  }

  return result;
}

static std::set<CompilationType> getCompilationTypes(const Declaration* decl) {
  std::set<CompilationType> result;
  for (const auto& it : decl->availability) {
    result.insert(it.first);
  }
  return result;
}

template<typename T>
static std::vector<T> Intersection(const std::set<T>& a, const std::set<T>& b) {
  std::vector<T> intersection;
  std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(intersection));
  return intersection;
}

// Perform a validity check on a symbol's declarations, enforcing the following invariants:
//   1. At most one inline definition of the function exists (overloaded inline functions for
//      _FORTIFY_SOURCE do not count because they are usually introduced to intercept the original
//      functions or usually have enable_if attributes).
//   2. All of the availability declarations for a symbol are compatible.
//      If a function is declared as an inline before a certain version, the inline definition
//      should have no version tag.
//   3. Each availability type must only be present globally or on a per-arch basis.
//      (e.g. __INTRODUCED_IN_32(21) __INTRODUCED_IN_64(22) __DEPRECATED_IN(23) is fine,
//      but not __INTRODUCED_IN(9) __INTRODUCED_IN_32(10))
static bool checkSymbol(const Symbol& symbol) {
  std::string cwd = getWorkingDir() + "/";

  std::unordered_map<const Declaration*, std::set<CompilationType>> inline_definitions;
  for (const auto& decl_it : symbol.declarations) {
    const Declaration* decl = &decl_it.second;
    if (decl->is_definition && !decl->fortify_inline) {
      std::set<CompilationType> compilation_types = getCompilationTypes(decl);
      for (const auto& inline_def_it : inline_definitions) {
        auto intersection = Intersection(compilation_types, inline_def_it.second);
        if (!intersection.empty()) {
          fprintf(stderr, "versioner: conflicting inline definitions for symbol %s:\n",
                  symbol.name.c_str());
          fprintf(stderr, "  declarations visible in: %s\n", Join(intersection, ", ").c_str());
          decl->dump(cwd, stderr, 4);
          inline_def_it.first->dump(cwd, stderr, 4);
          return false;
        }
      }

      inline_definitions[decl] = std::move(compilation_types);
    }

    DeclarationAvailability availability;
    if (!decl->calculateAvailability(&availability)) {
      fprintf(stderr, "versioner: failed to calculate availability for declaration:\n");
      decl->dump(cwd, stderr, 2);
      return false;
    }

    if (decl->is_definition && !availability.empty()) {
      fprintf(stderr, "versioner: inline definition has non-empty versioning information:\n");
      decl->dump(cwd, stderr, 2);
      return false;
    }
  }

  DeclarationAvailability availability;
  if (!symbol.calculateAvailability(&availability)) {
    fprintf(stderr, "versioner: inconsistent availability for symbol '%s'\n", symbol.name.c_str());
    symbol.dump(cwd);
    return false;
  }

  // TODO: Check invariant #3.
  return true;
}

static bool validityCheck(const HeaderDatabase* database) {
  bool error = false;
  std::string cwd = getWorkingDir() + "/";

  for (const auto& symbol_it : database->symbols) {
    if (!checkSymbol(symbol_it.second)) {
      error = true;
    }
  }
  return !error;
}

// Check that our symbol availability declarations match the actual NDK
// platform symbol availability.
static bool checkVersions(const std::set<CompilationType>& types,
                          const HeaderDatabase* header_database,
                          const NdkSymbolDatabase& symbol_database) {
  std::string cwd = getWorkingDir() + "/";
  bool failed = false;

  std::map<Arch, std::set<CompilationType>> arch_types;
  for (const CompilationType& type : types) {
    arch_types[type.arch].insert(type);
  }

  std::set<std::string> completely_unavailable;
  std::map<std::string, std::set<CompilationType>> missing_availability;
  std::map<std::string, std::set<CompilationType>> extra_availability;

  for (const auto& symbol_it : header_database->symbols) {
    const auto& symbol_name = symbol_it.first;
    DeclarationAvailability symbol_availability;

    if (!symbol_it.second.calculateAvailability(&symbol_availability)) {
      errx(1, "failed to calculate symbol availability");
    }

    const auto platform_availability_it = symbol_database.find(symbol_name);
    if (platform_availability_it == symbol_database.end()) {
      completely_unavailable.insert(symbol_name);
      continue;
    }

    const auto& platform_availability = platform_availability_it->second;

    for (const CompilationType& type : types) {
      bool should_be_available = true;
      const auto& global_availability = symbol_availability.global_availability;
      const auto& arch_availability = symbol_availability.arch_availability[type.arch];
      if (global_availability.introduced != 0 && global_availability.introduced > type.api_level) {
        should_be_available = false;
      }

      if (arch_availability.introduced != 0 && arch_availability.introduced > type.api_level) {
        should_be_available = false;
      }

      if (global_availability.obsoleted != 0 && global_availability.obsoleted <= type.api_level) {
        should_be_available = false;
      }

      if (arch_availability.obsoleted != 0 && arch_availability.obsoleted <= type.api_level) {
        should_be_available = false;
      }

      // The function declaration might be (validly) missing for the given CompilationType.
      if (!symbol_it.second.hasDeclaration(type)) {
        should_be_available = false;
      }

      bool is_available = platform_availability.count(type);

      if (should_be_available != is_available) {
        if (is_available) {
          extra_availability[symbol_name].insert(type);
        } else {
          missing_availability[symbol_name].insert(type);
        }
      }
    }
  }

  for (const auto& it : symbol_database) {
    const std::string& symbol_name = it.first;

    bool symbol_error = false;
    if (auto missing_it = missing_availability.find(symbol_name);
        missing_it != missing_availability.end()) {
      printf("%s: declaration marked available but symbol missing in [%s]\n", symbol_name.c_str(),
             Join(missing_it->second, ", ").c_str());
      symbol_error = true;
      failed = true;
    }

    if (strict) {
      if (auto extra_it = extra_availability.find(symbol_name);
          extra_it != extra_availability.end()) {
        printf("%s: declaration marked unavailable but symbol available in [%s]\n",
               symbol_name.c_str(), Join(extra_it->second, ", ").c_str());
        symbol_error = true;
        failed = true;
      }
    }

    if (symbol_error) {
      if (auto symbol_it = header_database->symbols.find(symbol_name);
          symbol_it != header_database->symbols.end()) {
        symbol_it->second.dump(cwd);
      } else {
        errx(1, "failed to find symbol in header database");
      }
    }
  }

  // TODO: Verify that function/variable declarations are actually function/variable symbols.
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
    fprintf(stderr, "    \t\tdefaults to %s\n", Join(default_levels).c_str());
    fprintf(stderr, "  -r ARCH\tbuild with specified architecture (can be repeated)\n");
    fprintf(stderr, "    \t\tvalid architectures are %s\n", Join(supported_archs).c_str());
    fprintf(stderr, "\n");
    fprintf(stderr, "Validation:\n");
    fprintf(stderr, "  -p PATH\tcompare against NDK platform at PATH\n");
    fprintf(stderr, "  -s\t\tenable strict warnings\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Preprocessing:\n");
    fprintf(stderr, "  -o PATH\tpreprocess header files and emit them at PATH\n");
    fprintf(stderr, "  -f\t\tpreprocess header files even if validation fails\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Miscellaneous:\n");
    fprintf(stderr, "  -F\t\tdo not ignore FORTIFY headers by default\n");
    fprintf(stderr, "  -d\t\tdump function availability\n");
    fprintf(stderr, "  -j THREADS\tmaximum number of threads to use\n");
    fprintf(stderr, "  -v\t\tenable verbose logging\n");
    fprintf(stderr, "  -h\t\tdisplay this message\n");
    exit(0);
  }
}

// versioner uses a prebuilt version of clang, which is not up-to-date wrt/
// container annotations. So disable container overflow checking. b/37775238
extern "C" const char* __asan_default_options() {
  return "detect_container_overflow=0";
}

int main(int argc, char** argv) {
  std::string cwd = getWorkingDir() + "/";
  std::string platform_dir;
  std::set<Arch> selected_architectures;
  std::set<int> selected_levels;
  std::string preprocessor_output_path;
  bool force = false;
  bool dump = false;
  bool ignore_fortify_headers = true;

  int c;
  while ((c = getopt(argc, argv, "a:r:p:so:fdj:vhFi")) != -1) {
    switch (c) {
      case 'a': {
        char* end;
        int api_level = strtol(optarg, &end, 10);
        if (end == optarg || strlen(end) > 0) {
          usage();
        }

        selected_levels.insert(api_level);
        break;
      }

      case 'r': {
        std::optional<Arch> arch = arch_from_string(optarg);
        if (!arch) {
          errx(1, "unknown architecture '%s'", optarg);
        }
        selected_architectures.insert(*arch);
        break;
      }

      case 'p': {
        if (!platform_dir.empty()) {
          usage();
        }

        platform_dir = optarg;

        if (platform_dir.empty()) {
          usage();
        }

        struct stat st;
        if (stat(platform_dir.c_str(), &st) != 0) {
          err(1, "failed to stat platform directory '%s'", platform_dir.c_str());
        }
        if (!S_ISDIR(st.st_mode) && !S_ISREG(st.st_mode)) {
          errx(1, "'%s' is not a file or directory", optarg);
        }
        break;
      }

      case 's':
        strict = true;
        break;

      case 'o':
        if (!preprocessor_output_path.empty()) {
          usage();
        }
        preprocessor_output_path = optarg;
        if (preprocessor_output_path.empty()) {
          usage();
        }
        break;

      case 'f':
        force = true;
        break;

      case 'd':
        dump = true;
        break;

      case 'j':
        if (!android::base::ParseInt<int>(optarg, &max_thread_count, 1)) {
          usage();
        }
        break;

      case 'v':
        verbose = true;
        break;

      case 'h':
        usage(true);
        break;

      case 'i':
        // Secret option for tests to -include <android/versioning.h>.
        add_include = true;
        break;

      case 'F':
        ignore_fortify_headers = false;
        break;

      default:
        usage();
        break;
    }
  }

  if (argc - optind > 2 || optind > argc) {
    usage();
  }

  HeaderLocationInformation location;

  const char* top = getenv("ANDROID_BUILD_TOP");
  if (!top && (optind == argc || add_include)) {
    fprintf(stderr, "versioner: failed to autodetect bionic paths. Is ANDROID_BUILD_TOP set?\n");
    usage();
  }

  if (optind == argc) {
    // Neither HEADER_PATH nor DEPS_PATH were specified, so try to figure them out.
    std::string versioner_dir = to_string(top) + "/bionic/tools/versioner";
    location.header_path = versioner_dir + "/current";
    location.dependency_dir = versioner_dir + "/dependencies";
    if (platform_dir.empty()) {
      platform_dir = versioner_dir + "/platforms";
    }
  } else {
    if (!android::base::Realpath(argv[optind], &location.header_path)) {
      err(1, "failed to get realpath for path '%s'", argv[optind]);
    }

    if (argc - optind == 2) {
      location.dependency_dir = argv[optind + 1];
    }
  }

  // Every file that lives in bits/fortify is logically a part of a header outside of bits/fortify.
  // This makes the files there impossible to build on their own.
  if (ignore_fortify_headers) {
    std::string fortify_path = location.header_path;
    if (!android::base::EndsWith(location.header_path, "/")) {
      fortify_path += '/';
    }
    fortify_path += "bits/fortify";
    location.ignored_directories.insert(std::move(fortify_path));
  }

  if (selected_levels.empty()) {
    selected_levels = default_levels;
  }

  if (selected_architectures.empty()) {
    selected_architectures = supported_archs;
  }


  struct stat st;
  if (const char *path = location.header_path.c_str(); stat(path, &st) != 0) {
    err(1, "failed to stat '%s'", path);
  }

  std::set<CompilationType> compilation_types;
  std::optional<NdkSymbolDatabase> symbol_database;

  compilation_types = generateCompilationTypes(selected_architectures, selected_levels);

  // Do this before compiling so that we can early exit if the platforms don't match what we
  // expect.
  if (!platform_dir.empty()) {
    symbol_database = parsePlatforms(compilation_types, platform_dir);
  }

  auto start = std::chrono::high_resolution_clock::now();
  std::unique_ptr<HeaderDatabase> declaration_database =
      compileHeaders(compilation_types, location);
  auto end = std::chrono::high_resolution_clock::now();

  if (verbose) {
    auto diff = (end - start) / 1.0ms;
    printf("Compiled headers for %zu targets in %0.2LFms\n", compilation_types.size(), diff);
  }

  bool failed = false;
  if (dump) {
    declaration_database->dump(location.header_path + "/");
  } else {
    if (!validityCheck(declaration_database.get())) {
      printf("versioner: validity check failed\n");
      failed = true;
    }

    if (symbol_database) {
      if (!checkVersions(compilation_types, declaration_database.get(), *symbol_database)) {
        printf("versioner: version check failed\n");
        failed = true;
      }
    }
  }

  if (!preprocessor_output_path.empty() && (force || !failed)) {
    failed = !preprocessHeaders(preprocessor_output_path, location.header_path,
                                declaration_database.get());
  }
  return failed;
}
