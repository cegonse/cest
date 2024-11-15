#include "directory.h"
#include <filesystem>
#include <algorithm>

constexpr bool hasPerms(std::filesystem::perms target, std::filesystem::perms other)
{
  return std::filesystem::perms::none != (other & target);
}

constexpr bool isExecutable(std::filesystem::perms target)
{
  return hasPerms(std::filesystem::perms::owner_exec, target) ||
         hasPerms(std::filesystem::perms::group_exec, target) ||
         hasPerms(std::filesystem::perms::others_exec, target);
}

static std::vector<std::string> filterBy(const std::string& filter, const std::vector<std::string>& entries)
{
  std::vector<std::string> result;

  std::copy_if(
    entries.begin(),
    entries.end(),
    std::back_inserter(result),
    [&filter](std::string path) {
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
    const auto permissions = entry.status().permissions();

    if (entry.is_regular_file() && isExecutable(permissions))
    {
      result.push_back(entry.path());
    }
    else if (entry.is_directory())
    {
      const auto sub_files = Directory::findExecutableFiles(entry.path(), filter);
      result.insert(result.end(), sub_files.begin(), sub_files.end());
    }
  }

  return filterBy(filter, result);
}

std::string Directory::cwd()
{
  return std::filesystem::current_path();
}
