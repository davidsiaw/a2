
#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

class Font
{
	std::shared_ptr<InternalSystem> intsys;
	std::shared_ptr<SDL_Renderer> renderer;
	std::string filename;
	std::map< int, std::shared_ptr<TTF_Font> > size_map;

	std::shared_ptr<TTF_Font> get_font(int size)
	{
		if (size_map.find(size) == size_map.end())
		{
			auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(filename.c_str(), size), TTF_CloseFont);
			if (!font)
			{
				std::stringstream ss;
				ss << "Unable to create font " << filename << " with size " << size;
				throw new InitException(ss.str().c_str(), TTF_GetError());
			}
			size_map[size] = font;
		}
		return size_map[size];
	}

public:
	Font(std::shared_ptr<InternalSystem> intsys, std::shared_ptr<SDL_Renderer> renderer, const std::string& filename) :
		renderer(renderer),
		filename(filename),
		intsys(intsys)
	{
	}

	~Font()
	{
	}

	mruby::NativeObject<Image> draw_string(const std::string& text, int size, mruby::NativeObject<Color> color)
	{
		auto surface = std::shared_ptr<SDL_Surface>(TTF_RenderUTF8_Blended(get_font(size).get(), text.c_str(), color->as_sdl_color()), SDL_FreeSurface);

		if (!surface)
		{
			throw new InitException("TTF_RenderUNICODE_Solid", TTF_GetError());
		}

		auto ptr = std::make_shared<Image>(intsys, renderer, surface);
		return mruby::NativeObject<Image>("Image", ptr);
	}
};

