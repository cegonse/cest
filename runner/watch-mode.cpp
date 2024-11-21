#include "watch-mode.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

#define ASCII_GRAY "\u001b[38;5;243m"
#define ASCII_BLACK "\u001b[38;5;232m"
#define ASCII_BOLD "\u001b[1m"
#define ASCII_RESET "\033[0m"

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

WatchMode::Option WatchMode::waitForInput()
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
    case 't':
      return Option::TestName;
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

void WatchMode::showHelp()
{
  std::cout << std::endl;
  std::cout << ASCII_BOLD << "Watch Usage" << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "f" << ASCII_RESET << ASCII_GRAY <<" to run only failed tests." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "p" << ASCII_RESET << ASCII_GRAY <<" to filter by a filename regex pattern." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "t" << ASCII_RESET << ASCII_GRAY <<" to filter by a test name regex pattern." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "q" << ASCII_RESET << ASCII_GRAY <<" to quit watch mode." << ASCII_RESET << std::endl;
  std::cout << ASCII_GRAY << " › Press " << ASCII_RESET << ASCII_BOLD << "Enter" << ASCII_RESET << ASCII_GRAY << " to trigger a test run." << ASCII_RESET << std::endl;
}
