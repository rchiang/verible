//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#ifndef MODULE_VISITOR_H
#define MODULE_VISITOR_H

#include <string>

#include "verible/common/text/concrete-syntax-leaf.h"
#include "verible/common/text/concrete-syntax-tree.h"
#include "verible/common/text/visitors.h"

namespace va2 {

/**
 * Class used to track state for finding all module instantiations
 */
enum class TrackingState {
  beginState,
  // parentIsDescriptionList,
  parentIsModuleDeclaration, 
  parentIsModuleHeader,
  foundModuleStart,
  foundModuleName,
  parentIsDataDeclaration,
  parentIsInstantiationBase,
  parentIsInstantiationType,
  parentIsDataType,
  parentIsUnqualifiedId,
  // foundEndmodule,
  unknownState
};

/**
 * Visitor pattern class for iterating through the concrete syntax tree of a single
 * file and finding all modules and instantiations.
 *
 * Using the "verible-verilog-syntax --printtree" command to generate a full parse tree,
 * it should be relatively easy to find every module and every instantiation within each
 * module.
 *
 *   Node @0 (tag: kDescriptionList) {
 *     Node @0 (tag: kModuleDeclaration) {
 *       Node @0 (tag: kModuleHeader) { 
 *         Leaf @0 (#"module" @375-381: "module")
 *         Leaf @2 (#SymbolIdentifier @383-393: "high_module")
 *         Node @5 (tag: kDataDeclaration) {
 *           Node @1 (tag: kInstantiationBase) {
 *             Node @0 (tag: kInstantiationType) {
 *               Node @0 (tag: kDataType) {
 *                 Node @1 (tag: kUnqualifiedId) {
 *                   Leaf @0 (#SymbolIdentifier @592-611: "low_module")
 *       Leaf @2 (#"endmodule" @636-645: "endmodule")
 *     Node @1 (tag: kModuleDeclaration) {
 *       Node @0 (tag: kModuleHeader) { 
 *         Leaf @0 (#"module" @648-654: "module")
 *         Leaf @2 (#SymbolIdentifier @655-674: "low_module")
 *       Leaf @2 (#"endmodule" @698-707: "endmodule")
 *     }
 *   }
 *
 * If this parser were "smarter", it should ensure that the inputs/outputs are matched up
 * correctly in addition to just matching the module and/or filename.
 *
 * Using state member variable to "remember" where we are while traversing the
 * syntax tree.
 */
class ModuleVisitor : public verible::TreeVisitorRecursive {
  public:
    explicit ModuleVisitor() :
      state(TrackingState::beginState)
      {}

    void Visit(const verible::SyntaxTreeLeaf& leaf) final;
    void Visit(const verible::SyntaxTreeNode& node) final;

    // Used to reset this class after crawling through the parse tree of one file
    void clearState();
    void clearCurrentModule();
    // Get needed information after crawling through the parse tree
    const std::vector<std::string>& getDefinedModules();
    const std::vector<std::string> getInstancesFound(const std::string& moduleName);

    // Debugging
    void printMap();

  protected:
    TrackingState state;
    std::string currentModule;
    std::vector<std::string> modules;
    std::unordered_map<std::string,std::vector<std::string>> moduleInstancesMap;

  private:
    // Used to record anything found
    void foundModuleName(const std::string& moduleName);
    void foundModuleInstantiation(const std::string& instance);
};

} // namespace

#endif
