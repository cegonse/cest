#pragma once

#ifndef __SANITIZE_ADDRESS__
namespace cest { bool leaksDetected() { return false; } }
#else
#include <sanitizer/lsan_interface.h>

namespace cest
{
  bool leaksDetected()
  {
    return __lsan_do_recoverable_leak_check() != 0;
  }
}
#endif
