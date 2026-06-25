#pragma once
#include <sstream>
#include <utility>
#include <tuple>
#include "assertion-base.hpp"

namespace cest
{
  template <class T, class U>
  class Assertion<std::pair<T, U>>
  {
  public:
    Assertion(const char *file, int line, std::pair<T, U> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::pair<T, U>>(file, line, value, true);
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

    void toBe(std::pair<T, U> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::string message;
        if (expected.first != actual.first)
          message = "Expected pair first element " + formatValue(actual.first) + std::string(negated ? " not" : "") + " to be " + formatValue(expected.first);
        else
          message = "Expected pair second element " + formatValue(actual.second) + std::string(negated ? " not" : "") + " to be " + formatValue(expected.second);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toEqual(std::pair<T, U> expected)
    {
      toBe(expected);
    }

    Assertion<std::pair<T, U>> *Not;

  private:
    bool negated;
    std::pair<T, U> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class... Ts>
  class Assertion<std::tuple<Ts...>>
  {
  public:
    Assertion(const char *file, int line, std::tuple<Ts...> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::tuple<Ts...>>(file, line, value, true);
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

    void toBe(std::tuple<Ts...> expected)
    {
      if ((expected != actual) ^ negated)
      {
        size_t mismatch_index = findMismatchIndex(expected, std::index_sequence_for<Ts...>{});
        std::stringstream message;
        message << "Expected tuple" << (negated ? " not" : "") << " to match at position " << mismatch_index;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::tuple<Ts...> expected)
    {
      toBe(expected);
    }

    Assertion<std::tuple<Ts...>> *Not;

  private:
    bool negated;
    std::tuple<Ts...> actual;
    std::string assertion_file;
    int assertion_line;

    template <std::size_t... Is>
    size_t findMismatchIndex(const std::tuple<Ts...> &expected, std::index_sequence<Is...>)
    {
      size_t index = 0;
      ((std::get<Is>(expected) != std::get<Is>(actual) ? (index = Is, true) : false) || ...);
      return index;
    }
  };
}
