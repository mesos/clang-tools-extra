//===--- FlagsCheck.cpp - clang-tidy---------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "FlagsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

void FlagsCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxRecordDecl(isDerivedFrom("FlagsBase")).bind("flags"),
                     this);
}

namespace {
bool isDerivedFromFlagsBase(const CXXRecordDecl &decl) {
  return llvm::any_of(decl.bases(), [](const CXXBaseSpecifier &base) {
    const auto *decl = base.getType()->getAsCXXRecordDecl();
    return decl->getName() == "FlagsBase" || isDerivedFromFlagsBase(*decl);
  });
}
}

void FlagsCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *derivedDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("flags");

  // We explicitly iterate the bases here so we can emit diagnostic pointing to
  // the exact inheritance introducing the issue.
  for (auto &&base : derivedDecl->bases()) {
    const auto *baseDecl = base.getType()->getAsCXXRecordDecl();

    // First check if this inheritance is from 'FlagsBase' or any of its
    // descendants.
    if (baseDecl->getName() != "FlagsBase" &&
        !isDerivedFromFlagsBase(*baseDecl)) {
      continue;
    }

    // If the inheritance was 'virtual' there's nothing we need to do here.
    if (base.isVirtual()) {
      continue;
    }

    diag(base.getLocStart(), "'%0' does not inherit virtually from '%1'%2")
        << derivedDecl->getName() << baseDecl->getName()
        << FixItHint::CreateInsertion(base.getLocStart(), "virtual ")
        << (baseDecl->getName() != "FlagsBase"
                ? " which inherits from 'FlagsBase'"
                : "");

    // Break if we found a match so any FixIts are in the correct location.
    // Rechecking after fixing the first issue might also find new issues as the
    // hierarchy is changed.
    break;
  }
}
} // namespace mesos
} // namespace tidy
} // namespace clang
