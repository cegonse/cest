#include <cest>
#include <helpers/test_suite_report_samples.h>


describe("test suite result generation", []() {
    beforeEach([&]() {
    });

    afterEach([&]() {
    });

    it("generates an empty report when no tests have been executed", []() {
        cest::TestSuite test_suite;
        test_suite.test_suite_name = "test something";

        auto xml_report = cest::generateSuiteReport(test_suite);

        expect(xml_report).toBe(sample_json_with_zero_test_cases);
    });

    it("generates a report when one test has passed", []() {
        cest::TestCase test_case;
        cest::TestSuite test_suite;

        test_case.name = "should pass";
        test_case.test_failed = false;
        test_suite.test_suite_name = "test something";
        test_suite.test_cases.push_back(&test_case);

        auto xml_report = cest::generateSuiteReport(test_suite);

        expect(xml_report).toBe(sample_json_with_one_passed_test_case);
    });

    it("generates a report when one test has failed", []() {
        cest::TestCase test_case;
        cest::TestSuite test_suite;

        test_case.name = "should fail";
        test_case.test_failed = true;
        test_case.failure_message = "expected blue, was red";
        test_suite.test_suite_name = "test something";
        test_suite.test_cases.push_back(&test_case);

        auto xml_report = cest::generateSuiteReport(test_suite);

        expect(xml_report).toBe(sample_json_with_one_failed_test_case);
    });
});
