#include <cest>
#include <complex>

describe("std::complex assertions", []() {
    it("asserts complex equality with epsilon", []() {
        std::complex<double> z(3.0, 4.0);

        expect(z).toBe(std::complex<double>(3.0, 4.0));
        expect(z).toEqual(std::complex<double>(3.0, 4.0));

        expect(z).Not->toBe(std::complex<double>(1.0, 2.0));
    });

    it("asserts real part", []() {
        std::complex<double> z(3.14, 2.71);

        expect(z).toHaveReal(3.14);

        expect(z).Not->toHaveReal(0.0);
    });

    it("asserts imaginary part", []() {
        std::complex<double> z(3.14, 2.71);

        expect(z).toHaveImaginary(2.71);

        expect(z).Not->toHaveImaginary(0.0);
    });

    it("works with float complex", []() {
        std::complex<float> z(1.0f, -1.0f);

        expect(z).toBe(std::complex<float>(1.0f, -1.0f));
        expect(z).toHaveReal(1.0f);
        expect(z).toHaveImaginary(-1.0f);

        expect(z).Not->toBe(std::complex<float>(0.0f, 0.0f));
    });

    it("handles zero complex", []() {
        std::complex<double> z(0.0, 0.0);

        expect(z).toHaveReal(0.0);
        expect(z).toHaveImaginary(0.0);
        expect(z).toBe(std::complex<double>(0.0, 0.0));
    });
});
