#include "runner.h"
#include "cmd-args.h"
#include "directory.h"
#include "watch-mode.h"
#include <iostream>

static void showHelp()
{
  std::cout << "cest-runner [(Optional) Search Path][--watch][--help]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  [Search Path]: Directory to look for test executables" << std::endl;
  std::cout << "  --watch: Run cest in watch mode" << std::endl;
  std::cout << "  --help: Show this help message" << std::endl;
  exit(0);
}

int main(int argc, char *argv[])
{
  CmdArgs args(argc, argv);

  if (args.help())
    showHelp();

  if (!args.watch())
  {
    std::vector<Runner::TestRun> results;
    const auto executables = Directory::findExecutableFiles(args.path(), "test_");
    return Runner::runTests(executables, results);
  }
  else
  {
    WatchMode::runInPath(args.path());
  }

  return 0;
}
