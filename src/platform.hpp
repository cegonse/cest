#pragma once
#include <csignal>
#include <cstring>
#include <string>

namespace cest
{
#if defined(_WIN32)
  inline std::string platformSignalName(int sig)
  {
    switch (sig)
    {
      case SIGSEGV: return "Segmentation fault";
      case SIGFPE:  return "Floating point exception";
      case SIGILL:  return "Illegal instruction";
      case SIGTERM: return "Terminated";
      case SIGABRT: return "Aborted";
      default:      return "Unknown signal (" + std::to_string(sig) + ")";
    }
  }

  template<typename Handler>
  void configurePlatformSignals(Handler handler)
  {
    signal(SIGSEGV, handler);
    signal(SIGFPE, handler);
    signal(SIGILL, handler);
    signal(SIGTERM, handler);
  }
#else
  inline std::string platformSignalName(int sig)
  {
    return std::string(strsignal(sig));
  }

  template<typename Handler>
  void configurePlatformSignals(Handler handler)
  {
    signal(SIGSEGV, handler);
    signal(SIGFPE, handler);
    signal(SIGBUS, handler);
    signal(SIGILL, handler);
    signal(SIGTERM, handler);
    signal(SIGXCPU, handler);
    signal(SIGXFSZ, handler);
  }
#endif
}
