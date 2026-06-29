#include "directory.h"
#include <filesystem>
#include <fstream>
#include <sstream>

std::string Directory::cwd()
{
  return std::filesystem::current_path().string();
}

std::string Directory::absolute(const std::string& path)
{
  if (!path.empty() && path[0] == '~')
  {
    auto expanded = Directory::homeDirectory() + path.substr(1);
    return std::filesystem::absolute(expanded).string();
  }
  return std::filesystem::absolute(path).string();
}

std::string Directory::readTextFile(const std::string& path)
{
  std::stringstream buffer;
  std::ifstream file(path);
  if (!file) return "";

  buffer << file.rdbuf();
  file.close();

  return buffer.str();
}
