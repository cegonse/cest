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
    return ['spec/{name}.cpp'.format(name=name)] + test['sources']


def compile(test_name, files):
    print GREEN + 'Building tests and sources' + RESET

    binary = 'build/' + test_name.replace('.cpp', '')
    cmd = 'g++ -Iinclude -Iframework -O0 -g {srcs} -o {obj}'.format(srcs=' '.join(files), obj=binary)

    if os.system(cmd) != 0:
        print RED + '❌ Build failed' + RESET
        sys.exit()


def execute(test_name):
    os.system('build/' + test_name + ' -s')


tests, names = test_cases()

for test_name in names:
    files = files_for_test(test_name, tests[test_name])

    compile(test_name, files)

    execute(test_name)
