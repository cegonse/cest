#pragma once
#include <functional>
#include <map>
#include <string>
#include <csetjmp>

namespace cest
{
  struct TestFunction
  {
    std::string file;
    int line;
    std::function<void()> fn;
  };

  enum class TestCaseCondition
  {
    Normal,
    Skipped,
    Focused
  };

  struct TestCase
  {
    std::string name;
    TestCaseCondition condition;
    TestFunction fn;
    bool failed;
    std::string failure_message;
    std::string failure_file;
    int failure_line;
  };

  struct TestSuite
  {
    std::string name;
    TestFunction before_each;
    TestFunction after_each;
    TestFunction before_all;
    TestFunction after_all;
    std::vector<TestCase *> test_cases;
    std::map<std::string, TestSuite *> test_suites;
  };

  class AssertionError : public std::exception
  {
  public:
    std::string file;
    int line;
    std::string message;

    AssertionError(std::string file, int line, std::string message) : file(file), line(line), message(message) {}
  };

  class ForcedPassError : public std::exception
  {
  };

  struct CommandLineOptions
  {
    bool help;
    bool randomize;
    unsigned int random_seed;
    bool random_seed_present;
    bool generate_test_report;
    bool only_test_suite_result;
    bool tree_test_suite_result;
  };

  struct CestGlobals
  {
    TestSuite root_test_suite;
    TestSuite *current_test_suite;
    TestCase *current_test_case;
    jmp_buf jump_env;
    bool leaks_detected;
    int saved_stderr;

    CestGlobals() : current_test_suite(nullptr),
    leaks_detected(false) {}
  };

  bool anyTestInSuiteFailed(cest::TestSuite *suite)
  {
    bool any_test_failed = false;

    for (cest::TestCase *test_case : suite->test_cases)
    {
      if (test_case->failed)
      {
        any_test_failed = true;
        break;
      }
    }

    for (auto &pair : suite->test_suites)
      any_test_failed |= anyTestInSuiteFailed(pair.second);

    return any_test_failed;
  }

  int countTestsMatching(cest::TestSuite *suite, std::function<bool(cest::TestCase *)> condition)
  {
    int num_tests = 0;

    for (cest::TestCase *test_case : suite->test_cases)
    {
      if (condition(test_case))
        num_tests++;
    }

    for (auto &pair : suite->test_suites)
      num_tests += countTestsMatching(pair.second, condition);

    return num_tests;
  }

  int numPassedTests(cest::TestSuite *suite)
  {
    return countTestsMatching(suite, [](cest::TestCase *test_case) {
      return !test_case->failed && test_case->condition != TestCaseCondition::Skipped;
    });
  }

  int numFailedTests(cest::TestSuite *suite)
  {
    return countTestsMatching(suite, [](cest::TestCase *test_case) {
      return test_case->failed;
    });
  }

  int numSkippedTests(cest::TestSuite *suite)
  {
    return countTestsMatching(suite, [](cest::TestCase *test_case) {
      return test_case->condition == TestCaseCondition::Skipped;
    });
  }

  std::string findSuiteSourceFile(cest::TestSuite *suite)
  {
    if (suite->test_cases.size() > 0)
    {
      return suite->test_cases[0]->fn.file;
    }
    else
    {
      for (const auto &pair : suite->test_suites)
        return findSuiteSourceFile(pair.second);
    }

    return "";
  }

  std::vector<cest::TestCase *> findFailedTestCases(cest::TestSuite *suite)
  {
    std::vector<cest::TestCase *> out;

    for (auto test_case : suite->test_cases)
    {
      if (test_case->failed)
        out.push_back(test_case);
    }

    for (const auto &pair : suite->test_suites)
    {
      auto nested = findFailedTestCases(pair.second);
      out.insert(out.end(), nested.begin(), nested.end());
    }

    return out;
  }
}
