#include "helpers.h"
#include <sstream>
#include <charconv>

std::vector<std::string> Helpers::tokenize(const std::string& str, char token)
{
  std::vector<std::string> out;
  std::istringstream stream(str);
  std::string element;

  while (std::getline(stream, element, token))
    out.push_back(element);

  return out;
}

int Helpers::fromString(const std::string& str)
{
  int out;
  std::from_chars(&str.c_str()[0], &str.c_str()[str.size()], out);
  return out;
}
