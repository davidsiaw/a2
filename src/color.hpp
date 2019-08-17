#pragma once
#include "top.h"

class Color
{
public:
	float r, g, b, a;
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
	{
	}

	Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0)
	{
	}

	SDL_Color as_sdl_color() const
	{
		SDL_Color c;
		c.r = r * 255;
		c.g = g * 255;
		c.b = b * 255;
		c.a = a * 255;
		return c;
	}

	static mruby::NativeObject<Color> Red()
	{
		return mruby::NativeObject<Color>("Color", std::make_shared<Color>(1, 0, 0));
	}
	static mruby::NativeObject<Color> Green()
	{
		return mruby::NativeObject<Color>("Color", std::make_shared<Color>(0, 1, 0));
	}
	static mruby::NativeObject<Color> Blue()
	{
		return mruby::NativeObject<Color>("Color", std::make_shared<Color>(0, 0, 1));
	}
	static mruby::NativeObject<Color> Black()
	{
		return mruby::NativeObject<Color>("Color", std::make_shared<Color>(0, 0, 0));
	}
	static mruby::NativeObject<Color> White()
	{
		return mruby::NativeObject<Color>("Color", std::make_shared<Color>(1, 1, 1));
	}
};
