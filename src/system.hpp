#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

class System
{
	std::shared_ptr<InternalSystem> intsys;
	int width;
	int height;

	Color background_color;
	bool running = false;

	std::map< int, std::shared_ptr<PositionedImage> > images;

	std::shared_ptr<SDL_Renderer> get_renderer() const
	{
		return intsys->get_renderer();
	}

public:

	System(int width, int height) :
		background_color(*Color::Black()),
		width(width),
		height(height),
		intsys(std::make_shared<InternalSystem>(width, height))
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
		{
			throw new InitException("Unable to Mix_OpenAudio " "SDLMixer", Mix_GetError());
		}
	}

	~System()
	{
		Mix_CloseAudio();
	}

	mruby::NativeObject<Font> load_font(const std::string& filename)
	{
		auto ptr = std::make_shared<Font>(intsys, get_renderer(), filename);
		return mruby::NativeObject<Font>("Font", ptr);
	}

	mruby::NativeObject<Image> load_image(const std::string& filename)
	{
		auto ptr = std::make_shared<Image>(intsys, get_renderer(), filename);
		return mruby::NativeObject<Image>("Image", ptr);
	}

	mruby::NativeObject<Music> load_music(const std::string& filename)
	{
		auto ptr = std::make_shared<Music>(filename);
		return mruby::NativeObject<Music>("Music", ptr);
	}

	void set_image(int idx, mruby::NativeObject<PositionedImage> image)
	{
		images[idx] = image.get_shared_instance();
	}

	mruby::NativeObject<PositionedImage> centered_image(mruby::NativeObject<IImage> image)
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
				running = onEvent.invoke(mruby::NativeObject<Event>("Event", std::make_shared<Event>(event)));
			} while (SDL_PollEvent(&event));

			auto bgcolor = background_color.as_sdl_color();
			SDL_SetRenderDrawColor(get_renderer().get(), bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
			SDL_RenderClear(get_renderer().get());

			Uint32 current_time_ms = SDL_GetTicks();

			for (auto k : images)
			{
				SDL_Rect dest_rect = { k.second->x, k.second->y, k.second->image->get_width(), k.second->image->get_height() };
				SDL_Rect src_rect = k.second->image->get_rect(current_time_ms);
				SDL_RenderCopy(get_renderer().get(), k.second->image->get_texture().get(), &src_rect, &dest_rect);
			}

			SDL_RenderPresent(get_renderer().get());
		}
	}
};
