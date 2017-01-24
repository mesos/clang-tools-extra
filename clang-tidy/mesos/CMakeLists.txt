set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyMesosModule
  FlagsInheritanceCheck.cpp
  MesosTidyModule.cpp
  NamespaceCommentCheck.cpp
  ThisCaptureCheck.cpp
  UnusedReturnValueCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTooling
  )
