#include <cstdlib>
#include "Core.hpp"

int main(int, char **)
{

  system("killall PTPCamera");

  Core core;

  if (!core.init())
    {
      std::cerr << "Error at initialisation." << std::endl;
    }

  // for test purposes
  //
  //
  PubSubKey keyWaitingImage("setWaitingImage");
  const std::string imagePath("./assets/defaultWaitingPicture.png");
  PubSub::pub(keyWaitingImage, imagePath);

  //
  //
  // end

  core.run();
  return EXIT_SUCCESS;
}
