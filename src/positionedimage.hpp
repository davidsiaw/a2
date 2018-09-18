#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

class PositionedImage
{
public:
	std::shared_ptr<IImage> image;
	int x, y;

	PositionedImage(std::shared_ptr<IImage> image, int x, int y) :
		image(image), x(x), y(y)
	{}
};
