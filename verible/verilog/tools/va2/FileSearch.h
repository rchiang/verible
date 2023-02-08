//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#ifndef FILE_SEARCH_H
#define FILE_SEARCH_H

#include <string>
#include <vector>

namespace va2 {

/**
 * Class for finding Verilog files via a search path and matching a list of valid extensions.
 * Verible seems to expect either a file containing all the Verilog files or all the Verilog
 * files for a design in a single directory. This allows for the classic -I/-v/-y Verilog
 * search flags in mainstream CAD tools.
 */
class FileSearch {
  public:
    FileSearch(const std::vector<std::string>& _searchPath,
               const std::vector<std::string>& _extList);
    ~FileSearch();

    std::string findFile(const std::string& filename);
    void setDebug(const bool& val);
    bool sanityCheckSearchPath();

  private:
    std::vector<std::string> searchPath;
    std::vector<std::string> extList;
    bool debug;
};

} // namespace

#endif
