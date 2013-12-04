#ifndef					__LOG_HPP__
# define				__LOG_HPP__

#include				"PubSub.hpp"

bool logAndQuit(bool val, const std::string &message)
{
  if (!val)
    return true;
  std::cerr << message << std::endl;
  PubSubKey key("stopRunning");
  PubSub::pub(key);
  return false;
}

#endif					//__LOG_HPP__
