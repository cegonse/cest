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
#define ASCII_BOLD "\u001b[1m"
#define ASCII_RESET "\033[0m"

namespace cest
{
  void showHelp(std::string binary)
  {
    std::cout << "usage: " << binary << " [options]" << std::endl
              << std::endl;
    std::cout << "Command line options:" << std::endl;
    std::cout << "    -r/--randomize: Randomize test executions" << std::endl;
    std::cout << "    -s/--seed <seed>: Inject seed for randomization uses (unsigned integer)";
    std::cout << std::endl;
  }

  // void printTestResult(TestCaseDeprecated *test_case, const std::stringstream & assertion_failures)
  // {
  //   if (test_case->test_failed)
  //   {
  //     std::cout << ASCII_BACKGROUND_RED << ASCII_BLACK << ASCII_BOLD << " FAIL " << ASCII_RESET;
  //   }
  //   else if (test_case->skip)
  //   {
  //     std::cout << ASCII_BACKGROUND_YELLOW << ASCII_BLACK << ASCII_BOLD << " SKIP " << ASCII_RESET;
  //   }
  //   else
  //   {
  //     std::cout << ASCII_BACKGROUND_GREEN << ASCII_BLACK << ASCII_BOLD << " PASS " << ASCII_RESET;
  //   }

  //   std::cout << ASCII_GRAY << " " << test_case->file << ":" << test_case->line << ASCII_RESET << ASCII_BOLD << " it " << test_case->name << ASCII_RESET << std::endl;

  //   std::cout << assertion_failures.str();
  // }
}
