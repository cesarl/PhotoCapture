#ifndef   __CAP_CAMERA_HPP__
# define  __CAP_CAMERA_HPP__

#include <iostream>
#include <cstdio>
#include <gphoto2/gphoto2.h>

    static void errordumper(GPLogLevel level, const char *domain, const char *str,
			    void *data) {
      std::cerr << str << std::endl;
      static_cast<void>(level);
      static_cast<void>(domain);
      static_cast<void>(data);
    }

    static void errorfunc (GPContext *context, const char *str, void *data)
    {
      std::cerr << "\n*** Contexterror ***              \n" << str << std::endl;
      fflush   (stderr);
      static_cast<void>(data);
      static_cast<void>(context);
    }


    static void statusfunc (GPContext *context, const char *str, void *data)
    {
      std::cerr << str << std::endl;
      fflush   (stderr);
      static_cast<void>(data);
      static_cast<void>(context);
    }


  class CapCamera
  {
  public:
    CapCamera()
      : _camera(nullptr)
      , _context(nullptr)
    {}

    virtual ~CapCamera()
    {}

    bool init()
    {
      gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
      gp_camera_new(&_camera);
      _context = gp_context_new();
      gp_context_set_error_func (_context, errorfunc, NULL);
      gp_context_set_status_func (_context, statusfunc, NULL);

      std::cout << "Camera initialize" << std::endl;
      return true;
    }

    void waitForCapture(bool &until)
    {
      CameraEventType type;
      void            *data;

      while (until)
	{
	  gp_camera_wait_for_event(_camera, 100, &type, &data, _context);
	  if (type == GP_EVENT_FILE_ADDED)
	    {
	      std::cout << "Capture !!!" << std::endl;
	    }
	}
    }
  private:
    Camera *_camera;
    GPContext *_context;
  private:
  };

#endif    //__CAP_CAMERA_HPP__