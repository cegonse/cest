#include "runner.h"
#include "process.h"
#include "directory.h"
#include "output.h"

void Runner::runTestsInCurrentPath()
{
  std::stringstream out;
  const auto executables = Directory::findExecutableFiles(Directory::cwd(), "test_");

  for (const auto& test_file : executables)
  {
    out << "Running test " << test_file << std::endl;
    Output::print(out);

    Process::runExecutable(test_file, [&out](const auto& output) {
      out << output;
      Output::print(out);
    });
  }
}
