//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include <iostream>

#include "verible/verilog/CST/verilog-nonterminals.h"
#include "ModuleVisitor.h"

using va2::TrackingState;
using va2::ModuleVisitor;

void ModuleVisitor::Visit(const verible::SyntaxTreeLeaf& leaf) {
  // Found module definition within a Verilog file
  if (state == TrackingState::parentIsModuleHeader) {
    state = TrackingState::foundModuleStart;
  } else if (state == TrackingState::foundModuleStart) {
    std::string foundModule{leaf.get().text()};
    foundModuleName(foundModule);
    state = TrackingState::foundModuleName;
  } else if (state == TrackingState::foundModuleName) {
    if (leaf.get().text() == "endmodule") {
      //std::cout << "Found endmodule" << std::endl;
      state = TrackingState::beginState;
      clearCurrentModule();
    }
  // Found instantiation within a module
  } else if (state == TrackingState::parentIsUnqualifiedId) {
    std::string foundInstance{leaf.get().text()};
    foundModuleInstantiation(foundInstance);
    state = TrackingState::foundModuleName;
  }
}

void ModuleVisitor::Visit(const verible::SyntaxTreeNode& node) {

  // Looking for module/<name>/endmodule
  if (node.MatchesTag(verilog::NodeEnum::kModuleDeclaration)) {
      state = TrackingState::parentIsModuleDeclaration;
  } else if (state == TrackingState::parentIsModuleDeclaration) {
    if (node.MatchesTag(verilog::NodeEnum::kModuleHeader)) {
      state = TrackingState::parentIsModuleHeader;
    }
  // Inside a module. Looking for any instantiations.
  } else if (state == TrackingState::foundModuleName) {
    if (node.MatchesTag(verilog::NodeEnum::kDataDeclaration)) {
      //std::cout << "Found kDataDeclaration" << std::endl;
      state = TrackingState::parentIsDataDeclaration;
    } else {
      state = TrackingState::foundModuleName;
    }
  } else if (state == TrackingState::parentIsDataDeclaration) {
    if (node.MatchesTag(verilog::NodeEnum::kInstantiationBase)) {
      //std::cout << "Found kInstantiationBase" << std::endl;
      state = TrackingState::parentIsInstantiationBase;
    } else {
      state = TrackingState::foundModuleName;
    }
  } else if (state == TrackingState::parentIsInstantiationBase) {
    if (node.MatchesTag(verilog::NodeEnum::kInstantiationType)) {
      //std::cout << "Found kInstantiationType" << std::endl;
      state = TrackingState::parentIsInstantiationType;
    } else {
      state = TrackingState::foundModuleName;
    }
  } else if (state == TrackingState::parentIsInstantiationType) {
    if (node.MatchesTag(verilog::NodeEnum::kDataType)) {
      //std::cout << "Found kDatatype" << std::endl;
      state = TrackingState::parentIsDataType;
    } else {
      state = TrackingState::foundModuleName;
    }
  } else if (state == TrackingState::parentIsDataType) {
    if (node.MatchesTag(verilog::NodeEnum::kUnqualifiedId)) {
      //std::cout << "Found kUnqualifiedId" << std::endl;
      state = TrackingState::parentIsUnqualifiedId;
    } else {
      state = TrackingState::foundModuleName;
    }
  }
}

void ModuleVisitor::clearState() {
  state = TrackingState::beginState;
  modules.clear();
  moduleInstancesMap.clear();
  clearCurrentModule();
}

void ModuleVisitor::clearCurrentModule() {
  currentModule = "";
}

void ModuleVisitor::foundModuleName(const std::string& moduleName) {
  currentModule = moduleName;
  if (moduleInstancesMap.find(moduleName) == moduleInstancesMap.end()) {
    modules.push_back(moduleName);
    moduleInstancesMap.insert(std::make_pair(moduleName,std::vector<std::string>()));
  }
}

void ModuleVisitor::foundModuleInstantiation(const std::string& instance) {
  auto entry = moduleInstancesMap.find(currentModule);
  if (moduleInstancesMap.find(currentModule) != moduleInstancesMap.end()) {
    (entry->second).push_back(instance);
  }
}

const std::vector<std::string>& ModuleVisitor::getDefinedModules() {
  return modules;
}

const std::vector<std::string> ModuleVisitor::getInstancesFound(const std::string& moduleName) {
  auto check = moduleInstancesMap.find(moduleName);
  if (check != moduleInstancesMap.end()) {
    return check->second;
  }
  return std::vector<std::string>();
}

void ModuleVisitor::printMap() {
  for (auto entry: moduleInstancesMap) {
    auto k = entry.first;
    std::cout << k << std::endl;
    auto v = entry.second;
    for (auto instance : v) {
      std::cout << "  " << instance << std::endl;
    }
  }
}
