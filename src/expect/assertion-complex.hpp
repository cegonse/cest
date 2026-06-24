#pragma once
#include <sstream>
#include <complex>
#include <cmath>
#include "assertion-base.hpp"

namespace cest
{
  template <class T>
  class Assertion<std::complex<T>>
  {
  public:
    Assertion(const char *file, int line, std::complex<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::complex<T>>(file, line, value, true);
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

    void toBe(std::complex<T> expected, T epsilon = static_cast<T>(0.0000001))
    {
      bool real_matches = std::fabs(actual.real() - expected.real()) <= epsilon;
      bool imag_matches = std::fabs(actual.imag() - expected.imag()) <= epsilon;

      if (!(real_matches && imag_matches) ^ negated)
      {
        std::stringstream message;
        message << "Expected (" << expected.real() << " + " << expected.imag() << "i)";
        message << ", was (" << actual.real() << " + " << actual.imag() << "i)";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::complex<T> expected, T epsilon = static_cast<T>(0.0000001))
    {
      toBe(expected, epsilon);
    }

    void toHaveReal(T expected, T epsilon = static_cast<T>(0.0000001))
    {
      if ((std::fabs(actual.real() - expected) > epsilon) ^ negated)
      {
        std::stringstream message;
        message << "Expected real part " << expected << ", was " << actual.real();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toHaveImaginary(T expected, T epsilon = static_cast<T>(0.0000001))
    {
      if ((std::fabs(actual.imag() - expected) > epsilon) ^ negated)
      {
        std::stringstream message;
        message << "Expected imaginary part " << expected << ", was " << actual.imag();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::complex<T>> *Not;

  private:
    bool negated;
    std::complex<T> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
