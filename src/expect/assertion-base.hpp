#pragma once
#include <sstream>
#include "../types.hpp"

namespace cest
{
  template <typename T, typename = void>
  struct is_streamable : std::false_type {};

  template <typename T>
  struct is_streamable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>
    : std::true_type {};

  template <typename T>
  std::string formatValue(const T& value)
  {
    if constexpr (is_streamable<T>::value)
    {
      std::stringstream ss;
      ss << value;
      return ss.str();
    }
    else
    {
      return "<non-printable>";
    }
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
      {
        this->Not = new Assertion<T>(file, line, value, true);
      }
      else
      {
        this->Not = nullptr;
        this->negated = true;
      }
    }

    ~Assertion()
    {
      if (this->Not)
        delete this->Not;
    }

    void toBeTruthy()
    {
      if (!actual ^ this->negated)
      {
        std::string message = "Expected " + formatValue(actual) + (negated ? " not" : "") + " to be truthy";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeFalsy()
    {
      if (actual ^ this->negated)
      {
        std::string message = "Expected " + formatValue(actual) + (negated ? " not" : "") + " to be falsy";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBe(T expected)
    {
      if ((expected != actual) ^ this->negated)
      {
        std::string message = "Expected " + formatValue(actual) + (negated ? " not" : "") + " to be " + formatValue(expected);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toEqual(T expected)
    {
      toBe(expected);
    }

    void toBeGreaterThan(T expected) requires requires(T a, T b) { { a > b }; }
    {
      if (!(actual > expected) ^ negated)
      {
        std::string message = "Expected " + formatValue(actual) + (negated ? " not" : "") + " to be greater than " + formatValue(expected);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeLessThan(T expected) requires requires(T a, T b) { { a < b }; }
    {
      if (!(actual < expected) ^ negated)
      {
        std::string message = "Expected " + formatValue(actual) + (negated ? " not" : "") + " to be less than " + formatValue(expected);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeInRange(T min, T max) requires requires(T a, T b) { { a >= b }; { a <= b }; }
    {
      bool in_range = actual >= min && actual <= max;
      if (!in_range ^ negated)
      {
        std::string message = "Expected " + formatValue(actual) + (negated ? " not" : "") + " to be in range [" + formatValue(min) + ", " + formatValue(max) + "]";
        throw AssertionError(assertion_file, assertion_line, message);
      }
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
        if (negated)
          message << "Expected memory not to match";
        else
          message << "Expected byte " << i << " to be 0x" << std::hex << std::uppercase << (int)expected[i] << ", was 0x" << std::hex << std::uppercase << (int)actual[i];
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeNotNull()
    {
      if ((actual == NULL) ^ negated)
      {
        std::string message = std::string("Expected value") + (negated ? "" : " not") + " to be null";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeNull()
    {
      if ((actual != NULL) ^ negated)
      {
        std::stringstream message;
        message << "Expected 0x" << std::hex << std::uppercase << actual << (negated ? " not" : "") << " to be null";
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
}
