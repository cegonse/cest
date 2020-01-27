#include <cest>
#include <cstdio>
#include <cstring>
#include <example.h>


describe("test example", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("asserts booleans", [&]() {
        bool variable = false;

        expect(true).toBe(true);
        expect(variable).toBe(false);
    });

    it("asserts integers", [&]() {
        unsigned int variable = 12;
        int negative = -32;
        int64_t big_number = 1000202029292923;

        expect(3223).toBe(3223);
        expect(variable).toBe(12);
        expect(negative).toBe(-32);
        expect(big_number).toBe(1000202029292923);
    });

    it("asserts strings", [&]() {
        expect("hello").toBe("hello");
        expect("world").toContain("rld");
        expect("cest").toHaveLength(4);
    });

    it("asserts pointers", [&]() {
        void *address = (void *)0xFA101132;
        char *string = (char *)"something";
        int year = 2019;
        float *this_is_null = NULL;

        expect(address).toBe((void *)0xFA101132);
        expect(string).toEqualMemory((char *)"something", strlen("something") + 1);
        expect(&year).toBeNotNull();
        expect(this_is_null).toBeNull();
    });
});
