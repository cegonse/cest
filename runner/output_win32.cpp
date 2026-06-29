#include "output.h"
#include <iostream>
#include <iomanip>

#define ASCII_BACKGROUND_RED "\u001b[41m"
#define ASCII_RED "\033[1m\033[31m"
#define ASCII_BOLD "\u001b[1m"
#define ASCII_RESET "\033[0m"

static const char* exceptionCodeName(unsigned int code)
{
  switch (code)
  {
    case 0xC0000005: return "ACCESS_VIOLATION";
    case 0xC00000FD: return "STACK_OVERFLOW";
    case 0xC0000094: return "INTEGER_DIVIDE_BY_ZERO";
    case 0xC000001D: return "ILLEGAL_INSTRUCTION";
    case 0xC0000025: return "NONCONTINUABLE_EXCEPTION";
    case 0x80000003: return "BREAKPOINT";
    case 0xC0000026: return "INVALID_DISPOSITION";
    case 0xC000008C: return "ARRAY_BOUNDS_EXCEEDED";
    case 0xC000008E: return "FLOAT_DIVIDE_BY_ZERO";
    case 0xC0000090: return "FLOAT_INVALID_OPERATION";
    case 0xC0000091: return "FLOAT_OVERFLOW";
    case 0xC0000093: return "FLOAT_UNDERFLOW";
    default:         return "UNKNOWN_EXCEPTION";
  }
}

void Output::killedBySignal(const std::string& binary, int signal)
{
  std::cout
    << ASCII_BACKGROUND_RED
    << ASCII_BOLD
    << " FAIL "
    << ASCII_RESET
    << " "
    << binary
    << " → Terminated with exception 0x"
    << std::hex << static_cast<unsigned int>(signal) << std::dec
    << " ("
    << ASCII_BOLD
    << ASCII_RED
    << exceptionCodeName(static_cast<unsigned int>(signal))
    << ASCII_RESET
    << ")"
    << std::endl;
}
