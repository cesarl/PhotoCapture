#include <cstdlib>
#include "Core.hpp"

int main(void)
{
  Core core;

  core.init();
  core.run();
  return EXIT_SUCCESS;
}
