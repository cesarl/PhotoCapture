#ifndef					  __CORE_HPP__
# define				  __CORE_HPP__

#include				<allegro5/allegro.h>
#include				<allegro5/allegro_opengl.h>
#include				<allegro5/allegro_image.h>

#include				"PubSub.hpp"
#include				"CameraContext.hpp"
#include				"CaptureListener.hpp"
#include				"CaptureDisplay.hpp"
#include				"SlideshowDisplay.hpp"
#include				"OptionParser.hpp"

class Core : public PubSub
{
public:
  Core()
    : _running(false)
    , _eventQueue(nullptr)
    , _timer(nullptr)
  {}

  virtual ~Core()
  {
    if (_eventQueue)
      al_destroy_event_queue(_eventQueue);
    if (_timer)
      al_destroy_timer(_timer);
  }

  bool					init()
  {
    if (!al_init())
      return false;
    if (!al_init_image_addon())
      return false;
    if (!al_install_keyboard())
      return false;
    if (!al_install_mouse())
      return false;
    if (!_cameraContext.init())
      return false;
    if (!_captureListener.init())
      return false;
    if (!_captureDisplay.init())
      return false;
    if (!_slideshowDisplay.init())
      return false;
    if (!_optionParser.init())
      return false;

    _eventQueue = al_create_event_queue();
    if (!_eventQueue)
      return false;

    _timer = al_create_timer(1.0f / 30.0f);
    if (!_timer)
      return (false);

    al_register_event_source(_eventQueue, al_get_keyboard_event_source());

    // I'll use pPhoto wait for event as timer
    //al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));

    al_start_timer(_timer);

    // init subscriptions

    sub("stopRunning", [&](){
	this->_running = false;
      });

    return true;
  }

  void					run()
  {
    _running = true;

    ALLEGRO_EVENT			event;

    while (_running)
      {
	while (al_get_next_event(_eventQueue, &event))
	  {
	    if (event.type == ALLEGRO_EVENT_KEY_UP)
	      {
		PubSubKey key("keyUp");
		pub(key, event);
		if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		  {
		    _running = false;
		    return;
		  }
	      }
	    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	      {
		PubSubKey key("keyDown");
		pub(key, event);
		if (event.keyboard.keycode == ALLEGRO_KEY_T)
		  {
		    PubSubKey testModeKey("toggleTestMode");
		    pub(testModeKey);
		  }
	      }
	  }
	_cameraContext.update();
	_captureDisplay.update();
	_slideshowDisplay.update();
      }
  }

private:
  CameraCtx				_cameraContext;
  CaptureListener			_captureListener;
  CaptureDisplay			_captureDisplay;
  SlideshowDisplay			_slideshowDisplay;
  OptionParser				_optionParser;
  bool					_running;
  ALLEGRO_EVENT_QUEUE			*_eventQueue;
  ALLEGRO_TIMER				*_timer;
};

#endif					//__CORE_HPP__
