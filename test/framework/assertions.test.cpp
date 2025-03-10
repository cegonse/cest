#include <cest>
#include <cstdio>
#include <cstring>

using namespace cest;

describe("test common assertions", []() {
    beforeEach([]() {
    });

    afterEach([]() {
    });

    it("asserts booleans", []() {
        bool first = true;
        bool second = false;

        expect(first).toBe(true);
        expect(second).toBe(false);

        expect(first).Not->toBe(false);
        expect(second).Not->toBe(true);
    });

    it("asserts integers", []() {
        unsigned int variable = 12;
        int negative = -32;
        int64_t big_number = 1000202029292923;

        expect(3223).toBe(3223);
        expect(variable).toBe(12);
        expect(negative).toBe(-32);
        expect(big_number).toBe(1000202029292923);

        expect(3223).Not->toBe(1000);
        expect(variable).Not->toBe(100);
        expect(negative).Not->toBe(32);
        expect(big_number).Not->toBe(0);
    });

    it("asserts strings", []() {
        expect("hello").toBe("hello");
        expect("world").toContain("rld");
        expect("cest").toHaveLength(4);

        expect("hello").Not->toBe("world");
        expect("world").Not->toContain("??");
        expect("cest").Not->toHaveLength(40);
    });

    it("asserts string matches", []() {
        expect("hello").toMatch("hell");
        expect("world").toMatch("orld");
        expect("cest").toMatch("cest");

        expect("hello").Not->toMatch("asd");
        expect("world").Not->toMatch("???");
        expect("cest").Not->toMatch("ooo");
    });

    it("asserts regexs matches", []() {
        expect("Hello world cest").toMatch(Regex("^Hell.*cest$"));
        expect("I have 12 apples").toMatch(Regex(".*\\d+ apples"));
        expect("To match a partial match").toMatch(Regex("\\w match$"));

        expect("Hello world cest").Not->toMatch(Regex("^Bye.*cest$"));
        expect("I have 12 apples").Not->toMatch(Regex(".*\\d+ oranges"));
        expect("To match a partial match").Not->toMatch(Regex("\\w potato$"));
    });

    it("asserts pointers", []() {
        void *address = (void *)0xFA101132;
        char *string = (char *)"something";
        int year = 2019;
        float *this_is_null = NULL;

        expect(address).toBe((void *)0xFA101132);
        expect(string).toEqualMemory((char *)"something", strlen("something") + 1);
        expect(&year).toBeNotNull();
        expect(this_is_null).toBeNull();

        expect(address).Not->toBe((void *)0xBBBB0000);
        expect(string).Not->toEqualMemory((char *)"oops", strlen("oops") + 1);
        expect(&year).Not->toBeNull();
        expect(this_is_null).Not->toBeNotNull();
    });

    it("asserts lists", []() {
        std::vector<int> numbers({ 10, 20, 30 });

        expect(numbers).toContain(30);
        expect(numbers).toBe(numbers);
        expect(numbers).toHaveLength(3);

        expect(numbers).Not->toContain(300);
        expect(numbers).Not->toHaveLength(4);
    });

    it("can be forced to always pass", []() {
        passTest();

        expect(false).toBe(true);
    });

    it("can assert exceptions", []() {
        std::string number = "potato";

        assertRaises<std::invalid_argument>([=]() {
            std::stoi(number);
        });
    });
});
