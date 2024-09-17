#pragma once
#include <csignal>
#include <csetjmp>
#include <cstring>
#include <string>

#include "globals.hpp"

namespace cest
{
  void onSignalRaised(int sig)
  {
    std::string signal_as_string(strsignal(sig));
    cest::TestCase *test_case = __cest_globals.current_test_case;

    test_case->failed = true;
    test_case->failure_message = signal_as_string;
    test_case->failure_file = test_case->fn.file;
    test_case->failure_line = test_case->fn.line;

    longjmp(__cest_globals.jump_env, 1);
  }

  void configureSignals()
  {
    signal(SIGSEGV, cest::onSignalRaised);
    signal(SIGFPE, cest::onSignalRaised);
    signal(SIGBUS, cest::onSignalRaised);
    signal(SIGILL, cest::onSignalRaised);
    signal(SIGTERM, cest::onSignalRaised);
    signal(SIGXCPU, cest::onSignalRaised);
    signal(SIGXFSZ, cest::onSignalRaised);
  }

  void registerSignalHandler()
  {
    setjmp(__cest_globals.jump_env);
  }
}
