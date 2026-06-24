#include <cest>

static int skipped_suite_counter = 0;

xdescribe("skipped suite", []() {
    it("should not run this test", []() {
        skipped_suite_counter++;
    });

    it("should not run this test either", []() {
        skipped_suite_counter++;
    });
});

describe("xdescribe and fdescribe", []() {
    it("skipped suite tests did not execute", []() {
        expect(skipped_suite_counter).toBe(0);
    });
});
