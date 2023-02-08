//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#ifndef CMD_LINE_ARGS_H
#define CMD_LINE_ARGS_H

#include <string>
#include <vector>

#include <getopt.h>

namespace va2 {

/**
 * Custom class for processing command line arguments.
 */
class CmdLineArgs {
  public:
    CmdLineArgs();
    ~CmdLineArgs();

    bool parseArgs(int argc, char** argv);
    const std::vector<std::string>& getTopModules();
    const std::vector<std::string>& getVerilogDirs();
    const std::vector<std::string>& getIncludeDirs();
    const std::vector<std::string>& getLibraryFiles();
    const std::vector<std::string>& getExtList();
    const std::vector<std::string>& getIgnoreRegex();

  private:
    std::vector<std::string> topModules;
    std::vector<std::string> verilogDirs;
    std::vector<std::string> includeDirs;
    std::vector<std::string> libraryFiles;
    std::vector<std::string> extList;
    std::vector<std::string> ignoreRegex;
};

} // namespace

#endif
