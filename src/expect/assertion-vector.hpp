#pragma once
#include <sstream>
#include <vector>
#include "assertion-base.hpp"

namespace cest
{
  template <class T>
  class Assertion<std::vector<T>>
  {
  public:
    Assertion(const char *file, int line, std::vector<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::vector<T>>(file, line, value, true);
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

    void toBe(std::vector<T> expected)
    {
      if (expected.size() != actual.size())
      {
        if (!negated)
        {
          std::stringstream message;
          message << "Expected vector of size " << actual.size() << " to have " << expected.size() << " items";
          throw AssertionError(assertion_file, assertion_line, message.str());
        }

        return;
      }

      bool any_item_differs = false;
      size_t found_difference = 0;

      for (size_t i = 0; i < expected.size(); ++i)
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

    void toEqual(std::vector<T> expected)
    {
      toBe(expected);
    }

    void toContain(T item)
    {
      bool found = false;

      for (size_t i = 0; i < actual.size(); ++i)
      {
        if (actual[i] == item)
        {
          found = true;
          break;
        }
      }

      if (!found ^ negated)
      {
        std::string message = "Expected vector" + std::string(negated ? " not" : "") + " to contain " + formatValue(item);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveLength(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected vector" << (negated ? " not" : "") << " to have length " << size << ", was " << actual.size();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::vector<T>> *Not;

  private:
    bool negated;
    std::vector<T> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
