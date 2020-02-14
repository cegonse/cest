#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define ASCII_BACKGROUND_GREEN  "\u001b[42m"
#define ASCII_BACKGROUND_RED    "\u001b[41m"
#define ASCII_BACKGROUND_BLUE   "\u001b[44m"
#define ASCII_RED               "\033[1m\033[31m"
#define ASCII_GREEN             "\033[1m\033[32m"
#define ASCII_GRAY              "\u001b[38;5;243m"
#define ASCII_BLACK             "\u001b[38;5;232m"
#define ASCII_BOLD              "\u001b[1m"
#define ASCII_RESET             "\033[0m"
#define TEST_NAME_LENGTH        4096

#define describe(...)           std::string test_suite_name = cest::describeFunction(__VA_ARGS__)
#define it(...)                 cest::itFunction(__FILE__, __LINE__, __VA_ARGS__)
#define expect(x)               cest::expectFunction(__FILE__, __LINE__, x)
#define beforeEach(x)           cest::beforeEachFunction(x)
#define afterEach(x)            cest::afterEachFunction(x)


namespace cest
{
    struct TestCase {
        std::string name;
        std::string file;
        int line;
        std::function<void()> test;
        bool test_failed;
        std::string failure_message;
    };

    struct TestSuite {
        std::vector<cest::TestCase *> test_cases;
        std::string test_suite_name;
    };
}

std::vector<cest::TestCase *> test_cases;
extern std::string test_suite_name;
std::function<void()> before_each;
std::function<void()> after_each;
std::stringstream assertion_failures;
bool current_test_failed;
cest::TestCase *current_test_case;


namespace cest
{
    std::string describeFunction(std::string test_name, std::function<void()> test)
    {
        test();
        return test_name;
    }

    void itFunction(std::string file, int line, std::string name, std::function<void()> test)
    {
        TestCase *test_case = new TestCase();
        
        test_case->name = name;
        test_case->file = file;
        test_case->test = test;
        test_case->line = line;

        test_cases.push_back(test_case);
    }

    void beforeEachFunction(std::function<void()> func)
    {
        before_each = func;
    }

    void afterEachFunction(std::function<void()> func)
    {
        after_each = func;
    }

    void appendAssertionFailure(std::stringstream *stream, std::string message, std::string file, int line)
    {
        (*stream) << ASCII_RED << "    ❌ Assertion Failed:" << ASCII_RESET << " " << current_test_case->failure_message << std::endl;
        (*stream) << "                        " << file << ":" << line << std::endl;
    }

    template<class T>
    class Assertion
    {
        public:
            Assertion(const char *file, int line, T value)
            {
                actual = value;
                assertion_file = std::string(file);
                assertion_line = line;
            }

            void toBe(T expected)
            {
                if (expected != actual) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << ", was " << actual;
                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                }
            }

            void toEqualMemory(T expected, int64_t length)
            {
                int i;

                for (i=0; i<length; ++i) {
                    if (expected[i] != actual[i]) {
                        current_test_failed = true;
                        failure_message << "Memory mismatch at byte " << i << ", expected ";
                        failure_message << std::hex << std::uppercase << (int)expected[i] << " but was " << std::hex << std::uppercase << (int)actual[i];
                        current_test_case->failure_message = failure_message.str();
                        appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    }
                }
            }

            void toBeNotNull()
            {
                if (actual == NULL) {
                    current_test_failed = true;
                    failure_message << "Expected 0x" << std::hex << std::uppercase << actual << " to be not null";
                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                }
            }

            void toBeNull()
            {
                if (actual != NULL) {
                    current_test_failed = true;
                    failure_message << "Expected 0x" << std::hex << std::uppercase << actual << " to be null";
                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                }
            }

