#ifndef					__CAPTURE_DISPLAY_HPP__
# define				__CAPTURE_DISPLAY_HPP__

#include				<allegro5/allegro.h>
#include				"PubSub.hpp"
#include				"Log.hpp"

class					CaptureDisplay : public PubSub
{
public:
  CaptureDisplay()
    : _display(nullptr)
  {}

  virtual ~CaptureDisplay()
  {}

  bool					init()
  {
    ALLEGRO_MONITOR_INFO		monitor;

    if (!al_get_monitor_info(0, &monitor))
      return false;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    _display = al_create_display(monitor.x2, monitor.y2);
    return true;
  }
private:
  ALLEGRO_DISPLAY			*_display;
};

#endif					//__CAPTURE_DISPLAY_HPP__
