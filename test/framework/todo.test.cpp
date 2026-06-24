#include <cest>

describe("todo", []() {
    todo("should implement this feature later");
    todo("should also implement this other feature");

    it("does not count todo tests as passed", []() {
        expect(true).toBeTruthy();
    });
});
