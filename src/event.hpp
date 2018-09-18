#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

class Event
{
	SDL_Event event;

public:
	Event(SDL_Event event) : event(event)
	{
	}

	int gettype()
	{
		return event.type;
	}
};
 