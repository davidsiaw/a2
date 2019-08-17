#pragma once
#include "top.h"

class PositionedImage
{
public:
	std::shared_ptr<IImage> image;
	int x, y;

	PositionedImage(std::shared_ptr<IImage> image, int x, int y) :
		image(image), x(x), y(y)
	{}
};
