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

#include "DeclarationDatabase.h"

#include <iostream>
#include <map>
#include <set>
#include <string>

#include <clang/AST/AST.h>
#include <clang/AST/Attr.h>
#include <clang/AST/Mangle.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/ASTUnit.h>
#include <llvm/Support/raw_ostream.h>

using namespace clang;

class Visitor : public RecursiveASTVisitor<Visitor> {
  HeaderDatabase& database;
  SourceManager& src_manager;
  std::unique_ptr<MangleContext> mangler;

 public:
  Visitor(HeaderDatabase& database, ASTContext& ctx)
      : database(database), src_manager(ctx.getSourceManager()) {
    mangler.reset(ItaniumMangleContext::create(ctx, ctx.getDiagnostics()));
  }

  std::string getDeclName(NamedDecl* decl) {
    if (auto var_decl = dyn_cast<VarDecl>(decl)) {
      if (!var_decl->isFileVarDecl()) {
        return "<local var>";
      }
    }

    if (mangler->shouldMangleDeclName(decl)) {
      std::string mangled;
      llvm::raw_string_ostream ss(mangled);
      mangler->mangleName(decl, ss);
      return mangled;
    }

    auto identifier = decl->getIdentifier();
    if (!identifier) {
      return "<error>";
    }
    return identifier->getName();
  }

  bool VisitDecl(Decl* decl) {
    // Skip declarations inside of functions (function arguments, variable declarations inside of
    // inline functions, etc).
    if (decl->getParentFunctionOrMethod()) {
      return true;
    }

    auto named_decl = dyn_cast<NamedDecl>(decl);
    if (!named_decl) {
      return true;
    }

    DeclarationType declaration_type;
    std::string declaration_name = getDeclName(named_decl);
    bool is_extern = named_decl->getFormalLinkage() == ExternalLinkage;
    bool is_definition = false;

    if (auto function_decl = dyn_cast<FunctionDecl>(decl)) {
      declaration_type = DeclarationType::function;
      is_definition = function_decl->isThisDeclarationADefinition();
    } else if (auto var_decl = dyn_cast<VarDecl>(decl)) {
      if (!var_decl->isFileVarDecl()) {
        return true;
      }

      declaration_type = DeclarationType::variable;
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

    // Look for availability annotations.
    DeclarationAvailability availability;
    for (const AvailabilityAttr* attr : decl->specific_attrs<AvailabilityAttr>()) {
      if (attr->getPlatform()->getName() != "android") {
        fprintf(stderr, "skipping non-android platform %s\n",
                attr->getPlatform()->getName().str().c_str());
        continue;
      }
      if (attr->getIntroduced().getMajor() != 0) {
        availability.introduced = attr->getIntroduced().getMajor();
      }
      if (attr->getDeprecated().getMajor() != 0) {
        availability.deprecated = attr->getDeprecated().getMajor();
      }
      if (attr->getObsoleted().getMajor() != 0) {
        availability.obsoleted = attr->getObsoleted().getMajor();
      }
    }

    // Find or insert an entry for the declaration.
    auto declaration_it = database.declarations.find(declaration_name);
    if (declaration_it == database.declarations.end()) {
      Declaration declaration = {.name = declaration_name };
      bool inserted;
      std::tie(declaration_it, inserted) =
          database.declarations.insert({ declaration_name, declaration });
    }

    auto& declaration_locations = declaration_it->second.locations;
    auto presumed_loc = src_manager.getPresumedLoc(decl->getLocation());
    DeclarationLocation location = {
      .filename = presumed_loc.getFilename(),
      .line_number = presumed_loc.getLine(),
      .column = presumed_loc.getColumn(),
      .type = declaration_type,
      .is_extern = is_extern,
      .is_definition = is_definition,
      .availability = availability,
    };

    // It's fine if the location is already there, we'll get an iterator to the existing element.
    auto location_it = declaration_locations.begin();
    bool inserted = false;
    std::tie(location_it, inserted) = declaration_locations.insert(location);

    // If we didn't insert, check to see if the availability attributes are identical.
    if (!inserted) {
      if (location_it->availability != availability) {
        fprintf(stderr, "ERROR: availability attribute mismatch\n");
        decl->dump();
        abort();
      }
    }

    return true;
  }
};

void HeaderDatabase::parseAST(ASTUnit* ast) {
  ASTContext& ctx = ast->getASTContext();
  Visitor visitor(*this, ctx);
  visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}
