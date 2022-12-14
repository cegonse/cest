#include <cest>

describe("Working with floating point numbers", []() {
  it("handles approximate equality of floats, with default ε=10⁻⁶", []() {
    float f1 = 4.7761f, f2 = 4.7762f;
    expect(f1).toEqual(f2);
  });

  it("handles approximate equality of doubles, with default ε=10⁻⁴", []() {
    double d1 = 4.776661, d2 = 4.776662;
    expect(d1).toEqual(d2);
  });

  it("can have a custom ε provided", []() {
    double d1 = 4.10, d2 = 4.12, epsilon = 0.0001;
    expect(d1).toEqual(d2, epsilon);
  });

  it("supports arithmetic operations with floats and doubles", []() {
    expect(2.0f).toBeGreaterThan(1.0f);
    expect(2.0f).toBeLessThan(3.0f);
    expect(2.0).toBeGreaterThan(1.0);
    expect(2.0).toBeLessThan(3.0);
  });
});
