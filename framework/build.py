#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys
import yaml

RED = '\033[1m\033[31m'
GREEN = '\033[1m\033[32m'
RESET = '\033[0m'

def read_file(file):
    f = open(file, 'r')
    data = f.read()
    f.close()

    return data


def test_cases():
    spec = yaml.load(read_file('spec/spec.yml'))
    return spec, spec.keys()


def files_for_test(name, test):
    sources_to_build = test['sources'] if 'sources' in test else []
    return ['spec/{name}.cpp'.format(name=name)] + sources_to_build


def compile(test_name, files):
    binary = 'build/' + test_name.replace('.cpp', '')
    cmd = 'g++ -Ispec -Iinclude -Iframework -O0 -g {srcs} -o {obj}'.format(srcs=' '.join(files), obj=binary)

    if os.system(cmd) != 0:
        print RED + '❌ Build failed' + RESET
        sys.exit()


def execute(test_name):
    return os.system('build/' + test_name + ' -s')


def clean_test_results():
    try:
        os.remove('log.xml')
    except:
        pass

    try:
        os.remove('test_summary.jsonl')
    except:
        pass


tests, names = test_cases()

clean_test_results()

print GREEN + 'Building tests and sources' + RESET

for test_name in names:
    files = files_for_test(test_name, tests[test_name])
    compile(test_name, files)

tests_failed = False

for test_name in names:
    if execute(test_name) != 0:
        tests_failed = True

sys.exit(1 if tests_failed else 0)