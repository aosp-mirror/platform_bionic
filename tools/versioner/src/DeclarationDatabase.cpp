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

#include "DeclarationDatabase.h"

#include <err.h>

#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include <clang/AST/AST.h>
#include <clang/AST/Attr.h>
#include <clang/AST/Mangle.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/ASTUnit.h>
#include <llvm/Support/raw_ostream.h>

using namespace clang;

static bool shouldMangle(MangleContext* mangler, NamedDecl* decl) {
  // Passing a decl with static linkage to the mangler gives incorrect results.
  // Check some things ourselves before handing it off to the mangler.
  if (auto FD = dyn_cast<FunctionDecl>(decl)) {
    if (FD->isExternC()) {
      return false;
    }

    if (FD->isInExternCContext()) {
      return false;
    }
  }

  return mangler->shouldMangleDeclName(decl);
}

class Visitor : public RecursiveASTVisitor<Visitor> {
  HeaderDatabase& database;
  CompilationType type;
  SourceManager& src_manager;
  std::unique_ptr<MangleContext> mangler;

 public:
  Visitor(HeaderDatabase& database, CompilationType type, ASTContext& ctx)
      : database(database), type(type), src_manager(ctx.getSourceManager()) {
    mangler.reset(ItaniumMangleContext::create(ctx, ctx.getDiagnostics()));
  }

  std::string getDeclName(NamedDecl* decl) {
    if (auto var_decl = dyn_cast<VarDecl>(decl)) {
      if (!var_decl->isFileVarDecl()) {
        return "<local var>";
      }
    }

    // <math.h> maps fool onto foo on 32-bit, since long double is the same as double.
    if (auto asm_attr = decl->getAttr<AsmLabelAttr>()) {
      return asm_attr->getLabel();
    }

    // The decl might not have a name (e.g. bitfields).
    if (auto identifier = decl->getIdentifier()) {
      if (shouldMangle(mangler.get(), decl)) {
        std::string mangled;
        llvm::raw_string_ostream ss(mangled);
        mangler->mangleName(decl, ss);
        return mangled;
      }

      return identifier->getName();
    }

    return "<unnamed>";
  }

