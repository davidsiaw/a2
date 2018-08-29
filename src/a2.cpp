#ifdef _WIN32
#include <stdafx.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/variable.h>

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


int main(int argc, char *argv[])
{
	mrb_state *mrb = mrb_open();
	mrb_load_string(mrb, "p 'hello world!'; $s = self; width = 1024;");
	mrb_sym selfsym = mrb_intern_lit(mrb, "$s");
	mrb_sym sym = mrb_intern_lit(mrb, "width");
	mrb_value self = mrb_gv_get(mrb, selfsym);
	mrb_value v = mrb_f_global_variables(mrb, self);

	auto a = mrb_funcall(mrb, self, "width", 0);


	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD  | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_OPUS | MIX_INIT_MID);

	if (SDL_CreateWindowAndRenderer(1024, 768, 0, &window, &renderer) < 0)
	{
		exit(2);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}

	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
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
