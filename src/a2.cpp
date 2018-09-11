#ifdef _WIN32
#include <stdafx.h>
#endif

#include "mruby.hpp"
#include "sdlwrappers.hpp"


int main(int argc, char *argv[])
{
	mruby::VM mrvm;

	auto system_class = mrvm.create_class<System, int, int>("System");
	system_class->bind_instance_method("load_font", &System::load_font);
	system_class->bind_instance_method("load_image", &System::load_image);
	system_class->bind_instance_method("load_music", &System::load_music);
	system_class->bind_instance_method("centered_image", &System::centered_image);
	system_class->bind_instance_method("start_event_loop", &System::start_event_loop);
	system_class->bind_instance_method("set_image", &System::set_image);

	auto image_class = mrvm.create_closed_class<Image>("Image");

	auto color_class = mrvm.create_class<Color, float, float, float, float>("Color");
	color_class->bind_instance_variable("r", &Color::r);
	color_class->bind_instance_variable("g", &Color::g);
	color_class->bind_instance_variable("b", &Color::b);
	color_class->bind_instance_variable("a", &Color::a);
	color_class->bind_method("Black", &Color::Black);
	color_class->bind_method("Red", &Color::Red);

	auto font_class = mrvm.create_closed_class<Font>("Font");
	font_class->bind_instance_method("draw_string", &Font::draw_string);

	auto music_class = mrvm.create_closed_class<Music>("Music");
	music_class->bind_instance_method("play", &Music::play);

	auto positioned_image_class = mrvm.create_closed_class<PositionedImage>("PositionedImage");
	positioned_image_class->bind_instance_variable("x", &PositionedImage::x);
	positioned_image_class->bind_instance_variable("y", &PositionedImage::y);

	auto event_class = mrvm.create_closed_class<Event>("Event");
	event_class->bind_instance_method("gettype", &Event
		::gettype);

	mrvm.run_file("main.rb");

	return EXIT_SUCCESS;
}
