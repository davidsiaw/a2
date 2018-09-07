#ifdef _WIN32
#include <stdafx.h>
#endif

#include "mruby.hpp"
#include "sdlwrappers.hpp"

typedef struct Sprite
{
	SDL_Texture* texture;
	Uint16 w;
	Uint16 h;
} Sprite;


Sprite LoadSprite(const char* file, SDL_Renderer* renderer)
{
	Sprite result;
	result.texture = NULL;
	result.w = 0;
	result.h = 0;

	SDL_Surface* temp;

	/* Load the sprite image */
	temp = IMG_Load(file);
	if (temp == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return result;
	}
	result.w = temp->w;
	result.h = temp->h;

	/* Create texture from the image */
	result.texture = SDL_CreateTextureFromSurface(renderer, temp);
	if (!result.texture) {
		fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(temp);
		return result;
	}
	SDL_FreeSurface(temp);

	return result;
}

void draw(SDL_Window *window, SDL_Renderer* renderer, const Sprite sprite)
{
	int w, h;

	SDL_GetWindowSize(window, &w, &h);
	SDL_Rect destRect = { w / 2 - sprite.w / 2, h / 2 - sprite.h / 2, sprite.w, sprite.h };
	/* Blit the sprite onto the screen */
	SDL_RenderCopy(renderer, sprite.texture, NULL, &destRect);
}

void aa(int a, int b, int c)
{
	printf("%d %d %d\n", a, b, c);
}

int ab(int a, int b, int c)
{
	printf("%d %d %d\n", a, b, c);
	return a + b + c;
}

int abb(int a, int b, int c)
{
	printf("%d %d %d\n", a, b, c);
	return a - b + c;
}

int ac()
{
	printf("le empty\n");
	return 102;
}

void ad()
{
	printf("la empty\n");
}

class Cat
{
	int m;
public:
	Cat(int m) : m(m)
	{
		printf("cat live\n");
	}
	~Cat()
	{
		printf("cat die\n");
	}
	int meow()
	{
		return printf("cat meow %d\n", m);
	}
	static std::string sss()
	{
		printf("cat sss\n");
		return "LOLERINO";
	}
};

int a()
{
	return 5;
}

void testruby()
{
	mruby::VM mrvm;

	auto play = mrvm.create_module("Play");
	play->create_module("Meow");
	auto play2 = mrvm.get_module("Play");

	play->bind_method("a", a);

	auto cat = mrvm.create_class<Cat, int>("Cat");
	cat->bind_instance_method("meow", &Cat::meow);
	cat->bind_method("sss", &Cat::sss);

	mrvm.run("@cat = Cat.new(5)");
	mrvm.run("p @cat");
	mrvm.run("p Cat.sss");
	mrvm.run("p @cat.meow");
	mrvm.run("p Play::a 1");

	mrvm.bind_method("aa", aa);
	mrvm.bind_method("ab", ab);
	mrvm.bind_method("abb", abb);
	mrvm.bind_method("ac", ac);
	mrvm.bind_method("ad", ad);


	mrvm.run("p aa 5, 7, 8");
	mrvm.run("p ab 5, 7, 8");
	mrvm.run("p abb 5, 7, 8");
	mrvm.run("p ac");
	mrvm.run("p ad");

	mrvm.set_class_variable("@width", "haha");

	mrvm.run("p @width");

	mrvm.run("p Play");
	mrvm.run("p Play::Meow");
}

int main(int argc, char *argv[])
{
	mruby::VM mrvm;

	auto sdl = std::make_shared<SDL>(1024, 768);
	
	mrvm.run_file("main.rb");

	auto font = sdl->LoadFont("sample.ttf");
	auto text = font->DrawString("鼻血のハローdesu", 16, Color::Black());
	auto image = sdl->LoadImage("sample.webp");
	auto music = sdl->LoadMusic("sample.mp3");

	music->Play();

	sdl->Images[0] = sdl->CenteredImage(image);
	sdl->Images[1] = sdl->CenteredImage(text);

	sdl->StartEventLoop([&](SDL_Event event)->bool 
	{
		if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN
			|| event.type == SDL_FINGERDOWN)
		{
			return false;
		}
		return true;
	});

	return EXIT_SUCCESS;
}
