#include "output.h"
#include <cstring>
#include <iostream>

#define ASCII_BACKGROUND_RED "\u001b[41m"
#define ASCII_RED "\033[1m\033[31m"
#define ASCII_BOLD "\u001b[1m"
#define ASCII_RESET "\033[0m"

void Output::killedBySignal(const std::string& binary, int signal)
{
  std::cout
    << ASCII_BACKGROUND_RED
    << ASCII_BOLD
    << " FAIL "
    << ASCII_RESET
    << " "
    << binary
    << " → Killed by signal "
    << signal
    << " ("
    << ASCII_BOLD
    << ASCII_RED
    << strsignal(signal)
    << ASCII_RESET
    << ")"
    << std::endl;
}
