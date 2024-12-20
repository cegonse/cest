#include <functional>
#include <string>

namespace Process
{
  int runExecutable(
    const std::string& path,
    std::function<void(std::string)> on_output,
    const std::vector<std::string>& args,
    int64_t& elapsed_time
  );

  bool killedBySignal(int status);
}
