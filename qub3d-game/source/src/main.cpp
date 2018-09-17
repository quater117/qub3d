#include <SDL.h>
#include <SDL_opengl.h>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Qub3d Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_ShowWindow(window);
	SDL_GL_CreateContext(window);

	SDL_Event e;
	bool running = true;
	while (running)
	{
		while (SDL_PollEvent(&e) > 0)
		{
			if (e.type == SDL_QUIT) running = false;
		}

		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
	}
	return 0;
}