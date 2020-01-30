#include <cest>

extern std::string sample_json_with_zero_test_cases;

describe("test suite result generation", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("should generate an empty report when no tests have been executed", []() {
        cest::TestSuite test_suite;
        test_suite.test_suite_name = "test something";

        auto xml_report = cest::generateSuiteReport(test_cases);

        expect(xml_report).toBe(sample_json_with_zero_test_cases);
    });
});


std::string sample_json_with_zero_test_cases = "\
[\
{\
\"name\":\"test something\",\
\"tests\":0,\
\"failures\":0,\
\"errors\":0,\
\"skipped\":0,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[]\
}\
]\
";
