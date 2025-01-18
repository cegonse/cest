#pragma once
#include <iostream>

#include "globals.hpp"
#include "signal-handler.hpp"
#include "address-sanitizer.hpp"

namespace cest
{
  void handleFailedTest(cest::TestCase *test_case, std::string message, std::string file, int line)
  {
    test_case->failed = true;
    test_case->failure_message = message;
    test_case->failure_file = file;
    test_case->failure_line = line;
  }

  void runTestSuite(TestSuite *suite)
  {
    if (suite->before_all.fn)
      suite->before_all.fn();

    for (cest::TestCase *test_case : suite->test_cases)
    {
      if (test_case->condition == cest::TestCaseCondition::Skipped)
        continue;

      __cest_globals.current_test_case = test_case;

      if (suite->before_each.fn)
        suite->before_each.fn();

      try
      {
        if (test_case->condition == cest::TestCaseCondition::Skipped)
          throw cest::ForcedPassError();

        test_case->fn.fn();
        cest::registerSignalHandler();
      }
      catch (const cest::AssertionError &error)
      {
        handleFailedTest(test_case, error.message, error.file, error.line);
      }
      catch (const cest::ForcedPassError &error)
      {
      }
      catch (const std::exception &error)
      {
        std::string message = "Unhandled exception: ";
        message += error.what();
        handleFailedTest(test_case, message, test_case->fn.file, test_case->fn.line);
      }
      catch (...)
      {
        std::string message = "Unhandled exception, non recoverable exception.";
        handleFailedTest(test_case, message, test_case->fn.file, test_case->fn.line);
      }

      if (cest::leaksDetected())
      {
        std::string message = "Detected memory leaks during test execution.";
        handleFailedTest(test_case, message, test_case->fn.file, test_case->fn.line);
      }

      if (suite->after_each.fn)
        suite->after_each.fn();
    }

    if (suite->after_all.fn)
      suite->after_all.fn();

    for (auto &pair : suite->test_suites)
      runTestSuite(pair.second);
  }

  void cleanUpSingleSuite(
    cest::TestSuite *test_suite,
    std::vector<cest::TestSuite *>& suites_to_delete,
    std::vector<cest::TestCase *>& tests_to_delete
  ) {
    if (test_suite != &__cest_globals.root_test_suite)
      suites_to_delete.push_back(test_suite);

    for (const auto test : test_suite->test_cases)
      tests_to_delete.push_back(test);

    for (auto &pair : test_suite->test_suites)
      cleanUpSingleSuite(pair.second, suites_to_delete, tests_to_delete);
  }

  void cleanUpTestSuite(cest::TestSuite *test_suite)
  {
    std::vector<cest::TestSuite *> suites_to_delete;
    std::vector<cest::TestCase *> tests_to_delete;

    cleanUpSingleSuite(test_suite, suites_to_delete, tests_to_delete);

    for (const auto test : tests_to_delete)
      delete test;

    for (const auto suite : suites_to_delete)
      delete suite;
  }
}