  bool VisitDeclaratorDecl(DeclaratorDecl* decl, SourceRange range) {
    // Skip declarations inside of functions (function arguments, variable declarations inside of
    // inline functions, etc).
    if (decl->getParentFunctionOrMethod()) {
      return true;
    }

    auto named_decl = dyn_cast<NamedDecl>(decl);
    if (!named_decl) {
      return true;
    }

    std::string declaration_name = getDeclName(named_decl);
    bool is_extern = named_decl->getFormalLinkage() == ExternalLinkage;
    bool is_definition = false;
    bool no_guard = false;

    if (auto function_decl = dyn_cast<FunctionDecl>(decl)) {
      is_definition = function_decl->isThisDeclarationADefinition();
    } else if (auto var_decl = dyn_cast<VarDecl>(decl)) {
      if (!var_decl->isFileVarDecl()) {
        return true;
      }

      switch (var_decl->isThisDeclarationADefinition()) {
        case VarDecl::DeclarationOnly:
          is_definition = false;
          break;

        case VarDecl::Definition:
          is_definition = true;
          break;

        case VarDecl::TentativeDefinition:
          // Forbid tentative definitions in headers.
          fprintf(stderr, "ERROR: declaration '%s' is a tentative definition\n",
                  declaration_name.c_str());
          decl->dump();
          abort();
      }
    } else {
      // We only care about function and variable declarations.
      return true;
    }

    if (decl->hasAttr<UnavailableAttr>()) {
      // Skip declarations that exist only for compile-time diagnostics.
      return true;
    }

    DeclarationAvailability availability;

    // Find and parse __ANDROID_AVAILABILITY_DUMP__ annotations.
    for (const AnnotateAttr* attr : decl->specific_attrs<AnnotateAttr>()) {
      llvm::StringRef annotation = attr->getAnnotation();
      if (annotation == "versioner_no_guard") {
        no_guard = true;
      } else if (annotation == "introduced_in_future") {
        // Tag the compiled-for arch, since this can vary across archs.
        availability.arch_availability[type.arch].future = true;
      } else {
        llvm::SmallVector<llvm::StringRef, 2> fragments;
        annotation.split(fragments, "=");
        if (fragments.size() != 2) {
          continue;
        }

        auto& global_availability = availability.global_availability;
        auto& arch_availability = availability.arch_availability;
        std::map<std::string, std::vector<int*>> prefix_map = {
          { "introduced_in", { &global_availability.introduced } },
          { "deprecated_in", { &global_availability.deprecated } },
          { "obsoleted_in", { &global_availability.obsoleted } },
          { "introduced_in_arm", { &arch_availability[Arch::arm].introduced } },
          { "introduced_in_mips", { &arch_availability[Arch::mips].introduced } },
          { "introduced_in_x86", { &arch_availability[Arch::x86].introduced } },
          { "introduced_in_32",
            { &arch_availability[Arch::arm].introduced,
              &arch_availability[Arch::mips].introduced,
              &arch_availability[Arch::x86].introduced } },
          { "introduced_in_64",
            { &arch_availability[Arch::arm64].introduced,
              &arch_availability[Arch::mips64].introduced,
              &arch_availability[Arch::x86_64].introduced } },
        };

        if (auto it = prefix_map.find(fragments[0]); it != prefix_map.end()) {
          int value;
          if (fragments[1].getAsInteger(10, value)) {
            errx(1, "invalid __ANDROID_AVAILABILITY_DUMP__ annotation: '%s'",
                 annotation.str().c_str());
          }

          for (int* ptr : it->second) {
            *ptr = value;
          }
        }
      }
    }

    auto symbol_it = database.symbols.find(declaration_name);
    if (symbol_it == database.symbols.end()) {
      Symbol symbol = {.name = declaration_name };
      bool dummy;
      std::tie(symbol_it, dummy) = database.symbols.insert({ declaration_name, symbol });
    }

    auto expansion_range = src_manager.getExpansionRange(range);
    auto filename = src_manager.getFilename(expansion_range.getBegin());
    if (filename != src_manager.getFilename(expansion_range.getEnd())) {
      errx(1, "expansion range filenames don't match");
    }

    Location location = {
      .filename = filename,
      .start = {
        .line = src_manager.getExpansionLineNumber(expansion_range.getBegin()),
        .column = src_manager.getExpansionColumnNumber(expansion_range.getBegin()),
      },
      .end = {
        .line = src_manager.getExpansionLineNumber(expansion_range.getEnd()),
        .column = src_manager.getExpansionColumnNumber(expansion_range.getEnd()),
      }
    };

    // Find or insert an entry for the declaration.
    if (auto declaration_it = symbol_it->second.declarations.find(location);
        declaration_it != symbol_it->second.declarations.end()) {
      if (declaration_it->second.is_extern != is_extern ||
          declaration_it->second.is_definition != is_definition ||
          declaration_it->second.no_guard != no_guard) {
        errx(1, "varying declaration of '%s' at %s:%u:%u", declaration_name.c_str(),
             location.filename.c_str(), location.start.line, location.start.column);
      }
      declaration_it->second.availability.insert(std::make_pair(type, availability));
    } else {
      Declaration declaration;
      declaration.name = declaration_name;
      declaration.location = location;
      declaration.is_extern = is_extern;
      declaration.is_definition = is_definition;
      declaration.no_guard = no_guard;
      declaration.availability.insert(std::make_pair(type, availability));
      symbol_it->second.declarations.insert(std::make_pair(location, declaration));
    }

    return true;
  }

  bool VisitDeclaratorDecl(DeclaratorDecl* decl) {
    return VisitDeclaratorDecl(decl, decl->getSourceRange());
  }

