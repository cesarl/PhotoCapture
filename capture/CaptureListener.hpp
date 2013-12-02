#ifndef					__CAPTURE_LISTENER_HPP__
# define				__CAPTURE_LISTENER_HPP__

#include				<gphoto2/gphoto2.h>
#include				<allegro5/allegro.h>
#include				<sys/types.h>
#include				<sys/stat.h>
#include				<fcntl.h>

#include				"PubSub.hpp"

class CaptureListener : public PubSub
{
public:
  CaptureListener()
    : _destinationFolder("./")
    , _onTestMode(false)
    , _counter(0)
    , _testCounter(0)
  {}

  virtual ~CaptureListener()
  {}

  bool					init()
  {
    sub("newImageCaptured", [&](CameraFilePath *infos, GPContext *context, Camera *camera){

	int				fd;
	CameraFile			*file;

	if (!_onTestMode)
	  fd = open(std::string(_destinationFolder + "frame" + std::to_string(_counter++) + ".jpg").c_str()
			 , O_CREAT | O_WRONLY, 0644);
	else
	  fd = open(std::string(_destinationFolder + "frame" + std::to_string(_testCounter++) + ".jpg").c_str()
			 , O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
	  {
	    std::cerr << "error oppening fd" << std::endl;
	  }
	if (gp_file_new_from_fd(&file, fd) < 0)
	  {
	    std::cerr << "error file from fd" << std::endl;
	  }
	if (gp_camera_file_get(camera, infos->folder, infos->name, GP_FILE_TYPE_NORMAL, file, context) < 0)
	  {
	    std::cerr << "error getting file" << std::endl;
	  }
	if (gp_camera_file_delete(camera, infos->folder, infos->name, context) < 0)
	  {
	    std::cerr << "error getting image on SD" << std::endl;
	  }
	gp_file_free(file);
      });

    return true;
  }
private:
  std::string				_destinationFolder;
  bool					_onTestMode;
  unsigned int				_counter;
  unsigned int				_testCounter;
};

#endif					//__CAPTURE_LISTENER_HPP__
