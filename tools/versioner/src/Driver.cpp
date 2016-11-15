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

#include "Driver.h"

#include <err.h>
#include <string.h>

#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/VirtualFileSystem.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/Utils.h>
#include <clang/FrontendTool/Utils.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include "Arch.h"
#include "DeclarationDatabase.h"
#include "versioner.h"

using namespace std::chrono_literals;
using namespace std::string_literals;

using namespace clang;

class VersionerASTConsumer : public clang::ASTConsumer {
 public:
  HeaderDatabase* header_database;
  CompilationType type;

  VersionerASTConsumer(HeaderDatabase* header_database, CompilationType type)
      : header_database(header_database), type(type) {
  }

  virtual void HandleTranslationUnit(ASTContext& ctx) override {
    header_database->parseAST(type, ctx);
  }
};

class VersionerASTAction : public clang::ASTFrontendAction {
 public:
  HeaderDatabase* header_database;
  CompilationType type;

  VersionerASTAction(HeaderDatabase* header_database, CompilationType type)
      : header_database(header_database), type(type) {
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance&, llvm::StringRef) override {
    return std::make_unique<VersionerASTConsumer>(header_database, type);
  }
};

static IntrusiveRefCntPtr<DiagnosticsEngine> constructDiags() {
  IntrusiveRefCntPtr<DiagnosticOptions> diag_opts(new DiagnosticOptions());
  auto diag_printer = std::make_unique<TextDiagnosticPrinter>(llvm::errs(), diag_opts.get());
  IntrusiveRefCntPtr<DiagnosticIDs> diag_ids(new DiagnosticIDs());
  IntrusiveRefCntPtr<DiagnosticsEngine> diags(
      new DiagnosticsEngine(diag_ids.get(), diag_opts.get(), diag_printer.release()));
  return diags;
}

// clang's driver is slow compared to the work it performs to compile our headers.
// Run it once to generate flags for each target, and memoize the results.
static std::unordered_map<CompilationType, std::vector<std::string>> cc1_flags;
static const char* filename_placeholder = "__VERSIONER_PLACEHOLDER__";
static void generateTargetCC1Flags(llvm::IntrusiveRefCntPtr<clang::vfs::FileSystem> vfs,
                                   CompilationType type,
                                   const std::vector<std::string>& include_dirs) {
  std::vector<std::string> cmd = { "versioner" };
  cmd.push_back("-std=c11");
  cmd.push_back("-x");
  cmd.push_back("c-header");
  cmd.push_back("-fsyntax-only");

  cmd.push_back("-Wall");
  cmd.push_back("-Wextra");
  cmd.push_back("-Werror");
  cmd.push_back("-Wundef");
  cmd.push_back("-Wno-unused-macros");
  cmd.push_back("-Wno-unused-function");
  cmd.push_back("-Wno-unused-variable");
  cmd.push_back("-Wno-unknown-attributes");
  cmd.push_back("-Wno-pragma-once-outside-header");

  cmd.push_back("-target");
  cmd.push_back(arch_targets[type.arch]);

  cmd.push_back("-DANDROID");
  cmd.push_back("-D__ANDROID_API__="s + std::to_string(type.api_level));
  cmd.push_back("-D_FORTIFY_SOURCE=2");
  cmd.push_back("-D_GNU_SOURCE");
  cmd.push_back("-D_FILE_OFFSET_BITS="s + std::to_string(type.file_offset_bits));

  cmd.push_back("-nostdinc");

  if (add_include) {
    cmd.push_back("-include");
    cmd.push_back("android/versioning.h");
  }

  for (const auto& dir : include_dirs) {
    cmd.push_back("-isystem");
    cmd.push_back(dir);
  }

  cmd.push_back(filename_placeholder);

  auto diags = constructDiags();
  driver::Driver driver("versioner", llvm::sys::getDefaultTargetTriple(), *diags, vfs);
  driver.setCheckInputsExist(false);

  llvm::SmallVector<const char*, 32> driver_args;
  for (const std::string& str : cmd) {
    driver_args.push_back(str.c_str());
  }

  std::unique_ptr<driver::Compilation> Compilation(driver.BuildCompilation(driver_args));
  const driver::JobList& jobs = Compilation->getJobs();
  if (jobs.size() != 1) {
    errx(1, "driver returned %zu jobs for %s", jobs.size(), to_string(type).c_str());
  }

  const driver::Command& driver_cmd = llvm::cast<driver::Command>(*jobs.begin());
  const driver::ArgStringList& cc_args = driver_cmd.getArguments();

  if (cc_args.size() == 0) {
    errx(1, "driver returned empty command for %s", to_string(type).c_str());
  }

  std::vector<std::string> result(cc_args.begin(), cc_args.end());

  {
    static std::mutex cc1_init_mutex;
    std::unique_lock<std::mutex> lock(cc1_init_mutex);
    if (cc1_flags.count(type) > 0) {
      errx(1, "attemped to generate cc1 flags for existing CompilationType %s",
           to_string(type).c_str());
    }

    cc1_flags.emplace(std::make_pair(type, std::move(result)));
  }
}

