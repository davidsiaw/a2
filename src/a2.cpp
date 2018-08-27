#ifdef _WIN32
#include <stdafx.h>
#endif

#include <SDL.h>

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_CreateWindowAndRenderer(1024, 768, 0, &window, &renderer) < 0)
		exit(2);
	
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


		/* Update the screen! */
		SDL_RenderPresent(renderer);
	}

	return EXIT_SUCCESS;
}
