#include <cest>
#include "../runner.h"
#include "../directory.h"
#include "helpers/helpers.h"

describe("Runner", []() {
  beforeEach([]() {
    Directory::readTextFile_mockOutput("second.test.cpp|test/second_test|1|1|1");
    Directory::readTextFile_mockOutput("first.test.cpp|test/first_test|1|1|1");
    Process::runExecutable_mockElapsedTime(1000000);
  });

  it("executes a set of test binaries", []() {
    Runner::runTests({ "test/first_test", "test/second_test" });

    expect(Process::runExecutable_hasBeenCalledWith("test/first_test")).toBeTruthy();
    expect(Process::runExecutable_hasBeenCalledWith("test/second_test")).toBeTruthy();
  });

  it("outputs the test execution summary, parsing results from results file", []() {
    auto num_passed_suites = 0;
    auto num_failed_suites = 2;
    auto num_passed_tests = 2;
    auto num_failed_tests = 2;
    auto num_skipped_tests = 2;
    auto elapsed_time = 2.0f;

    Runner::runTests({ "test/first_test", "test/second_test" });

    expect(Directory::readTextFile_hasBeenCalledWith("/tmp/cest_first_test")).toBeTruthy();
    expect(Directory::readTextFile_hasBeenCalledWith("/tmp/cest_second_test")).toBeTruthy();
    expect(Output::printSummary_hasBeenCalledWith(
      num_passed_suites,
      num_failed_suites,
      num_passed_tests,
      num_failed_tests,
      num_skipped_tests,
      elapsed_time)
    ).toBeTruthy();
  });
});
