#pragma once
#include <csignal>
#include <csetjmp>
#include <string>

#include "globals.hpp"

namespace cest
{
    void onSignalRaised(int sig)
    {
        std::string signal_as_string(strsignal(sig));
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
}
