#include <cest>

describe("the first test suite", []() {
  int v = 0;

  beforeEach([&]() {
    v = 10;
  });

  afterAll([]() {});

  it("contains a test", [&]() {
    expect(v).toEqual(10);
  });

  describe("and a nested suite", [&]() {
    beforeEach([&]() {
      v = 20;
    });

    afterAll([]() {});

    it("with another test", [&]() {
      expect(v).toBe(20);
    });
  });
});
