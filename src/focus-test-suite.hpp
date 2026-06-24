#include "globals.hpp"

namespace cest
{
  void configureFocusedTestSuite(cest::TestSuite *test_suite)
  {
    int focused_test_idx = -1;

    for (int i = 0; i < (int)test_suite->test_cases.size(); i++)
    {
      cest::TestCase *test_case = test_suite->test_cases[i];
      if (test_case->condition == cest::TestCaseCondition::Focused)
      {
        focused_test_idx = i;
        break;
      }
    }

    if (focused_test_idx != -1)
    {
      for (int i = 0; i < (int)test_suite->test_cases.size(); i++)
      {
        cest::TestCase *test_case = test_suite->test_cases[i];

        if (i != focused_test_idx && test_case->condition != cest::TestCaseCondition::Todo)
          test_case->condition = cest::TestCaseCondition::Skipped;
      }
    }

    for (cest::TestSuite *nested_suite : test_suite->test_suites)
      configureFocusedTestSuite(nested_suite);
  }

  void filterTestSuite(cest::TestSuite *suite, const std::string& filter)
  {
    for (cest::TestCase *test_case : suite->test_cases)
    {
      if (test_case->condition == cest::TestCaseCondition::Todo)
        continue;
      if (test_case->name.find(filter) == std::string::npos)
        test_case->condition = cest::TestCaseCondition::Skipped;
    }

    for (cest::TestSuite *nested_suite : suite->test_suites)
      filterTestSuite(nested_suite, filter);
  }
}