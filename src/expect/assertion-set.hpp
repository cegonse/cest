#pragma once
#include <sstream>
#include <set>
#include <unordered_set>
#include "assertion-base.hpp"

namespace cest
{
  template <class T>
  class Assertion<std::set<T>>
  {
  public:
    Assertion(const char *file, int line, std::set<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::set<T>>(file, line, value, true);
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

    void toBe(std::set<T> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Expected set of size " << actual.size() << (negated ? " not" : "") << " to be " << expected.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::set<T> expected)
    {
      toBe(expected);
    }

    void toInclude(T item)
    {
      if (!actual.count(item) ^ negated)
      {
        std::string message = "Expected set" + std::string(negated ? " not" : "") + " to include " + formatValue(item);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected set" << (negated ? " not" : "") << " to have size " << size << ", was " << actual.size();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::set<T>> *Not;

  private:
    bool negated;
    std::set<T> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class T>
  class Assertion<std::unordered_set<T>>
  {
  public:
    Assertion(const char *file, int line, std::unordered_set<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::unordered_set<T>>(file, line, value, true);
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

    void toBe(std::unordered_set<T> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Expected unordered_set of size " << actual.size() << (negated ? " not" : "") << " to be " << expected.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::unordered_set<T> expected)
    {
      toBe(expected);
    }

    void toInclude(T item)
    {
      if (!actual.count(item) ^ negated)
      {
        std::string message = "Expected unordered_set" + std::string(negated ? " not" : "") + " to include " + formatValue(item);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected unordered_set" << (negated ? " not" : "") << " to have size " << size << ", was " << actual.size();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::unordered_set<T>> *Not;

  private:
    bool negated;
    std::unordered_set<T> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
