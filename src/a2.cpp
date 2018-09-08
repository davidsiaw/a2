#ifdef _WIN32
#include <stdafx.h>
#endif

#include "mruby.hpp"
#include "sdlwrappers.hpp"


int main(int argc, char *argv[])
{
	//auto sdl = std::make_shared<SDL>(1024, 768);

	mruby::VM mrvm;

	auto system_class = mrvm.create_class<System, int, int>("System");
	system_class->bind_instance_method("load_font", &System::load_font);
	system_class->bind_instance_method("load_image", &System::load_image);
	system_class->bind_instance_method("load_music", &System::load_music);
	system_class->bind_instance_method("centered_image", &System::centered_image);

	auto image_class = mrvm.create_closed_class<Image>("Image");

	auto color_class = mrvm.create_class<Color, float, float, float, float>("Color");
	color_class->bind_instance_variable("r", &Color::r);
	color_class->bind_instance_variable("g", &Color::g);
	color_class->bind_instance_variable("b", &Color::b);
	color_class->bind_instance_variable("a", &Color::a);

	auto font_class = mrvm.create_closed_class<Font>("Font");
	font_class->bind_instance_method("draw_string", &Font::draw_string);

	auto music_class = mrvm.create_closed_class<Music>("Music");
	music_class->bind_instance_method("play", &Music::play);

	auto positioned_image_class = mrvm.create_closed_class<PositionedImage>("PositionedImage");
	positioned_image_class->bind_instance_variable("x", &PositionedImage::x);
	positioned_image_class->bind_instance_variable("y", &PositionedImage::y);

	auto event_class = mrvm.create_closed_class<Event>("Event");

	mrvm.run_file("main.rb");

	auto m = mrvm.get_global_variable< mruby::Function<int(int)> >("$m");
	auto n = mrvm.get_global_variable< mruby::Function<void(int)> >("$m");
	auto o = mrvm.get_global_variable< mruby::Function<int()> >("$m");
	auto p = mrvm.get_global_variable< mruby::Function<void()> >("$m");
	int a = m.invoke(5);
	m.invoke(5);
	int b = o.invoke();
	p.invoke();

	//auto font = sdl->load_font("sample.ttf");
	//auto text = font->draw_string("鼻血のハローdesu", 16, Color::Black());
	//auto image = sdl->load_image("sample.webp");
	//auto music = sdl->load_music("sample.mp3");

	//music->play();

	//sdl->Images[0] = sdl->centered_image(image);
	//sdl->Images[1] = sdl->centered_image(text);

	//sdl->start_event_loop([&](SDL_Event event)->bool 
	//{
	//	if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN
	//		|| event.type == SDL_FINGERDOWN)
	//	{
	//		return false;
	//	}
	//	return true;
	//});

	return EXIT_SUCCESS;
}
