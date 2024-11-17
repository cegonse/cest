#include "output.h"
#include <iostream>
#include <sstream>

#define ASCII_RED "\033[1m\033[31m"
#define ASCII_GREEN "\033[1m\033[32m"
#define ASCII_GRAY "\u001b[38;5;243m"
#define ASCII_BLACK "\u001b[38;5;232m"
#define ASCII_YELLOW "\u001b[33m"
#define ASCII_BOLD "\u001b[1m"
#define ASCII_RESET "\033[0m"

void Output::print(const std::string& text)
{
  std::cout << text;
}

void Output::printSummary(
  int num_passed_suites,
  int num_failed_suites,
  int num_passed_tests,
  int num_failed_tests,
  int num_skipped_tests,
  float time
) {
  int num_total =
    num_passed_tests +
    num_failed_tests +
    num_skipped_tests;
  int num_total_suites =
    num_passed_suites +
    num_failed_suites;
  std::stringstream passed_suites,
    failed_suites,
    total_suites,
    passed,
    failed,
    skipped,
    total;

  passed_suites
    << ASCII_BOLD
    << ASCII_GREEN
    << num_passed_suites
    << " passed"
    << ASCII_BLACK
    << ", "
    << ASCII_RESET;

  failed_suites
    << ASCII_BOLD
    << ASCII_RED
    << num_failed_suites
    << " failed"
    << ASCII_BLACK
    << ", "
    << ASCII_RESET;

  total_suites
    << num_total_suites
    << " total";

  passed
    << ASCII_BOLD
    << ASCII_GREEN
    << num_passed_tests
    << " passed"
    << ASCII_BLACK
    << ", "
    << ASCII_RESET;

  failed
    << ASCII_BOLD
    << ASCII_RED
    << num_failed_tests
    << " failed"
    << ASCII_BLACK
    << ", "
    << ASCII_RESET;

  skipped
    << ASCII_BOLD
    << ASCII_YELLOW
    << num_skipped_tests
    << " skipped"
    << ASCII_BLACK
    << ", "
    << ASCII_RESET;

  total
    << num_total
    << " total";

  std::cout << std::endl;

  std::cout
    << ASCII_BOLD
    << "Test Suites: "
    << (num_failed_suites > 0 ? failed_suites.str() : "")
    << (num_passed_suites > 0 ? passed_suites.str() : "")
    << total_suites.str()
    << std::endl;

  std::cout
    << ASCII_BOLD
    << "Tests:       "
    << (num_failed_tests > 0 ? failed.str() : "")
    << (num_passed_tests > 0 ? passed.str() : "")
    << (num_skipped_tests > 0 ? skipped.str() : "")
    << total.str()
    << std::endl;

  std::cout
    << ASCII_BOLD
    << "Time:        "
    << ASCII_RESET
    << time
    << " s"
    << std::endl;

  std::cout
    << ASCII_GRAY
    << "Ran all test suites."
    << ASCII_RESET
    << std::endl;
}
