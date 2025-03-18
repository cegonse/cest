#pragma once
#include "types.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>

namespace cest
{
  CommandLineOptions parseArgs(int argc, const char *argv[])
  {
    CommandLineOptions options = {0};

    if (argc > 1)
    {
      for (int i = 0; i < argc; ++i)
      {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
          options.help = true;
        }

        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--randomize") == 0)
        {
          options.randomize = true;
        }

        if (strcmp(argv[i], "-j") == 0 || strcmp(argv[i], "--json") == 0)
        {
          options.json_output = true;
        }

        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--only-suite-result") == 0)
        {
          options.only_test_suite_result = true;
        }

        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tree-suite-result") == 0)
        {
          options.tree_test_suite_result = true;
        }

        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--seed") == 0)
        {
          if (i + 1 < argc)
          {
            try
            {
              options.random_seed = std::stoi(argv[i + 1]);
              options.random_seed_present = true;
            }
            catch (const std::invalid_argument &err)
            {
            }
          }
        }
      }
    }

    return options;
  }
}
