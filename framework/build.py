#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys
import yaml
import argparse

from compilation import test_suites
from compilation import files

RED = '\033[1m\033[31m'
GREEN = '\033[1m\033[32m'
RESET = '\033[0m'


def compile(test_name, files):
    binary = 'build/' + test_name
    cmd = 'g++ -Wall -Ispec -Iinclude -Iframework -O0 -g {srcs} -o {obj}'.format(srcs=' '.join(files), obj=binary)

    if os.system(cmd) != 0:
        print RED + '❌ Build failed' + RESET
        sys.exit()


def execute(test_name):
    return os.system('build/' + test_name + ' -s')


def generate_junit_report():
    os.system('framework/junit.py')


def parse_args():
    if len(sys.argv) == 2:
        return { 'tests': sys.argv[1] }

    return { 'tests': 'all' }


def main():
    print GREEN + 'Building tests and sources' + RESET

    args = parse_args()

    spec_files = files.all_files_in('spec')
    tests = test_suites.all_suites(spec_files)

    if args['tests'] != 'all':
        tests = filter(lambda test: args['tests'] in test.name, tests)

    for test in tests:
        compile(test.name, ['spec/' + test.path])

    tests_failed = False

    for test in tests:
        if execute(test.name) != 0:
            tests_failed = True

    generate_junit_report()

    sys.exit(1 if tests_failed else 0)


if __name__ == '__main__':
    main()
