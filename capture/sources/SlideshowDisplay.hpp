#ifndef					__SLIDESHOW_DISPLAY_HPP__
# define				__SLIDESHOW_DISPLAY_HPP__

#include				<allegro5/allegro.h>
#include				<memory>
#include				<map>
#include				"PubSub.hpp"
#include				"Log.hpp"
#include				"Image.hpp"
#include				"FrameInfo.hpp"

class					SlideshowDisplay : public PubSub
{
public:
  SlideshowDisplay()
    : _display(nullptr)
    , _currentImage(new Image())
    , _testingImage(new Image())
    , _counter(0.0f)
    , _delay(3.0f)
    , _last(0.0f)
    , _testing(false)
  {}

  virtual ~SlideshowDisplay()
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

    sub("newFrame", [&](FrameInfo infos){
	newFrame(infos);
      });

    sub("existingFrame", [&](FrameInfo infos){
	existingFrame(infos);
      });

    sub("toggleTestMode", [&](){
	toggleTestMode();
      });

    sub("setSlideDelay", [&](int d){
	_delay = (float)d / 1000.0f;
      });

    _index = std::begin(_list);

    return true;
  }

  void					toggleTestMode()
  {
    _testing = !_testing;
    if (!_testing)
      {
	auto e = std::begin(_list);
	while (e != std::end(_list))
	  {
	    auto t = e;
	    ++e;
	    if (t->second.test)
	      _list.erase(t);
	  }
      }
    if (_testing)
      {
	_testingImage = std::unique_ptr<Image>(new Image("./assets/test-mode.png"));
      }
    else
      _testingImage = std::unique_ptr<Image>(new Image());
    _index = std::begin(_list);
  }

  void					update()
  {
    al_set_target_backbuffer(_display);
    al_clear_to_color(al_map_rgb(0,0,0));
    if (_counter <= 0.0f)
      {
	if (_index == std::end(_list))
	  _index = std::begin(_list);
	if (_index != std::end(_list))
	  _currentImage = std::unique_ptr<Image>(new Image(_index->second.path));
	if (_list.size() != 0 && _index != std::end(_list))
	  ++_index;
	_counter = _delay;
      }

    float d = al_get_time() - _last;
    _counter -= d;
    _currentImage->display();
    _testingImage->displayMini();
    _last = al_get_time();

    al_flip_display();
  }

  void					newFrame(FrameInfo &infos)
  {
    _list.insert(std::make_pair(infos.number, infos));
    _currentImage = std::unique_ptr<Image>(new Image(infos.path));
    _counter = _delay;
    _last = al_get_time();
    update();
  }

  void					existingFrame(FrameInfo &infos)
  {
    _list.insert(std::make_pair(infos.number, infos));
    _index = std::begin(_list);
  }

private:
  ALLEGRO_DISPLAY			*_display;
  std::unique_ptr<Image>		_currentImage;
  std::unique_ptr<Image>		_testingImage;
  std::map<unsigned int, FrameInfo>	_list;
  float					_counter;
  float					_delay;
  float					_last;
  std::map<unsigned int, FrameInfo>::iterator	_index;
  bool					_testing;
};

#endif					//__SLIDESHOW_DISPLAY_HPP__
