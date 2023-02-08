//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#ifndef VA2_TREE_H
#define VA2_TREE_H

#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "verible/verilog/analysis/verilog-project.h"

#include "FileSearch.h"
#include "VA2TreeNode.h"

namespace va2 {

/**
 * Encapsulates all the design information for one or more top-level modules and its
 * submodules.
 * 
 *   Design Config --------> VA2Design --------> YAML Design Information
 *                               ^
 *                               |
 *                 CAD Files (Verilog, Library, etc)
 *
 * Given a config, the VA2Design will attempt to find all CAD file and attempt to infer
 * as much information about the design and put that output into YAML. The resulting YAML
 * will have sufficient information about the design that can subsequently be used in all
 * sorts of CAD tool flows.
 */
class VA2Design {
  public:
    VA2Design(FileSearch _fSearch,
              const std::vector<std::string>& _includeDirs,
              const std::vector<std::string>& _ignoreRegex);
    ~VA2Design();

    // Step 1: Pre-load any "known/special" Verilog files
    // TODO

    // Step 2: Initialize with top modules
    void setTopModules(const std::vector<std::string>& _topModules);

    // Step 3: Recursively find all submodules and create VA2TreeNode hierarchy
    void findAllSubmodules();
    void findInstantiatedModules(int indent, const std::string parent);
    // Step 3a: Exception. Any given file can have more than one module defined inside. Such
    //          modules need to be recorded in order to implement proper recursive module search.

    // Output methods
    // TODO

    // Query methods (useful in libraries and internal to class)
    bool moduleExists(const std::string& moduleName);
    bool ignoreModule(const std::string& moduleName);
    std::shared_ptr<VA2TreeNode> getModule(const std::string& moduleName);
    const std::string getModuleFile(const std::string& moduleName);

    // Debugging methods
    void setDebug(bool value);
    void printTree();

  private:
    FileSearch fSearch;
    std::vector<std::string> includeDirs;
    std::vector<std::regex> ignoreRegex;

    std::vector<std::shared_ptr<VA2TreeNode>> topModules;
    std::unordered_map<std::string,std::shared_ptr<VA2TreeNode>> foundModules;

    bool debug;

    void printTreeNode(int indent, VA2TreeNode node);
};

} // namespace
#endif
