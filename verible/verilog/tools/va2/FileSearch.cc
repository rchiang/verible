//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include <iostream>
#include <filesystem>

#include "FileSearch.h"

using va2::FileSearch;

FileSearch::FileSearch(const std::vector<std::string>& _searchPath,
                       const std::vector<std::string>& _extList) :
  searchPath(_searchPath),
  extList(_extList),
  debug(false)
{
}

FileSearch::~FileSearch() {
}

// Brute force file and extension search
std::string FileSearch::findFile(const std::string& filename) {
  if (debug) {
    std::cout << " Begin search for filename " << filename << std::endl;
  }
  for (const auto &dir : searchPath) {
    if (debug) {
      std::cout << "  Listing files in directory " << dir << std::endl;
    }
    for (auto &p : std::filesystem::directory_iterator(dir)) {
      if (debug) {
        std::cout << "   Comparing file " << p << std::endl;
      }
      for (const auto &ext : extList) {
        if (p.path().stem() == filename && p.path().extension() == ext) {
          if (debug) {
            std::cout << "    MATCHED filename to file " << p << std::endl;
          }
          return p.path().string();
        }
      }
    }
  }
  if (debug) {
    std::cout << " NO MATCH filename " << filename << std::endl;
  }
  return "";
}

void FileSearch::setDebug(const bool& val) {
  debug = val;
}

bool FileSearch::sanityCheckSearchPath() {
  bool retVal = true;
  for (const auto &dir : searchPath) {
    if (!std::filesystem::exists(dir)) {
      std::cerr << "Invalid directory " << dir << std::endl;
      retVal = false;
    }
  }
  return retVal;
}
