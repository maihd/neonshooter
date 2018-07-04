// NeonShooter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define TIMER_CPP_IMPL
#include <timer.hpp>
#include <vmath.h>

//#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#undef main
int main()
{
	timer timer;
	SDL_Event event;
	SDL_Window* window;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "[Error] SDL_Init: %s\n", SDL_GetError());
		goto error;
	}

	window = SDL_CreateWindow("NeonShooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window)
	{
		fprintf(stderr, "[Error] SDL_CreateWindow: %s\n", SDL_GetError());
		goto error;
	}

	while (true)
	{
		timer::start(timer);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			}
		}
		if (event.type == SDL_QUIT)
		{
			break;
		}

		timer::stop(timer);
		timer::sleep(timer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
    return 0;

error:
	getchar();
	return 1;
}

