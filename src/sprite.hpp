#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

#include "image.hpp"

class Sprite : public IImage
{
	std::shared_ptr<Image> img;
	int x, y, w, h;

public:
	int xframe, yframe;
	Sprite(mruby::NativeObject<Image> img, int x, int y, int w, int h) :
		img(img.get_shared_instance()), x(x), y(y), w(w), h(h), xframe(0), yframe(0)
	{
	}

	virtual int get_width() const
	{
		return w;
	}

	virtual int get_height() const
	{
		return h;
	}

	virtual std::shared_ptr<SDL_Texture> get_texture() const
	{
		return img->get_texture();
	}

	virtual SDL_Rect get_rect() const
	{
		SDL_Rect r;
		r.x = x + w * xframe;
		r.y = y + h * yframe;
		r.w = w;
		r.h = h;
		return r;
	}
};
