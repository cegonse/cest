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
#include "globals.hpp"
#include "output.hpp"
#include "arg-parser.hpp"
#include "parametrized.hpp"
#include "test-builder.hpp"
#include "suite-runner.hpp"

#define TEST_NAME_LENGTH 4096
#define CLIP_STRING_LENGTH 16

#define expect(...) cest::expectFunction(__FILE__, __LINE__, __VA_ARGS__)
#define passTest() cest::forcedPass()
#define failTest() cest::forcedFailure(__FILE__, __LINE__)
#define Regex(x) x, std::regex(x)

namespace cest
{
    void forcedPass()
    {
        throw ForcedPassError();
    }

    void appendAssertionFailure(std::stringstream *stream, std::string message, std::string file, int line)
    {
        (*stream) << ASCII_RED << "    ❌ Assertion Failed:" << ASCII_RESET << " " << message << std::endl;
        (*stream) << "                        " << file << ":" << line << std::endl;
    }

    void forcedFailure(std::string file, int line)
    {
        throw AssertionError(file, line, "Test failure forced manually");
    }

    template <class T>
    void assertRaises(std::function<void()> expression)
    {
        std::string file = __cest_globals.current_test_case->fn.file;
        int line = __cest_globals.current_test_case->fn.line;

        try
        {
            expression();
        }
        catch (const T &err)
        {
            return;
        }

        throw AssertionError(file, line, "Expected exception not raised");
    }

