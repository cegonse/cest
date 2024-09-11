#pragma once
#include <vector>
#include <functional>

namespace cest
{
  template <class T>
  class Parameter
  {
  public:
    Parameter() {}

    Parameter<T> withValue(T v)
    {
      values.push_back(v);
      return *this;
    }

    void thenDo(std::function<void(T)> call)
    {
      for (T v : values)
        call(v);
    }

  private:
    std::vector<T> values;
  };

  template <class T>
  Parameter<T> withParameter()
  {
    return Parameter<T>();
  }
}