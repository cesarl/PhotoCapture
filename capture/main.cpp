#include <cstdlib>
#include "Core.hpp"

int main(void)
{
  Core core;

  core.init();

  // for test purposes
  //
  //
  PubSubKey keyWaitingImage("setWaitingImage");
  const std::string imagePath("./assets/defaultWaitingPicture.png");
  PubSub::pub(keyWaitingImage, imagePath);

  PubSubKey keyDestinationPath ("setDestinationFolder");
  const std::string destinationPath("/tmp/testFolder/");
  PubSub::pub(keyDestinationPath, destinationPath);

  //
  //
  // end

  core.run();
  return EXIT_SUCCESS;
}
