#include <cstdlib>
#include "CapCamera.hpp"

int main(void)
{
  CapCamera camera;

  camera.init();

  bool run = true;
  camera.waitForCapture(run);

  return EXIT_SUCCESS;
}
