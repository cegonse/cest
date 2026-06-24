#pragma once
#include <sstream>
#include <optional>
#include "assertion-base.hpp"

namespace cest
{
  template <class T>
  class Assertion<std::optional<T>>
  {
  public:
    Assertion(const char *file, int line, std::optional<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::optional<T>>(file, line, value, true);
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

    void toBe(std::optional<T> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        if (!expected.has_value())
          message << "Expected std::nullopt, was " << formatValue(*actual);
        else if (!actual.has_value())
          message << "Expected " << formatValue(*expected) << ", was std::nullopt";
        else
          message << "Expected " << formatValue(*expected) << ", was " << formatValue(*actual);
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::optional<T> expected)
    {
      toBe(expected);
    }

    void toHaveValue()
    {
      if (!actual.has_value() ^ negated)
      {
        throw AssertionError(assertion_file, assertion_line, "Expected optional to have a value");
      }
    }

    void toHaveValue(T expected)
    {
      bool matches = actual.has_value() && *actual == expected;

      if (!matches ^ negated)
      {
        std::stringstream message;
        if (!actual.has_value())
          message << "Expected optional with value " << formatValue(expected) << ", was std::nullopt";
        else
          message << "Expected optional with value " << formatValue(expected) << ", was " << formatValue(*actual);
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeEmpty()
    {
      if (actual.has_value() ^ negated)
      {
        std::string message = "Expected optional to be empty, but had value " + formatValue(*actual);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    Assertion<std::optional<T>> *Not;

  private:
    bool negated;
    std::optional<T> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
