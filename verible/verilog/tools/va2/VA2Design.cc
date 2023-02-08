//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include <iostream>
#include <regex>

#include "absl/strings/string_view.h"
#include "verible/verilog/analysis/symbol-table.h"

#include "ModuleVisitor.h"
#include "VA2Design.h"

using va2::FileSearch;
using va2::ModuleVisitor;
using va2::VA2Design;
using va2::VA2TreeNode;

VA2Design::VA2Design(const FileSearch _fSearch,
                     const std::vector<std::string>& _includeDirs,
                     const std::vector<std::string>& _ignoreRegex) :
  fSearch(_fSearch),
  includeDirs(_includeDirs),
  debug(false)
{
  for (auto& re : _ignoreRegex) {
    ignoreRegex.push_back(std::regex(re));
  }
}

VA2Design::~VA2Design() {
}

/**
 * Save the list of top modules to begin parsing.
 */
void VA2Design::setTopModules(const std::vector<std::string>& _topModules) {
  for (auto &top : _topModules) {
    std::cout << "Looking for " << top << std::endl;
    auto topPath = fSearch.findFile(top);
    if (topPath != "") {
      std::cout << " Found TOP " << topPath << std::endl;
      auto topNode = std::make_shared<VA2TreeNode>(top, topPath, true);
      topModules.push_back(topNode);
      foundModules.insert(std::make_pair(top,topNode));
      continue;
    }
  }
}

/**
 * Takes topModules and recursively searches for module instantiations and looks for the
 * matching filename in the Verilog search path. Continues doing so until no more modules
 * can be resolved.
 *
 * Notes on recursively searching:
 * - Library cells have priority over Verilog file search
 * - Preloaded Verilog files have priority over Verilog file search
 * - Two modules with the same filename will rely on search path order
 */
void VA2Design::findAllSubmodules() {
  std::vector<std::string> modulesToFind;
  int indent = 0;
  for (auto& top : topModules) {
    findInstantiatedModules(indent, top->getName());
  }
}

/**
 */
void VA2Design::findInstantiatedModules(int indent, const std::string parent) {
  // Sanity Checks
  auto parentEntry = foundModules.find(parent);
  if (parentEntry == foundModules.end()) {
    std::cerr << std::string(indent, ' ') << "Unable to find module " << parent << std::endl;
    return;
  }
  std::cout << std::string(indent, ' ') << "Processing module " << parent << std::endl;
  auto parentNode = parentEntry->second;
  // Begin using Verible code to parse Verilog
  verilog::VerilogProject project("/", includeDirs);
  auto currentPath = parentNode->getSourceFile().string();
  auto statusOrFile = project.OpenTranslationUnit(currentPath);
  verilog::VerilogSourceFile *sourceFile = *statusOrFile;
  if (sourceFile->Status().ok()) {
    verilog::SymbolTable symbols(&project);
    std::vector<absl::Status> diagnostics1;
    symbols.Build(&diagnostics1);
    // - (Verible) Following code from the --printtokens flag in verilog/tools/syntax/verilog_syntax.cc
    const auto& textStructure = sourceFile->GetTextStructure();
    const auto& syntaxTree = textStructure->SyntaxTree();
    ModuleVisitor mVisitor;
    syntaxTree->Accept(&mVisitor);
    //std::cout << "=== DEBUG ===" << std::endl;
    //mVisitor.printMap();
    //std::cout << "=== DEBUG ===" << std::endl;
    // Add all modules from the same file before recursing
    auto definedModules = mVisitor.getDefinedModules();
    for (auto newModule : definedModules) {
      if (!moduleExists(newModule)) {
        if (debug) {
          std::cerr << std::string(indent, ' ') << "  Found module definition " << newModule << " while processing " << parent << std::endl;
        }
        std::shared_ptr<VA2TreeNode> newNode = std::make_shared<VA2TreeNode>(newModule,currentPath,false);
        foundModules.insert(std::make_pair(newModule,newNode));
      }
    }
    // Recursively process instanced modules
    auto instantiatedModules = mVisitor.getInstancesFound(parent);
    ++indent;
    for (auto newModule : instantiatedModules) {
      // Case 1: Ignore modules matching ignoreRegex list
      if (ignoreModule(newModule)) {
        continue;
      }
      // Case 2: Ignore if this is already a submodule of parent
      else if (parentNode->submoduleExists(newModule)) {
        continue;
      }
      // Case 3: Use already existing module (frequently from being found while parsing the same file)
      else if (moduleExists(newModule)) {
        std::shared_ptr<VA2TreeNode> existingNode = getModule(newModule);
        parentNode->addNewSubmodule(existingNode);
        continue;
      }
      // Case 4: Search for new module in searchPath
      else {
        std::filesystem::path newPath = fSearch.findFile(newModule);
        if (newPath == "") {
          std::cerr << std::string(indent, ' ') << "  Unable to find file for module " << newModule << std::endl;
          continue;
        } else {
          std::shared_ptr<VA2TreeNode> newNode = std::make_shared<VA2TreeNode>(newModule,newPath,false);
          foundModules.insert(std::make_pair(newModule,newNode));
          parentNode->addNewSubmodule(newNode);
          findInstantiatedModules(indent, newModule);
        }
      }
    }
    --indent;
  } else {
    std::cerr << std::string(indent, ' ') << "  Error parsing " << parentNode->getSourceFile().string() << std::endl;
  }
}

bool VA2Design::moduleExists(const std::string& moduleName) {
  return foundModules.find(moduleName) != foundModules.end();
}

bool VA2Design::ignoreModule(const std::string& ignoreName) {
  for (auto& re: ignoreRegex) {
    if (std::regex_match(ignoreName, re)) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<VA2TreeNode> VA2Design::getModule(const std::string& moduleName) {
  auto moduleEntry = foundModules.find(moduleName);
  if (moduleEntry != foundModules.end()) {
    return moduleEntry->second;
  }
  return nullptr;
}

const std::string VA2Design::getModuleFile(const std::string& moduleName) {
  auto nodePtr = getModule(moduleName);
  if (nodePtr != nullptr) {
    return nodePtr->getSourceFile().stem();
  }
  return "";
}

void VA2Design::setDebug(bool value) {
  debug = value;
}

void VA2Design::printTree() {
  int indent = 0;
  for (auto& t : topModules) {
    printTreeNode(indent, *t);
  }
}

void VA2Design::printTreeNode(int indent, VA2TreeNode node) {
  std::cerr << std::string(indent, ' ') << node.getName() << std::endl;
  ++indent;
  for (auto& m : node.getSubmodules()) {
    printTreeNode(indent, *m);
  }
  --indent;
}
