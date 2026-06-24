#include <cest>

struct Point
{
  int x, y;
  bool operator==(const Point&) const = default;
};

struct Color
{
  int r, g, b;
  bool operator==(const Color&) const = default;

  friend std::ostream& operator<<(std::ostream& os, const Color& c)
  {
    return os << "Color(" << c.r << ", " << c.g << ", " << c.b << ")";
  }
};

describe("assertions with custom types", []() {
  it("compares equal structs with toEqual", []() {
    Point a{1, 2};
    Point b{1, 2};
    expect(a).toEqual(b);
  });

  it("compares equal structs with toBe", []() {
    Point a{3, 4};
    Point b{3, 4};
    expect(a).toBe(b);
  });

  it("detects unequal structs with Not", []() {
    Point a{1, 2};
    Point b{3, 4};
    expect(a).Not->toEqual(b);
  });

  it("compares structs that provide operator<<", []() {
    Color a{255, 0, 0};
    Color b{255, 0, 0};
    expect(a).toEqual(b);
  });

  it("detects unequal structs that provide operator<< with Not", []() {
    Color a{255, 0, 0};
    Color b{0, 255, 0};
    expect(a).Not->toEqual(b);
  });

  it("compares vectors of custom types", []() {
    std::vector<Point> a = {{1, 2}, {3, 4}};
    std::vector<Point> b = {{1, 2}, {3, 4}};
    expect(a).toEqual(b);
  });

  it("detects unequal vectors of custom types with Not", []() {
    std::vector<Point> a = {{1, 2}, {3, 4}};
    std::vector<Point> b = {{1, 2}, {5, 6}};
    expect(a).Not->toEqual(b);
  });
});
