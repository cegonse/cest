#pragma once

#include <csetjmp>
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <chrono>
#include <random>

#include "types.hpp"
#include "output.hpp"
#include "arg-parser.hpp"

#define TEST_NAME_LENGTH 4096
#define CLIP_STRING_LENGTH 16

#define describe(...) std::string test_suite_name = cest::describeFunction(__VA_ARGS__)
#define it(...) cest::itFunction(__FILE__, __LINE__, __VA_ARGS__)
#define xit(...) cest::xitFunction(__FILE__, __LINE__, __VA_ARGS__)
#define fit(...) cest::fitFunction(__FILE__, __LINE__, __VA_ARGS__)
#define expect(...) cest::expectFunction(__FILE__, __LINE__, __VA_ARGS__)
#define beforeEach(x) cest::beforeEachFunction(x)
#define afterEach(x) cest::afterEachFunction(x)
#define beforeAll(x) cest::beforeAllFunction(x)
#define afterAll(x) cest::afterAllFunction(x)
#define passTest() cest::forcedPass()
#define failTest() cest::forcedFailure(__FILE__, __LINE__)
#define Regex(x) x, std::regex(x)

std::vector<cest::TestCase *> test_cases;
extern std::string test_suite_name;
std::function<void()> before_each;
std::function<void()> after_each;
std::function<void()> before_all;
std::function<void()> after_all;
std::stringstream assertion_failures;
bool current_test_failed;
cest::TestCase *current_test_case;
cest::CommandLineOptions command_line_options;
jmp_buf jump_env;
unsigned int random_seed;

namespace cest
{
    class AssertionError : public std::exception
    {
    };
    class ForcedPassError : public std::exception
    {
    };

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
        test_case->skip = false;
        test_case->fit = false;

