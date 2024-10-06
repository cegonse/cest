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
  };

  struct CestGlobals
  {
    TestSuite root_test_suite;
    TestSuite *current_test_suite;
    TestCase *current_test_case;
    jmp_buf jump_env;

    CestGlobals() : current_test_suite(nullptr) {}
  };
}
