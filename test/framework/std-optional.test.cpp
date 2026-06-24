#include <cest>
#include <optional>

describe("std::optional assertions", []() {
    it("asserts optional equality", []() {
        std::optional<int> value = 42;

        expect(value).toBe(std::optional<int>(42));
        expect(value).toEqual(std::optional<int>(42));

        expect(value).Not->toBe(std::optional<int>(99));
        expect(value).Not->toBe(std::nullopt);
    });

    it("asserts optional has a value", []() {
        std::optional<int> value = 42;
        std::optional<int> empty;

        expect(value).toHaveValue();
        expect(value).toHaveValue(42);

        expect(value).Not->toHaveValue(99);
        expect(empty).Not->toHaveValue();
    });

    it("asserts optional is empty", []() {
        std::optional<int> empty;
        std::optional<int> value = 42;

        expect(empty).toBeEmpty();

        expect(value).Not->toBeEmpty();
    });

    it("works with string optionals", []() {
        std::optional<std::string> greeting = "hello";
        std::optional<std::string> empty;

        expect(greeting).toHaveValue();
        expect(greeting).toHaveValue(std::string("hello"));
        expect(empty).toBeEmpty();

        expect(greeting).Not->toBeEmpty();
        expect(empty).Not->toHaveValue();
    });

    it("asserts nullopt equality", []() {
        std::optional<int> empty;

        expect(empty).toBe(std::nullopt);
        expect(empty).toEqual(std::nullopt);

        expect(empty).Not->toBe(std::optional<int>(1));
    });
});
