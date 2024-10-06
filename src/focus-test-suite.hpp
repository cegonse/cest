#include "globals.hpp"

namespace cest
{
  void configureFocusedTestSuite(cest::TestSuite *test_suite)
  {
    int focused_test_idx = -1;

    for (int i = 0; i < (int)test_suite->test_cases.size(); i++)
    {
      cest::TestCase *test_case = test_suite->test_cases[i];
      if (test_case->condition == cest::TestCaseCondition::Fitted)
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

        if (i != focused_test_idx)
          test_case->condition = cest::TestCaseCondition::Skipped;
      }
    }

    for (auto &pair : test_suite->test_suites)
      configureFocusedTestSuite(pair.second);
  }
}