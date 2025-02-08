#pragma once

#ifndef __SANITIZE_ADDRESS__
namespace cest
{
  bool leaksDetected() { return false; }
  void initAddressSanitizer() {}
  void deinitAddressSanitizer() {}
}
#else
#include <sanitizer/lsan_interface.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "globals.hpp"

namespace cest
{
  void initAddressSanitizer()
  {
    dup2(STDOUT_FILENO, STDERR_FILENO);
    __cest_globals.saved_stderr = dup(STDERR_FILENO);
    close(STDERR_FILENO);
  }

  void deinitAddressSanitizer()
  {
    dup2(__cest_globals.saved_stderr, STDERR_FILENO);
  }

  bool leaksDetected()
  {
    const auto has_leaks = __lsan_do_recoverable_leak_check() != 0;
    __cest_globals.leaks_detected |= has_leaks;
    return has_leaks;
  }
}
#endif
