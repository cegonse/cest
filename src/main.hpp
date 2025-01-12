#include <iostream>
#include <chrono>

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

int main(int argc, const char *argv[])
{
  cest::configureSignals();
  cest::TestSuite *root_suite = &__cest_globals.root_test_suite;

  cest::CommandLineOptions command_line_options = cest::parseArgs(argc, argv);
  if (command_line_options.help)
  {
    cest::showHelp(argv[0]);
    return 0;
  }

  if (command_line_options.randomize)
  {
    int seed = command_line_options.random_seed_present ? command_line_options.random_seed : std::chrono::system_clock::now().time_since_epoch().count();
    cest::randomizeTests(root_suite, seed, cest::defaultRandomFn);
  }

  cest::configureFocusedTestSuite(root_suite);
  cest::runTestSuite(root_suite);

  if (command_line_options.only_test_suite_result)
    cest::printSuiteSummaryResult(root_suite);
  else if (command_line_options.tree_test_suite_result)
    cest::printTreeSuiteResult(root_suite);
  else
    cest::printTestSuiteResult(root_suite);

  std::string binary_path(argv[0]);
  auto binary_name = binary_path.substr(binary_path.rfind('/') + 1);
  cest::saveSummaryFile(binary_name, binary_path, root_suite);

  auto status_code = cest::numFailedTests(root_suite);

  cest::cleanUpTestSuite(root_suite);
  return status_code;
}
