#include "runner.h"
#include "process.h"
#include "directory.h"
#include "output.h"
#include "test-results.hpp"
#include <chrono>

struct TestCounts
{
  int total_passed_tests;
  int total_failed_tests;
  int total_skipped_tests;
  int total_passed_suites;
  int total_failed_suites;

  TestCounts() : total_passed_tests(0),
    total_failed_tests(0),
    total_skipped_tests(0),
    total_passed_suites(0),
    total_failed_suites(0) {}
};

constexpr double toSeconds(int64_t us)
{
  return ((double)us) / 1000000.f;
}

int Runner::runTests(const std::vector<std::string>& executables)
{
  int status_code = 0;
  int64_t total_time_us = 0;
  TestCounts counts;
  std::vector<std::string> cest_args = { "-o" };

  for (const auto& test_file : executables)
  {
    int64_t test_time = 0;
    const auto test_status = Process::runExecutable(test_file, [](const auto& output) {
      Output::print(output);
    }, cest_args, test_time);

    status_code |= test_status;
    total_time_us += test_time;

    if (!Process::killedBySignal(status_code))
    {
      const auto results_path = "/tmp/cest_" + test_file.substr(test_file.rfind('/') + 1);
      const auto test_result = TestResults(Directory::readTextFile(results_path));

      counts.total_passed_tests += test_result.num_passed_tests;
      counts.total_failed_tests += test_result.num_failed_tests;
      counts.total_skipped_tests += test_result.num_skipped_tests;

      if (test_result.num_failed_tests > 0)
        counts.total_failed_suites++;
      else
        counts.total_passed_suites++;
    }
    else
    {
      Output::killedBySignal(test_file, test_status);
      counts.total_failed_suites++;
    }
  }

  Output::printSummary(
    counts.total_passed_suites,
    counts.total_failed_suites,
    counts.total_passed_tests,
    counts.total_failed_tests,
    counts.total_skipped_tests,
    toSeconds(total_time_us)
  );

  return status_code;
}
