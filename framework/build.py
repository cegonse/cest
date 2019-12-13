#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys
import yaml


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
    print '\033[1m\033[32m⌚ Building tests and sources\033[0m'

    binary = 'build/' + test_name.replace('.cpp', '')
    cmd = 'g++ -Wno-write-strings -Iinclude -Iframework -O0 -g {srcs} -o {obj}'.format(srcs=' '.join(files), obj=binary)

    if os.system(cmd) != 0:
        print '\033[1m\033[31m❌ Build failed\033[0m'
        sys.exit()


def execute(test_name):
    os.system('build/' + test_name + ' -s')


tests, names = test_cases()

for test_name in names:
    files = files_for_test(test_name, tests[test_name])

    compile(test_name, files)

    execute(test_name)
