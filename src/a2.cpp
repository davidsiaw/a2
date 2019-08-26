#include "top.h"
#include "mruby.hpp"
#include "sdlwrappers.hpp"

#include <iostream>

void run()
{
	mruby::VM mrvm;

	auto system_class = mrvm.create_class<System, int, int>("System");
	system_class->bind_instance_method("load_font", &System::load_font);
	system_class->bind_instance_method("load_image", &System::load_image);
	system_class->bind_instance_method("load_music", &System::load_music);
	system_class->bind_instance_method("centered_image", &System::centered_image);
	system_class->bind_instance_method("start_event_loop", &System::start_event_loop);
	system_class->bind_instance_method("set_image", &System::set_image);
	system_class->bind_instance_method("set_window_title", &System::set_window_title);
	system_class->bind_instance_method("get_window_title", &System::get_window_title);

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
	event_class->bind_instance_method("gettype", &Event::gettype);
	event_class->bind_instance_method("getusercode", &Event::getusercode);

	auto sprite_class = mrvm.create_class<Sprite, mruby::NativeObject<Image>, int, int, int, int>("Sprite");
	sprite_class->bind_instance_variable("xframe", &Sprite::xframe);
	sprite_class->bind_instance_variable("yframe", &Sprite::yframe);

	auto animated_sprite_class = mrvm.create_class<AnimatedSprite, mruby::NativeObject<Image>, int, int, int, int>("AnimatedSprite");
	animated_sprite_class->bind_instance_variable("xframe", &AnimatedSprite::xframe);
	animated_sprite_class->bind_instance_variable("yframe", &AnimatedSprite::yframe);
	animated_sprite_class->bind_instance_variable("animating", &AnimatedSprite::animating);

	mrvm.run_file("main.rb");
}

int main(int argc, char *argv[])
{
	run();
	exit(0);

	return EXIT_SUCCESS;
}

