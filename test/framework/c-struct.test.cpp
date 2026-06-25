#include <cest>

struct Point {
    int x, y;
};

enum Color { RED, GREEN, BLUE };

describe("C struct byte comparison", []() {
    it("compares plain structs with toEqualBytes", []() {
        Point a = {1, 2};
        Point b = {1, 2};

        expect(a).toEqualBytes(b);
    });

    it("detects struct differences", []() {
        Point a = {1, 2};
        Point b = {3, 4};

        expect(a).Not->toEqualBytes(b);
    });

    it("compares enums with toBe", []() {
        Color a = RED;

        expect(a).toBe(RED);
        expect(a).toEqual(RED);

        expect(a).Not->toBe(GREEN);
    });
});
