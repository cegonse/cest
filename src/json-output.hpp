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
    bool is_root = test_suite == &__cest_globals.root_test_suite;

    if (is_root)
    {
      for (cest::TestSuite *nested_suite : test_suite->test_suites)
        addSuiteJsonResult(nested_suite, output);

      return;
    }

    nlohmann::json suite;

    suite["tests"] = std::vector<nlohmann::json>();
    suite["name"] = test_suite->name;

    for (auto &test_case : test_suite->test_cases)
    {
      nlohmann::json test;
      test["name"] = test_case->name;
      test["failed"] = test_case->failed;
      test["skipped"] = test_case->condition == cest::TestCaseCondition::Skipped;
      test["todo"] = test_case->condition == cest::TestCaseCondition::Todo;
      test["duration_ms"] = test_case->duration_us / 1000;

      if (test_case->failed)
      {
        test["failure_message"] = test_case->failure_message;
        test["failure_line"] = test_case->failure_line;
      }

      suite["tests"].push_back(test);
    }

    output["suites"].push_back(suite);

    for (cest::TestSuite *nested_suite : test_suite->test_suites)
      addSuiteJsonResult(nested_suite, output);
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

    for (cest::TestSuite *nested_suite : test_suite->test_suites)
      addSuiteJsonTestCases(nested_suite, output);
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
