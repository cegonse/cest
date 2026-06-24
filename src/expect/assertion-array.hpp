#pragma once
#include <sstream>
#include <array>
#include "assertion-base.hpp"

namespace cest
{
  template <class T, std::size_t N>
  class Assertion<std::array<T, N>>
  {
  public:
    Assertion(const char *file, int line, std::array<T, N> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::array<T, N>>(file, line, value, true);
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

    void toBe(std::array<T, N> expected)
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
        std::string message = "Array item mismatch at position " + std::to_string(found_difference) + ", expected " + formatValue(expected[found_difference]) + " but was " + formatValue(actual[found_difference]);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toEqual(std::array<T, N> expected)
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
        std::string message = "Item " + formatValue(item) + " not found in array";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveLength(size_t size)
    {
      if ((N != size) ^ negated)
      {
        std::stringstream message;
        message << "Array length does not match, expected " << size << " items but had " << N << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::array<T, N>> *Not;

  private:
    bool negated;
    std::array<T, N> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
