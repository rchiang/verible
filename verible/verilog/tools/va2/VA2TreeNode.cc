//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include "VA2TreeNode.h"

using va2::VA2TreeNode;

VA2TreeNode::VA2TreeNode(std::string _name, std::filesystem::path _sourceFile, bool _isTop) :
  name(_name),
  sourceFile(_sourceFile),
  isTop(_isTop) {
}

VA2TreeNode::~VA2TreeNode() {
}

void VA2TreeNode::addNewSubmodule(std::shared_ptr<VA2TreeNode> submodule) {
  submodules.push_back(submodule);
  seenModules.insert(submodule->getName());
}

const bool VA2TreeNode::submoduleExists(const std::string& submodule) {
  return seenModules.find(submodule) != seenModules.end();
}

const std::string& VA2TreeNode::getName() {
  return name;
}

const std::filesystem::path& VA2TreeNode::getSourceFile() {
  return sourceFile.getPath();
}

const bool& VA2TreeNode::getIsTop() {
  return isTop;
}

const std::vector<std::shared_ptr<VA2TreeNode>>& VA2TreeNode::getSubmodules() {
  return submodules;
}

