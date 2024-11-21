#include <cest>

struct Color
{
  Color(int r, int g, int b, int a)
  {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }

  Color() {}

  int r;
  int g;
  int b;
  int a;
};

namespace cest
{
  template<>
  class Assertion<Color>
  {
  public:
    Assertion(const char *file, int line, Color value)
    {
      actual = value;
      assertion_file = std::string(file);
      assertion_line = line;
    }

    void toBeTransparent()
    {
      if (actual.a != 0)
      {
        std::stringstream message;
        message << "Color (" << actual.r << "," << actual.g << "," << actual.b << "," << actual.a << ") is not transparent";
        throw AssertionError(assertion_file, assertion_line, message.str());
      }
    }

  private:
    Color actual;
    std::string assertion_file;
    int assertion_line;
  };

  template<>
  Assertion<Color> expectFunction(const char *file, int line, Color actual)
  {
    return Assertion<Color>(file, line, actual);
  }
}

describe("creating custom assertions", []() {
  it("overrides default expect() behaviour", []() {
    Color color(34, 12, 64, 0);
    expect(color).toBeTransparent();
  });
});
