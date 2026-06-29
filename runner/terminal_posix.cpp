#include "terminal.h"
#include <termios.h>
#include <unistd.h>
#include <cstdio>

char Terminal::waitForKey()
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
