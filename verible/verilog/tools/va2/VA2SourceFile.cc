//
// Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
//

#include "VA2SourceFile.h"

using va2::VA2SourceFile;

VA2SourceFile::VA2SourceFile(std::filesystem::path _path) :
  path(_path)
{
}

VA2SourceFile::~VA2SourceFile() {
}

void VA2SourceFile::addModule(const std::string& _name) {
  modulesDefined.push_back(_name);
}

const std::filesystem::path& VA2SourceFile::getPath() {
  return path;
}
