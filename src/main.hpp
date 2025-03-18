#include <iostream>
#include <chrono>
#include <unistd.h>

#include "types.hpp"
#include "globals.hpp"
#include "output.hpp"
#include "arg-parser.hpp"
#include "parametrized.hpp"
#include "test-builder.hpp"
#include "suite-runner.hpp"
#include "expect.hpp"
#include "randomized-tests.hpp"
#include "signal-handler.hpp"
#include "focus-test-suite.hpp"
#include "summary-file.hpp"
#include "json-output.hpp"

static std::string binaryPath(const char *argv)
{
  std::string binary_path(argv);
  return binary_path.substr(binary_path.rfind('/') + 1);
}

int main(int argc, const char *argv[])
{
  cest::TestSuite *root_suite = &__cest_globals.root_test_suite;
  cest::CommandLineOptions command_line_options = cest::parseArgs(argc, argv);

  if (command_line_options.help)
  {
    cest::showHelp(argv[0]);
    cest::cleanUpTestSuite(root_suite);
    return 0;
  }

  cest::configureSignals();

  if (command_line_options.randomize)
  {
    int seed = command_line_options.random_seed_present ? command_line_options.random_seed : std::chrono::system_clock::now().time_since_epoch().count();
    cest::randomizeTests(root_suite, seed, cest::defaultRandomFn);
  }

  cest::configureFocusedTestSuite(root_suite);
  cest::initAddressSanitizer();

  cest::runTestSuite(root_suite);

  if (!command_line_options.json_output)
  {
    if (command_line_options.only_test_suite_result)
      cest::printSuiteSummaryResult(root_suite);
    else if (command_line_options.tree_test_suite_result)
      cest::printTreeSuiteResult(root_suite);
    else
      cest::printTestSuiteResult(root_suite);

    cest::printAddressSanitizerClaim();
  }
  else
  {
    cest::dumpJsonResult(root_suite);
  }

  auto status_code = cest::numFailedTests(root_suite);
  cest::saveSummaryFile(binaryPath(argv[0]), std::string(argv[0]), root_suite);
  cest::cleanUpTestSuite(root_suite);
  cest::deinitAddressSanitizer();

  return status_code;
}
