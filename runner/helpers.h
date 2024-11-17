#pragma once
#include <vector>
#include <string>

namespace Helpers
{
  std::vector<std::string> tokenize(const std::string& str, char token);
  int fromString(const std::string& str);
}
