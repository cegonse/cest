#include "runner.h"
#include "cmd-args.h"
#include "directory.h"
#include "watch-mode.h"
#include <iostream>

static void showHelp()
{
  std::cout << "cest-runner [(Optional) Search Path][--watchAll][--help]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  [Search Path]: Directory to look for test executables" << std::endl;
  std::cout << "  --watchAll: Run cest in watch mode" << std::endl;
  std::cout << "  --help: Show this help message" << std::endl;
  exit(0);
}

static int runTestsInPath(const std::string& path)
{
  const auto executables = Directory::findExecutableFiles(path, "test_");
  return Runner::runTests(executables);
}

int main(int argc, char *argv[])
{
  CmdArgs args(argc, argv);

  if (args.help())
    showHelp();

  auto status = runTestsInPath(args.path());

  if (args.watch())
  {
    while (1)
    {
      auto option = WatchMode::Option::None;
      WatchMode::showHelp();

      do
      {
        option = WatchMode::waitForInput();
      } while (option == WatchMode::Option::None);

      switch (option)
      {
        case WatchMode::Option::Quit:
          exit(0);
          break;
        case WatchMode::Option::Trigger:
          runTestsInPath(args.path());
          break;
        default:
          break;
      }
    }
  }

  return status;
}
