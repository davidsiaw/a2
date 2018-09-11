#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

class Image
{
	std::shared_ptr<InternalSystem> intsys;
	std::shared_ptr<SDL_Texture> texture;
	int w, h;

	std::shared_ptr<SDL_Texture> load_texture(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<SDL_Surface> surface)
	{
		this->w = surface->w;
		this->h = surface->h;

		auto tex = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer.get(), surface.get()), SDL_DestroyTexture);
		if (!tex)
		{
			throw InitException("Could not make texture ", SDL_GetError());
		}
		return tex;
	}

	std::shared_ptr<SDL_Surface> load_image(std::shared_ptr<SDL_Renderer> renderer, const std::string& filename)
	{
		auto surf = std::shared_ptr<SDL_Surface>(IMG_Load(filename.c_str()), SDL_FreeSurface);
		if (!surf)
		{
			throw InitException("Could not load image " + filename, SDL_GetError());
		}

		return surf;
	}

public:
	Image(std::shared_ptr<InternalSystem> intsys, std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<SDL_Surface> surface) :
		texture(load_texture(renderer, surface)), intsys(intsys)
	{
	}

	Image(std::shared_ptr<InternalSystem> intsys, std::shared_ptr<SDL_Renderer> renderer, const std::string& filename) :
		texture(load_texture(renderer, load_image(renderer, filename))), intsys(intsys)
	{
	}

	~Image()
	{
	}

	int get_width() const
	{
		return w;
	}

	int get_height() const
	{
		return h;
	}

	std::shared_ptr<SDL_Texture> Texture() const
	{
		return texture;
	}
};