  bool TraverseLinkageSpecDecl(LinkageSpecDecl* decl) {
    // Make sure that we correctly calculate the SourceRange of a declaration that has a non-braced
    // extern "C"/"C++".
    if (!decl->hasBraces()) {
      DeclaratorDecl* child = nullptr;
      for (auto child_decl : decl->decls()) {
        if (child != nullptr) {
          errx(1, "LinkageSpecDecl has multiple children");
        }

        if (DeclaratorDecl* declarator_decl = dyn_cast<DeclaratorDecl>(child_decl)) {
          child = declarator_decl;
        } else {
          errx(1, "child of LinkageSpecDecl is not a DeclaratorDecl");
        }
      }

      return VisitDeclaratorDecl(child, decl->getSourceRange());
    }

    for (auto child : decl->decls()) {
      if (!TraverseDecl(child)) {
        return false;
      }
    }
    return true;
  }
};

bool DeclarationAvailability::merge(const DeclarationAvailability& other) {
#define check_avail(expr) error |= (!this->expr.empty() && this->expr != other.expr);
  bool error = false;

  if (!other.global_availability.empty()) {
    check_avail(global_availability);
    this->global_availability = other.global_availability;
  }

  for (Arch arch : supported_archs) {
    if (!other.arch_availability[arch].empty()) {
      check_avail(arch_availability[arch]);
      this->arch_availability[arch] = other.arch_availability[arch];
    }
  }
#undef check_avail

  return !error;
}

bool Declaration::calculateAvailability(DeclarationAvailability* output) const {
  DeclarationAvailability avail;
  for (const auto& it : this->availability) {
    if (!avail.merge(it.second)) {
      return false;
    }
  }
  *output = avail;
  return true;
}

bool Symbol::calculateAvailability(DeclarationAvailability* output) const {
  DeclarationAvailability avail;
  for (const auto& it : this->declarations) {
    // Don't merge availability for inline functions (because they shouldn't have any).
    if (it.second.is_definition) {
      continue;
    }

    DeclarationAvailability decl_availability;
    if (!it.second.calculateAvailability(&decl_availability)) {
      return false;
      abort();
    }

    if (!avail.merge(decl_availability)) {
      return false;
    }
  }
  *output = avail;
  return true;
}

bool Symbol::hasDeclaration(const CompilationType& type) const {
  for (const auto& decl_it : this->declarations) {
    for (const auto& compilation_it : decl_it.second.availability) {
      if (compilation_it.first == type) {
        return true;
      }
    }
  }
  return false;
}

void HeaderDatabase::parseAST(CompilationType type, ASTContext& ctx) {
  std::unique_lock<std::mutex> lock(this->mutex);
  Visitor visitor(*this, type, ctx);
  visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}

std::string to_string(const AvailabilityValues& av) {
  std::stringstream ss;

  if (av.future) {
    ss << "future, ";
  }

  if (av.introduced != 0) {
    ss << "introduced = " << av.introduced << ", ";
  }

  if (av.deprecated != 0) {
    ss << "deprecated = " << av.deprecated << ", ";
  }

  if (av.obsoleted != 0) {
    ss << "obsoleted = " << av.obsoleted << ", ";
  }

  std::string result = ss.str();
  if (!result.empty()) {
    result = result.substr(0, result.length() - 2);
  }
  return result;
}

std::string to_string(const DeclarationType& type) {
  switch (type) {
    case DeclarationType::function:
      return "function";
    case DeclarationType::variable:
      return "variable";
    case DeclarationType::inconsistent:
      return "inconsistent";
  }
  abort();
}

std::string to_string(const DeclarationAvailability& decl_av) {
  std::stringstream ss;
  if (!decl_av.global_availability.empty()) {
    ss << to_string(decl_av.global_availability) << ", ";
  }

  for (const auto& it : decl_av.arch_availability) {
    if (!it.second.empty()) {
      ss << to_string(it.first) << ": " << to_string(it.second) << ", ";
    }
  }

  std::string result = ss.str();
  if (result.size() == 0) {
    return "no availability";
  }

  return result.substr(0, result.length() - 2);
}

std::string to_string(const Location& loc) {
  std::stringstream ss;
  ss << loc.filename << ":" << loc.start.line << ":" << loc.start.column;
  return ss.str();
}
