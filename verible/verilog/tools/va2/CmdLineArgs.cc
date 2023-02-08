//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include "CmdLineArgs.h"

using va2::CmdLineArgs;

CmdLineArgs::CmdLineArgs() {
}

CmdLineArgs::~CmdLineArgs() {
}

bool CmdLineArgs::parseArgs(int argc, char** argv) {
  int optChar;
  int optIndex;

  static struct option longOptions[] = {
    // Module search flags
    { "top", required_argument, 0, 't' },
    { "verilog_dir", required_argument, 0, 'y' },
    { "include_dir", required_argument, 0, 'I' },
    { "library_dir", required_argument, 0, 'v' },
    { "ext_list", required_argument, 0, 'x' },
    { "library_dir", required_argument, 0, 'v' },
    // Module ignore flags
    { "ignore_re", required_argument, 0, 'r' },
    { NULL,  0,                 0, 0 }
  };

  while((optChar = getopt_long(argc, argv, "t:", longOptions, &optIndex)) != -1) {
    switch (optChar) {
      // Module search flags
      case static_cast<int>('t'):
        topModules.push_back(optarg);
        break;
      case static_cast<int>('y'):
        verilogDirs.push_back(optarg);
        break;
      case static_cast<int>('I'):
        includeDirs.push_back(optarg);
        break;
      case static_cast<int>('x'):
        extList.push_back(optarg);
        break;
      case static_cast<int>('v'):
        libraryFiles.push_back(optarg);
        break;
      // Module ignore flags
      case static_cast<int>('r'):
        ignoreRegex.push_back(optarg);
        break;
    }
  }

  return true;
}

const std::vector<std::string>& CmdLineArgs::getTopModules() {
  return topModules;
}

const std::vector<std::string>& CmdLineArgs::getVerilogDirs() {
  return verilogDirs;
}

const std::vector<std::string>& CmdLineArgs::getIncludeDirs() {
  return includeDirs;
}

const std::vector<std::string>& CmdLineArgs::getLibraryFiles() {
  return libraryFiles;
}

const std::vector<std::string>& CmdLineArgs::getExtList() {
  return extList;
}

const std::vector<std::string>& CmdLineArgs::getIgnoreRegex() {
  return ignoreRegex;
}
