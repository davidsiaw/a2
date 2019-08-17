#pragma once
#include "top.h"

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

	int getusercode()
	{
		return event.user.code;
	}
};
 