#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>

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

#define describe(x, y)          std::string test_suite_name = cest::describeFunction(x, y)
#define it(x, y)                cest::itFunction(__FILE__, __LINE__, x, y)
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
    };
}

std::vector<cest::TestCase *> test_cases;
extern std::string test_suite_name;
std::function<void()> before_each;
std::function<void()> after_each;
std::stringstream assertion_failures;
bool current_test_failed;


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
                    assertion_failures << ASCII_RED << "    ✖ Assertion Failed:" << ASCII_RESET << " Expected " << expected << ", was " << actual << std::endl;
                    assertion_failures << "                        " << assertion_file << ":" << assertion_line << std::endl;
                }
            }

            void toEqualMemory(T expected, int64_t length)
            {
                int i;

                for (i=0; i<length; ++i) {
                    if (expected[i] != actual[i]) {
                        current_test_failed = true;
                        assertion_failures << ASCII_RED << "    ✖ Assertion Failed:" << ASCII_RESET << " Memory mismatch at byte " << i << ", expected ";
                        assertion_failures << std::hex << std::uppercase << (int)expected[i] << " but was " << std::hex << std::uppercase << (int)actual[i] << std::endl;
                        assertion_failures << "                        " << assertion_file << ":" << assertion_line << std::endl;
                    }
                }
            }

            void toBeNotNull()
            {
                if (actual == NULL) {
                    current_test_failed = true;
                    assertion_failures << ASCII_RED << "    ✖ Assertion Failed:" << ASCII_RESET << " Expected 0x";
                    assertion_failures << std::hex << std::uppercase << actual << " to be not null" << std::endl;
                    assertion_failures << "                        " << assertion_file << ":" << assertion_line << std::endl;
                }
            }

            void toBeNull()
            {
                if (actual != NULL) {
                    current_test_failed = true;
                    assertion_failures << ASCII_RED << "    ✖ Assertion Failed:" << ASCII_RESET << " Expected ";
                    assertion_failures << std::hex << std::uppercase << actual << " to be null" << std::endl;
                    assertion_failures << "                        " << assertion_file << ":" << assertion_line << std::endl;
                }
            }

        private:
            T actual;
            std::string assertion_file;
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
                    assertion_failures << ASCII_RED << "    ✖ Assertion Failed:" << ASCII_RESET << " Expected \"" << expected << "\", was \"" << actual << "\"" << std::endl;
                    assertion_failures << "                        " << assertion_file << ":" << assertion_line << std::endl;
                }
            }

            void toContain(std::string expected)
            {
                if (actual.find(expected) == std::string::npos) {
                    current_test_failed = true;
                    assertion_failures << ASCII_RED << "    ✖ Assertion Failed:" << ASCII_RESET << " \"" << expected << "\" not present inside \"" << actual << "\"" << std::endl;
                    assertion_failures << "                        " << assertion_file << ":" << assertion_line << std::endl;
                }
            }

            void toHaveLength(int64_t length)
            {
                if (actual.length() != length) {
                    current_test_failed = true;
                    assertion_failures << ASCII_RED << "    ✖ Assertion Failed:" << ASCII_RESET << " Length of \"" << actual << "\" expected to be " << length << ", was " << actual.length() << std::endl;
                    assertion_failures << "                        " << assertion_file << ":" << assertion_line << std::endl;
                }
            }

        private:
            std::string actual;
            std::string assertion_file;
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


    void printTestResult(TestCase *test_case)
    {
        if (test_case->test_failed) {
            std::cout << ASCII_RED << "FAIL" << ASCII_RESET << std::endl;
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
}


int main(void)
{
    for (cest::TestCase *test_case : test_cases) {
        current_test_failed = false;

        if (before_each) {
            before_each();
        }

        test_case->test();

        if (after_each) {
            after_each();
        }

        test_case->test_failed = current_test_failed;

        printTestResult(test_case);

        delete test_case;
    }

    return 0;
}
