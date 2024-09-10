#include "types.hpp"
#include "globals.hpp"

#define describe(...) __attribute__((unused)) static int dummy = cest::describeFn(__VA_ARGS__)
#define it(...) cest::itFn(__FILE__, __LINE__, __VA_ARGS__)
#define xit(...) cest::xitFn(__FILE__, __LINE__, __VA_ARGS__)
#define fit(...) cest::fitFn(__FILE__, __LINE__, __VA_ARGS__)
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
    }

    TestCaseBuilder *skipped()
    {
      test->condition = TestCaseCondition::Skipped;
      return this;
    }

    TestCaseBuilder *fitted()
    {
      test->condition = TestCaseCondition::Fitted;
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

  void itFn(std::string file, int line, std::string name, std::function<void()> fn)
  {
    TestCase *test = TestCaseBuilder(file, line, name, fn).build();
    __cest_globals.current_test_suite->test_cases.push_back(test);
  }

  void fitFn(std::string file, int line, std::string name, std::function<void()> fn)
  {
    TestCase *test = TestCaseBuilder(file, line, name, fn).fitted()->build();
    __cest_globals.current_test_suite->test_cases.push_back(test);
  }

  void xitFn(std::string file, int line, std::string name, std::function<void()> fn)
  {
    TestCase *test = TestCaseBuilder(file, line, name, fn).skipped()->build();
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
