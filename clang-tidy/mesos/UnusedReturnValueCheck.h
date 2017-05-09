//===--- UnusedReturnValueCheck.h - clang-tidy-------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MESOS_UNUSED_RETURN_VALUE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MESOS_UNUSED_RETURN_VALUE_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

#include <string>

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace mesos {

/// Check if certain return values of certain types are ignored.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/mesos-unused-return-value.html
class UnusedReturnValueCheck : public ClangTidyCheck {
public:
  UnusedReturnValueCheck(StringRef Name, ClangTidyContext *Context);
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  const std::string RawTypes;
  SmallVector<StringRef, 5> Types;
};

} // namespace mesos
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MESOS_UNUSED_RETURN_VALUE_H
