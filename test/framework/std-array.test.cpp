#include <cest>
#include <array>

describe("std::array assertions", []() {
    it("asserts array equality", []() {
        std::array<int, 3> numbers = {1, 2, 3};

        expect(numbers).toBe(std::array<int, 3>({1, 2, 3}));
        expect(numbers).toEqual(std::array<int, 3>({1, 2, 3}));

        expect(numbers).Not->toBe(std::array<int, 3>({4, 5, 6}));
    });

    it("asserts array contains an item", []() {
        std::array<int, 3> numbers = {10, 20, 30};

        expect(numbers).toContain(10);
        expect(numbers).toContain(20);
        expect(numbers).toContain(30);

        expect(numbers).Not->toContain(40);
        expect(numbers).Not->toContain(0);
    });

    it("asserts array length", []() {
        std::array<std::string, 2> words = {"hello", "world"};

        expect(words).toHaveLength(2);

        expect(words).Not->toHaveLength(0);
        expect(words).Not->toHaveLength(3);
    });
});
