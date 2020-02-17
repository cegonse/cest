#!/usr/bin/env python
# -*- coding: utf-8 -*-
from compilation import junit_generation
from compilation import files


def main():
    test_suites = files.parse_jsonl(files.read_file('test_summary.jsonl'))
    xml = junit_generation.generate_junit_xml(test_suites)
    files.create_text_file('log.xml', xml)


if __name__ == "__main__":
    main()
