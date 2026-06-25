#pragma once
#include <sstream>
#include <bitset>
#include "assertion-base.hpp"

namespace cest
{
  template <std::size_t N>
  class Assertion<std::bitset<N>>
  {
  public:
    Assertion(const char *file, int line, std::bitset<N> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::bitset<N>>(file, line, value, true);
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

    void toBe(std::bitset<N> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::string message = "Expected " + actual.to_string() + std::string(negated ? " not" : "") + " to be " + expected.to_string();
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toEqual(std::bitset<N> expected)
    {
      toBe(expected);
    }

    void toHaveBitSet(size_t pos)
    {
      if (!actual.test(pos) ^ negated)
      {
        std::stringstream message;
        message << "Expected bit " << pos << (negated ? " not" : "") << " to be set in " << actual.to_string();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toHaveCount(size_t n)
    {
      if ((actual.count() != n) ^ negated)
      {
        std::stringstream message;
        message << "Expected " << actual.count() << " bits" << (negated ? " not" : "") << " to be " << n << " in " << actual.to_string();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toHaveAll()
    {
      if (!actual.all() ^ negated)
      {
        std::string message = std::string("Expected") + (negated ? " not" : "") + " all bits set in " + actual.to_string();
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveNone()
    {
      if (!actual.none() ^ negated)
      {
        std::string message = std::string("Expected") + (negated ? " not" : "") + " no bits set in " + actual.to_string();
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    Assertion<std::bitset<N>> *Not;

  private:
    bool negated;
    std::bitset<N> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
