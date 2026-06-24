#pragma once
#include <sstream>
#include <string>
#include <regex>
#include "assertion-base.hpp"

#define CLIP_STRING_LENGTH 16

namespace cest
{
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
      {
        this->Not = new Assertion<std::string>(file, line, value, true);
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
}
