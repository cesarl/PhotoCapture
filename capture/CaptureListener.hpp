#ifndef					__CAPTURE_LISTENER_HPP__
# define				__CAPTURE_LISTENER_HPP__

#include				<gphoto2/gphoto2.h>
#include				<allegro5/allegro.h>
#include				<sys/types.h>
#include				<dirent.h>
#include				<sys/stat.h>
#include				<fcntl.h>
#include				<stdio.h>
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
      path = _destinationFolder + "frame_" + std::to_string(id) + ".jpg";
    else
      path = _destinationFolder + "TEST_frame_" + std::to_string(id) + ".jpg";

    // copy image from camera to path
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
    FrameInfo frameInfo(path, id, _onTestMode);
    _list.push_back(frameInfo);

    // inform others about new frame
    PubSubKey key("newFrame");
    pub(key, frameInfo);
  }

  void					setDestinationFolder(const std::string &path)
  {
    // reset values
    _list.clear();
    _testCounter = 0;
    _counter = 0;
    _destinationFolder = path;

    // ckeck if directory exists or create it
    ALLEGRO_FS_ENTRY *fs = al_create_fs_entry(path.c_str());
    if (!logAndQuit(fs == nullptr, "Allegro FS creation failed"))
      return;
    if (!al_fs_entry_exists(fs))
      {
	if (!logAndQuit(!al_make_directory(path.c_str()), "Directory creation failed"))
	  return;
      }

    // scan directory
    DIR					*dir;
    struct dirent			*ent;

    dir = opendir(path.c_str());
    if (!logAndQuit(dir == nullptr, "Failed to open new destination dir"))
      return;
    while ((ent = readdir(dir)) != nullptr)
      {
	std::string name(ent->d_name);
	if(name.substr(name.find_last_of(".") + 1) == "jpg"
	   && name.find("TEST") == std::string::npos)
	  {
	    unsigned int imageId = 0;
	    std::string strId = name.substr(0, name.size() - 4);
	    strId = name.substr(6, name.size() - 6);
	    imageId = std::stoul(strId);
	    if (imageId >= _counter)
	      {
		_counter = imageId + 1;
	      }
	    // register image information in list
	    FrameInfo frameInfo(path + name, imageId, false);
	    _list.push_back(frameInfo);

	    // inform others about new frame
	    PubSubKey key("existingFrame");
	    pub(key, frameInfo);
	  }
      }
    closedir(dir);
  }

  void					toggleTestMode()
  {
    _onTestMode = !_onTestMode;
    if (!_onTestMode)
      {
	auto e = std::begin(_list);
	while (e != std::end(_list))
	  {
	    auto t = e;
	    ++e;
	    if (t->test)
	      {
		remove(t->path.c_str());
		_list.erase(t);
	      }
	  }
      }
  }

  bool					init()
  {
    sub("newImageCaptured", [&](CameraFilePath *infos, GPContext *context, Camera *camera){
	copyImageToFolder(infos, context, camera);
      });

    sub("setDestinationFolder", [&](const std::string path){
	setDestinationFolder(path);
      });

    sub("toggleTestMode", [&](){
	toggleTestMode();
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
