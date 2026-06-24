#include <cest>
#include <deque>

describe("std::deque assertions", []() {
    it("asserts deque equality", []() {
        std::deque<int> numbers({1, 2, 3});

        expect(numbers).toBe(std::deque<int>({1, 2, 3}));
        expect(numbers).toEqual(std::deque<int>({1, 2, 3}));

        expect(numbers).Not->toBe(std::deque<int>({4, 5, 6}));
        expect(numbers).Not->toEqual(std::deque<int>({1, 2}));
    });

    it("asserts deque contains an item", []() {
        std::deque<int> numbers({10, 20, 30});

        expect(numbers).toContain(10);
        expect(numbers).toContain(20);
        expect(numbers).toContain(30);

        expect(numbers).Not->toContain(40);
        expect(numbers).Not->toContain(0);
    });

    it("asserts deque length", []() {
        std::deque<std::string> words({"hello", "world"});

        expect(words).toHaveLength(2);

        expect(words).Not->toHaveLength(0);
        expect(words).Not->toHaveLength(3);
    });

    it("works with an empty deque", []() {
        std::deque<int> empty;

        expect(empty).toHaveLength(0);
        expect(empty).toBe(std::deque<int>({}));

        expect(empty).Not->toContain(1);
        expect(empty).Not->toHaveLength(1);
    });
});
