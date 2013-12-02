#ifndef					__CAP_CAMERA_HPP__
# define				__CAP_CAMERA_HPP__

#include				"PubSub.hpp"

#include				<iostream>
#include				<cstdio>
#include				<gphoto2/gphoto2.h>
#include				<allegro5/allegro.h>

static void				errordumper(GPLogLevel level, const char *domain, const char *str,
						    void *data)
{
  std::cerr << str << std::endl;
  static_cast<void>(level);
  static_cast<void>(domain);
  static_cast<void>(data);
}

static void				errorfunc (GPContext *context, const char *str, void *data)
{
  std::cerr << "\n*** Contexterror ***              \n" << str << std::endl;
  fflush(stderr);
  static_cast<void>(data);
  static_cast<void>(context);
}


static void				statusfunc (GPContext *context, const char *str, void *data)
{
  std::cerr << str << std::endl;
  fflush(stderr);
  static_cast<void>(data);
  static_cast<void>(context);
}


class CameraCtx : public PubSub
{
public:
  CameraCtx()
    : _camera(nullptr)
    , _context(nullptr)
  {}
  
  virtual ~CameraCtx()
  {}

  bool					init()
  {
    gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
    gp_camera_new(&_camera);
    _context = gp_context_new();
    gp_context_set_error_func (_context, errorfunc, NULL);
    gp_context_set_status_func (_context, statusfunc, NULL);

    // init subscriptions

    sub("keyDown", [&](ALLEGRO_EVENT &event){
	std::cout << "EVENT" << std::endl;
	static_cast<void>(event);
      });

    sub("updateCapture", [&](){
	CameraEventType			type;
	void				*data;

	gp_camera_wait_for_event(_camera, _refeshDelay, &type, &data, _context);
	if (type == GP_EVENT_FILE_ADDED)
	  {
	    PubSubKey key("newImageCaptured");
	    CameraFilePath *fileInfos = static_cast<CameraFilePath*>(data);
	    pub(key, fileInfos, _context, _camera);
	    std::cout << "Capture !!!" << std::endl;
	    std::cout <<fileInfos->name << " " << fileInfos->folder << std::endl;
	  }	
      });

    std::cout << "Camera initialize" << std::endl;
    return true;
  }

  void setRefreshDelay(const unsigned int delay)
  {
    _refeshDelay = delay;
  }

  unsigned int getRefreshDelay() const
  {
    return _refeshDelay;
  }

private:
  Camera				*_camera;
  GPContext				*_context;
  unsigned int				_refeshDelay;
private:
};

#endif    //__CAP_CAMERA_HPP__
