#include <cest>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

describe("std::multiset assertions", []() {
    it("asserts multiset equality", []() {
        std::multiset<int> numbers({1, 1, 2, 3});

        expect(numbers).toBe(std::multiset<int>({1, 1, 2, 3}));
        expect(numbers).toEqual(std::multiset<int>({1, 1, 2, 3}));

        expect(numbers).Not->toBe(std::multiset<int>({1, 2, 3}));
    });

    it("asserts multiset includes an item", []() {
        std::multiset<int> numbers({10, 10, 20, 30});

        expect(numbers).toInclude(10);
        expect(numbers).toInclude(20);

        expect(numbers).Not->toInclude(40);
    });

    it("asserts multiset size", []() {
        std::multiset<int> numbers({1, 1, 2});

        expect(numbers).toHaveSize(3);

        expect(numbers).Not->toHaveSize(2);
    });
});

describe("std::unordered_multiset assertions", []() {
    it("asserts unordered_multiset equality", []() {
        std::unordered_multiset<int> numbers({1, 1, 2, 3});

        expect(numbers).toBe(std::unordered_multiset<int>({1, 1, 2, 3}));

        expect(numbers).Not->toBe(std::unordered_multiset<int>({1, 2, 3}));
    });

    it("asserts unordered_multiset includes an item", []() {
        std::unordered_multiset<int> numbers({10, 10, 20});

        expect(numbers).toInclude(10);
        expect(numbers).toInclude(20);

        expect(numbers).Not->toInclude(40);
    });

    it("asserts unordered_multiset size", []() {
        std::unordered_multiset<int> numbers({1, 1, 2});

        expect(numbers).toHaveSize(3);

        expect(numbers).Not->toHaveSize(2);
    });
});

describe("std::multimap assertions", []() {
    it("asserts multimap equality", []() {
        std::multimap<std::string, int> data({{"a", 1}, {"a", 2}, {"b", 3}});

        expect(data).toBe(std::multimap<std::string, int>({{"a", 1}, {"a", 2}, {"b", 3}}));

        expect(data).Not->toBe(std::multimap<std::string, int>({{"a", 1}, {"b", 3}}));
    });

    it("asserts multimap includes an entry", []() {
        std::multimap<std::string, int> data({{"a", 1}, {"a", 2}, {"b", 3}});

        expect(data).toInclude({"a", 1});
        expect(data).toInclude({"a", 2});
        expect(data).toInclude({"b", 3});

        expect(data).Not->toInclude({"a", 99});
        expect(data).Not->toInclude({"c", 1});
    });

    it("asserts multimap has a key", []() {
        std::multimap<std::string, int> data({{"a", 1}, {"a", 2}, {"b", 3}});

        expect(data).toHaveKey("a");
        expect(data).toHaveKey("b");

        expect(data).Not->toHaveKey("c");
    });

    it("asserts multimap size", []() {
        std::multimap<std::string, int> data({{"a", 1}, {"a", 2}, {"b", 3}});

        expect(data).toHaveSize(3);

        expect(data).Not->toHaveSize(2);
    });
});

describe("std::unordered_multimap assertions", []() {
    it("asserts unordered_multimap equality", []() {
        std::unordered_multimap<std::string, int> data({{"a", 1}, {"a", 2}, {"b", 3}});

        expect(data).toBe(std::unordered_multimap<std::string, int>({{"a", 1}, {"a", 2}, {"b", 3}}));

        expect(data).Not->toBe(std::unordered_multimap<std::string, int>({{"a", 1}, {"b", 3}}));
    });

    it("asserts unordered_multimap includes an entry", []() {
        std::unordered_multimap<std::string, int> data({{"a", 1}, {"a", 2}, {"b", 3}});

        expect(data).toInclude({"a", 1});
        expect(data).toInclude({"b", 3});

        expect(data).Not->toInclude({"a", 99});
        expect(data).Not->toInclude({"c", 1});
    });

    it("asserts unordered_multimap has a key", []() {
        std::unordered_multimap<std::string, int> data({{"a", 1}, {"b", 3}});

        expect(data).toHaveKey("a");
        expect(data).toHaveKey("b");

        expect(data).Not->toHaveKey("c");
    });

    it("asserts unordered_multimap size", []() {
        std::unordered_multimap<std::string, int> data({{"a", 1}, {"a", 2}, {"b", 3}});

        expect(data).toHaveSize(3);

        expect(data).Not->toHaveSize(2);
    });
});
