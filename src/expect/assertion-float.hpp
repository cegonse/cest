#pragma once
#include <sstream>
#include <cmath>
#include "assertion-base.hpp"

namespace cest
{
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
      {
        this->Not = new Assertion<float>(file, line, value, true);
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
}
