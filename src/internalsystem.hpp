#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

class InternalSystem
{
	std::shared_ptr<SDL_Window> window;
	std::shared_ptr<SDL_Renderer> renderer;
public:
	InternalSystem(int width, int height)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			throw new InitException("Unable to initialize " "SDL", SDL_GetError());
		}

		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP))
		{
			throw new InitException("Unable to initialize " "SDLImage", IMG_GetError());
		}

		if (TTF_Init() != 0)
		{
			throw new InitException("Unable to initialize " "SDLTTF", TTF_GetError());
		}

		if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID) != (MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID))
		{
			throw new InitException("Unable to initialize " "SDLMixer", Mix_GetError());
		}

		SDL_Window* win;
		SDL_Renderer* rend;

		if (SDL_CreateWindowAndRenderer(width, height, 0, &win, &rend) < 0)
		{
			throw new InitException("Unable to initialize window and renderer", SDL_GetError());
		}

		window = std::shared_ptr<SDL_Window>(win, SDL_DestroyWindow);
		renderer = std::shared_ptr<SDL_Renderer>(rend, SDL_DestroyRenderer);

	}

	~InternalSystem()
	{
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	std::shared_ptr<SDL_Renderer> get_renderer() const
	{
		return renderer;
	}

	std::shared_ptr<SDL_Window> get_window() const
	{
		return window;
	}
};
