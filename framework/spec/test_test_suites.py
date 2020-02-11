import mock
import unittest

from compilation import test_suites
from compilation.test_suites import TestSuite

class TestTestSuites(unittest.TestCase):
    def test_no_test_suites_are_reported_when_no_files_exist_on_path(self):
        files = []

        suites = test_suites.all_suites(files)

        assert suites == []

    def test_no_test_suites_are_reported_when_no_cpp_exist_on_path(self):
        files = ['helpers/some_helper.h', 'data/helper.csv']

        suites = test_suites.all_suites(files)

        assert suites == []

    def test_reporting_test_suites_when_cpp_files_matching_suite_pattern_are_found(self):
        files = [
            'test_something.cpp',
            'dir/something_spec.cpp',
            'some/dir/describe_feature.cpp'
        ]

        suites = test_suites.all_suites(files)

        assert suites == [
           TestSuite(path='test_something.cpp', name='test_something'),
           TestSuite(path='dir/something_spec.cpp', name='something_spec'),
           TestSuite(path='some/dir/describe_feature.cpp', name='describe_feature')
        ]

    def test_reporting_test_suites_with_different_extensions(self):
        files = [
            'test/is/here/test_something.cpp',
            'dir/something_spec.cxx',
            'some/dir/describe_feature.cc'
        ]

        suites = test_suites.all_suites(files)

        assert suites == [
           TestSuite(path='test/is/here/test_something.cpp', name='test_something'),
           TestSuite(path='dir/something_spec.cxx', name='something_spec'),
           TestSuite(path='some/dir/describe_feature.cc', name='describe_feature')
        ]
