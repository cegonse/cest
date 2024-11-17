#include <sstream>
#include <vector>
#include <functional>
#include "helpers.h"
#include "../../output.h"
#include "../../process.h"
#include "../../directory.h"

static std::vector<std::string> __mock_executable_files;
static bool __find_executable_files_called = false;
static std::string __find_executable_files_path;
static std::string __find_executable_files_filter;

static std::string __mock_run_executable_output;
static bool __run_executable_has_been_called = false;
static std::vector<std::string> __run_executable_path;

void Output::print(const std::string& text)
{
}
void Output::printSummary(
  int num_passed_suites,
  int num_failed_suites,
  int num_failed_tests,
  int num_passed_tests,
  int num_skipped_tests,
  float time
) {
}

void Directory::findExecutableFiles_mockFiles(std::vector<std::string> files)
{
  __mock_executable_files = files;
}
bool Directory::findExecutableFiles_hasBeenCalledWith(const std::string& path, const std::string& filter)
{
  return __find_executable_files_called && __find_executable_files_path == path && __find_executable_files_filter == filter;
}
std::vector<std::string> Directory::findExecutableFiles(const std::string& path, const std::string& filter)
{
  __find_executable_files_path = path;
  __find_executable_files_filter = filter;
  __find_executable_files_called = true;
  return __mock_executable_files;
}
std::string Directory::cwd()
{
  return "/cwd";
}
std::string Directory::readTextFile(const std::string& path)
{
  return "a|a|0|0|0";
}


void Process::runExecutable_mockOutput(const std::string& output)
{
  __mock_run_executable_output = output;
}
int Process::runExecutable(
  const std::string& path,
  std::function<void(std::string)> on_output,
  const std::vector<std::string>& args,
  int64_t& elapsed_time
) {
  __run_executable_has_been_called = true;
  __run_executable_path.push_back(path);
  on_output(__mock_run_executable_output);

  return 0;
}
bool Process::runExecutable_hasBeenCalledWith(const std::string& path)
{
  auto it = std::find(__run_executable_path.cbegin(), __run_executable_path.cend(), path);
  return __run_executable_has_been_called && it != __run_executable_path.end();
}
