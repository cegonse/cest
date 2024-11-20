#pragma once
#include <string>

class CmdArgs
{
  public:
    CmdArgs(int argc, char* argv[]);
    const std::string& path();
    bool watch();
    bool help();

  private:
    void parseArg(const std::string& arg);
    std::string _path;
    bool _watch;
    bool _help;
};
