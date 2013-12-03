#ifndef				__OPTION_PARSER_HPP__
# define			__OPTION_PARSER_HPP__

#include			<iostream>
#include			<fstream>
#include			<string>
#include			"PubSub.hpp"
#include			"vendors/cJSON.h" 

class				OptionParser
{
public:
  OptionParser()
  {}

  virtual ~OptionParser()
  {}

  bool				init()
  {
    std::string			configFilePath("./config.json");
    std::string			line;
    std::ifstream		myfile(configFilePath);
    std::string			data;
    cJSON			*json;

    if (!myfile.is_open())
      {
	std::cerr << "config file " << configFilePath << " not found." << std::endl;
	return false;
      }

    while ( getline (myfile,line) )
      {
	data += line + "\n";
      }
    myfile.close();
    if (data.empty())
      {
	std::cerr << "config file " << configFilePath << " empty." << std::endl;
	return false;
      }
    json = cJSON_Parse(data.c_str());
    if (!json)
      {
	std::cerr << "Error parsing config file " << configFilePath << "." << std::endl;
      }

    // set destination path
    if (!setDestination(json))
      {
	std::cerr << "Destination error" << std::endl;
	cJSON_Delete(json);
	return false;
      }

    if (!setDelay(json))
      {
	std::cerr << "Setting delay error" << std::endl;
      }

    cJSON_Delete(json);
    return true;
  }
private:
  bool				setDestination(cJSON *json)
  {
      cJSON *appPath;
      cJSON *folder;
      std::string path;
      
      appPath = cJSON_GetObjectItem(json, "application-path");
      folder = cJSON_GetObjectItem(json, "destination-folder");
      if (appPath && folder)
	{
	  char *strAppPath;
	  char *strFolder;

	  strAppPath = cJSON_Print(appPath);
	  strFolder = cJSON_Print(folder);
	  if (strAppPath)
	    {
	      path = &(strAppPath)[1];
	      path = path.substr(0, path.size() - 1);
	      free(strAppPath);
	    }
	  if (strFolder)
	    {
	      path += &(strFolder[1]);
	      path = path.substr(0, path.size() - 1);
	      free(strFolder);
	    }
	  if (path.size() == 0)
	    return false;
	  if (path.back() != '/')
	    path += "/";
	  PubSubKey keyDestinationPath ("setDestinationFolder");
	  const std::string destinationPath(path);
	  PubSub::pub(keyDestinationPath, destinationPath);
	}
    return true;
  }

  bool				setDelay(cJSON *json)
  {
      cJSON *captureDelay;
      cJSON *slideDelay;

      captureDelay = cJSON_GetObjectItem(json, "delay-capture");
      slideDelay = cJSON_GetObjectItem(json, "delay-slideshow");
      if (captureDelay)
	{
	  char *str;
	  std::string delay;

	  str = cJSON_Print(captureDelay);
	  if (str)
	    {
	      delay = &(str)[1];
	      delay = delay.substr(0, delay.size() - 1);
	      free(str);
	    }
	  if (delay.size() > 0)
	    {
	      PubSubKey key ("setCaptureDelay");
	      PubSub::pub(key, std::atoi(delay.c_str()));
	    }
	}

      if (slideDelay)
	{
	  char *str;
	  std::string delay;

	  str = cJSON_Print(slideDelay);
	  if (str)
	    {
	      delay = &(str)[1];
	      delay = delay.substr(0, delay.size() - 1);
	      free(str);
	    }
	  if (delay.size() > 0)
	    {
	      PubSubKey key ("setSlideDelay");
	      PubSub::pub(key, std::atoi(delay.c_str()));
	    }
	}

      return true;
  }
};

#endif				//__OPTION_PARSER_HPP__
