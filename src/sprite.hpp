#pragma once
#include "top.h"

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

	virtual ~Sprite()
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

	virtual SDL_Rect get_rect()
	{
		SDL_Rect r;
		r.x = x + w * xframe;
		r.y = y + h * yframe;
		r.w = w;
		r.h = h;
		return r;
	}
};

class AnimatedSprite : public Sprite
{
	Uint32 last_update;
public:
	bool animating;
	AnimatedSprite(mruby::NativeObject<Image> img, int x, int y, int w, int h) : 
		Sprite(img, x, y, w, h), last_update(0), animating(true)
	{
	}

	virtual ~AnimatedSprite()
	{
	}

	virtual SDL_Rect get_rect()
	{
		Uint32 now = SDL_GetTicks();
		if (animating)
		{
			if (now - last_update > 200)
			{
				xframe += 1;
				xframe = xframe % 4;
				this->last_update = now;
			}
		}
		return Sprite::get_rect();
	}
};