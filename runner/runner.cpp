#include "runner.h"
#include "process.h"
#include "directory.h"
#include "output.h"
#include "test-results.hpp"
#include <chrono>

int Runner::runTestsInCurrentPath()
{
  int status_code = 0;
  int64_t total_time_us = 0;
  int total_passed_tests = 0,
      total_failed_tests = 0,
      total_skipped_tests = 0,
      total_passed_suites = 0,
      total_failed_suites = 0;
  std::vector<std::string> cest_args = { "-o" };
  const auto executables = Directory::findExecutableFiles(Directory::cwd(), "test_");

  for (const auto& test_file : executables)
  {
    const auto start = std::chrono::high_resolution_clock::now();
    const auto test_status = Process::runExecutable(test_file, [](const auto& output) {
      Output::print(output);
    }, cest_args);
    const auto end = std::chrono::high_resolution_clock::now();

    status_code |= test_status;

    total_time_us += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    const auto results_path = "/tmp/cest_" + test_file.substr(test_file.rfind('/') + 1);
    const auto test_result = TestResults(Directory::readTextFile(results_path));

    total_passed_tests += test_result.num_passed_tests;
    total_failed_tests += test_result.num_failed_tests;
    total_skipped_tests += test_result.num_skipped_tests;

    if (test_result.num_failed_tests > 0)
      total_failed_suites++;
    else
      total_passed_suites++;
  }

  Output::printSummary(
    total_passed_suites,
    total_failed_suites,
    total_passed_tests,
    total_failed_tests,
    total_skipped_tests,
    ((double)total_time_us) / 1000000.f
  );

  return status_code;
}