        private:
            T actual;
            std::string assertion_file;
            std::stringstream failure_message;
            int assertion_line;
    };

    template <>
    class Assertion<std::string>
    {
        public:
            Assertion(const char *file, int line, std::string value)
            {
                actual = value;
                assertion_file = std::string(file);
                assertion_line = line;
            }

            void toBe(std::string expected)
            {
                if (expected != actual) {
                    current_test_failed = true;
                    failure_message << "Expected \"" << expected << "\", was \"" << actual << "\"";
                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                }
            }

            void toContain(std::string expected)
            {
                if (actual.find(expected) == std::string::npos) {
                    current_test_failed = true;
                    failure_message << "\"" << expected << "\" not present inside \"" << actual << "\"";
                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                }
            }

            void toHaveLength(int64_t length)
            {
                if (actual.length() != length) {
                    current_test_failed = true;
                    failure_message << "Length of \"" << actual << "\" expected to be " << length << ", was " << actual.length();
                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                }
            }

        private:
            std::string actual;
            std::string assertion_file;
            std::stringstream failure_message;
            int assertion_line;
    };

    template<class T>
    Assertion<T> expectFunction(const char *file, int line, T actual)
    {
        return Assertion<T>(file, line, actual);
    }

    Assertion<bool> expectFunction(const char *file, int line, bool actual)
    {
        return Assertion<bool>(file, line, actual);
    }

    Assertion<int64_t> expectFunction(const char *file, int line, int64_t actual)
    {
        return Assertion<int64_t>(file, line, actual);
    }

    Assertion<std::string> expectFunction(const char *file, int line, std::string actual)
    {
        return Assertion<std::string>(file, line, actual);
    }

    Assertion<std::string> expectFunction(const char *file, int line, const char *actual)
    {
        return Assertion<std::string>(file, line, (std::string)actual);
    }

    void emitStringField(std::stringstream *stream, std::string key, std::string value, bool add_comma)
    {
        (*stream) << "\"" << key << "\":" << "\"" << value << "\"" << (add_comma? "," : "");
    }

    void emitIntegerField(std::stringstream *stream, std::string key, int value, bool add_comma)
    {
        (*stream) << "\"" << key << "\":" << value << (add_comma? "," : "");
    }

    int countFailedTests(std::vector<TestCase *> test_cases)
    {
        int failed_tests = 0;

        for (TestCase *test : test_cases) {
            if (test->test_failed) {
                failed_tests++;
            }
        }

        return failed_tests;
    }

    std::string sanitize(std::string text)
    {
        int start=0;
        std::string from("\"");
        std::string to("\\\"");

        while ((start = text.find(from, start)) != std::string::npos) {
            text.replace(start, from.length(), to);
            start += to.length();
        }

        return text;
    }

    std::string generateSuiteReport(TestSuite test_suite)
    {
        std::stringstream suite_report;

        suite_report << "{";

        emitStringField(&suite_report, "name", test_suite.test_suite_name, true);
        emitIntegerField(&suite_report, "tests", test_suite.test_cases.size(), true);
        emitIntegerField(&suite_report, "failures", countFailedTests(test_suite.test_cases), true);
        emitIntegerField(&suite_report, "errors", 0, true);
        emitIntegerField(&suite_report, "skipped", 0, true);
        emitStringField(&suite_report, "time", "", true);
        emitStringField(&suite_report, "timestamp", "", true);
        emitStringField(&suite_report, "hostname", "", true);

        suite_report << "\"test_cases\":[";

        for (int i=0; i<test_suite.test_cases.size(); ++i) {
            suite_report << "{";
            emitStringField(&suite_report, "name", test_suite.test_cases[i]->name, true);
            emitStringField(&suite_report, "time", "", test_suite.test_cases[i]->test_failed);

            if (test_suite.test_cases[i]->test_failed) {
                emitStringField(&suite_report, "failure_message", sanitize(test_suite.test_cases[i]->failure_message), false);
            }

            suite_report << "}" << (i==test_suite.test_cases.size()-1? "" : ",");
        }

        suite_report << "]}";

        return suite_report.str();
    }

    void printTestResult(TestCase *test_case)
    {
        if (test_case->test_failed) {
            std::cout <<
                ASCII_BACKGROUND_RED << ASCII_BLACK << ASCII_BOLD << " FAIL " << ASCII_RESET <<
                ASCII_GRAY << " " << test_case->file << ":" << test_case->line << ASCII_RESET <<
                ASCII_BOLD << " it " << test_case->name << ASCII_RESET <<
                std::endl;
        } else {
            std::cout <<
                ASCII_BACKGROUND_GREEN << ASCII_BLACK << ASCII_BOLD << " PASS " << ASCII_RESET <<
                ASCII_GRAY << " " << test_case->file << ":" << test_case->line << ASCII_RESET <<
                ASCII_BOLD << " it " << test_case->name << ASCII_RESET <<
                std::endl;
        }

        std::cout << assertion_failures.str();
        assertion_failures.str(std::string());
    }

    void writeTestSummaryToFile(TestSuite test_suite)
    {
        std::ofstream summary_file;

        summary_file.open("test_summary.jsonl", std::ios_base::app);
        summary_file << generateSuiteReport(test_suite) << std::endl;
        summary_file.close();
    }

    bool anyTestFailed()
    {
        return std::any_of(test_cases.begin(), test_cases.end(), [](cest::TestCase *test_case) {
            return test_case->test_failed;
        });
    }
}


int main(void)
{
    using namespace cest;

    int return_code = 0;
    TestSuite test_suite;

    test_suite.test_suite_name = test_suite_name;
    test_suite.test_cases = test_cases;

    for (TestCase *test_case : test_cases) {
        current_test_failed = false;
        current_test_case = test_case;

        if (before_each) {
            before_each();
        }

        test_case->test();

        if (after_each) {
            after_each();
        }

        test_case->test_failed = current_test_failed;

        printTestResult(test_case);
    }

    return_code = anyTestFailed();

    writeTestSummaryToFile(test_suite);

    for (TestCase *test_case : test_cases) {
        delete test_case;
    }

    return return_code;
}
