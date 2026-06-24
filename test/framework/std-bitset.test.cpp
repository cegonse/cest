#include <cest>
#include <bitset>

describe("std::bitset assertions", []() {
    it("asserts bitset equality", []() {
        std::bitset<8> bits("10101010");

        expect(bits).toBe(std::bitset<8>("10101010"));
        expect(bits).toEqual(std::bitset<8>("10101010"));

        expect(bits).Not->toBe(std::bitset<8>("11111111"));
    });

    it("asserts individual bit is set", []() {
        std::bitset<8> bits("10100000");

        expect(bits).toHaveBitSet(5);
        expect(bits).toHaveBitSet(7);

        expect(bits).Not->toHaveBitSet(0);
        expect(bits).Not->toHaveBitSet(1);
    });

    it("asserts bit count", []() {
        std::bitset<8> bits("10101010");

        expect(bits).toHaveCount(4);

        expect(bits).Not->toHaveCount(0);
        expect(bits).Not->toHaveCount(8);
    });

    it("asserts all bits set", []() {
        std::bitset<4> all("1111");
        std::bitset<4> partial("1010");

        expect(all).toHaveAll();

        expect(partial).Not->toHaveAll();
    });

    it("asserts no bits set", []() {
        std::bitset<4> none("0000");
        std::bitset<4> some("0010");

        expect(none).toHaveNone();

        expect(some).Not->toHaveNone();
    });

    it("works with larger bitsets", []() {
        std::bitset<32> flags;
        flags.set(0);
        flags.set(8);
        flags.set(16);

        expect(flags).toHaveBitSet(0);
        expect(flags).toHaveBitSet(8);
        expect(flags).toHaveBitSet(16);
        expect(flags).toHaveCount(3);

        expect(flags).Not->toHaveBitSet(1);
        expect(flags).Not->toHaveAll();
        expect(flags).Not->toHaveNone();
    });
});
