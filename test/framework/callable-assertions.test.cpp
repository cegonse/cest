#include <cest>
#include <stdexcept>

describe("callable assertions", []() {
    it("asserts that a function throws any exception", []() {
        expect([]() { throw std::runtime_error("oops"); }).toThrow();
    });

    it("asserts that a function does not throw", []() {
        expect([]() { }).Not->toThrow();
    });

    it("asserts that a function throws a specific exception type", []() {
        expect([]() { throw std::runtime_error("oops"); }).toThrow<std::runtime_error>();
    });

    it("asserts that a function does not throw a specific exception type", []() {
        expect([]() { throw std::logic_error("wrong"); }).Not->toThrow<std::runtime_error>();
    });

    it("asserts that a function throws with a specific message", []() {
        expect([]() { throw std::runtime_error("oops"); }).toThrowMessage("oops");
    });

    it("asserts that a function does not throw with a specific message", []() {
        expect([]() { throw std::runtime_error("oops"); }).Not->toThrowMessage("different message");
    });

    it("fails when function does not throw but toThrow is expected", []() {
        bool caught = false;
        try {
            expect([]() { }).toThrow();
        } catch (const cest::AssertionError&) {
            caught = true;
        }
        expect(caught).toBeTruthy();
    });

    it("fails when function throws but Not->toThrow is expected", []() {
        bool caught = false;
        try {
            expect([]() { throw std::runtime_error("oops"); }).Not->toThrow();
        } catch (const cest::AssertionError&) {
            caught = true;
        }
        expect(caught).toBeTruthy();
    });

    it("fails when function throws wrong message", []() {
        bool caught = false;
        try {
            expect([]() { throw std::runtime_error("oops"); }).toThrowMessage("expected");
        } catch (const cest::AssertionError&) {
            caught = true;
        }
        expect(caught).toBeTruthy();
    });
});
