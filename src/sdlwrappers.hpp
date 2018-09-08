#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

#include "initexception.hpp"

class Image
{
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
	Image(std::shared_ptr<SDL_Renderer> renderer, std::shared_ptr<SDL_Surface> surface) : 
		texture(load_texture(renderer, surface))
	{
	}

	Image(std::shared_ptr<SDL_Renderer> renderer, const std::string& filename) : 
		texture(load_texture(renderer, load_image(renderer, filename)))
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

class Color
{
public:
	float r, g, b, a;
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
	{
	}

	Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0)
	{
	}

	SDL_Color as_sdl_color() const
	{
		SDL_Color c;
		c.r = r * 255;
		c.g = g * 255;
		c.b = b * 255;
		c.a = a * 255;
		return c;
	}

	static Color Red()
	{
		return Color(1, 0, 0);
	}
	static Color Green()
	{
		return Color(0, 1, 0);
	}
	static Color Blue()
	{
		return Color(0, 0, 1);
	}
	static Color Black()
	{
		return Color(0, 0, 0);
	}
	static Color White()
	{
		return Color(1, 1, 1);
	}
};

class Font
{
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
	Font(std::shared_ptr<SDL_Renderer> renderer, const std::string& filename) :
		renderer(renderer),
		filename(filename)
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

		auto ptr = std::make_shared<Image>(renderer, surface);
		return mruby::NativeObject<Image>("Image", ptr);
	}
};

class Music
{
	std::shared_ptr<Mix_Music> music;
public:
	Music(const std::string& filename)
	{
		music = std::shared_ptr<Mix_Music>(Mix_LoadMUS(filename.c_str()), Mix_FreeMusic);
		if (!music)
		{
			throw new InitException("Mix_LoadMUS", Mix_GetError());
		}
	}

	~Music()
	{

	}

	void play()
	{
		Mix_PlayMusic(music.get(), 0);
		Mix_VolumeMusic(MIX_MAX_VOLUME / 6);
	}
};

class PositionedImage
{
public:
	std::shared_ptr<Image> image;
	int x, y;

	PositionedImage(std::shared_ptr<Image> image, int x, int y) : 
		image(image), x(x), y(y)
	{}
};

class Event
{
public:
	Event(SDL_Event event)
	{

	}
};

class System
{
	int width;
	int height;
	std::shared_ptr<SDL_Window> window;
	std::shared_ptr<SDL_Renderer> renderer;

	Color background_color;
	bool running = false;

public:
	std::map< int, std::shared_ptr<PositionedImage> > Images;

	System(int width, int height) : background_color(Color::Black()), width(width), height(height)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			throw new InitException("Unable to initialize " "SDL", SDL_GetError());
		}

		SDL_Window* win;
		SDL_Renderer* rend;

		if (SDL_CreateWindowAndRenderer(width, height, 0, &win, &rend) < 0)
		{
			throw new InitException("Unable to initialize window and renderer", SDL_GetError());
		}

		window = std::shared_ptr<SDL_Window>(win, SDL_DestroyWindow);
		renderer = std::shared_ptr<SDL_Renderer>(rend, SDL_DestroyRenderer);

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

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
		{
			throw new InitException("Unable to Mix_OpenAudio " "SDLMixer", Mix_GetError());
		}
		
	}

	~System()
	{
		TTF_Quit();
		Mix_CloseAudio();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	mruby::NativeObject<Font> load_font(const std::string& filename)
	{
		auto ptr = std::make_shared<Font>(renderer, filename);
		return mruby::NativeObject<Font>("Font", ptr);
	}

	mruby::NativeObject<Image> load_image(const std::string& filename)
	{
		auto ptr = std::make_shared<Image>(renderer, filename);
		return mruby::NativeObject<Image>("Image", ptr);
	}

	mruby::NativeObject<Music> load_music(const std::string& filename)
	{
		auto ptr = std::make_shared<Music>(filename);
		return mruby::NativeObject<Music>("Music", ptr);
	}

	mruby::NativeObject<PositionedImage> centered_image(mruby::NativeObject<Image> image)
	{
		auto ptr = std::make_shared<PositionedImage>(image.get_shared_instance(), (width - image->get_width()) / 2, (height - image->get_height()) / 2);
		return mruby::NativeObject<PositionedImage>("Font", ptr);
	}

	void start_event_loop(mruby::Function<bool(mruby::NativeObject<Event>)> onEvent)
	{
		running = true;
		SDL_Event event;
		while (running)
		{
			SDL_WaitEvent(&event);
			do
			{
				running = onEvent.invoke( mruby::NativeObject<Event>("Event", std::make_shared<Event>(event)) );
			} while (SDL_PollEvent(&event));

			auto bgcolor = background_color.as_sdl_color();
			SDL_SetRenderDrawColor(renderer.get(), bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
			SDL_RenderClear(renderer.get());

			for (auto k : Images)
			{
				SDL_Rect dest_rect = { k.second->x, k.second->y, k.second->image->get_width(), k.second->image->get_height() };
				SDL_RenderCopy(renderer.get(), k.second->image->Texture().get(), NULL, &dest_rect);
			}
			
			SDL_RenderPresent(renderer.get());
		}
	}
};
