#pragma once
#include "top.h"

class IImage
{
public:
	virtual int get_width() const = 0;
	virtual int get_height() const = 0;
	virtual std::shared_ptr<SDL_Texture> get_texture() const = 0;
	virtual SDL_Rect get_rect() = 0;
};