        test_cases.push_back(test_case);
    }

    void xitFunction(std::string file, int line, std::string name, std::function<void()> test)
    {
        TestCase *test_case = new TestCase();

        test_case->name = name;
        test_case->file = file;
        test_case->test = test;
        test_case->line = line;
        test_case->skip = true;
        test_case->fit = false;

        test_cases.push_back(test_case);
    }

    void fitFunction(std::string file, int line, std::string name, std::function<void()> test)
    {
        TestCase *test_case = new TestCase();

        test_case->name = name;
        test_case->file = file;
        test_case->test = test;
        test_case->line = line;
        test_case->skip = false;
        test_case->fit = true;

        test_cases.push_back(test_case);
    }

    void forcedPass()
    {
        throw ForcedPassError();
    }

    template <class T>
    class Parameter
    {
    public:
        Parameter() {}

        Parameter<T> withValue(T v)
        {
            values.push_back(v);
            return *this;
        }

        void thenDo(std::function<void(T)> call)
        {
            for (T v : values)
                call(v);
        }

    private:
        std::vector<T> values;
    };

    template <class T>
    Parameter<T> withParameter()
    {
        return Parameter<T>();
    }

    void appendAssertionFailure(std::stringstream *stream, std::string message, std::string file, int line)
    {
        (*stream) << ASCII_RED << "    ❌ Assertion Failed:" << ASCII_RESET << " " << message << std::endl;
        (*stream) << "                        " << file << ":" << line << std::endl;
    }

    void forcedFailure(std::string file, int line)
    {
        current_test_failed = true;
        current_test_case->failure_message = "Test failure forced manually";

        appendAssertionFailure(&assertion_failures, current_test_case->failure_message, file, line);

        throw AssertionError();
    }

    void beforeEachFunction(std::function<void()> func)
    {
        before_each = func;
    }

    void afterEachFunction(std::function<void()> func)
    {
        after_each = func;
    }

    void beforeAllFunction(std::function<void()> func)
    {
        before_all = func;
    }

    void afterAllFunction(std::function<void()> func)
    {
        after_all = func;
    }

    template<class T>
    void assertRaises(std::function<void()> expression)
    {
        int line = current_test_case->line;
        std::string file = current_test_case->file;

        try {
            expression();
        } catch (const T& err) {
            return;
        } catch (const std::exception& err) {
            current_test_failed = true;
            current_test_case->failure_message = "Expected exception not raised";
            appendAssertionFailure(&assertion_failures, current_test_case->failure_message, file, line);
        }

        current_test_failed = true;
        current_test_case->failure_message = "Expected exception not raised";
        appendAssertionFailure(&assertion_failures, current_test_case->failure_message, file, line);
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

            void toBeTruthy()
            {
                if (!actual) {
                    current_test_failed = true;
                    failure_message << "Expresion is not truthy";
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toBeFalsy()
            {
                if (actual) {
                    current_test_failed = true;
                    failure_message << "Expresion is not falsy";
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toBe(T expected)
            {
                if (expected != actual) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << ", was " << actual;
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toEqual(T expected)
            {
                toBe(expected);
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
                        throw AssertionError();
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
                    throw AssertionError();
                }
            }

            void toBeNull()
            {
                if (actual != NULL) {
                    current_test_failed = true;
                    failure_message << "Expected 0x" << std::hex << std::uppercase << actual << " to be null";
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

        private:
            T actual;
            std::string assertion_file;
            std::stringstream failure_message;
            int assertion_line;
    };

    template<>
    class Assertion<double>
    {
        public:
            Assertion(const char *file, int line, double value)
            {
                actual = value;
                assertion_file = std::string(file);
                assertion_line = line;
            }

            void toBe(double expected, double epsilon = 0.0000001)
            {
                if (fabs(actual - expected) > epsilon) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << ", was " << actual;
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toEqual(double expected, double epsilon = 0.0000001)
            {
                toBe(expected, epsilon);
            }

            void toBeGreaterThan(double expected)
            {
                if (actual < expected) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << " to be greather than" << actual;
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toBeLessThan(double expected)
            {
                if (actual > expected) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << " to be less than" << actual;
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

        private:
            double actual;
            std::string assertion_file;
            std::stringstream failure_message;
            int assertion_line;
    };

    template<>
    class Assertion<float>
    {
        public:
            Assertion(const char *file, int line, float value)
            {
                actual = value;
                assertion_file = std::string(file);
                assertion_line = line;
            }

            void toBe(float expected, float epsilon = 0.00001f)
            {
                if (fabs(actual - expected) > epsilon) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << ", was " << actual;
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toEqual(float expected, float epsilon = 0.00001f)
            {
                toBe(expected, epsilon);
            }

            void toBeGreaterThan(float expected)
            {
                if (actual < expected) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << " to be greather than" << actual;
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toBeLessThan(float expected)
            {
                if (actual > expected) {
                    current_test_failed = true;
                    failure_message << "Expected " << expected << " to be less than" << actual;
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

        private:
            float actual;
            std::string assertion_file;
            std::stringstream failure_message;
            int assertion_line;
    };

    template <class T>
    class Assertion<std::vector<T> >
    {
        public:
            Assertion(const char *file, int line, std::vector<T> value)
            {
                actual = value;
                assertion_file = std::string(file);
                assertion_line = line;
            }

            void toBe(std::vector<T> expected)
            {
                if (expected.size() != actual.size()) {
                    current_test_failed = true;
                    failure_message << "Vector sizes do not match, expected " << expected.size() << " items but had " << actual.size() << " items";
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }

                for (size_t i=0; i<expected.size(); ++i) {
                    if (expected[i] != actual[i]) {
                        current_test_failed = true;
                        failure_message << "Vector item mismatch at position " << i << ", expected " << expected[i] << " but was " << actual[i];
                        current_test_case->failure_message = failure_message.str();

                        appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                        throw AssertionError();
                    }
                }
            }

            void toEqual(std::vector<T> expected)
            {
                toBe(expected);
            }

            void toContain(T item)
            {
                bool found = false;

                for (size_t i=0; i<actual.size(); ++i) {
                    if (actual[i] == item) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    current_test_failed = true;
                    failure_message << "Item " << item << " not found in vector";
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toHaveLength(size_t size)
            {
                if (actual.size() != size) {
                    current_test_failed = true;
                    failure_message << "Vector sizes does not match, expected " << size << " items but had " << actual.size() << " items";
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

        private:
            std::vector<T> actual;
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
                    if (expected.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH) {
                        failure_message << "String mismatch," << std::endl;
                        failure_message << "  Expected: \"" << expected << "\"" << std::endl;
                        failure_message << "  Actual  : \"" << actual << "\"";
                    } else {
                        failure_message << "Expected \"" << expected << "\", was \"" << actual << "\"";
                    }

                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toMatch(std::string expected)
            {
                toContain(expected);
            }

            void toMatch(std::string expected_string, std::regex expected)
            {
                if (!std::regex_search(actual, expected)) {
                    current_test_failed = true;
                    if (expected_string.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH) {
                        failure_message << "Expected pattern mismatch," << std::endl;
                        failure_message << "  Regexp: \"" << expected_string << "\"" << std::endl;
                        failure_message << "  Actual: \"" << actual << "\"";
                    } else {
                        failure_message << "Expected pattern \"" << expected_string << "\" did not match with \"" << actual << "\"";
                    }

                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toEqual(std::string expected)
            {
                toBe(expected);
            }

            void toContain(std::string expected)
            {
                if (actual.find(expected) == std::string::npos) {
                    current_test_failed = true;

                    if (expected.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH) {
                        failure_message << "Substring not found," << std::endl;
                        failure_message << "  Expected: \"" << expected << "\"" << std::endl;
                        failure_message << "  Actual  : \"" << actual << "\"";
                    } else {
                        failure_message << "\"" << expected << "\" not present inside \"" << actual << "\"";
                    }

                    current_test_case->failure_message = failure_message.str();
                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
                }
            }

            void toHaveLength(size_t length)
            {
                if (actual.length() != length) {
                    current_test_failed = true;
                    failure_message << "Length of \"" << actual << "\" expected to be " << length << ", was " << actual.length();
                    current_test_case->failure_message = failure_message.str();

                    appendAssertionFailure(&assertion_failures, current_test_case->failure_message, assertion_file, assertion_line);
                    throw AssertionError();
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

    int countFailedTests(std::vector<TestCase *> test_cases)
    {
        int failed_tests = 0;

        for (TestCase *test : test_cases)
        {
            if (test->test_failed)
                failed_tests++;
        }

        return failed_tests;
    }

    int countSkippedTests(std::vector<TestCase *> test_cases)
    {
        int skipped_tests = 0;

        for (TestCase *test : test_cases)
        {
            if (test->skip)
                skipped_tests++;
        }

        return skipped_tests;
    }

    bool anyTestFailed()
    {
        return std::any_of(test_cases.begin(), test_cases.end(), [](cest::TestCase *test_case)
                           { return test_case->test_failed; });
    }

    void handleFailedTest(TestCase *test_case)
    {
        current_test_failed = true;
        test_case->test_failed = true;
    }

    void handleTestException(TestCase *test_case)
    {
        std::string exception_message("Unhandled exception in test case");

        appendAssertionFailure(&assertion_failures, exception_message, test_case->file, test_case->line);
        handleFailedTest(test_case);
    }

    void onSignalRaised(int sig)
    {
        std::string signal_as_string(strsignal(sig));

        current_test_failed = true;
        current_test_case->failure_message = "Signal raised by test (" + signal_as_string + ")";

        appendAssertionFailure(&assertion_failures, current_test_case->failure_message, current_test_case->file, current_test_case->line);

        longjmp(jump_env, 1);
    }

    void configureFittedTests(TestSuite *test_suite)
    {
        TestCase *fitted_test = NULL;

        for (TestCase *test : test_suite->test_cases)
        {
            if (test->fit)
            {
                fitted_test = test;
                break;
            }
        }

        if (!fitted_test)
            return;

        for (TestCase *test : test_suite->test_cases)
        {
            if (test != fitted_test)
                test->skip = true;
        }
    }

    void configureRandomizedTests(TestSuite *test_suite, unsigned int seed)
    {
        std::cout << ASCII_BACKGROUND_MAGENTA << ASCII_BLACK << ASCII_BOLD << " SEED "
                  << ASCII_RESET << " Randomizing test execution order with seed " << seed << std::endl;

        std::shuffle(test_suite->test_cases.begin(), test_suite->test_cases.end(), std::default_random_engine(seed));
    }
}

int main(int argc, const char *argv[])
{
    int return_code = 0;
    cest::TestSuite test_suite;

    command_line_options = cest::parseArgs(argc, argv);

    if (command_line_options.help)
    {
        cest::showHelp(argv[0]);
        return 0;
    }

    signal(SIGSEGV, cest::onSignalRaised);
    signal(SIGFPE, cest::onSignalRaised);
    signal(SIGBUS, cest::onSignalRaised);
    signal(SIGILL, cest::onSignalRaised);
    signal(SIGTERM, cest::onSignalRaised);
    signal(SIGXCPU, cest::onSignalRaised);
    signal(SIGXFSZ, cest::onSignalRaised);

    test_suite.test_suite_name = test_suite_name;
    test_suite.test_cases = test_cases;

    cest::configureFittedTests(&test_suite);

    if (command_line_options.random_seed_present)
        random_seed = command_line_options.random_seed;
    else
        random_seed = std::chrono::system_clock::now().time_since_epoch().count();

    if (command_line_options.randomize)
        cest::configureRandomizedTests(&test_suite, random_seed);

    if (before_all)
        before_all();

    for (cest::TestCase *test_case : test_suite.test_cases)
    {
        current_test_failed = false;
        current_test_case = test_case;

        if (before_each)
            before_each();

        try
        {
            if (test_case->skip)
                throw cest::ForcedPassError();

            test_case->test();
            setjmp(jump_env);
        }
        catch (const cest::AssertionError &error)
        {
            handleFailedTest(test_case);
        }
        catch (const cest::ForcedPassError &error)
        {
            if (after_each)
                after_each();

            cest::printTestResult(test_case, assertion_failures);
            continue;
        }
        catch (...)
        {
            handleTestException(test_case);
        }

        if (after_each)
            after_each();

        test_case->test_failed = current_test_failed;

        cest::printTestResult(test_case, assertion_failures);
    }

    if (after_all)
        after_all();

    return_code = cest::anyTestFailed();

    for (cest::TestCase *test_case : test_cases)
        delete test_case;

    return return_code;
}
