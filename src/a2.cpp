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

mrb_value hello(mrb_state* mrb, mrb_value self)
{
	mrb_value* args;
	int narg;
	mrb_get_args(mrb, "*", &args, &narg);
	return mrb_fixnum_value(5);
}

void hellow()
{
	SDL_Log("hello\n");
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
	return add(first, args...);
}

template<typename T>
T adder(T first) {
	return first;
}

template<typename Fn, Fn fn, typename... Args>
typename std::result_of<Fn(Args...)>::type MRubyFunction(Args&&... args) {
	return fn(std::forward<Args>(args)...);
}

std::function<void()> arg_adder(std::function<void()> fun)
{
	return fun;
}

template<typename TArgHead, typename... TArgTail>
typename std::function<void(TArgTail...)> arg_adder(std::function<void(TArgHead, TArgTail...)> fun, TArgHead h, TArgTail... args)
{
}

template<typename TRet, typename... TArgs>
class LeFun {
	std::function<TRet(TArgs...)> func;
public:
	LeFun(std::function<TRet(TArgs...)> func) : func(func)
	{
	}

	void testcall()
	{
	}

};

template<typename TRet, typename... TArgs>
static typename LeFun<typename TRet, typename TArgs...> make(std::function<TRet(TArgs...)> func)
{
	return typename LeFun<TRet, TArgs...>(func);
}

int aa(int a, int b, int c)
{
	printf("%d %d %d\n", a, b, c);
	return 100;
}



int main(int argc, char *argv[])
{
	SDL sdl;
	SDLImage sdlImage;
	SDLMixer sdlMixer;
	SDLTTF sdlTTF;

	{
		MRuby mruby;

		//mruby.set_function("hellow", hellow);

		int* x = new int();
		auto play = mruby.create_module("Play");
		play->create_module("Meow");
		auto play2 = mruby.get_module("Play");

		auto a = mruby.create_function(hello);

		auto cat = mruby.create_class("Cat");
		mruby.run("p Cat.new");

		mruby.set_class_variable("@width", "haha");

		mruby.run("p @width");

		mruby.run("p Play");
		mruby.run("p Play::Meow");
	}

	printf("s\n");
	
	

	//mrb_state *mrb = mrb_open();
	//mrb_sym hello_name_s = mrb_intern_cstr(mrb, "hello");
	//mrb_value env[] = {
	//	mrb_cptr_value(mrb, (void*)hello),  // 0: c function pointer
	//	mrb_symbol_value(hello_name_s),          // 1: function name
	//};
	//RProc* hello_proc = mrb_proc_new_cfunc_with_env(mrb, hello, 2, env);
	//auto kernelmod = mrb->kernel_module;
	//mrb_method_t p;
	//MRB_METHOD_FROM_PROC(p, hello_proc);
	//mrb_define_method_raw(mrb, kernelmod, hello_name_s, p);
	//
	//mrb_load_string(mrb, "p 'hello world!'; $s = hello(5,3); width = 1024;");
	//mrb_sym selfsym = mrb_intern_lit(mrb, "$s");
	//mrb_value self = mrb_gv_get(mrb, selfsym);



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
