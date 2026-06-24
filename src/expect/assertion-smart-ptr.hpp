#pragma once
#include <sstream>
#include <memory>
#include "assertion-base.hpp"

namespace cest
{
  template <class T>
  class AssertionSharedPtr
  {
  public:
    AssertionSharedPtr(const char *file, int line, T *raw, long use_count, bool negated = false) : negated(false)
    {
      actual = raw;
      actual_use_count = use_count;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new AssertionSharedPtr<T>(file, line, raw, use_count, true);
      }
      else
      {
        this->Not = nullptr;
        this->negated = true;
      }
    }

    ~AssertionSharedPtr()
    {
      if (this->Not)
        delete this->Not;
    }

    void toBeNull()
    {
      if ((actual != nullptr) ^ negated)
      {
        throw AssertionError(assertion_file, assertion_line, "Expected shared_ptr to be null");
      }
    }

    void toBeNotNull()
    {
      if ((actual == nullptr) ^ negated)
      {
        throw AssertionError(assertion_file, assertion_line, "Expected shared_ptr to be not null");
      }
    }

    void toPointTo(T expected)
    {
      if (actual == nullptr)
      {
        if (!negated)
          throw AssertionError(assertion_file, assertion_line, "Expected shared_ptr to point to " + formatValue(expected) + ", but was null");
        return;
      }

      if ((*actual != expected) ^ negated)
      {
        std::string message = "Expected shared_ptr to point to " + formatValue(expected) + ", was " + formatValue(*actual);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveUseCount(long expected)
    {
      if ((actual_use_count != expected) ^ negated)
      {
        std::stringstream message;
        message << "Expected use_count " << expected << ", was " << actual_use_count;
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

    AssertionSharedPtr<T> *Not;

  private:
    bool negated;
    T *actual;
    long actual_use_count;
    std::string assertion_file;
    int assertion_line;
  };

  template <class T>
  AssertionSharedPtr<T> expectFunction(const char *file, int line, const std::shared_ptr<T> &actual)
  {
    return AssertionSharedPtr<T>(file, line, actual.get(), actual.use_count());
  }

  template <class T>
  class AssertionUniquePtr
  {
  public:
    AssertionUniquePtr(const char *file, int line, T *raw, bool negated = false) : negated(false)
    {
      actual = raw;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new AssertionUniquePtr<T>(file, line, raw, true);
      }
      else
      {
        this->Not = nullptr;
        this->negated = true;
      }
    }

    ~AssertionUniquePtr()
    {
      if (this->Not)
        delete this->Not;
    }

    void toBeNull()
    {
      if ((actual != nullptr) ^ negated)
      {
        throw AssertionError(assertion_file, assertion_line, "Expected unique_ptr to be null");
      }
    }

    void toBeNotNull()
    {
      if ((actual == nullptr) ^ negated)
      {
        throw AssertionError(assertion_file, assertion_line, "Expected unique_ptr to be not null");
      }
    }

    void toPointTo(T expected)
    {
      if (actual == nullptr)
      {
        if (!negated)
          throw AssertionError(assertion_file, assertion_line, "Expected unique_ptr to point to " + formatValue(expected) + ", but was null");
        return;
      }

      if ((*actual != expected) ^ negated)
      {
        std::string message = "Expected unique_ptr to point to " + formatValue(expected) + ", was " + formatValue(*actual);
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    AssertionUniquePtr<T> *Not;

  private:
    bool negated;
    T *actual;
    std::string assertion_file;
    int assertion_line;
  };

  template <class T>
  AssertionUniquePtr<T> expectFunction(const char *file, int line, const std::unique_ptr<T> &actual)
  {
    return AssertionUniquePtr<T>(file, line, actual.get());
  }
}
