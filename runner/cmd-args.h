#pragma once
#include <string>

class CmdArgs
{
  public:
    CmdArgs(int argc, char* argv[]);
    const std::string& path();
    bool watch();
    bool help();
    const std::string& grep();

  private:
    void parseArg(const std::string& arg);
    std::string _path;
    bool _watch;
    bool _help;
    std::string _grep;
};
