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
        message << "Map mismatch, expected " << expected.size() << " entries but had " << actual.size() << " entries";
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
        std::string message = "Entry {" + formatValue(entry.first) + ": " + formatValue(entry.second) + "} not found in map";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveKey(K key)
    {
      if (!actual.count(key) ^ negated)
      {
        std::string message = "Key " + formatValue(key) + " not found in map";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Map size does not match, expected " << size << " entries but had " << actual.size() << " entries";
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
        message << "Unordered map mismatch, expected " << expected.size() << " entries but had " << actual.size() << " entries";
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
        std::string message = "Entry {" + formatValue(entry.first) + ": " + formatValue(entry.second) + "} not found in unordered map";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveKey(K key)
    {
      if (!actual.count(key) ^ negated)
      {
        std::string message = "Key " + formatValue(key) + " not found in unordered map";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Unordered map size does not match, expected " << size << " entries but had " << actual.size() << " entries";
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
