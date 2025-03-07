#pragma once
#include <source_location>
#include "types.hpp"
#include "globals.hpp"

#define PP(x,y) x##y
#define P(x,y) PP(x,y)

#define describe(...) __attribute__((unused)) static int P(dummy, __LINE__) = cest::describeFn(__VA_ARGS__)

namespace cest
{
  class TestCaseBuilder
  {
  private:
    TestCase *test;

  public:
    TestCaseBuilder(std::string file, int line, std::string name, std::function<void()> fn)
    {
      test = new TestCase();
      test->name = name;
      test->condition = TestCaseCondition::Normal;
      test->fn.file = file;
      test->fn.line = line;
      test->fn.fn = fn;
      test->failed = false;
    }

    TestCaseBuilder *skipped()
    {
      test->condition = TestCaseCondition::Skipped;
      return this;
    }

    TestCaseBuilder *Focused()
    {
      test->condition = TestCaseCondition::Focused;
      return this;
    }

    TestCase *build()
    {
      return test;
    }
  };

  int describeFn(std::string name, std::function<void()> fn)
  {
    if (__cest_globals.current_test_suite == nullptr)
    {
      __cest_globals.current_test_suite = &__cest_globals.root_test_suite;
    }
    else
    {
      TestSuite *new_suite = new TestSuite();
      __cest_globals.current_test_suite->test_suites[name] = new_suite;
      __cest_globals.current_test_suite = new_suite;
    }

    __cest_globals.current_test_suite->name = name;

    fn();

    return 0;
  }
}

void it(
  const std::string& name,
  const std::function<void()>& fn,
  const std::source_location location = std::source_location::current())
{
  const auto file = location.file_name();
  const auto line = location.line();

  cest::TestCase *test = cest::TestCaseBuilder(file, line, name, fn).build();
  __cest_globals.current_test_suite->test_cases.push_back(test);
}

void fit(
  const std::string& name,
  const std::function<void()>& fn,
  const std::source_location location = std::source_location::current())
{
  const auto file = location.file_name();
  const auto line = location.line();

  cest::TestCase *test = cest::TestCaseBuilder(file, line, name, fn).Focused()->build();
  __cest_globals.current_test_suite->test_cases.push_back(test);
}

void xit(
  const std::string& name,
  const std::function<void()>& fn,
  const std::source_location location = std::source_location::current())
{
  const auto file = location.file_name();
  const auto line = location.line();

  cest::TestCase *test = cest::TestCaseBuilder(file, line, name, fn).skipped()->build();
  __cest_globals.current_test_suite->test_cases.push_back(test);
}

void beforeEach(
  const std::function<void()>& fn,
  const std::source_location location = std::source_location::current())
{
  const auto file = std::string(location.file_name());
  int line = location.line();

  __cest_globals.current_test_suite->before_each = { file, line, fn};
}

void afterEach(
  const std::function<void()>& fn,
  const std::source_location location = std::source_location::current())
{
  const auto file = location.file_name();
  int line = location.line();

  __cest_globals.current_test_suite->after_each = { file, line, fn};
}

void beforeAll(
  const std::function<void()>& fn,
  const std::source_location location = std::source_location::current())
{
  const auto file = location.file_name();
  int line = location.line();

  __cest_globals.current_test_suite->before_all = { file, line, fn};
}

void afterAll(
  const std::function<void()>& fn,
  const std::source_location location = std::source_location::current())
{
  const auto file = location.file_name();
  int line = location.line();

  __cest_globals.current_test_suite->after_all = { file, line, fn};
}
