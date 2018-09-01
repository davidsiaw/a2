#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

#include "initexception.hpp"

class SDL
{
public:
	SDL()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			throw new InitException("Unable to initialize " "SDL", SDL_GetError());
		}
	}
	~SDL()
	{
		SDL_Quit();
	}
};


class SDLImage
{
public:
	SDLImage()
	{
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP))
		{
			throw new InitException("Unable to initialize " "SDLImage", IMG_GetError());
		}
	}
	~SDLImage()
	{
		IMG_Quit();
	}
};

class SDLMixer
{
public:
	SDLMixer()
	{
		if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID) != (MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID))
		{
			throw new InitException("Unable to initialize " "SDLMixer", Mix_GetError());
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
		{
			throw new InitException("Unable to Mix_OpenAudio " "SDLMixer", Mix_GetError());
		}
	}
	~SDLMixer()
	{
		Mix_CloseAudio();
		Mix_Quit();
	}
};

class SDLTTF
{
public:
	SDLTTF()
	{
		if (TTF_Init() != 0)
		{
			throw new InitException("Unable to initialize " "SDLTTF", TTF_GetError());
		}
	}
	~SDLTTF()
	{
		TTF_Quit();
	}
};