    template <class T>
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
            if (!actual)
                throw AssertionError(assertion_file, assertion_line, "Expresion is not truthy");
        }

        void toBeFalsy()
        {
            if (actual)
                throw AssertionError(assertion_file, assertion_line, "Expresion is not falsy");
        }

        void toBe(T expected)
        {
            if (expected != actual)
            {

                std::stringstream message;
                message << "Expected " << expected << ", was " << actual;
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toEqual(T expected)
        {
            toBe(expected);
        }

        void toEqualMemory(T expected, int64_t length)
        {
            int i;

            for (i = 0; i < length; ++i)
            {
                if (expected[i] != actual[i])
                {
                    std::stringstream message;
                    message << "Memory mismatch at byte " << i << ", expected ";
                    message << std::hex << std::uppercase << (int)expected[i] << " but was " << std::hex << std::uppercase << (int)actual[i];
                    throw AssertionError(assertion_file, assertion_line, message.str());
                }
            }
        }

        void toBeNotNull()
        {
            if (actual == NULL)
            {
                std::stringstream message;
                message << "Expected 0x" << std::hex << std::uppercase << actual << " to be not null";
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toBeNull()
        {
            if (actual != NULL)
            {
                std::stringstream message;
                message << "Expected 0x" << std::hex << std::uppercase << actual << " to be null";
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

    private:
        T actual;
        std::string assertion_file;
        int assertion_line;
    };

    template <>
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
            if (fabs(actual - expected) > epsilon)
            {
                std::stringstream message;
                message << "Expected " << expected << ", was " << actual;
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toEqual(double expected, double epsilon = 0.0000001)
        {
            toBe(expected, epsilon);
        }

        void toBeGreaterThan(double expected)
        {
            if (actual < expected)
            {
                std::stringstream message;
                message << "Expected " << expected << " to be greather than" << actual;
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toBeLessThan(double expected)
        {
            if (actual > expected)
            {
                std::stringstream message;
                message << "Expected " << expected << " to be less than" << actual;
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

    private:
        double actual;
        std::string assertion_file;
        int assertion_line;
    };

    template <>
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
            if (fabs(actual - expected) > epsilon)
            {
                std::stringstream message;
                message << "Expected " << expected << ", was " << actual;
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toEqual(float expected, float epsilon = 0.00001f)
        {
            toBe(expected, epsilon);
        }

        void toBeGreaterThan(float expected)
        {
            if (actual < expected)
            {
                std::stringstream message;
                message << "Expected " << expected << " to be greather than" << actual;
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toBeLessThan(float expected)
        {
            if (actual > expected)
            {
                std::stringstream message;
                message << "Expected " << expected << " to be less than" << actual;
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

    private:
        float actual;
        std::string assertion_file;
        int assertion_line;
    };

    template <class T>
    class Assertion<std::vector<T>>
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
            if (expected.size() != actual.size())
            {
                std::stringstream message;
                message << "Vector sizes do not match, expected " << expected.size() << " items but had " << actual.size() << " items";
                throw AssertionError(assertion_file, assertion_line, message.str());
            }

            for (size_t i = 0; i < expected.size(); ++i)
            {
                if (expected[i] != actual[i])
                {
                    std::stringstream message;
                    message << "Vector item mismatch at position " << i << ", expected " << expected[i] << " but was " << actual[i];
                    throw AssertionError(assertion_file, assertion_line, message.str());
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

            for (size_t i = 0; i < actual.size(); ++i)
            {
                if (actual[i] == item)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                std::stringstream message;
                message << "Item " << item << " not found in vector";
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toHaveLength(size_t size)
        {
            if (actual.size() != size)
            {
                std::stringstream message;
                message << "Vector sizes does not match, expected " << size << " items but had " << actual.size() << " items";
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

    private:
        std::vector<T> actual;
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
            if (expected != actual)
            {
                std::stringstream message;

                if (expected.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH)
                {
                    message << "String mismatch," << std::endl;
                    message << "  Expected: \"" << expected << "\"" << std::endl;
                    message << "  Actual  : \"" << actual << "\"";
                }
                else
                {
                    message << "Expected \"" << expected << "\", was \"" << actual << "\"";
                }

                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toMatch(std::string expected)
        {
            toContain(expected);
        }

        void toMatch(std::string expected_string, std::regex expected)
        {
            if (!std::regex_search(actual, expected))
            {
                std::stringstream message;
                if (expected_string.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH)
                {
                    message << "Expected pattern mismatch," << std::endl;
                    message << "  Regexp: \"" << expected_string << "\"" << std::endl;
                    message << "  Actual: \"" << actual << "\"";
                }
                else
                {
                    message << "Expected pattern \"" << expected_string << "\" did not match with \"" << actual << "\"";
                }

                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toEqual(std::string expected)
        {
            toBe(expected);
        }

        void toContain(std::string expected)
        {
            if (actual.find(expected) == std::string::npos)
            {
                std::stringstream message;

                if (expected.size() > CLIP_STRING_LENGTH || actual.size() > CLIP_STRING_LENGTH)
                {
                    message << "Substring not found," << std::endl;
                    message << "  Expected: \"" << expected << "\"" << std::endl;
                    message << "  Actual  : \"" << actual << "\"";
                }
                else
                {
                    message << "\"" << expected << "\" not present inside \"" << actual << "\"";
                }

                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

        void toHaveLength(size_t length)
        {
            if (actual.length() != length)
            {
                std::stringstream message;
                message << "Length of \"" << actual << "\" expected to be " << length << ", was " << actual.length();
                throw AssertionError(assertion_file, assertion_line, message.str());
            }
        }

    private:
        std::string actual;
        std::string assertion_file;

        int assertion_line;
    };

    template <class T>
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

    void onSignalRaised(int sig)
    {
        std::string signal_as_string(strsignal(sig));

        // current_test_case->message = "Signal raised by test (" + signal_as_string + ")";

        // appendAssertionFailure(&assertion_failures, current_test_case->message, current_test_case->file, current_test_case->line);

        longjmp(__cest_globals.jump_env, 1);
    }
}

int main(int argc, const char *argv[])
{
    __cest_globals.command_line_options = cest::parseArgs(argc, argv);

    if (__cest_globals.command_line_options.help)
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

    if (__cest_globals.command_line_options.random_seed_present)
        __cest_globals.random_seed = __cest_globals.command_line_options.random_seed;
    else
        __cest_globals.random_seed = std::chrono::system_clock::now().time_since_epoch().count();

    // if (command_line_options.randomize)
    //     cest::configureRandomizedTests(&test_suite, random_seed);

    cest::TestSuite *suite = &__cest_globals.root_test_suite;
    cest::runTestSuite(suite, suite->name);

    return 0;
}
