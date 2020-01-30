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
