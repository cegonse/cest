#include <unittest>
#include <cstdio>
#include <cstring>
#include <example.h>


describe("test example", []() {
    char *text;

    beforeEach([&]() {
        text = new char[32];
        strcpy(text, "hello");
    });

    afterEach([&]() {
        delete[] text;
    });

    it("does nothing", [&]() {
        expect<bool>(IsTruthy()).toBe(false);
    });
});
