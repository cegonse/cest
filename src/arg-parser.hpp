#pragma once
#include <stdexcept>
#include <string>

namespace cest
{
  struct CommandLineOptions
  {
    bool help;
    bool randomize;
    unsigned int random_seed;
    bool random_seed_present;
    bool generate_test_report;
  };

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

        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--report") == 0)
        {
          options.generate_test_report = true;
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