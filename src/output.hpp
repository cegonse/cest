#pragma once
#include <iostream>
#include <string>
#include <sstream>

#include "types.hpp"

#define ASCII_BACKGROUND_GREEN "\u001b[42m"
#define ASCII_BACKGROUND_YELLOW "\u001b[43m"
#define ASCII_BACKGROUND_RED "\u001b[41m"
#define ASCII_BACKGROUND_MAGENTA "\u001b[45;1m"
#define ASCII_RED "\033[1m\033[31m"
#define ASCII_GREEN "\033[1m\033[32m"
#define ASCII_GRAY "\u001b[38;5;243m"
#define ASCII_BLACK "\u001b[38;5;232m"
#define ASCII_YELLOW "\u001b[33m"
#define ASCII_BOLD "\u001b[1m"
#define ASCII_RESET "\033[0m"
#define ASCII_CROSS "\u2715"
#define ASCII_CHECK "\u2713"
#define ASCII_TRIANGLE "\u25CB"

namespace cest
{
  void showHelp(std::string binary)
  {
    std::cout << "usage: " << binary << " [options]" << std::endl
              << std::endl;
    std::cout << "Command line options:" << std::endl;
    std::cout << "    -r/--randomize: Randomize test executions" << std::endl;
    std::cout << "    -o/--only-suite-result: Only output the test suite result" << std::endl;
    std::cout << "    -t/--tree-suite-result: Output the test suite result in tree format" << std::endl;
    std::cout << "    -s/--seed <seed>: Inject seed for randomization uses (unsigned integer)";
    std::cout << std::endl;
  }

  void printTestBadge(bool failed, bool skipped = false)
  {
    if (failed)
    {
      std::cout << ASCII_BACKGROUND_RED << ASCII_BLACK << ASCII_BOLD << " FAIL " << ASCII_RESET;
    }
    else if (skipped)
    {
      std::cout << ASCII_BACKGROUND_YELLOW << ASCII_BLACK << ASCII_BOLD << " SKIP " << ASCII_RESET;
    }
    else
    {
      std::cout << ASCII_BACKGROUND_GREEN << ASCII_BLACK << ASCII_BOLD << " PASS " << ASCII_RESET;
    }
  }

  void printTestCaseResult(cest::TestCase *test_case)
  {
    printTestBadge(test_case->failed, test_case->condition == cest::TestCaseCondition::Skipped);
    std::cout << ASCII_GRAY << " " << test_case->fn.file << ":" << test_case->fn.line << ASCII_RESET << ASCII_BOLD << " " << test_case->name << ASCII_RESET << std::endl;

    if (test_case->failed)
    {
      std::cout << " Failed at line " << test_case->failure_line << ": " << test_case->failure_message << std::endl;
    }
  }

  void printTestSuiteResult(cest::TestSuite *suite)
  {
    for (cest::TestCase *test_case : suite->test_cases)
      printTestCaseResult(test_case);

    for (auto &pair : suite->test_suites)
      printTestSuiteResult(pair.second);
  }

  void printSingleSuiteSummaryResult(cest::TestSuite *suite)
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

    printTestBadge(any_test_failed);
    std::cout << ASCII_BOLD << " " << suite->name << ASCII_RESET << std::endl;
  }

  void printTreeSuiteResult(cest::TestSuite *suite, int indentation = 0)
  {
    std::string spacing = "  ";

    for (int i = 0; i < indentation; ++i)
      spacing += "  ";

    std::cout << spacing << ASCII_BOLD << suite->name << ASCII_RESET << std::endl;

    for (cest::TestCase *test_case : suite->test_cases)
    {
      if (test_case->failed)
        std::cout << spacing << ASCII_RED << ASCII_CROSS << ASCII_RESET;
      else if (test_case->condition == cest::TestCaseCondition::Skipped)
        std::cout << spacing << ASCII_YELLOW << ASCII_TRIANGLE << ASCII_RESET;
      else
        std::cout << spacing << ASCII_GREEN << ASCII_CHECK << ASCII_RESET;

      std::cout << " " << ASCII_GRAY << test_case->name << ASCII_RESET << std::endl;
    }

    for (auto &pair : suite->test_suites)
      printTreeSuiteResult(pair.second, indentation + 1);
  }

  void printSuiteSummaryResult(cest::TestSuite *suite)
  {
    printSingleSuiteSummaryResult(suite);

    for (auto &pair : suite->test_suites)
      printSuiteSummaryResult(pair.second);
  }
}
