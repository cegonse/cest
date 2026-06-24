#include <cest>
#include <string_view>

describe("std::string_view assertions", []() {
    it("asserts string_view equality", []() {
        std::string_view sv = "hello";

        expect(sv).toBe("hello");
        expect(sv).toEqual("hello");

        expect(sv).Not->toBe("world");
    });

    it("asserts string_view contains substring", []() {
        std::string_view sv = "hello world";

        expect(sv).toContain("hello");
        expect(sv).toContain("world");

        expect(sv).Not->toContain("xyz");
    });

    it("asserts string_view length", []() {
        std::string_view sv = "cest";

        expect(sv).toHaveLength(4);

        expect(sv).Not->toHaveLength(0);
        expect(sv).Not->toHaveLength(10);
    });

    it("asserts string_view matches", []() {
        std::string_view sv = "hello world";

        expect(sv).toMatch("hello");

        expect(sv).Not->toMatch("xyz");
    });
});
