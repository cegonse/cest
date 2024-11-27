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

constexpr size_t MAX_TESTS_TO_SHOW = 5;

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

static constexpr bool isBackspace(char key)
{
  return key == '\b' || key == '\x7f';
}

static constexpr bool isPrintableCharacter(char key)
{
  return key >= ' ' && key <= '~';
}

static std::vector<std::string> findTestSourceFiles(
  const std::vector<std::string>& tests,
  const std::vector<Runner::TestRun>& runs
) {
  std::vector<std::string> out;

  for (const auto& run : runs)
  {
    for (const auto& test : tests)
    {
      if (run.binary_path == test)
        out.push_back(run.source_path);
    }
  }

  return out;
}

static void showWatchModeSearchResults(
  const std::string& input,
  const std::vector<Runner::TestRun>& last_results,
  const std::vector<std::string>& tests_to_run
) {
  std::cout
    << input
    << std::endl
    << std::endl;

  std::cout
    << ASCII_GRAY
    << "Found matches:"
    << ASCII_RESET
    << std::endl;

  const auto source_files = findTestSourceFiles(
    tests_to_run,
    last_results
  );

  for (size_t i=0; i < std::min(MAX_TESTS_TO_SHOW, source_files.size()); ++i)
    std::cout << "  " << source_files[i] << std::endl;

  if (source_files.size() > MAX_TESTS_TO_SHOW)
    std::cout << ASCII_GRAY << "...plus other " << source_files.size() - MAX_TESTS_TO_SHOW << " tests" << ASCII_RESET << std::endl;
}

static void readWatchModeInput(
  std::string& input,
  bool& input_complete
) {
  const auto key = waitForKey();

  if (isBackspace(key) && input.size() != 0)
    input.erase(input.size() - 1);
  else if (isPrintableCharacter(key))
    input += key;
  else if (key == '\n')
    input_complete = true;
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

          std::string input;
          bool input_complete = false;

          while (!input_complete)
          {
            readWatchModeInput(input, input_complete);
            clearScreen();
            showFileModeHelp();

            tests_to_run = filterTestsBy(
              last_results,
              input,
              [](Runner::TestRun result, std::string input) {
                return result.source_path.find(input) != std::string::npos;
              }
            );

            showWatchModeSearchResults(
              input,
              last_results,
              tests_to_run
            );
          }

          clearScreen();
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
