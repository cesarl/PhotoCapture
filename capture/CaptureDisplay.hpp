#ifndef					__CAPTURE_DISPLAY_HPP__
# define				__CAPTURE_DISPLAY_HPP__

#include				<allegro5/allegro.h>
#include				<memory>
#include				"PubSub.hpp"
#include				"Log.hpp"
#include				"Image.hpp"
#include				"FrameInfo.hpp"

class					CaptureDisplay : public PubSub
{
public:
  CaptureDisplay()
    : _display(nullptr)
    , _waitingImage(new Image())
    , _currentImage(new Image())
    , _testingImage(new Image())
    , _counter(0.0f)
    , _delay(3.0f)
    , _last(0.0f)
    , _testing(false)
  {}

  virtual ~CaptureDisplay()
  {
    if (_display)
      al_destroy_display(_display);
  }

  bool					init()
  {
    ALLEGRO_MONITOR_INFO		monitor;

    if (!al_get_monitor_info(0, &monitor))
      return false;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    _display = al_create_display(monitor.x2 / 4, monitor.y2 / 4);
    if (!_display)
      return false;

    sub("setWaitingImage", [&](std::string path){
	setWaitingImage(path);
      });

    sub("setCaptureDelay", [&](int d){
	_delay = (float)d / 1000.0f;
      });

    sub("newFrame", [&](FrameInfo infos){
	newFrame(infos);
      });

    sub("toggleTestMode", [&](){
	_testing = !_testing;
	if (_testing)
	  {
	    _testingImage = std::unique_ptr<Image>(new Image("./assets/test-mode.png"));
	  }
	else
	  _testingImage = std::unique_ptr<Image>(new Image());
      });

    return true;
  }

  void					setWaitingImage(const std::string &path)
  {
    _waitingImage = std::unique_ptr<Image>(new Image(path));
  }

  void					update()
  {
    al_set_target_backbuffer(_display);
    al_clear_to_color(al_map_rgb(0,255,0));
    if (_counter <= 0.0f)
      {
	_waitingImage->display();
      }
    else
      {
	float d = al_get_time() - _last;
	_counter -= d;
	_currentImage->display();
	_last = al_get_time();
      }
    _testingImage->displayMini();
    al_flip_display();
  }

  void					newFrame(FrameInfo &infos)
  {
    _currentImage = std::unique_ptr<Image>(new Image(infos.path));
    _counter = _delay;
    _last = al_get_time();
    update();
  }

private:
  ALLEGRO_DISPLAY			*_display;
  std::unique_ptr<Image>		_waitingImage;
  std::unique_ptr<Image>		_currentImage;
  std::unique_ptr<Image>		_testingImage;
  float					_counter;
  float					_delay;
  float					_last;
  bool					_testing;
};

#endif					//__CAPTURE_DISPLAY_HPP__
