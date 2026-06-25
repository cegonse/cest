#pragma once
#include <sstream>
#include <list>
#include <forward_list>
#include <iterator>
#include "assertion-base.hpp"

namespace cest
{
  template <class T>
  class Assertion<std::list<T>>
  {
  public:
    Assertion(const char *file, int line, std::list<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::list<T>>(file, line, value, true);
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

    void toBe(std::list<T> expected)
    {
      if (expected.size() != actual.size())
      {
        if (!negated)
        {
          std::stringstream message;
          message << "Expected list of size " << actual.size() << " to have " << expected.size() << " items";
          throw AssertionError(assertion_file, assertion_line, message.str());
        }

        return;
      }

      auto it_expected = expected.begin();
      auto it_actual = actual.begin();
      size_t position = 0;

      while (it_expected != expected.end())
      {
        if (*it_expected != *it_actual)
        {
          if (!negated)
          {
            std::string message = "Expected " + formatValue(*it_actual) + " at position " + std::to_string(position) + " to be " + formatValue(*it_expected);
            throw AssertionError(assertion_file, assertion_line, message);
          }

          return;
        }

        ++it_expected;
        ++it_actual;
        ++position;
      }

      if (negated)
      {
        throw AssertionError(assertion_file, assertion_line, "Expected list not to be equal");
      }
    }

    void toEqual(std::list<T> expected)
    {
      toBe(expected);
    }

    void toContain(T item)
    {
      bool found = false;

      for (const auto &element : actual)
      {
        if (element == item)
        {
          found = true;
          break;
        }
      }

      if (!found ^ negated)
      {
        std::string message = "Expected list" + std::string(negated ? " not" : "") + " to contain " + formatValue(item);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected list" << (negated ? " not" : "") << " to have size " << size << ", was " << actual.size();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::list<T>> *Not;

  private:
    bool negated;
    std::list<T> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class T>
  class Assertion<std::forward_list<T>>
  {
  public:
    Assertion(const char *file, int line, std::forward_list<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::forward_list<T>>(file, line, value, true);
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

    void toBe(std::forward_list<T> expected)
    {
      auto it_expected = expected.begin();
      auto it_actual = actual.begin();
      size_t position = 0;

      while (it_expected != expected.end() && it_actual != actual.end())
      {
        if (*it_expected != *it_actual)
        {
          if (!negated)
          {
            std::string message = "Expected " + formatValue(*it_actual) + " at position " + std::to_string(position) + " to be " + formatValue(*it_expected);
            throw AssertionError(assertion_file, assertion_line, message);
          }

          return;
        }

        ++it_expected;
        ++it_actual;
        ++position;
      }

      bool same_length = (it_expected == expected.end() && it_actual == actual.end());

      if (!same_length)
      {
        if (!negated)
        {
          size_t expected_size = std::distance(expected.begin(), expected.end());
          size_t actual_size = std::distance(actual.begin(), actual.end());
          std::stringstream message;
          message << "Expected forward_list of size " << actual_size << " to have " << expected_size << " items";
          throw AssertionError(assertion_file, assertion_line, message.str());
        }

        return;
      }

      if (negated)
      {
        throw AssertionError(assertion_file, assertion_line, "Expected forward_list not to be equal");
      }
    }

    void toEqual(std::forward_list<T> expected)
    {
      toBe(expected);
    }

    void toContain(T item)
    {
      bool found = false;

      for (const auto &element : actual)
      {
        if (element == item)
        {
          found = true;
          break;
        }
      }

      if (!found ^ negated)
      {
        std::string message = "Expected forward_list" + std::string(negated ? " not" : "") + " to contain " + formatValue(item);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      size_t actual_size = std::distance(actual.begin(), actual.end());

      if ((actual_size != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected forward_list" << (negated ? " not" : "") << " to have size " << size << ", was " << actual_size;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::forward_list<T>> *Not;

  private:
    bool negated;
    std::forward_list<T> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
