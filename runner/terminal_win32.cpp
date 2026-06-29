#include "terminal.h"
#include <conio.h>

char Terminal::waitForKey()
{
  return static_cast<char>(_getch());
}
