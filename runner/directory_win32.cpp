#include "directory.h"
#include <filesystem>
#include <algorithm>
#include <cstdlib>

static bool hasExecutableExtension(const std::filesystem::path& p)
{
  auto ext = p.extension().string();
  if (ext.size() != 4) return false;
  return (ext[0] == '.') &&
         (ext[1] == 'e' || ext[1] == 'E') &&
         (ext[2] == 'x' || ext[2] == 'X') &&
         (ext[3] == 'e' || ext[3] == 'E');
}

static std::vector<std::string> filterBy(const std::string& filter, const std::vector<std::string>& entries)
{
  std::vector<std::string> result;
  std::copy_if(
    entries.begin(), entries.end(),
    std::back_inserter(result),
    [&filter](const std::string& path) {
      return path.find(filter) != std::string::npos;
    }
  );
  return result;
}

std::vector<std::string> Directory::findExecutableFiles(
  const std::string& path,
  const std::string& filter
) {
  std::vector<std::string> result;

  if (!std::filesystem::is_directory(path)) return result;

  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    if (entry.is_regular_file() && hasExecutableExtension(entry.path()))
    {
      result.push_back(entry.path().string());
    }
    else if (entry.is_directory())
    {
      const auto sub_files = Directory::findExecutableFiles(entry.path().string(), filter);
      result.insert(result.end(), sub_files.begin(), sub_files.end());
    }
  }

  return filterBy(filter, result);
}

std::string Directory::homeDirectory()
{
  auto home = std::getenv("USERPROFILE");
  if (!home) home = std::getenv("HOME");
  return home ? std::string(home) : std::string();
}
