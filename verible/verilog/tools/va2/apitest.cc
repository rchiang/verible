//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include <iostream>

#include "verible/verilog/analysis/symbol-table.h"
#include "verible/verilog/analysis/verilog-project.h"

int main(int argc, char**argv) {
  std::vector<std::string> includeDirs;
  verilog::VerilogProject project("/", includeDirs);
  auto statusOrFile = project.OpenTranslationUnit(argv[1]);
  // Otherwise, search for the file and add all the submodules
  verilog::VerilogSourceFile *sourceFile = *statusOrFile;
  if (sourceFile->Status().ok()) {
    verilog::SymbolTable symbols(&project);
    std::vector<absl::Status> diagnostics1;
    symbols.Build(&diagnostics1);
    // - (Verible) Following code from the --printtokens flag in verilog/tools/syntax/verilog_syntax.cc
    const auto& textStructure = sourceFile->GetTextStructure();
    const auto& syntaxTree = textStructure->SyntaxTree();
  }
}
