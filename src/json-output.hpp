#pragma once
#include "types.hpp"
#include "globals.hpp"
#include "json.hpp"
#include <iostream>

namespace cest
{
  void addSuiteJsonResult(
    cest::TestSuite *test_suite,
    nlohmann::json& output)
  {
    nlohmann::json suite;

    suite["tests"] = std::vector<nlohmann::json>();
    suite["name"] = test_suite->name;

    for (auto &test_case : test_suite->test_cases)
    {
      nlohmann::json test;
      test["name"] = test_case->name;
      test["failed"] = test_case->failed;

      if (test_case->failed)
      {
        test["failure_message"] = test_case->failure_message;
        test["failure_line"] = test_case->failure_line;
      }

      suite["tests"].push_back(test);
    }

    output["suites"].push_back(suite);

    for (auto &pair : test_suite->test_suites)
      addSuiteJsonResult(pair.second, output);
  }

  void dumpJsonResult(cest::TestSuite *root_suite)
  {
    nlohmann::json output;
    output["leaks_detected"] = __cest_globals.leaks_detected;
    output["file"] = cest::findSuiteSourceFile(root_suite);
    output["suites"] = std::vector<nlohmann::json>();

    addSuiteJsonResult(root_suite, output);

    std::cout << output.dump(2) << std::endl;
  }

  void addSuiteJsonTestCases(
    cest::TestSuite *test_suite,
    nlohmann::json& output)
  {
    for (auto &test_case : test_suite->test_cases)
      output["test_cases"].push_back(test_case->name);

    for (auto &pair : test_suite->test_suites)
      addSuiteJsonTestCases(pair.second, output);
  }

  void dumpJsonTestList(cest::TestSuite *root_suite)
  {
    nlohmann::json output;
    output["file"] = cest::findSuiteSourceFile(root_suite);
    output["test_cases"] = std::vector<std::string>();

    addSuiteJsonTestCases(root_suite, output);

    std::cout << output.dump(2) << std::endl;
  }
}
