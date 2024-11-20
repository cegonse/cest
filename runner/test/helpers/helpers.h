#pragma once
#include <sstream>
#include <functional>
#include <vector>

namespace Output
{
  bool printSummary_hasBeenCalledWith(
    int num_passed_suites,
    int num_failed_suites,
    int num_passed_tests,
    int num_failed_tests,
    int num_skipped_tests,
    float time
  );
}

namespace Directory
{
  void findExecutableFiles_mockFiles(std::vector<std::string> files);
  bool findExecutableFiles_hasBeenCalledWith(const std::string& path, const std::string& filter);
  void readTextFile_mockOutput(const std::string& output);
  bool readTextFile_hasBeenCalledWith(const std::string& path);
}

namespace Process
{
  void runExecutable_mockOutput(const std::string& output);
  bool runExecutable_hasBeenCalledWith(const std::string& path);
  void runExecutable_mockElapsedTime(int64_t time);
}
