//===-  misracpp2008.h - A MISRA C++ 2008 rules checker--------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef MISRA_CPP_2008_H
#define MISRA_CPP_2008_H

#include <map>
#include <set>
#include <string>
#include <memory>
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/Support/Registry.h"

namespace clang {
class CompilerInstance;
class ASTConsumer;
class IdentifierTable;
class ASTContext;
}

namespace llvm {
class raw_ostream;
}

namespace misracpp2008 {

class RuleCheckerASTContext {
protected:
  clang::ASTContext *context;
  clang::DiagnosticsEngine *diagEngine;
  clang::DiagnosticsEngine::Level diagLevel;
  RuleCheckerASTContext();

public:
  virtual ~RuleCheckerASTContext() {}
  void setContext(clang::ASTContext &context);
  void setDiagLevel(clang::DiagnosticsEngine::Level diagLevel);
  virtual void doWork() = 0;
};

typedef llvm::Registry<RuleCheckerASTContext> RuleCheckerASTContextRegistry;

class Consumer : public clang::ASTConsumer {
private:
  typedef std::map<std::string, clang::DiagnosticsEngine::Level> DiagLevelMap;
  static std::set<std::string> &getEnabledCheckers();
  static DiagLevelMap &getDiagnosticLevels();

public:
  Consumer();
  virtual void HandleTranslationUnit(clang::ASTContext &ctx);
  static void dumpRegisteredCheckers(llvm::raw_ostream &OS);
  static void dumpRequestedCheckers(llvm::raw_ostream &OS);
  static bool enableChecker(const std::string &name,
                            clang::DiagnosticsEngine::Level diagLevel);
};

class Action : public clang::PluginASTAction {
protected:
  clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &CI,
                                        llvm::StringRef);
  bool ParseArgs(const clang::CompilerInstance &CI,
                 const std::vector<std::string> &args);
  void PrintHelp(llvm::raw_ostream &ros);
};
}

#endif
