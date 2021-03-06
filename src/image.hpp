#pragma once
#include "top.h"

#include "iimage.hpp"

// For unused expressions
#define UNUSED(expr) do { (void)(expr); } while (0)

class Image : public IImage
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
		return texture;
	}

	virtual SDL_Rect get_rect()
	{
		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		r.w = w;
		r.h = h;
		return r;
	}
};

