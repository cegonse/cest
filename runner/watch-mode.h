#pragma once

namespace WatchMode
{
  enum class Option
  {
    Failed,
    FileName,
    TestName,
    Trigger,
    Quit,
    None
  };

  void showHelp();
  Option waitForInput();
}
