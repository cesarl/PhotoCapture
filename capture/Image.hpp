#ifndef					__IMAGE_HPP__
# define				__IMAGE_HPP__

#include				<allegro5/allegro.h>

class Image
{
public:
  Image(const std::string &path = "")
    : _bmp(nullptr)
    , _loaded(false)
  {
    if (path.size() == 0)
      return;
    _bmp = al_load_bitmap(path.c_str());
    if (!_bmp)
      {
	std::cerr << "error loading waiting image" << std::endl;
      }
    else
      {
	_loaded = true;
      }
  }

  void display()
  {
    if (!_loaded)
      return;
    al_draw_scaled_bitmap(_bmp,
			  0, 0,
			  al_get_bitmap_width(_bmp),
			  al_get_bitmap_height(_bmp),
			  0, 0,
			  al_get_display_width(al_get_current_display()),
			  al_get_display_height(al_get_current_display()),
			  0);
  }

  virtual ~Image()
  {}

private:
  ALLEGRO_BITMAP			*_bmp;
  bool					_loaded;
};

#endif					//__IMAGE_HPP__
