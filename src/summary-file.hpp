#pragma once
#include <sstream>
#include <fstream>
#include "types.hpp"

namespace cest
{
  void writeTextFile(const std::string& path, const std::string& data)
  {
    std::ofstream file(path);
    if (!file) return;
    file << data;
    file.close();
  }

  void saveSummaryFile(const std::string& file_name, const std::string& binary_path, cest::TestSuite *root_suite)
  {
    std::stringstream buffer;
    int passed_tests = numPassedTests(root_suite);
    int failed_tests = numFailedTests(root_suite);
    int skipped_tests = numSkippedTests(root_suite);

    buffer
      << root_suite->test_cases[0]->fn.file
      << "|"
      << binary_path
      << "|"
      << passed_tests
      << "|"
      << failed_tests
      << "|"
      << skipped_tests;

    writeTextFile("/tmp/cest_" + file_name, buffer.str());
  }
}
