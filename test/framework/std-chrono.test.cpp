#include <cest>
#include <chrono>

using namespace std::chrono_literals;

describe("std::chrono::duration assertions", []() {
    it("asserts duration equality", []() {
        auto duration = 100ms;

        expect(duration).toBe(100ms);
        expect(duration).toEqual(100ms);

        expect(duration).Not->toBe(200ms);
        expect(duration).Not->toEqual(50ms);
    });

    it("asserts duration greater than", []() {
        auto duration = 500ms;

        expect(duration).toBeGreaterThan(100ms);
        expect(duration).toBeGreaterThan(499ms);

        expect(duration).Not->toBeGreaterThan(500ms);
        expect(duration).Not->toBeGreaterThan(1000ms);
    });

    it("asserts duration less than", []() {
        auto duration = 100ms;

        expect(duration).toBeLessThan(200ms);
        expect(duration).toBeLessThan(101ms);

        expect(duration).Not->toBeLessThan(100ms);
        expect(duration).Not->toBeLessThan(50ms);
    });

    it("asserts duration close to", []() {
        auto duration = 105ms;

        expect(duration).toBeCloseTo(100ms, 10ms);
        expect(duration).toBeCloseTo(110ms, 5ms);

        expect(duration).Not->toBeCloseTo(200ms, 10ms);
        expect(duration).Not->toBeCloseTo(100ms, 1ms);
    });

    it("works with different duration types", []() {
        auto seconds = 2s;
        auto microseconds = 500us;

        expect(seconds).toBe(2s);
        expect(microseconds).toBe(500us);

        expect(seconds).toBeGreaterThan(1s);
        expect(microseconds).toBeLessThan(1000us);
    });
});
