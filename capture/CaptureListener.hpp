#ifndef					__CAPTURE_LISTENER_HPP__
# define				__CAPTURE_LISTENER_HPP__

#include				<gphoto2/gphoto2.h>
#include				<allegro5/allegro.h>
#include				<sys/types.h>
#include				<sys/stat.h>
#include				<fcntl.h>
#include				<list>
#include				<ctime>

#include				"PubSub.hpp"
#include				"Log.hpp"
#include				"FrameInfo.hpp"

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

  void					copyImageToFolder(CameraFilePath *infos, GPContext *context, Camera *camera)
  {
    int					fd;
    CameraFile				*file;
    std::string				path;
    unsigned int			id;

    // build path
    id = _onTestMode ? _testCounter++ : _counter++;
    if (!_onTestMode)
      path = _destinationFolder + "frame-" + std::to_string(id) + ".jpg";
    else
      path = _destinationFolder + "frame-" + std::to_string(id) + ".jpg";

    // copy image frome camera to path
    fd = open(path.c_str(), O_CREAT | O_WRONLY, 0644);
    logAndQuit(fd < 0, "error oppening fd");
    logAndQuit(gp_file_new_from_fd(&file, fd) < 0, "error file from fd");
    logAndQuit(gp_camera_file_get(camera, infos->folder, infos->name, GP_FILE_TYPE_NORMAL, file, context) < 0
	       ,"error getting file");
    logAndQuit (gp_camera_file_delete(camera, infos->folder, infos->name, context) < 0
		,"error getting image on SD");
    gp_file_free(file);
    close(fd);

    // register image information in list
    FrameInfo frameInfo(path, id - 1, _onTestMode);
    _list.push_back(frameInfo);

    // inform others about new frame
    PubSubKey key("newFrame");
    pub(key, frameInfo);
  }

  bool					init()
  {
    sub("newImageCaptured", [&](CameraFilePath *infos, GPContext *context, Camera *camera){
	copyImageToFolder(infos, context, camera);
      });

    return true;
  }
private:
  std::string				_destinationFolder;
  bool					_onTestMode;
  unsigned int				_counter;
  unsigned int				_testCounter;
  std::list<FrameInfo>			_list;
};

#endif					//__CAPTURE_LISTENER_HPP__
