//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#ifndef VA2_SOURCE_FILE_H
#define VA2_SOURCE_FILE_H

#include <filesystem>
#include <string>
#include <vector>

namespace va2 {

class VA2SourceFile {
  public:
    VA2SourceFile(std::filesystem::path _path);
    ~VA2SourceFile();

    // Use this to build trees
    void addModule(const std::string& name);
    const std::filesystem::path& getPath();

  private:
    std::filesystem::path path;
    std::vector<std::string> modulesDefined;
};

} // namespace
#endif
