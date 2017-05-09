//===--- UnusedReturnValueCheck.cpp - clang-tidy---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "UnusedReturnValueCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace mesos {

UnusedReturnValueCheck::UnusedReturnValueCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      RawTypes(Options.getLocalOrGlobal("Types", "Try,Result")) {
  StringRef(RawTypes).split(Types, ",", -1, false);
}

void UnusedReturnValueCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "Types", RawTypes);
}

void UnusedReturnValueCheck::registerMatchers(MatchFinder *Finder) {
  for (auto &&type : Types) {
    auto returnValue =
        callExpr(hasDeclaration(functionDecl(returns(hasDeclaration(
                                                 namedDecl(hasName(type)))))
                                    .bind("function_decl")))
            .bind("call");

    // For trivial types an ignored return value would be a `callExpr` directly
    // below a `compoundStmt`; otherwise an extra `cxxBindTemporaryExpr` and
    // `exprWithCleanups` are created.
    Finder->addMatcher(
        compoundStmt(eachOf(has(returnValue),
                            has(exprWithCleanups(
                                has(cxxBindTemporaryExpr(has(returnValue))))))),
        this);
  }
}

void UnusedReturnValueCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *call = Result.Nodes.getNodeAs<CallExpr>("call");
  const auto *function_decl =
      Result.Nodes.getNodeAs<FunctionDecl>("function_decl");

  diag(call->getLocStart(), "%0 returns a %1 which should not be ignored")
      << function_decl << function_decl->getReturnType();
}

} // namespace mesos
} // namespace tidy
} // namespace clang
