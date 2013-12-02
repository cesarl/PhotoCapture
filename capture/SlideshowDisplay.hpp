#ifndef					__SLIDESHOW_DISPLAY_HPP__
# define				__SLIDESHOW_DISPLAY_HPP__

#include				<allegro5/allegro.h>
#include				<memory>
#include				<vector>
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
    , _counter(0.0f)
    , _delay(3.0f)
    , _last(0.0f)
    , _index(0)
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

    return true;
  }

  void					update()
  {
    al_set_target_backbuffer(_display);
    al_clear_to_color(al_map_rgb(0,0,0));
    if (_counter <= 0.0f)
      {
	if (_index >= _list.size() - 1)
	  {
	    _index = 0;
	  }
	else
	  ++_index;
	if (_list.size() != 0)
	  {
	    _currentImage = std::unique_ptr<Image>(new Image(_list[_index].path));
	  }
	_counter = _delay;
      }

    float d = al_get_time() - _last;
    _counter -= d;
    _currentImage->display();
    _last = al_get_time();

    al_flip_display();
  }

  void					newFrame(FrameInfo &infos)
  {
    _list.push_back(infos);
    _currentImage = std::unique_ptr<Image>(new Image(infos.path));
    _counter = _delay;
    _last = al_get_time();
    update();
  }

  void					existingFrame(FrameInfo &infos)
  {
    _list.push_back(infos);
    std::cout << "existingFrame" << std::endl;
  }

private:
  ALLEGRO_DISPLAY			*_display;
  std::unique_ptr<Image>		_currentImage;
  std::vector<FrameInfo>		_list;
  float					_counter;
  float					_delay;
  float					_last;
  unsigned int				_index;
};

#endif					//__SLIDESHOW_DISPLAY_HPP__
