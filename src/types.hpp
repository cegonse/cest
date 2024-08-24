#pragma once

#include <functional>
#include <string>

namespace cest
{
  struct TestCase
  {
    std::string name;
    std::string file;
    int line;
    std::function<void()> test;
    bool test_failed;
    bool forced_pass;
    bool skip;
    bool fit;
    std::string failure_message;

    TestCase() : test_failed(false), forced_pass(false), skip(false)
    {
    }
  };

  struct TestSuite
  {
    std::vector<cest::TestCase *> test_cases;
    std::string test_suite_name;
  };
}