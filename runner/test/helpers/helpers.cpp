#include <sstream>
#include <vector>
#include <functional>
#include "helpers.h"
#include "../../output.h"
#include "../../process.h"
#include "../../directory.h"
#include <iostream>

static int __output_print_summary_num_passed_suites;
static int __output_print_summary_num_failed_suites;
static int __output_print_summary_num_passed_tests;
static int __output_print_summary_num_failed_tests;
static int __output_print_summary_num_skipped_tests;
static float __output_print_summary_time;

static std::vector<std::string> __mock_executable_files;
static bool __find_executable_files_called = false;
static std::string __find_executable_files_path;
static std::string __find_executable_files_filter;

static std::string __mock_run_executable_output;
static bool __run_executable_has_been_called = false;
static std::vector<std::string> __run_executable_path;
static int64_t __run_executable_time = 1000000;

static std::vector<std::string> __read_text_file_mock_output;
static std::vector<std::string> __read_text_file_path;

void Output::print(const std::string& text)
{
}
void Output::printSummary(
  int num_passed_suites,
  int num_failed_suites,
  int num_passed_tests,
  int num_failed_tests,
  int num_skipped_tests,
  float time
) {
  __output_print_summary_num_passed_suites = num_passed_suites;
  __output_print_summary_num_failed_suites = num_failed_suites;
  __output_print_summary_num_failed_tests = num_failed_tests;
  __output_print_summary_num_passed_tests = num_passed_tests;
  __output_print_summary_num_skipped_tests = num_skipped_tests;
  __output_print_summary_time = time;
}
bool Output::printSummary_hasBeenCalledWith(
  int num_passed_suites,
  int num_failed_suites,
  int num_passed_tests,
  int num_failed_tests,
  int num_skipped_tests,
  float time
) {
  return
    __output_print_summary_num_passed_suites == num_passed_suites &&
    __output_print_summary_num_failed_suites == num_failed_suites &&
    __output_print_summary_num_failed_tests == num_failed_tests &&
    __output_print_summary_num_passed_tests == num_passed_tests &&
    __output_print_summary_num_skipped_tests == num_skipped_tests &&
    __output_print_summary_time == time;
}
void Output::killedBySignal(const std::string& binary, int signal)
{
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
std::string Directory::absolute(const std::string& path)
{
  return path;
}
void Directory::readTextFile_mockOutput(const std::string& output)
{
  __read_text_file_mock_output.push_back(output);
}
std::string Directory::readTextFile(const std::string& path)
{
  const auto out = __read_text_file_mock_output.back();
  __read_text_file_mock_output.pop_back();
  __read_text_file_path.push_back(std::string(path));
  return out;
}
bool Directory::readTextFile_hasBeenCalledWith(const std::string& path)
{
  auto x = std::find(__read_text_file_path.begin(), __read_text_file_path.end(), path);

  return x != __read_text_file_path.end();
}


void Process::runExecutable_mockOutput(const std::string& output)
{
  __mock_run_executable_output = output;
}
void Process::runExecutable_mockElapsedTime(int64_t time)
{
  __run_executable_time = time;
}
int Process::runExecutable(
  const std::string& path,
  std::function<void(std::string)> on_output,
  const std::vector<std::string>& args,
  int64_t& elapsed_time
) {
  __run_executable_has_been_called = true;
  __run_executable_path.push_back(path);
  elapsed_time = __run_executable_time;
  on_output(__mock_run_executable_output);

  return 0;
}
bool Process::runExecutable_hasBeenCalledWith(const std::string& path)
{
  auto it = std::find(__run_executable_path.cbegin(), __run_executable_path.cend(), path);
  return __run_executable_has_been_called && it != __run_executable_path.end();
}
bool Process::killedBySignal(int status)
{
  return false;
}
