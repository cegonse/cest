#pragma once
#include "types.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>

namespace cest
{
  CommandLineOptions parseArgs(int argc, const char *argv[])
  {
    CommandLineOptions options;
    std::map<std::string, std::function<void()>> parseSingle = {
      {"--help", [&]() { options.help = true; }},
      {"-h", [&]() { options.help = true; }},
      {"--randomize", [&]() { options.randomize = true; }},
      {"-r", [&]() { options.randomize = true; }},
      {"--json", [&]() { options.json_output = true; }},
      {"-j", [&]() { options.json_output = true; }},
      {"--only-suite-result", [&]() { options.only_test_suite_result = true; }},
      {"-o", [&]() { options.only_test_suite_result = true; }},
      {"--tree-suite-result", [&]() { options.only_test_suite_result = true; }},
      {"-t", [&]() { options.tree_test_suite_result = true; }},
      {"--print-test-list", [&]() { options.print_test_list = true; }},
      {"-l", [&]() { options.print_test_list = true; }}
    };

    if (argc > 1)
    {
      for (int i = 0; i < argc; ++i)
      {
        const auto arg = std::string(argv[i]);

        if (parseSingle.contains(arg))
          parseSingle[arg]();

        if (arg == "-s" || arg == "--seed")
        {
          if (i + 1 < argc)
          {
            try
            {
              options.random_seed = std::stoi(argv[i + 1]);
              options.random_seed_present = true;
            }
            catch (const std::invalid_argument &err) {}
          }
        }
      }
    }

    return options;
  }
}
