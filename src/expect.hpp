#pragma once
#include "expect/assertion-base.hpp"
#include "expect/assertion-double.hpp"
#include "expect/assertion-float.hpp"
#include "expect/assertion-vector.hpp"
#include "expect/assertion-string.hpp"
#include "expect/assertion-set.hpp"
#include "expect/assertion-map.hpp"
#include "expect/assertion-array.hpp"
#include "expect/assertion-optional.hpp"
#include "expect/assertion-tuple.hpp"
#include "expect/assertion-deque.hpp"
#include "expect/assertion-list.hpp"

#define expect(...) cest::expectFunction(__FILE__, ((__LINE__ - 1)), __VA_ARGS__)
#define passTest() cest::forcedPass()
#define failTest() cest::forcedFailure(__FILE__, __LINE__)
#define Regex(x) x, std::regex(x)

namespace cest
{
  void forcedPass()
  {
    throw ForcedPassError();
  }

  void forcedFailure(std::string file, int line)
  {
    throw AssertionError(file, line, "Test failure forced manually");
  }

  template <class T>
  void assertRaises(std::function<void()> expression)
  {
    std::string file = __cest_globals.current_test_case->fn.file;
    int line = __cest_globals.current_test_case->fn.line;

    try
    {
      expression();
    }
    catch (const T &err)
    {
      return;
    }

    throw AssertionError(file, line, "Expected exception not raised");
  }

  template <class T>
  Assertion<T> expectFunction(const char *file, int line, T actual)
  {
    return Assertion<T>(file, line, actual);
  }

  Assertion<bool> expectFunction(const char *file, int line, bool actual)
  {
    return Assertion<bool>(file, line, actual);
  }

  Assertion<int64_t> expectFunction(const char *file, int line, int64_t actual)
  {
    return Assertion<int64_t>(file, line, actual);
  }

  Assertion<std::string> expectFunction(const char *file, int line, std::string actual)
  {
    return Assertion<std::string>(file, line, actual);
  }

  Assertion<std::string> expectFunction(const char *file, int line, const char *actual)
  {
    return Assertion<std::string>(file, line, (std::string)actual);
  }
}
