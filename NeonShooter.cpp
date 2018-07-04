// NeonShooter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define TIMER_CPP_IMPL
#include <timer.hpp>
#include <vmath.h>

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

// Request high performance GPU
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement                  = 0x1;
	__declspec(dllexport) int   AmdPowerXpressRequestHighPerformance = 0x1;
}

namespace game
{
	void update(float dt);
	void render(void);
}

#undef main
int main()
{
	timer timer;
	SDL_Event event;
	SDL_Window* window;
	SDL_GLContext glcontext;

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
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	glcontext = SDL_GL_CreateContext(window);
	if (!glcontext)
	{
		fprintf(stderr, "[Error] SDL_GL_CreateContext: %s\n", SDL_GetError());
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

		game::update((float)timer.seconds);

		glClear(GL_COLOR_BUFFER_BIT);

		game::render();

		SDL_GL_SwapWindow(window);

		timer::stop(timer);
		timer::sleep(timer);
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
    return 0;

error:
	getchar();
	return 1;
}
