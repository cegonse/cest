#pragma once
#include <sstream>
#include <functional>
#include <vector>

namespace Directory
{
  void findExecutableFiles_mockFiles(std::vector<std::string> files);
  bool findExecutableFiles_hasBeenCalledWith(const std::string& path, const std::string& filter);
}

namespace Process
{
  void runExecutable_mockOutput(const std::string& output);
  bool runExecutable_hasBeenCalledWith(const std::string& path);
}
