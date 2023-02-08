//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#ifndef VA2_TREE_NODE_H
#define VA2_TREE_NODE_H

#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

#include "VA2SourceFile.h"

namespace va2 {

class VA2TreeNode {
  public:
    VA2TreeNode(std::string _name, std::filesystem::path _sourceFile, bool _isTop);
    ~VA2TreeNode();

    // Use this to build trees
    void addNewSubmodule(std::shared_ptr<VA2TreeNode> submodule);
    const bool submoduleExists(const std::string& submodule);
   
    const std::string& getName();
    const std::filesystem::path& getSourceFile();
    const bool& getIsTop();
    const std::vector<std::shared_ptr<VA2TreeNode>>& getSubmodules();

  private:
    std::string name;
    VA2SourceFile sourceFile;
    bool isTop;

    std::vector<std::shared_ptr<VA2TreeNode>> submodules;
    std::unordered_set<std::string> seenModules;
};

} // namespace
#endif
