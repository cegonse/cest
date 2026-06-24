#include <cest>
#include <map>
#include <unordered_map>

describe("std::map assertions", []() {
    it("asserts map equality", []() {
        std::map<std::string, int> ages({{"alice", 30}, {"bob", 25}});

        expect(ages).toBe(std::map<std::string, int>({{"alice", 30}, {"bob", 25}}));
        expect(ages).toEqual(std::map<std::string, int>({{"alice", 30}, {"bob", 25}}));

        expect(ages).Not->toBe(std::map<std::string, int>({{"alice", 30}}));
        expect(ages).Not->toEqual(std::map<std::string, int>({{"alice", 99}, {"bob", 25}}));
    });

    it("asserts map includes an entry", []() {
        std::map<std::string, int> ages({{"alice", 30}, {"bob", 25}});

        expect(ages).toInclude({"alice", 30});
        expect(ages).toInclude({"bob", 25});

        expect(ages).Not->toInclude({"alice", 99});
        expect(ages).Not->toInclude({"charlie", 30});
    });

    it("asserts map has a key", []() {
        std::map<std::string, int> ages({{"alice", 30}, {"bob", 25}});

        expect(ages).toHaveKey("alice");
        expect(ages).toHaveKey("bob");

        expect(ages).Not->toHaveKey("charlie");
        expect(ages).Not->toHaveKey("dave");
    });

    it("asserts map size", []() {
        std::map<int, int> squares({{1, 1}, {2, 4}, {3, 9}});

        expect(squares).toHaveSize(3);

        expect(squares).Not->toHaveSize(0);
        expect(squares).Not->toHaveSize(2);
    });

    it("works with an empty map", []() {
        std::map<std::string, int> empty;

        expect(empty).toHaveSize(0);
        expect(empty).toBe(std::map<std::string, int>({}));

        expect(empty).Not->toHaveKey("anything");
        expect(empty).Not->toHaveSize(1);
    });
});

describe("std::unordered_map assertions", []() {
    it("asserts unordered_map equality", []() {
        std::unordered_map<std::string, int> ages({{"alice", 30}, {"bob", 25}});

        expect(ages).toBe(std::unordered_map<std::string, int>({{"alice", 30}, {"bob", 25}}));
        expect(ages).toEqual(std::unordered_map<std::string, int>({{"bob", 25}, {"alice", 30}}));

        expect(ages).Not->toBe(std::unordered_map<std::string, int>({{"alice", 30}}));
        expect(ages).Not->toEqual(std::unordered_map<std::string, int>({{"alice", 99}, {"bob", 25}}));
    });

    it("asserts unordered_map includes an entry", []() {
        std::unordered_map<std::string, int> ages({{"alice", 30}, {"bob", 25}});

        expect(ages).toInclude({"alice", 30});
        expect(ages).toInclude({"bob", 25});

        expect(ages).Not->toInclude({"alice", 99});
        expect(ages).Not->toInclude({"charlie", 30});
    });

    it("asserts unordered_map has a key", []() {
        std::unordered_map<std::string, int> ages({{"alice", 30}, {"bob", 25}});

        expect(ages).toHaveKey("alice");
        expect(ages).toHaveKey("bob");

        expect(ages).Not->toHaveKey("charlie");
        expect(ages).Not->toHaveKey("dave");
    });

    it("asserts unordered_map size", []() {
        std::unordered_map<int, int> squares({{1, 1}, {2, 4}, {3, 9}});

        expect(squares).toHaveSize(3);

        expect(squares).Not->toHaveSize(0);
        expect(squares).Not->toHaveSize(2);
    });

    it("works with an empty unordered_map", []() {
        std::unordered_map<std::string, int> empty;

        expect(empty).toHaveSize(0);
        expect(empty).toBe(std::unordered_map<std::string, int>({}));

        expect(empty).Not->toHaveKey("anything");
        expect(empty).Not->toHaveSize(1);
    });
});
