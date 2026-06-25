#pragma once
#include <string_view>
#include <type_traits>
#include "expect/assertion-base.hpp"
#include "expect/assertion-callable.hpp"
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
#include "expect/assertion-multi.hpp"
#include "expect/assertion-smart-ptr.hpp"
#include "expect/assertion-chrono.hpp"
#include "expect/assertion-path.hpp"
#include "expect/assertion-complex.hpp"
#include "expect/assertion-bitset.hpp"

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

    throw AssertionError(file, line, "Expected function to throw, but no exception was thrown");
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

  Assertion<std::string> expectFunction(const char *file, int line, std::string_view actual)
  {
    return Assertion<std::string>(file, line, std::string(actual));
  }

  template <class Fn>
    requires std::is_invocable_r_v<void, Fn>
  CallableAssertion expectFunction(const char *file, int line, Fn actual)
  {
    return CallableAssertion(file, line, std::function<void()>(actual));
  }
}
