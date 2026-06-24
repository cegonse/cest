#pragma once
#include <sstream>
#include <chrono>
#include <cmath>
#include "assertion-base.hpp"

namespace cest
{
  template <class Rep, class Period>
  std::string formatDuration(std::chrono::duration<Rep, Period> d)
  {
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(d).count();

    std::stringstream ss;

    if (ns == 0)
      ss << "0s";
    else if (std::abs(ns) >= 1000000000)
      ss << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() / 1000.0 << "s";
    else if (std::abs(ns) >= 1000000)
      ss << std::chrono::duration_cast<std::chrono::microseconds>(d).count() / 1000.0 << "ms";
    else if (std::abs(ns) >= 1000)
      ss << ns / 1000.0 << "us";
    else
      ss << ns << "ns";

    return ss.str();
  }

  template <class Rep, class Period>
  class Assertion<std::chrono::duration<Rep, Period>>
  {
  public:
    using DurationType = std::chrono::duration<Rep, Period>;

    Assertion(const char *file, int line, DurationType value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<DurationType>(file, line, value, true);
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

    void toBe(DurationType expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::string message = "Expected " + formatDuration(expected) + ", was " + formatDuration(actual);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toEqual(DurationType expected)
    {
      toBe(expected);
    }

    void toBeGreaterThan(DurationType expected)
    {
      if ((actual <= expected) ^ negated)
      {
        std::string message = "Expected " + formatDuration(actual) + " to be greater than " + formatDuration(expected);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeLessThan(DurationType expected)
    {
      if ((actual >= expected) ^ negated)
      {
        std::string message = "Expected " + formatDuration(actual) + " to be less than " + formatDuration(expected);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeCloseTo(DurationType expected, DurationType tolerance)
    {
      auto diff = actual - expected;
      auto abs_diff = diff.count() < 0 ? -diff : diff;
      bool within = abs_diff <= tolerance;

      if (!within ^ negated)
      {
        std::string message = "Expected " + formatDuration(actual) + " to be within " + formatDuration(tolerance) + " of " + formatDuration(expected);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    Assertion<DurationType> *Not;

  private:
    bool negated;
    DurationType actual;
    std::string assertion_file;
    int assertion_line;
  };
}
