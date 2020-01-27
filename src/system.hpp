#pragma once
#include "top.h"
#include "video_subsystem.hpp"

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

	System(const System& other) :
		intsys(other.intsys),
		width(other.width),
		height(other.height),
		background_color(other.background_color),
		running(other.running),
		images(other.images)
	{ }

	~System()
	{
		#ifdef __linux__
		system("pulseaudio -k");	// hack to stop pulseaudio from hanging Mix_CloseAudio
		#endif
		Mix_CloseAudio();
	}

	void set_window_title(std::string str)
	{
		SDL_SetWindowTitle(intsys->get_window().get(), str.c_str());
	}

	std::string get_window_title()
	{
		return SDL_GetWindowTitle(intsys->get_window().get());
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
		return mruby::NativeObject<PositionedImage>("PositionedImage", ptr);
	}

	static Uint32 send_render(Uint32 interval, void *param)
	{
		SDL_Event event;
		SDL_UserEvent userevent;

		/* In this example, our callback pushes a function
		into the queue, and causes our callback to be called again at the
		same interval: */

		userevent.type = SDL_USEREVENT;
		userevent.code = 0;
		userevent.data1 = NULL;
		userevent.data2 = NULL;

		event.type = SDL_USEREVENT;
		event.user = userevent;

		SDL_PushEvent(&event);
		return interval;
	}
    
    static int get_display_count()
    {
        VideoSubsystem v;
        return v.get_display_count();
    }
    
    static int get_video_width(int display)
    {
        VideoSubsystem v;
        return v.get_video_width(display);
    }
    
    static int get_video_height(int display)
    {
        VideoSubsystem v;
        return v.get_video_height(display);
    }
    
    static int get_video_refresh(int display)
    {
        VideoSubsystem v;
        return v.get_video_refresh(display);
    }
    
	void start_event_loop(mruby::Function<bool(mruby::NativeObject<Event>)> onEvent)
	{
        SDL_TimerID render_trigger = SDL_AddTimer(50, send_render, NULL);

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

			for (auto k : images)
			{
				SDL_Rect dest_rect = { k.second->x, k.second->y, k.second->image->get_width(), k.second->image->get_height() };
				SDL_Rect src_rect = k.second->image->get_rect();
				SDL_RenderCopy(get_renderer().get(), k.second->image->get_texture().get(), &src_rect, &dest_rect);
			}

			SDL_RenderPresent(get_renderer().get());
		}
	}
};
