#pragma once
#include <charconv>
#include "helpers.h"

constexpr char RESULTS_TOKEN = '|';



struct TestResults
{
  TestResults(const std::string& input)
  {
    const auto parsed_results = Helpers::tokenize(input, RESULTS_TOKEN);

    if (parsed_results.size() != 5) return;

    this->src_path = parsed_results[0];
    this->bin_path = parsed_results[1];
    this->num_passed_tests = Helpers::fromString(parsed_results[2]);
    this->num_failed_tests = Helpers::fromString(parsed_results[3]);
    this->num_skipped_tests = Helpers::fromString(parsed_results[4]);
  }

  std::string src_path;
  std::string bin_path;
  int num_passed_tests;
  int num_failed_tests;
  int num_skipped_tests;
};
