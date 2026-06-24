#include <cest>
#include <list>
#include <forward_list>

describe("std::list assertions", []() {
    it("asserts list equality", []() {
        std::list<int> numbers({1, 2, 3});

        expect(numbers).toBe(std::list<int>({1, 2, 3}));
        expect(numbers).toEqual(std::list<int>({1, 2, 3}));

        expect(numbers).Not->toBe(std::list<int>({4, 5, 6}));
        expect(numbers).Not->toEqual(std::list<int>({1, 2}));
    });

    it("asserts list contains an item", []() {
        std::list<int> numbers({10, 20, 30});

        expect(numbers).toContain(10);
        expect(numbers).toContain(30);

        expect(numbers).Not->toContain(40);
        expect(numbers).Not->toContain(0);
    });

    it("asserts list size", []() {
        std::list<std::string> words({"hello", "world"});

        expect(words).toHaveSize(2);

        expect(words).Not->toHaveSize(0);
        expect(words).Not->toHaveSize(3);
    });

    it("works with an empty list", []() {
        std::list<int> empty;

        expect(empty).toHaveSize(0);
        expect(empty).toBe(std::list<int>({}));

        expect(empty).Not->toContain(1);
        expect(empty).Not->toHaveSize(1);
    });
});

describe("std::forward_list assertions", []() {
    it("asserts forward_list equality", []() {
        std::forward_list<int> numbers({1, 2, 3});

        expect(numbers).toBe(std::forward_list<int>({1, 2, 3}));
        expect(numbers).toEqual(std::forward_list<int>({1, 2, 3}));

        expect(numbers).Not->toBe(std::forward_list<int>({4, 5, 6}));
        expect(numbers).Not->toEqual(std::forward_list<int>({1, 2}));
    });

    it("asserts forward_list contains an item", []() {
        std::forward_list<int> numbers({10, 20, 30});

        expect(numbers).toContain(10);
        expect(numbers).toContain(30);

        expect(numbers).Not->toContain(40);
        expect(numbers).Not->toContain(0);
    });

    it("asserts forward_list size", []() {
        std::forward_list<std::string> words({"hello", "world"});

        expect(words).toHaveSize(2);

        expect(words).Not->toHaveSize(0);
        expect(words).Not->toHaveSize(3);
    });

    it("works with an empty forward_list", []() {
        std::forward_list<int> empty;

        expect(empty).toHaveSize(0);
        expect(empty).toBe(std::forward_list<int>({}));

        expect(empty).Not->toContain(1);
        expect(empty).Not->toHaveSize(1);
    });
});
