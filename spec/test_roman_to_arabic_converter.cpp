/*
  Ported from Sandro Mancuso's Roman numerals kata,
  to describe parametrized tests API in Cest

  https://github.com/sandromancuso/roman-numerals-kata
*/
#include <cest>

using namespace cest;


class ArabicToRoman {
    public:
        ArabicToRoman(int arabic, std::string roman) {
            this->arabic = arabic;
            this->roman = roman;
        }

        int arabic;
        std::string roman;
};

std::vector<ArabicToRoman> arabicToRoman; 

ArabicToRoman THOUSAND(1000, "M");
ArabicToRoman NINE_HUNDRED(900, "CM");
ArabicToRoman FIVE_HUNDRED(500, "D");
ArabicToRoman FOUR_HUNDRED(400, "CD");
ArabicToRoman HUNDRED(100, "C");
ArabicToRoman NINETY(90, "XC");
ArabicToRoman FIFTY(50, "L");
ArabicToRoman FORTY(40, "XL");
ArabicToRoman TEN(10, "X");
ArabicToRoman NINE(9, "IX");
ArabicToRoman FIVE(5, "V");
ArabicToRoman FOUR(4, "IV");
ArabicToRoman ONE(1, "I");

void initialize() {
    arabicToRoman.clear();

    arabicToRoman.push_back(THOUSAND);
    arabicToRoman.push_back(NINE_HUNDRED);
    arabicToRoman.push_back(FIVE_HUNDRED);
    arabicToRoman.push_back(FOUR_HUNDRED);
    arabicToRoman.push_back(HUNDRED);
    arabicToRoman.push_back(NINETY);
    arabicToRoman.push_back(FIFTY);
    arabicToRoman.push_back(FORTY);
    arabicToRoman.push_back(TEN);
    arabicToRoman.push_back(NINE);
    arabicToRoman.push_back(FIVE);
    arabicToRoman.push_back(FOUR);
    arabicToRoman.push_back(ONE);
}

namespace ArabicToRomanConverter {
    std::string romanFor(int arabic) {
        std::string roman = "";

        for (ArabicToRoman value : arabicToRoman) {
            while (arabic >= value.arabic) {
                roman.append(value.roman);
                arabic -= value.arabic;
            }
        }

        return roman;
    }
}

describe("Roman to arabic numeral converter", []() {
    beforeAll([&]() {
        initialize();
    });

    it("converts arabic numbers into their respective roman numerals", [&]() {
        withParameter<ArabicToRoman>().
            withValue(ArabicToRoman(1, "I")).
            withValue(ArabicToRoman(2, "II")).
            withValue(ArabicToRoman(3, "III")).
            withValue(ArabicToRoman(4, "IV")).
            withValue(ArabicToRoman(5, "V")).
            withValue(ArabicToRoman(7, "VII")).
            withValue(ArabicToRoman(9, "IX")).
            withValue(ArabicToRoman(10, "X")).
            withValue(ArabicToRoman(17, "XVII")).
            withValue(ArabicToRoman(30, "XXX")).
            withValue(ArabicToRoman(38, "XXXVIII")).
            withValue(ArabicToRoman(479, "CDLXXIX")).
            withValue(ArabicToRoman(3999, "MMMCMXCIX")).
            thenDo([&](ArabicToRoman x) {
                expect(ArabicToRomanConverter::romanFor(x.arabic)).toBe(x.roman);
            });
    });
});
