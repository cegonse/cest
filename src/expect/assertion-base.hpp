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

        std::string message = "Expected " + formatValue(expected) + ", was " + formatValue(actual);
        throw AssertionError(assertion_file, assertion_line, message);
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
}
