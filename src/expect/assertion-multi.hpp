#pragma once
#include <sstream>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "assertion-base.hpp"

namespace cest
{
  template <class T>
  class Assertion<std::multiset<T>>
  {
  public:
    Assertion(const char *file, int line, std::multiset<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::multiset<T>>(file, line, value, true);
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

    void toBe(std::multiset<T> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Multiset mismatch, expected " << expected.size() << " items but had " << actual.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::multiset<T> expected)
    {
      toBe(expected);
    }

    void toInclude(T item)
    {
      if (!actual.count(item) ^ negated)
      {
        std::string message = "Item " + formatValue(item) + " not found in multiset";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Multiset size does not match, expected " << size << " items but had " << actual.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::multiset<T>> *Not;

  private:
    bool negated;
    std::multiset<T> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class T>
  class Assertion<std::unordered_multiset<T>>
  {
  public:
    Assertion(const char *file, int line, std::unordered_multiset<T> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::unordered_multiset<T>>(file, line, value, true);
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

    void toBe(std::unordered_multiset<T> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Unordered multiset mismatch, expected " << expected.size() << " items but had " << actual.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::unordered_multiset<T> expected)
    {
      toBe(expected);
    }

    void toInclude(T item)
    {
      if (!actual.count(item) ^ negated)
      {
        std::string message = "Item " + formatValue(item) + " not found in unordered multiset";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Unordered multiset size does not match, expected " << size << " items but had " << actual.size() << " items";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::unordered_multiset<T>> *Not;

  private:
    bool negated;
    std::unordered_multiset<T> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class K, class V>
  class Assertion<std::multimap<K, V>>
  {
  public:
    Assertion(const char *file, int line, std::multimap<K, V> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::multimap<K, V>>(file, line, value, true);
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

    void toBe(std::multimap<K, V> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Multimap mismatch, expected " << expected.size() << " entries but had " << actual.size() << " entries";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::multimap<K, V> expected)
    {
      toBe(expected);
    }

    void toInclude(std::pair<K, V> entry)
    {
      bool found = false;
      auto range = actual.equal_range(entry.first);

      for (auto it = range.first; it != range.second; ++it)
      {
        if (it->second == entry.second)
        {
          found = true;
          break;
        }
      }

      if (!found ^ negated)
      {
        std::string message = "Entry {" + formatValue(entry.first) + ": " + formatValue(entry.second) + "} not found in multimap";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveKey(K key)
    {
      if (!actual.count(key) ^ negated)
      {
        std::string message = "Key " + formatValue(key) + " not found in multimap";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Multimap size does not match, expected " << size << " entries but had " << actual.size() << " entries";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::multimap<K, V>> *Not;

  private:
    bool negated;
    std::multimap<K, V> actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class K, class V>
  class Assertion<std::unordered_multimap<K, V>>
  {
  public:
    Assertion(const char *file, int line, std::unordered_multimap<K, V> value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::unordered_multimap<K, V>>(file, line, value, true);
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

    void toBe(std::unordered_multimap<K, V> expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::stringstream message;
        message << "Unordered multimap mismatch, expected " << expected.size() << " entries but had " << actual.size() << " entries";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    void toEqual(std::unordered_multimap<K, V> expected)
    {
      toBe(expected);
    }

    void toInclude(std::pair<K, V> entry)
    {
      bool found = false;
      auto range = actual.equal_range(entry.first);

      for (auto it = range.first; it != range.second; ++it)
      {
        if (it->second == entry.second)
        {
          found = true;
          break;
        }
      }

      if (!found ^ negated)
      {
        std::string message = "Entry {" + formatValue(entry.first) + ": " + formatValue(entry.second) + "} not found in unordered multimap";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveKey(K key)
    {
      if (!actual.count(key) ^ negated)
      {
        std::string message = "Key " + formatValue(key) + " not found in unordered multimap";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveSize(size_t size)
    {
      if ((actual.size() != size) ^ negated)
      {
        std::stringstream message;
        message << "Unordered multimap size does not match, expected " << size << " entries but had " << actual.size() << " entries";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    Assertion<std::unordered_multimap<K, V>> *Not;

  private:
    bool negated;
    std::unordered_multimap<K, V> actual;
    std::string assertion_file;
    int assertion_line;
  };
}
