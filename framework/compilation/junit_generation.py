def total_tests(test_suites):
    num_tests = 0

    for suite in test_suites:
        num_tests += suite['tests']

    return num_tests

def total_failures(test_suites):
    num_tests = 0

    for suite in test_suites:
        num_tests += suite['failures']

    return num_tests

def sanitize(text):
    sanitized = text

    sanitized = sanitized.replace('"', '&quot;')
    sanitized = sanitized.replace('&', '&amp;')
    sanitized = sanitized.replace('\'', '&apos;')
    sanitized = sanitized.replace('<', '&lt;')
    sanitized = sanitized.replace('>', '&gt;')

    return sanitized.encode('utf-8')

def generate_junit_xml(test_suites):
    junit_xml = '<?xml version="1.0" encoding="utf-8"?>'
    junit_xml += '<testsuites name="Cest Test Results" time="" tests="{tests}" failures="{failures}" disabled="" errors="">'.format(
        tests=total_tests(test_suites),
        failures=total_failures(test_suites)
    )

    for suite in test_suites:
        junit_xml += '<testsuite name="{name}" tests="{tests}" failures="{failures}" time="{time}" skipped="{skipped}" timestamp="{timestamp}" hostname="{hostname}">'.format(
            name=sanitize(suite['name']),
            tests=suite['tests'],
            failures=suite['failures'],
            time=suite['time'],
            skipped=suite['skipped'],
            timestamp=suite['timestamp'],
            hostname=sanitize(suite['hostname'])
        )

        for test_case in suite['test_cases']:
            junit_xml += '<testcase name="{name}" time="{time}">'.format(
                name=sanitize(test_case['name']),
                time=test_case['time']
            )

            if 'failure_message' in test_case:
                junit_xml += '<failure message="{message}"/>'.format(
                    message=sanitize(test_case['failure_message'])
                )

            junit_xml += '</testcase>'
        junit_xml += '</testsuite>'
    junit_xml += '</testsuites>'

    return junit_xml.encode('utf-8')
