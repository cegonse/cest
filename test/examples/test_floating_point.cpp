#include <cest>

describe("Working with floating point numbers", []() {
  it("handles approximate equality of floats, with default ε=10⁻⁴", []() {
    float f1 = 4.777661f, f2 = 4.777662f;
    expect(f1).toEqual(f2);
  });

  it("handles approximate equality of doubles, with default ε=10⁻⁶", []() {
    double d1 = 4.777766661, d2 = 4.777766662;
    expect(d1).toEqual(d2);
  });

  it("can have a custom ε provided", []() {
    double d1 = 4.100, d2 = 4.102, epsilon = 0.01;
    expect(d1).toEqual(d2, epsilon);
  });

  it("supports arithmetic operations with floats and doubles", []() {
    expect(2.0f).toBeGreaterThan(1.0f);
    expect(2.0f).toBeLessThan(3.0f);
    expect(2.0).toBeGreaterThan(1.0);
    expect(2.0).toBeLessThan(3.0);
  });
});
