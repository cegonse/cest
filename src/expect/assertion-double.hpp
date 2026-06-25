#pragma once
#include <sstream>
#include <cmath>
#include "assertion-base.hpp"

namespace cest
{
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
      {
        this->Not = new Assertion<double>(file, line, value, true);
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

    void toBe(double expected, double epsilon = 0.0000001)
    {
      if ((fabs(actual - expected) > epsilon) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << actual << (negated ? " not" : "") << " to be " << expected;
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
        message << "Expected " << actual << (negated ? " not" : "") << " to be greater than " << expected;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeLessThan(double expected)
    {
      if ((actual > expected) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << actual << (negated ? " not" : "") << " to be less than " << expected;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toBeInRange(double min, double max)
    {
      bool in_range = actual >= min && actual <= max;
      if (!in_range ^ negated)
      {
        std::stringstream message;
        message << "Expected " << actual << (negated ? " not" : "") << " to be in range [" << min << ", " << max << "]";
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
}
