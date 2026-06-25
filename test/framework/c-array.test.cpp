#include <cest>

describe("C array assertions", []() {
    it("asserts int array equality", []() {
        int arr[] = {1, 2, 3};
        int expected[] = {1, 2, 3};

        expect(arr).toBe(expected);
        expect(arr).toEqual(expected);

        int other[] = {4, 5, 6};
        expect(arr).Not->toBe(other);
    });

    it("asserts char array equality", []() {
        char buf[] = {'a', 'b', 'c'};
        char expected[] = {'a', 'b', 'c'};

        expect(buf).toBe(expected);

        char other[] = {'x', 'y', 'z'};
        expect(buf).Not->toBe(other);
    });

    it("asserts array contains an item", []() {
        int arr[] = {10, 20, 30};

        expect(arr).toContain(10);
        expect(arr).toContain(20);
        expect(arr).toContain(30);

        expect(arr).Not->toContain(40);
        expect(arr).Not->toContain(0);
    });

    it("asserts array length", []() {
        int arr[] = {1, 2, 3, 4, 5};

        expect(arr).toHaveLength(5);

        expect(arr).Not->toHaveLength(0);
        expect(arr).Not->toHaveLength(3);
    });
});
