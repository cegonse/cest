#pragma once
#include <sstream>
#include <regex>
#include <cmath>
#include "types.hpp"

#define CLIP_STRING_LENGTH 16

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
  class Assertion
  {
  public:
    Assertion(const char *file, int line, T value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
        this->Not = new Assertion<T>(file, line, value, true);
      else
        this->negated = true;
    }

    void toBeTruthy()
    {
      if (!actual ^ this->negated)
        throw AssertionError(assertion_file, assertion_line, "Expresion is not truthy");
    }

    void toBeFalsy()
    {
      if (actual ^ this->negated)
        throw AssertionError(assertion_file, assertion_line, "Expresion is not falsy");
    }

    void toBe(T expected)
    {
      if ((expected != actual) ^ this->negated)
      {

        std::stringstream message;
        message << "Expected " << expected << ", was " << actual;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(T expected)
    {
      toBe(expected);
    }

    void toEqualMemory(T expected, int64_t length)
    {
      int i;
      bool any_byte_differs = false;

      for (i = 0; i < length; ++i)
        any_byte_differs |= expected[i] != actual[i];

      if (any_byte_differs ^ negated)
      {
        std::stringstream message;
        message << "Memory mismatch at byte " << i << ", expected ";
        message << std::hex << std::uppercase << (int)expected[i] << " but was " << std::hex << std::uppercase << (int)actual[i];
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeNotNull()
    {
      if ((actual == NULL) ^ negated)
      {
        std::stringstream message;
        message << "Expected 0x" << std::hex << std::uppercase << actual << " to be not null";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeNull()
    {
      if ((actual != NULL) ^ negated)
      {
        std::stringstream message;
        message << "Expected 0x" << std::hex << std::uppercase << actual << " to be null";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<T> *Not;

  private:
    bool negated;
    T actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <>
  class Assertion<double>
  {
  public:
    Assertion(const char *file, int line, double value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
        this->Not = new Assertion<double>(file, line, value, true);
      else
        this->negated = true;
    }

    void toBe(double expected, double epsilon = 0.0000001)
    {
      if ((fabs(actual - expected) > epsilon) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << expected << ", was " << actual;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(double expected, double epsilon = 0.0000001)
    {
      toBe(expected, epsilon);
    }

    void toBeGreaterThan(double expected)
    {
      if ((actual < expected) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << expected << " to be greather than" << actual;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeLessThan(double expected)
    {
      if ((actual > expected) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << expected << " to be less than" << actual;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<double> *Not;

  private:
    bool negated;
    double actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <>
  class Assertion<float>
  {
  public:
    Assertion(const char *file, int line, float value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
        this->Not = new Assertion<float>(file, line, value, true);
      else
        this->negated = true;
    }

    void toBe(float expected, float epsilon = 0.00001f)
    {
      if ((fabs(actual - expected) > epsilon) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << expected << ", was " << actual;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(float expected, float epsilon = 0.00001f)
    {
      toBe(expected, epsilon);
    }

    void toBeGreaterThan(float expected)
    {
      if ((actual < expected) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << expected << " to be greather than" << actual;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeLessThan(float expected)
    {
      if ((actual > expected) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << expected << " to be less than" << actual;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<float> *Not;

  private:
    bool negated;
    float actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class T>
  class Assertion<std::vector<T>>
  {
  public:
    Assertion(const char *file, int line, std::vector<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
        this->Not = new Assertion<std::vector<T>>(file, line, value, true);
      else
        this->negated = true;
    }

    void toBe(std::vector<T> expected)
    {
      if ((expected.size() != actual.size()) ^ negated)
      {
        std::stringstream message;
        message << "Vector sizes do not match, expected " << expected.size() << " items but had " << actual.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }

      bool any_item_differs = false;
      size_t found_difference = 0;

      for (size_t i = 0; i < expected.size(); ++i)
      {
        any_item_differs |= expected[i] != actual[i];

        if (expected[i] != actual[i])
          found_difference = i;
      }

      if (any_item_differs ^ negated)
      {
        std::stringstream message;
        message << "Vector item mismatch at position " << found_difference << ", expected " << expected[found_difference] << " but was " << actual[found_difference];
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::vector<T> expected)
    {
      toBe(expected);
    }

    void toContain(T item)
    {
      bool found = false;

      for (size_t i = 0; i < actual.size(); ++i)
      {
        if (actual[i] == item)
        {
          found = true;
          break;
        }
      }

      if (!found ^ negated)
      {
        std::stringstream message;
        message << "Item " << item << " not found in vector";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toHaveLength(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Vector sizes does not match, expected " << size << " items but had " << actual.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::vector<T>> *Not;

  private:
    bool negated;
    std::vector<T> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <>
  class Assertion<std::string>
  {
  public:
    Assertion(const char *file, int line, std::string value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
        this->Not = new Assertion<std::string>(file, line, value, true);
      else
        this->negated = true;
    }

    void toBe(std::string expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;

        if (expected.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH)
        {
          message << "String mismatch," << std::endl;
          message << "  Expected: \"" << expected << "\"" << std::endl;
          message << "  Actual  : \"" << actual << "\"";
        }
        else
        {
          message << "Expected \"" << expected << "\", was \"" << actual << "\"";
        }

        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toMatch(std::string expected)
    {
      toContain(expected);
    }

    void toMatch(std::string expected_string, std::regex expected)
    {
      if (!std::regex_search(actual, expected) ^ negated)
      {
        std::stringstream message;
        if (expected_string.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH)
        {
          message << "Expected pattern mismatch," << std::endl;
          message << "  Regexp: \"" << expected_string << "\"" << std::endl;
          message << "  Actual: \"" << actual << "\"";
        }
        else
        {
          message << "Expected pattern \"" << expected_string << "\" did not match with \"" << actual << "\"";
        }

        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::string expected)
    {
      toBe(expected);
    }

    void toContain(std::string expected)
    {
      if ((actual.find(expected) == std::string::npos) ^ negated)
      {
        std::stringstream message;

        if (expected.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH)
        {
          message << "Substring not found," << std::endl;
          message << "  Expected: \"" << expected << "\"" << std::endl;
          message << "  Actual  : \"" << actual << "\"";
        }
        else
        {
          message << "\"" << expected << "\" not present inside \"" << actual << "\"";
        }

        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toHaveLength(size_t length)
    {
      if ((actual.length() != length) ^ negated)
      {
        std::stringstream message;
        message << "Length of \"" << actual << "\" expected to be " << length << ", was " << actual.length();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::string> *Not;

  private:
    bool negated;
    std::string actual;
    std::string assertion_file;

    int assertion_line;
  };

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
