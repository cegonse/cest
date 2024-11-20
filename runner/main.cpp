#include "runner.h"
#include "directory.h"

int main(int argc, char *argv[])
{
  const auto executables = Directory::findExecutableFiles(Directory::cwd(), "test_");

  return Runner::runTests(executables);
}
