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
	SDL sdl;
	SDLImage sdlImage;
	SDLMixer sdlMixer;
	SDLTTF sdlTTF;

	mrvm.run_file("main.rb");

	SDL_Window *window;
	SDL_Renderer *renderer;


	if (SDL_CreateWindowAndRenderer(1024, 768, 0, &window, &renderer) < 0)
	{
		exit(2);
	}

	// load font.ttf at size 16 into font
	TTF_Font *font;
	font = TTF_OpenFont("sample.ttf", 16);
	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}

	SDL_Color color = { 0,0,0 };
	SDL_Surface *text_surface;


	if (!(text_surface = TTF_RenderUTF8_Blended(font, "鼻血のハローdesu", color))) {
		printf("TTF_RenderUNICODE_Solid: %s\n", TTF_GetError());
	}
	printf("%x\n", L'は');

	Sprite t;
	t.texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	if (!t.texture) {
		fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(text_surface);
	}
	t.w = text_surface->w;
	t.h = text_surface->h;
	SDL_FreeSurface(text_surface);

	Sprite sprite = LoadSprite("sample.webp", renderer);
	if (sprite.texture == NULL)
	{
		exit(2);
	}

	Mix_Music *music;
	music = Mix_LoadMUS("sample.mp3");
	if (!music) {
		printf("Mix_LoadMUS: %s\n", Mix_GetError());
	}

	if (Mix_PlayMusic(music, -1) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
		// well, there's no music, but most games don't break without music...
	}

	/* Main render loop */
	Uint8 done = 0;
	SDL_Event event;
	while (!done)
	{
		/* Wait for events */
		SDL_WaitEvent(&event);
		do {
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN
				|| event.type == SDL_FINGERDOWN)
			{
				done = 1;
			}
		} while (SDL_PollEvent(&event));


		/* Draw a gray background */
		SDL_SetRenderDrawColor(renderer, 0x0F, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		draw(window, renderer, sprite);
		draw(window, renderer, t);

		/* Update the screen! */
		SDL_RenderPresent(renderer);
	}

	return EXIT_SUCCESS;
}
