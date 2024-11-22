#include "watch-mode.h"
#include "directory.h"
#include "runner.h"
#include <iostream>
#include <array>
#include <functional>
#include <termios.h>
#include <unistd.h>

#define ASCII_GRAY "\u001b[38;5;243m"
#define ASCII_BLACK "\u001b[38;5;232m"
#define ASCII_BOLD "\u001b[1m"
#define ASCII_RESET "\033[0m"
#define ASCII_CLEAR "\033c"

enum class Option
{
  Failed,
  FileName,
  Trigger,
  Quit,
  None
};

static void clearScreen()
{
  std::cout << ASCII_CLEAR;
}

static char waitForKey()
{
    struct termios old_term, new_term;
    tcgetattr(STDIN_FILENO, &old_term);

    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

    return ch;
}

static Option waitForInput()
{
  auto key = waitForKey();

  switch (key)
  {
    case 'f':
      return Option::Failed;
      break;
    case 'p':
      return Option::FileName;
      break;
    case 'q':
      return Option::Quit;
      break;
    case '\n':
      return Option::Trigger;
      break;
  }

  return Option::None;
}

static std::string waitForString()
{
  std::string out;
  std::getline(std::cin, out);
  return out;
}

static void showHelp()
{
  std::cout << std::endl;
  std::cout << ASCII_BOLD << "Watch Usage" << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "f" << ASCII_RESET << ASCII_GRAY <<" to run only failed tests." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "p" << ASCII_RESET << ASCII_GRAY <<" to filter by a filename regex pattern." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "q" << ASCII_RESET << ASCII_GRAY <<" to quit watch mode." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "Enter" << ASCII_RESET << ASCII_GRAY << " to trigger a test run." << ASCII_RESET << std::endl;
}

static std::vector<Runner::TestRun> runTests(const std::vector<std::string>& tests)
{
  std::vector<Runner::TestRun> results;
  Runner::runTests(tests, results);
  return results;
}

static void showFileModeHelp()
{
  std::cout << ASCII_GRAY << "Input the test .cpp file filter to run." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << "Press " << ASCII_RESET << ASCII_BOLD << "Enter" << ASCII_RESET << ASCII_GRAY << " to submit:" << ASCII_RESET << std::endl;
  std::cout << " › ";
}

static std::vector<std::string> filterTestsBy(
  const std::vector<Runner::TestRun>& results,
  const std::string& input,
  std::function<bool(Runner::TestRun, std::string)> fn
) {
  std::vector<std::string> tests_to_run;

  for (const auto& result : results)
    if (fn(result, input)) tests_to_run.push_back(result.binary_path);

  return tests_to_run;
}

void WatchMode::runInPath(const std::string& path)
{
  std::vector<Runner::TestRun> last_results;
  std::vector<std::string> tests_to_run;
  auto option = Option::None;

  tests_to_run = Directory::findExecutableFiles(path, "test_");
  last_results = runTests(tests_to_run);

  while (1)
  {
    showHelp();

    do
    {
      option = waitForInput();
    } while (option == Option::None);

    switch (option)
    {
      case Option::FileName:
        {
          clearScreen();
          showFileModeHelp();
          const auto input = waitForString();
          clearScreen();

          tests_to_run = filterTestsBy(
            last_results,
            input,
            [](Runner::TestRun result, std::string input) {
              return result.source_path.find(input) != std::string::npos;
            }
          );

          runTests(tests_to_run);
        }
        break;
      case Option::Failed:
        {
          clearScreen();

          tests_to_run = filterTestsBy(
            last_results,
            "",
            [](Runner::TestRun result, std::string input) {
              return result.failed;
            }
          );

          runTests(tests_to_run);
        }
        break;
      case Option::Trigger:
        clearScreen();
        tests_to_run = Directory::findExecutableFiles(path, "test_");
        runTests(tests_to_run);
        break;
      case Option::Quit:
        exit(0);
        break;
      default:
        break;
    }
  }
}
