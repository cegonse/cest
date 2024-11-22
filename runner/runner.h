#pragma once
#include <vector>
#include <string>

namespace Runner
{
  struct TestRun
  {
    std::string source_path;
    std::string binary_path;
    bool failed;
  };

  int runTests(
    const std::vector<std::string>& executables,
    std::vector<TestRun>& results
  );
}
