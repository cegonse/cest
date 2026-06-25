#pragma once
#include <sstream>
#include <map>
#include <unordered_map>
#include "assertion-base.hpp"

namespace cest
{
  template <class K, class V>
  class Assertion<std::map<K, V>>
  {
  public:
    Assertion(const char *file, int line, std::map<K, V> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::map<K, V>>(file, line, value, true);
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

    void toBe(std::map<K, V> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Expected map of size " << actual.size() << (negated ? " not" : "") << " to have " << expected.size() << " entries";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::map<K, V> expected)
    {
      toBe(expected);
    }

    void toInclude(std::pair<K, V> entry)
    {
      auto it = actual.find(entry.first);
      bool found = it != actual.end() && it->second == entry.second;

      if (!found ^ negated)
      {
        std::string message = "Expected map" + std::string(negated ? " not" : "") + " to include {" + formatValue(entry.first) + ": " + formatValue(entry.second) + "}";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveKey(K key)
    {
      if (!actual.count(key) ^ negated)
      {
        std::string message = "Expected map" + std::string(negated ? " not" : "") + " to have key " + formatValue(key);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected map" << (negated ? " not" : "") << " to have size " << size << ", was " << actual.size();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::map<K, V>> *Not;

  private:
    bool negated;
    std::map<K, V> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class K, class V>
  class Assertion<std::unordered_map<K, V>>
  {
  public:
    Assertion(const char *file, int line, std::unordered_map<K, V> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::unordered_map<K, V>>(file, line, value, true);
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

    void toBe(std::unordered_map<K, V> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Expected unordered_map of size " << actual.size() << (negated ? " not" : "") << " to have " << expected.size() << " entries";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::unordered_map<K, V> expected)
    {
      toBe(expected);
    }

    void toInclude(std::pair<K, V> entry)
    {
      auto it = actual.find(entry.first);
      bool found = it != actual.end() && it->second == entry.second;

      if (!found ^ negated)
      {
        std::string message = "Expected unordered_map" + std::string(negated ? " not" : "") + " to include {" + formatValue(entry.first) + ": " + formatValue(entry.second) + "}";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveKey(K key)
    {
      if (!actual.count(key) ^ negated)
      {
        std::string message = "Expected unordered_map" + std::string(negated ? " not" : "") + " to have key " + formatValue(key);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Expected unordered_map" << (negated ? " not" : "") << " to have size " << size << ", was " << actual.size();
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::unordered_map<K, V>> *Not;

  private:
    bool negated;
    std::unordered_map<K, V> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
