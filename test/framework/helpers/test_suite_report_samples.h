#pragma once
#include <string>

std::string sample_json_with_zero_test_cases = "\
{\
\"name\":\"test something\",\
\"tests\":0,\
\"failures\":0,\
\"errors\":0,\
\"skipped\":0,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[]\
}\
";

std::string sample_json_with_one_passed_test_case = "\
{\
\"name\":\"test something\",\
\"tests\":1,\
\"failures\":0,\
\"errors\":0,\
\"skipped\":0,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[\
{\
\"name\":\"should pass\",\
\"time\":\"\"\
}\
]\
}\
";

std::string sample_json_with_one_failed_test_case = "\
{\
\"name\":\"test something\",\
\"tests\":1,\
\"failures\":1,\
\"errors\":0,\
\"skipped\":0,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[\
{\
\"name\":\"should fail\",\
\"time\":\"\",\
\"failure_message\":\"expected blue, was red\"\
}\
]\
}\
";

std::string sample_json_with_one_failed_test_case_and_escaped_backslashes_and_double_quotes = "\
{\
\"name\":\"test something\",\
\"tests\":1,\
\"failures\":1,\
\"errors\":0,\
\"skipped\":0,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[\
{\
\"name\":\"should fail\",\
\"time\":\"\",\
\"failure_message\":\"Expected pattern .*\\\\d+ apples did not match with I have 12 \\\"apples\"\
}\
]\
}\
";

std::string sample_json_with_two_passed_test_cases = "\
{\
\"name\":\"test something\",\
\"tests\":2,\
\"failures\":0,\
\"errors\":0,\
\"skipped\":0,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[\
{\
\"name\":\"should pass\",\
\"time\":\"\"\
},\
{\
\"name\":\"should also pass\",\
\"time\":\"\"\
}\
]\
}";

std::string sample_json_with_two_passed_and_one_failed_test_cases = "\
{\
\"name\":\"test something\",\
\"tests\":3,\
\"failures\":1,\
\"errors\":0,\
\"skipped\":0,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[\
{\
\"name\":\"should pass\",\
\"time\":\"\"\
},\
{\
\"name\":\"should also pass\",\
\"time\":\"\"\
},\
{\
\"name\":\"should fail\",\
\"time\":\"\",\
\"failure_message\":\"expected blue, was red\"\
}\
]\
}";

std::string sample_json_with_one_skipped_test_case = "\
{\
\"name\":\"test something\",\
\"tests\":1,\
\"failures\":0,\
\"errors\":0,\
\"skipped\":1,\
\"time\":\"\",\
\"timestamp\":\"\",\
\"hostname\":\"\",\
\"test_cases\":[\
{\
\"name\":\"should not run\",\
\"time\":\"\",\
\"skipped\":true\
}\
]\
}\
";
