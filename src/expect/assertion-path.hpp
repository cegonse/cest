#pragma once
#include <sstream>
#include <filesystem>
#include "assertion-base.hpp"

namespace cest
{
  template <>
  class Assertion<std::filesystem::path>
  {
  public:
    Assertion(const char *file, int line, std::filesystem::path value, bool negated = false) : negated(false)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;

      if (!negated)
      {
        this->Not = new Assertion<std::filesystem::path>(file, line, value, true);
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

    void toBe(std::filesystem::path expected)
    {
      if ((expected != actual) ^ negated)
      {
        std::string message = "Expected path \"" + actual.string() + "\"" + (negated ? " not" : "") + " to be \"" + expected.string() + "\"";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toEqual(std::filesystem::path expected)
    {
      toBe(expected);
    }

    void toHaveExtension(std::string ext)
    {
      std::string actual_ext = actual.extension().string();

      if ((actual_ext != ext) ^ negated)
      {
        std::string message = "Expected extension \"" + actual_ext + "\"" + (negated ? " not" : "") + " to be \"" + ext + "\" in path \"" + actual.string() + "\"";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toHaveFilename(std::string name)
    {
      std::string actual_name = actual.filename().string();

      if ((actual_name != name) ^ negated)
      {
        std::string message = "Expected filename \"" + actual_name + "\"" + (negated ? " not" : "") + " to be \"" + name + "\" in path \"" + actual.string() + "\"";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeAbsolute()
    {
      if (!actual.is_absolute() ^ negated)
      {
        std::string message = "Expected path \"" + actual.string() + "\"" + (negated ? " not" : "") + " to be absolute";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    void toBeRelative()
    {
      if (!actual.is_relative() ^ negated)
      {
        std::string message = "Expected path \"" + actual.string() + "\"" + (negated ? " not" : "") + " to be relative";
        throw AssertionError(assertion_file, assertion_line, message);
      }
    }

    Assertion<std::filesystem::path> *Not;

  private:
    bool negated;
    std::filesystem::path actual;
    std::string assertion_file;
    int assertion_line;
  };
}
