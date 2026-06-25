#pragma once
#include <sstream>
#include "assertion-base.hpp"

namespace cest
{
  template <class T, std::size_t N>
  class Assertion<T[N]>
  {
  public:
    Assertion(const char *file, int line, const T (&value)[N], bool negated = false) : negated(false)
    {
      for (size_t i = 0; i < N; ++i)
        actual[i] = value[i];

      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<T[N]>(file, line, value, true);
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

    void toBe(const T (&expected)[N])
    {
      bool any_item_differs = false;
      size_t found_difference = 0;

      for (size_t i = 0; i < N; ++i)
      {
        any_item_differs |= expected[i] != actual[i];

        if (expected[i] != actual[i])
          found_difference = i;
      }

      if (any_item_differs ^ negated)
      {
        std::string message = "Expected " + formatValue(actual[found_difference]) + " at position " + std::to_string(found_difference) + (negated ? " not" : "") + " to be " + formatValue(expected[found_difference]);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toEqual(const T (&expected)[N])
    {
      toBe(expected);
    }

    void toContain(T item)
    {
      bool found = false;

      for (size_t i = 0; i < N; ++i)
      {
        if (actual[i] == item)
        {
          found = true;
          break;
        }
      }

      if (!found ^ negated)
      {
        std::string message = "Expected array" + std::string(negated ? " not" : "") + " to contain " + formatValue(item);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveLength(size_t size)
    {
      if ((N != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected array" << (negated ? " not" : "") << " to have length " << size << ", was " << N;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<T[N]> *Not;

  private:
    bool negated;
    T actual[N];
    std::string assertion_file;
    int assertion_line;
  };
}
