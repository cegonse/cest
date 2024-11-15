#include <cest>
#include "../runner.h"
#include "../directory.h"
#include "helpers/helpers.h"

describe("Runner", []() {
  it("executes all tests found in the current directory", []() {
    std::vector<std::string> test_files = { "first/test", "second/test" };
    Directory::findExecutableFiles_mockFiles(test_files);

    Runner::runTestsInCurrentPath();

    expect(Directory::findExecutableFiles_hasBeenCalledWith(Directory::cwd(), "test_")).toBeTruthy();
    expect(Process::runExecutable_hasBeenCalledWith("first/test")).toBeTruthy();
    expect(Process::runExecutable_hasBeenCalledWith("second/test")).toBeTruthy();
  });
});
