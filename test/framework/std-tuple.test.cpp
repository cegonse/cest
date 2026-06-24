#include <cest>
#include <tuple>
#include <utility>

describe("std::pair assertions", []() {
    it("asserts pair equality", []() {
        std::pair<std::string, int> entry = {"alice", 30};

        expect(entry).toBe(std::pair<std::string, int>{"alice", 30});
        expect(entry).toEqual(std::pair<std::string, int>{"alice", 30});

        expect(entry).Not->toBe(std::pair<std::string, int>{"bob", 30});
        expect(entry).Not->toBe(std::pair<std::string, int>{"alice", 99});
    });

    it("asserts pair with different types", []() {
        std::pair<int, double> values = {1, 3.14};

        expect(values).toBe(std::pair<int, double>{1, 3.14});

        expect(values).Not->toBe(std::pair<int, double>{2, 3.14});
    });
});

describe("std::tuple assertions", []() {
    it("asserts tuple equality", []() {
        std::tuple<int, std::string, bool> data = {42, "hello", true};

        expect(data).toBe(std::tuple<int, std::string, bool>{42, "hello", true});
        expect(data).toEqual(std::tuple<int, std::string, bool>{42, "hello", true});

        expect(data).Not->toBe(std::tuple<int, std::string, bool>{99, "hello", true});
        expect(data).Not->toBe(std::tuple<int, std::string, bool>{42, "world", true});
        expect(data).Not->toBe(std::tuple<int, std::string, bool>{42, "hello", false});
    });

    it("asserts tuple with two elements", []() {
        std::tuple<int, int> point = {10, 20};

        expect(point).toBe(std::tuple<int, int>{10, 20});

        expect(point).Not->toBe(std::tuple<int, int>{0, 0});
    });
});
