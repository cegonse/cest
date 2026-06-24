#pragma once
#include <functional>
#include <string>
#include "../types.hpp"

namespace cest
{
  class CallableAssertion
  {
  public:
    CallableAssertion(const char *file, int line, std::function<void()> fn, bool negated = false)
      : fn(fn), assertion_file(file), assertion_line(line), negated(negated)
    {
      if (!negated)
      {
        this->Not = new CallableAssertion(file, line, fn, true);
      }
      else
      {
        this->Not = nullptr;
        this->negated = true;
      }
    }

    ~CallableAssertion()
    {
      if (this->Not)
        delete this->Not;
    }

    void toThrow()
    {
      bool threw = false;
      try { fn(); }
      catch (...) { threw = true; }

      if (!threw ^ negated)
      {
        std::string message = negated
          ? "Expected function not to throw"
          : "Expected function to throw";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    template <class E>
    void toThrow()
    {
      bool threw_expected = false;
      bool threw_something = false;
      try { fn(); }
      catch (const E&) { threw_expected = true; threw_something = true; }
      catch (...) { threw_something = true; }

      if (!threw_expected ^ negated)
      {
        std::string message;
        if (negated)
          message = "Expected function not to throw specific exception type";
        else if (threw_something)
          message = "Expected function to throw specific exception type, but a different exception was thrown";
        else
          message = "Expected function to throw, but no exception was thrown";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toThrowMessage(std::string expected_message)
    {
      std::string actual_message;
      bool threw = false;
      try { fn(); }
      catch (const std::exception& e) { threw = true; actual_message = e.what(); }
      catch (...) { threw = true; }

      bool matches = threw && actual_message == expected_message;
      if (!matches ^ negated)
      {
        std::string message;
        if (negated)
          message = "Expected function not to throw with message \"" + expected_message + "\"";
        else if (!threw)
          message = "Expected function to throw, but no exception was thrown";
        else
          message = "Expected exception message \"" + expected_message + "\", got \"" + actual_message + "\"";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    CallableAssertion *Not;

  private:
    std::function<void()> fn;
    std::string assertion_file;
    int assertion_line;
    bool negated;
  };
}
