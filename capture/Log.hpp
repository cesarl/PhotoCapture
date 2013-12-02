#ifndef					__LOG_HPP__
# define				__LOG_HPP__

#include				"PubSub.hpp"

void logAndQuit(bool val, const std::string &message)
{
  if (!val)
    return;
  std::cerr << message << std::endl;
  PubSubKey key("stopRunning");
  PubSub::pub(key);
}

#endif					//__LOG_HPP__
