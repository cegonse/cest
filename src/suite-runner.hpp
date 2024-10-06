#pragma once
#include <iostream>

#include "globals.hpp"
#include "signal-handler.hpp"

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

      if (suite->after_each.fn)
        suite->after_each.fn();
    }

    if (suite->after_all.fn)
      suite->after_all.fn();

    for (auto &pair : suite->test_suites)
      runTestSuite(pair.second);
  }
}
