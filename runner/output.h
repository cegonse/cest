#pragma once
#include <sstream>

namespace Output
{
  void print(const std::string& text);
  void printSummary(
    int num_passed_suites,
    int num_failed_suites,
    int num_failed_tests,
    int num_passed_tests,
    int num_skipped_tests,
    float time
  );
}
