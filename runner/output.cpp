#include "output.h"
#include <iostream>

void Output::print(std::stringstream& text)
{
  std::cout << text.str();
  text.clear();
}
