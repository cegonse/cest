empty_junit_xml = (
    '<?xml version="1.0" encoding="utf-8"?>'
    '<testsuites name="Cest Test Results" time="" tests="0" failures="0" disabled="" errors="">'
    '</testsuites>'
).encode('utf-8')

junit_xml_with_one_executed_test_suite_and_one_passed_test = (
    '<?xml version="1.0" encoding="utf-8"?>'
    '<testsuites name="Cest Test Results" time="" tests="1" failures="0" disabled="" errors="">'
        '<testsuite name="example test" tests="1" failures="0" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should pass" time="">'
            '</testcase>'
        '</testsuite>'
    '</testsuites>'
).encode('utf-8')

junit_xml_with_one_executed_test_suite_and_one_failed_test = (
    '<?xml version="1.0" encoding="utf-8"?>'
    '<testsuites name="Cest Test Results" time="" tests="1" failures="1" disabled="" errors="">'
        '<testsuite name="example test" tests="1" failures="1" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should fail" time="">'
                '<failure message="terrible, apocalyptical"/>'
            '</testcase>'
        '</testsuite>'
    '</testsuites>'
).encode('utf-8')

junit_xml_with_one_executed_test_suite_and_many_passed_tests = (
    '<?xml version="1.0" encoding="utf-8"?>'
    '<testsuites name="Cest Test Results" time="" tests="2" failures="0" disabled="" errors="">'
        '<testsuite name="example test" tests="2" failures="0" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should pass" time="">'
            '</testcase>'
            '<testcase name="should also pass" time="">'
            '</testcase>'
        '</testsuite>'
    '</testsuites>'
).encode('utf-8')

junit_xml_with_one_executed_test_suite_and_many_passed_and_failed_tests = (
    '<?xml version="1.0" encoding="utf-8"?>'
    '<testsuites name="Cest Test Results" time="" tests="2" failures="1" disabled="" errors="">'
        '<testsuite name="example test" tests="2" failures="1" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should pass" time="">'
            '</testcase>'
            '<testcase name="should fail" time="">'
                '<failure message="terrible, apocalyptical"/>'
            '</testcase>'
        '</testsuite>'
    '</testsuites>'
).encode('utf-8')

junit_xml_with_many_executed_test_suites_and_one_passed_test_in_each = (
    '<?xml version="1.0" encoding="utf-8"?>'
    '<testsuites name="Cest Test Results" time="" tests="2" failures="0" disabled="" errors="">'
        '<testsuite name="example test" tests="1" failures="0" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should pass" time="">'
            '</testcase>'
        '</testsuite>'
        '<testsuite name="other test" tests="1" failures="0" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should also pass" time="">'
            '</testcase>'
        '</testsuite>'
    '</testsuites>'
).encode('utf-8')

junit_xml_with_many_executed_test_suites_and_many_passed_and_failed_tests = (
    '<?xml version="1.0" encoding="utf-8"?>'
    '<testsuites name="Cest Test Results" time="" tests="4" failures="1" disabled="" errors="">'
        '<testsuite name="example test" tests="2" failures="1" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should fail" time="">'
                '<failure message="this is sad"/>'
            '</testcase>'
            '<testcase name="should pass" time="">'
            '</testcase>'
        '</testsuite>'
        '<testsuite name="other test" tests="2" failures="0" time="" skipped="0" timestamp="" hostname="">'
            '<testcase name="should also pass" time="">'
            '</testcase>'
            '<testcase name="this is fine" time="">'
            '</testcase>'
        '</testsuite>'
    '</testsuites>'
).encode('utf-8')
