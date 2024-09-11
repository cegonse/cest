#pragma once
#include <iostream>

#include "globals.hpp"

namespace cest
{
  void runTestSuite(TestSuite *suite, std::string name)
    {
        std::cout << "Running test suite -> " << name << std::endl;

        if (suite->before_all.fn)
        {
            std::cout << "beforeAll() for " << name << std::endl;
            suite->before_all.fn();
        }

        for (cest::TestCase *test_case : suite->test_cases)
        {
            std::cout << "Running test case -> " << test_case->name << std::endl;
            __cest_globals.current_test_case = test_case;

            if (suite->before_each.fn)
            {
                std::cout << "beforeEach() for " << name << std::endl;
                suite->before_each.fn();
            }

            try
            {
                if (test_case->condition == cest::TestCaseCondition::Skipped)
                    throw cest::ForcedPassError();

                test_case->fn.fn();
                setjmp(__cest_globals.jump_env);
            }
            catch (const cest::AssertionError &error)
            {
                // handleFailedTest(test_case);
                std::cout << "Failed test -> " << error.file << " , " << error.line << " , " << error.message << std::endl;
            }
            catch (const cest::ForcedPassError &error)
            {
                if (suite->after_each.fn)
                    suite->after_each.fn();

                // cest::printTestResult(test_case, assertion_failures);
                continue;
            }
            catch (...)
            {
                // handleTestException(test_case);
                if (suite->after_each.fn)
                    suite->after_each.fn();
                continue;
            }

            if (suite->after_each.fn)
            {
                std::cout << "afterEach() for " << name << std::endl;
                suite->after_each.fn();
            }

            // test_case->test_failed = current_test_failed;
            // cest::printTestResult(test_case, assertion_failures);
        }

        if (suite->after_all.fn)
        {
            std::cout << "beforeAll() for " << name << std::endl;
            suite->after_all.fn();
        }

        for (auto &pair : suite->test_suites)
            runTestSuite(pair.second, pair.first);
    }
}