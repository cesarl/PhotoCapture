#ifndef					__CAPTURE_DISPLAY_HPP__
# define				__CAPTURE_DISPLAY_HPP__

#include				<allegro5/allegro.h>
#include				<memory>
#include				"PubSub.hpp"
#include				"Log.hpp"
#include				"Image.hpp"

class					CaptureDisplay : public PubSub
{
public:
  CaptureDisplay()
    : _display(nullptr)
    , _waitingImage(new Image())
    , _currentImage(new Image())
    , _counter(0.0f)
    , _delay(3.0f)
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
    if (!_display)
      return false;

    sub("setWaitingImage", [&](std::string path){
	setWaitingImage(path);
      });
    return true;
  }

  void					setWaitingImage(const std::string &path)
  {
    _waitingImage = std::unique_ptr<Image>(new Image(path));
  }

  void					update()
  {
    if (_counter <= 0.0f)
      {
	_waitingImage->display();
      }
    else
      {
	static float last = al_get_time();
	float d = al_get_time() - last;
	_counter -= d;
	_currentImage->display();
	last = al_get_time();
      }
  }

private:
  ALLEGRO_DISPLAY			*_display;
  std::unique_ptr<Image>		_waitingImage;
  std::unique_ptr<Image>		_currentImage;
  float					_counter;
  float					_delay;
};

#endif					//__CAPTURE_DISPLAY_HPP__
