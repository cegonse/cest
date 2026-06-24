#include <cest>
#include <set>
#include <unordered_set>

describe("std::set assertions", []() {
    it("asserts set equality", []() {
        std::set<int> numbers({1, 2, 3});

        expect(numbers).toBe(std::set<int>({1, 2, 3}));
        expect(numbers).toEqual(std::set<int>({1, 2, 3}));

        expect(numbers).Not->toBe(std::set<int>({4, 5, 6}));
        expect(numbers).Not->toEqual(std::set<int>({1, 2}));
    });

    it("asserts set includes an item", []() {
        std::set<int> numbers({10, 20, 30});

        expect(numbers).toInclude(10);
        expect(numbers).toInclude(20);
        expect(numbers).toInclude(30);

        expect(numbers).Not->toInclude(40);
        expect(numbers).Not->toInclude(0);
    });

    it("asserts set size", []() {
        std::set<std::string> words({"hello", "world"});

        expect(words).toHaveSize(2);

        expect(words).Not->toHaveSize(0);
        expect(words).Not->toHaveSize(3);
    });

    it("works with an empty set", []() {
        std::set<int> empty;

        expect(empty).toHaveSize(0);
        expect(empty).toBe(std::set<int>({}));

        expect(empty).Not->toInclude(1);
        expect(empty).Not->toHaveSize(1);
    });
});

describe("std::unordered_set assertions", []() {
    it("asserts unordered_set equality", []() {
        std::unordered_set<int> numbers({1, 2, 3});

        expect(numbers).toBe(std::unordered_set<int>({1, 2, 3}));
        expect(numbers).toEqual(std::unordered_set<int>({3, 2, 1}));

        expect(numbers).Not->toBe(std::unordered_set<int>({4, 5, 6}));
        expect(numbers).Not->toEqual(std::unordered_set<int>({1, 2}));
    });

    it("asserts unordered_set includes an item", []() {
        std::unordered_set<int> numbers({10, 20, 30});

        expect(numbers).toInclude(10);
        expect(numbers).toInclude(20);
        expect(numbers).toInclude(30);

        expect(numbers).Not->toInclude(40);
        expect(numbers).Not->toInclude(0);
    });

    it("asserts unordered_set size", []() {
        std::unordered_set<std::string> words({"hello", "world"});

        expect(words).toHaveSize(2);

        expect(words).Not->toHaveSize(0);
        expect(words).Not->toHaveSize(3);
    });

    it("works with an empty unordered_set", []() {
        std::unordered_set<int> empty;

        expect(empty).toHaveSize(0);
        expect(empty).toBe(std::unordered_set<int>({}));

        expect(empty).Not->toInclude(1);
        expect(empty).Not->toHaveSize(1);
    });
});
