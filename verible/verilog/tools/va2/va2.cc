//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include <iostream>

#include "CmdLineArgs.h"
#include "FileSearch.h"
#include "VA2Design.h"

using va2::CmdLineArgs;
using va2::FileSearch;
using va2::VA2Design;

int main(int argc, char** argv) {
  // Parse command line arguments
  CmdLineArgs vaArgs;
  vaArgs.parseArgs(argc, argv);

  // Crawl through Verilog design and build the internal representation
  std::string topPath;
  FileSearch fSearch(vaArgs.getVerilogDirs(), vaArgs.getExtList());
  bool pass = fSearch.sanityCheckSearchPath();
  if (!pass) {
    std::cerr << "Exiting..." << std::endl;
    exit(1);
  }
  VA2Design va2Design(fSearch, vaArgs.getIncludeDirs(), vaArgs.getIgnoreRegex());
  va2Design.setTopModules(vaArgs.getTopModules());
  va2Design.findAllSubmodules();
  //std::cout << "===== DEBUG =====" << std::endl;
  //va2Design.printTree();
  //std::cout << "===== DEBUG =====" << std::endl;

  return 0;
}
