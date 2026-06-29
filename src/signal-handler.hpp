#pragma once
#include <csignal>
#include <csetjmp>
#include <string>

#include "globals.hpp"
#include "platform.hpp"

namespace cest
{
  void onSignalRaised(int sig)
  {
    std::string signal_as_string = cest::platformSignalName(sig);
    cest::TestCase *test_case = __cest_globals.current_test_case;

    test_case->failed = true;
    test_case->failure_message = signal_as_string;
    test_case->failure_file = test_case->fn.file;
    test_case->failure_line = test_case->fn.line;

    longjmp(__cest_globals.jump_env, 1);
  }

  void configureSignals()
  {
    cest::configurePlatformSignals(cest::onSignalRaised);
  }

  void registerSignalHandler()
  {
    setjmp(__cest_globals.jump_env);
  }
}
