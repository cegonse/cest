import unittest

from spec.helpers import junit_xml_samples
from compilation import junit_generation


class TestJunitGeneration(unittest.TestCase):
    def test_generating_a_junit_report_when_no_tests_have_been_executed(self):
        test_suite_results = []

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.empty_junit_xml

    def test_generating_a_junit_report_when_one_test_suite_has_been_executed_and_one_test_has_passed(self):
        test_suite_results = [
            {
                'name': 'example test',
                'tests': 1,
                'failures': 0,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should pass',
                        'time': ''
                    }
                ]
            }
        ]

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.junit_xml_with_one_executed_test_suite_and_one_passed_test

    def test_generating_a_junit_report_when_one_test_suite_has_been_executed_and_one_test_has_failed(self):
        test_suite_results = [
            {
                'name': 'example test',
                'tests': 1,
                'failures': 1,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should fail',
                        'time': '',
                        'failure_message': 'terrible, apocalyptical'
                    }
                ]
            }
        ]

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.junit_xml_with_one_executed_test_suite_and_one_failed_test

    def test_generating_a_junit_report_when_one_test_suite_has_been_executed_and_several_tests_have_passed(self):
        test_suite_results = [
            {
                'name': 'example test',
                'tests': 2,
                'failures': 0,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should pass',
                        'time': ''
                    },
                    {
                        'name': 'should also pass',
                        'time': ''
                    }
                ]
            }
        ]

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.junit_xml_with_one_executed_test_suite_and_many_passed_tests

    def test_generating_a_junit_report_when_one_test_suite_has_been_executed_and_several_tests_have_passed_and_failed(self):
        test_suite_results = [
            {
                'name': 'example test',
                'tests': 2,
                'failures': 1,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should pass',
                        'time': ''
                    },
                    {
                        'name': 'should fail',
                        'time': '',
                        'failure_message': 'terrible, apocalyptical'
                    }
                ]
            }
        ]

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.junit_xml_with_one_executed_test_suite_and_many_passed_and_failed_tests

    def test_generating_a_junit_report_when_many_test_suites_have_been_executed_and_one_test_has_passed_in_each(self):
        test_suite_results = [
            {
                'name': 'example test',
                'tests': 1,
                'failures': 0,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should pass',
                        'time': ''
                    }
                ]
            },
            {
                'name': 'other test',
                'tests': 1,
                'failures': 0,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should also pass',
                        'time': ''
                    }
                ]
            }
        ]

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.junit_xml_with_many_executed_test_suites_and_one_passed_test_in_each

    def test_generating_a_junit_report_when_many_test_suites_have_been_executed_with_many_passed_and_failed_tests(self):
        test_suite_results = [
            {
                'name': 'example test',
                'tests': 2,
                'failures': 1,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should fail',
                        'time': '',
                        'failure_message': 'this is sad'
                    },
                    {
                        'name': 'should pass',
                        'time': ''
                    }
                ]
            },
            {
                'name': 'other test',
                'tests': 2,
                'failures': 0,
                'errors': 0,
                'skipped': 0,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should also pass',
                        'time': ''
                    },
                    {
                        'name': 'this is fine',
                        'time': ''
                    }
                ]
            }
        ]

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.junit_xml_with_many_executed_test_suites_and_many_passed_and_failed_tests

    def test_generating_a_junit_report_when_one_test_suite_has_been_executed_and_one_test_has_been_skipped(self):
        test_suite_results = [
            {
                'name': 'example test',
                'tests': 1,
                'failures': 0,
                'errors': 0,
                'skipped': 1,
                'time': '',
                'timestamp': '',
                'hostname': '',
                'test_cases': [
                    {
                        'name': 'should not run',
                        'time': '',
                        'skipped': True
                    }
                ]
            }
        ]

        xml = junit_generation.generate_junit_xml(test_suite_results)

        assert xml == junit_xml_samples.junit_xml_with_one_executed_test_suite_and_one_skipped_test