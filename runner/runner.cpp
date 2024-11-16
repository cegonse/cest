#include "runner.h"
#include "process.h"
#include "directory.h"
#include "output.h"
#include <iostream>

void Runner::runTestsInCurrentPath()
{
  std::vector<std::string> cest_args = { "-o" };
  const auto executables = Directory::findExecutableFiles(Directory::cwd(), "test_");

  for (const auto& test_file : executables)
  {
    Process::runExecutable(test_file, [](const auto& output) {
      Output::print(output);
    }, cest_args);

    const auto results_path = "/tmp/cest_" + test_file.substr(test_file.rfind('/') + 1);
    const auto test_result = Directory::readTextFile(results_path);
  }
}