static std::vector<const char*> getCC1Command(CompilationType type, const std::string& filename) {
  const auto& target_flag_it = cc1_flags.find(type);
  if (target_flag_it == cc1_flags.end()) {
    errx(1, "failed to find target flags for CompilationType %s", to_string(type).c_str());
  }

  std::vector<const char*> result;
  for (const std::string& flag : target_flag_it->second) {
    if (flag == "-disable-free") {
      continue;
    } else if (flag == filename_placeholder) {
      result.push_back(filename.c_str());
    } else {
      result.push_back(flag.c_str());
    }
  }
  return result;
}

void initializeTargetCC1FlagCache(llvm::IntrusiveRefCntPtr<clang::vfs::FileSystem> vfs,
                                  const std::set<CompilationType>& types,
                                  const std::unordered_map<Arch, CompilationRequirements>& reqs) {
  if (!cc1_flags.empty()) {
    errx(1, "reinitializing target CC1 flag cache?");
  }

  auto start = std::chrono::high_resolution_clock::now();
  std::vector<std::thread> threads;
  for (const CompilationType type : types) {
    threads.emplace_back([type, &vfs, &reqs]() {
      const auto& arch_req_it = reqs.find(type.arch);
      if (arch_req_it == reqs.end()) {
        errx(1, "CompilationRequirement map missing entry for CompilationType %s",
             to_string(type).c_str());
      }

      generateTargetCC1Flags(vfs, type, arch_req_it->second.dependencies);
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  auto end = std::chrono::high_resolution_clock::now();

  if (verbose) {
    auto diff = (end - start) / 1.0ms;
    printf("Generated compiler flags for %zu targets in %0.2Lfms\n", types.size(), diff);
  }

  if (cc1_flags.empty()) {
    errx(1, "failed to initialize target CC1 flag cache");
  }
}

void compileHeader(llvm::IntrusiveRefCntPtr<clang::vfs::FileSystem> vfs,
                   HeaderDatabase* header_database, CompilationType type,
                   const std::string& filename) {
  auto diags = constructDiags();
  std::vector<const char*> cc1_flags = getCC1Command(type, filename);
  auto invocation = std::make_unique<CompilerInvocation>();
  if (!CompilerInvocation::CreateFromArgs(*invocation.get(), &cc1_flags.front(),
                                          &cc1_flags.front() + cc1_flags.size(), *diags)) {
    errx(1, "failed to create CompilerInvocation");
  }

  clang::CompilerInstance Compiler;
  Compiler.setInvocation(invocation.release());
  Compiler.setDiagnostics(diags.get());
  Compiler.setVirtualFileSystem(vfs);

  VersionerASTAction versioner_action(header_database, type);
  if (!Compiler.ExecuteAction(versioner_action)) {
    errx(1, "compilation generated warnings or errors");
  }

  if (diags->getNumWarnings() || diags->hasErrorOccurred()) {
    errx(1, "compilation generated warnings or errors");
  }
}
