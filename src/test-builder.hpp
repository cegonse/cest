#pragma once
#include "types.hpp"
#include "globals.hpp"

#define PP(x,y) x##y
#define P(x,y) PP(x,y)

#define describe(...) __attribute__((unused)) static int P(dummy, __LINE__) = cest::describeFn(__VA_ARGS__)
#define xdescribe(...) __attribute__((unused)) static int P(dummy, __LINE__) = cest::xdescribeFn(__VA_ARGS__)
#define fdescribe(...) __attribute__((unused)) static int P(dummy, __LINE__) = cest::fdescribeFn(__VA_ARGS__)
#define it(...) cest::itFn(__FILE__, __LINE__, __VA_ARGS__)
#define xit(...) cest::xitFn(__FILE__, __LINE__, __VA_ARGS__)
#define fit(...) cest::fitFn(__FILE__, __LINE__, __VA_ARGS__)
#define todo(...) cest::todoFn(__FILE__, __LINE__, __VA_ARGS__)
#define beforeEach(x) cest::beforeEachFn(__FILE__, __LINE__, x)
#define afterEach(x) cest::afterEachFn(__FILE__, __LINE__, x)
#define beforeAll(x) cest::beforeAllFn(__FILE__, __LINE__, x)
#define afterAll(x) cest::afterAllFn(__FILE__, __LINE__, x)

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

  bool suiteNameAlreadyUsed(TestSuite *parent, const std::string &name)
  {
    for (TestSuite *sibling : parent->test_suites)
    {
      if (sibling->name == name)
        return true;
    }

    return false;
  }

  int describeFnWithCondition(std::string name, std::function<void()> fn, TestCaseCondition condition)
  {
    TestSuite *parent = &__cest_globals.root_test_suite;

    if (__cest_globals.current_test_suite != nullptr)
      parent = __cest_globals.current_test_suite;

    if (suiteNameAlreadyUsed(parent, name))
    {
      __cest_globals.registration_errors.push_back(
        "Duplicate test suite name \"" + name + "\" at the same level.");
    }

    TestSuite *suite = new TestSuite();
    suite->name = name;
    suite->condition = condition;
    parent->test_suites.push_back(suite);

    __cest_globals.current_test_suite = suite;
    fn();
    __cest_globals.current_test_suite = parent;

    return 0;
  }

  int describeFn(std::string name, std::function<void()> fn)
  {
    return describeFnWithCondition(name, fn, TestCaseCondition::Normal);
  }

  int xdescribeFn(std::string name, std::function<void()> fn)
  {
    return describeFnWithCondition(name, fn, TestCaseCondition::Skipped);
  }

  int fdescribeFn(std::string name, std::function<void()> fn)
  {
    return describeFnWithCondition(name, fn, TestCaseCondition::Focused);
  }

  void itFn(std::string file, int line, std::string name, std::function<void()> fn)
  {
    TestCaseCondition suite_condition = __cest_globals.current_test_suite->condition;
    TestCaseBuilder builder(file, line, name, fn);

    if (suite_condition == TestCaseCondition::Skipped)
      builder.skipped();
    else if (suite_condition == TestCaseCondition::Focused)
      builder.Focused();

    TestCase *test = builder.build();
    __cest_globals.current_test_suite->test_cases.push_back(test);
  }

  void fitFn(std::string file, int line, std::string name, std::function<void()> fn)
  {
    TestCase *test = TestCaseBuilder(file, line, name, fn).Focused()->build();
    __cest_globals.current_test_suite->test_cases.push_back(test);
  }

  void xitFn(std::string file, int line, std::string name, std::function<void()> fn)
  {
    TestCase *test = TestCaseBuilder(file, line, name, fn).skipped()->build();
    __cest_globals.current_test_suite->test_cases.push_back(test);
  }

  void todoFn(std::string file, int line, std::string name)
  {
    TestCase *test = new TestCase();
    test->name = name;
    test->condition = TestCaseCondition::Todo;
    test->fn.file = file;
    test->fn.line = line;
    test->failed = false;
    __cest_globals.current_test_suite->test_cases.push_back(test);
  }

  void beforeEachFn(std::string file, int line, std::function<void()> fn)
  {
    __cest_globals.current_test_suite->before_each = { file, line, fn};
  }

  void afterEachFn(std::string file, int line, std::function<void()> fn)
  {
    __cest_globals.current_test_suite->after_each = { file, line, fn};
  }

  void beforeAllFn(std::string file, int line, std::function<void()> fn)
  {
    __cest_globals.current_test_suite->before_all = { file, line, fn};
  }

  void afterAllFn(std::string file, int line, std::function<void()> fn)
  {
    __cest_globals.current_test_suite->after_all = { file, line, fn};
  }
}
