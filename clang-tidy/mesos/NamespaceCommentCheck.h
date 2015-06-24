#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MESOS_NAMESPACECOMMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MESOS_NAMESPACECOMMENTCHECK_H

#include "../ClangTidy.h"
#include "llvm/Support/Regex.h"

namespace clang {
namespace tidy {
namespace mesos {

/// Checks that long namespaces have a closing comment.
///
/// http://llvm.org/docs/CodingStandards.html#namespace-indentation
///
/// http://google-styleguide.googlecode.com/svn/trunk/cppguide.html#Namespaces
class NamespaceCommentCheck : public ClangTidyCheck {
public:
  NamespaceCommentCheck(StringRef Name, ClangTidyContext *Context);
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  llvm::Regex NamespaceCommentPattern;
};

} // namespace mesos
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MESOS_NAMESPACECOMMENTCHECK_H
