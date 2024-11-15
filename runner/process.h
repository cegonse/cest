#include <functional>
#include <string>

namespace Process
{
  void runExecutable(const std::string& path, std::function<void(std::string)> on_output);
}
