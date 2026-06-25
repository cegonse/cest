#include <cest>
#include <fcntl.h>

describe("bit flag assertions", []() {
    it("asserts bits are set", []() {
        expect(0xFF).toHaveBitsSet(0x0F);
        expect(0xFF).toHaveBitsSet(0xF0);
        expect(0x0F).toHaveBitsSet(0x01);

        expect(0x00).Not->toHaveBitsSet(0x01);
        expect(0xF0).Not->toHaveBitsSet(0x0F);
    });

    it("asserts bits are clear", []() {
        expect(0xF0).toHaveBitsClear(0x0F);
        expect(0x00).toHaveBitsClear(0xFF);

        expect(0xFF).Not->toHaveBitsClear(0x0F);
        expect(0x0F).Not->toHaveBitsClear(0x01);
    });

    it("works with C-style flag constants", []() {
        int flags = O_RDONLY | O_CREAT;

        expect(flags).toHaveBitsSet(O_CREAT);
    });
});
