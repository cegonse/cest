#include <cest>
#include <memory>

describe("std::shared_ptr assertions", []() {
    it("asserts null and not null", []() {
        auto ptr = std::make_shared<int>(42);
        std::shared_ptr<int> null_ptr;

        expect(ptr).toBeNotNull();
        expect(null_ptr).toBeNull();

        expect(ptr).Not->toBeNull();
        expect(null_ptr).Not->toBeNotNull();
    });

    it("asserts pointed-to value", []() {
        auto ptr = std::make_shared<int>(42);

        expect(ptr).toPointTo(42);

        expect(ptr).Not->toPointTo(99);
    });

    it("asserts use count", []() {
        auto ptr = std::make_shared<int>(42);

        expect(ptr).toHaveUseCount(1);

        auto copy = ptr;

        expect(copy).toHaveUseCount(2);

        expect(ptr).Not->toHaveUseCount(99);
    });

    it("asserts shared_ptr to string", []() {
        auto ptr = std::make_shared<std::string>("hello");

        expect(ptr).toPointTo(std::string("hello"));
        expect(ptr).toBeNotNull();

        expect(ptr).Not->toPointTo(std::string("world"));
    });
});

describe("std::unique_ptr assertions", []() {
    it("asserts null and not null", []() {
        auto ptr = std::make_unique<int>(42);
        std::unique_ptr<int> null_ptr;

        expect(ptr).toBeNotNull();
        expect(null_ptr).toBeNull();

        expect(ptr).Not->toBeNull();
        expect(null_ptr).Not->toBeNotNull();
    });

    it("asserts pointed-to value", []() {
        auto ptr = std::make_unique<int>(42);

        expect(ptr).toPointTo(42);

        expect(ptr).Not->toPointTo(99);
    });

    it("asserts unique_ptr to string", []() {
        auto ptr = std::make_unique<std::string>("hello");

        expect(ptr).toPointTo(std::string("hello"));
        expect(ptr).toBeNotNull();

        expect(ptr).Not->toPointTo(std::string("world"));
    });
});
