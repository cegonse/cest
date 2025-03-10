#pragma once
#include <vector>
#include <string>

namespace Directory
{
  std::vector<std::string> findExecutableFiles(const std::string& path, const std::string& filter);
  std::string cwd();
  std::string absolute(const std::string& path);
  std::string readTextFile(const std::string& path);
}
