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
          message << "Expected string" << (negated ? " not" : "") << " to be" << std::endl;
          message << "  Expected: \"" << expected << "\"" << std::endl;
          message << "  Actual:   \"" << actual << "\"";
        }
        else
        {
          message << "Expected \"" << actual << "\"" << (negated ? " not" : "") << " to be \"" << expected << "\"";
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
          message << "Expected string" << (negated ? " not" : "") << " to match pattern" << std::endl;
          message << "  Pattern: \"" << expected_string << "\"" << std::endl;
          message << "  Actual:  \"" << actual << "\"";
        }
        else
        {
          message << "Expected \"" << actual << "\"" << (negated ? " not" : "") << " to match pattern \"" << expected_string << "\"";
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
          message << "Expected string" << (negated ? " not" : "") << " to contain" << std::endl;
          message << "  Substring: \"" << expected << "\"" << std::endl;
          message << "  Actual:    \"" << actual << "\"";
        }
        else
        {
          message << "Expected \"" << actual << "\"" << (negated ? " not" : "") << " to contain \"" << expected << "\"";
        }

        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toHaveLength(size_t length)
    {
      if ((actual.length() != length) ^ negated)
      {
        std::stringstream message;
        message << "Expected \"" << actual << "\"" << (negated ? " not" : "") << " to have length " << length << ", was " << actual.length();
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
