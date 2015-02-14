/**
 * @file Main.cpp
 *
 * OpenGL Template
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 *
 * OpenGL Template is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <iostream>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <Application.h>

using namespace std;

static void loop(SDL_Window* window);

int main()
{
	int result = SDL_Init(SDL_INIT_VIDEO);
	assert(result == 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,		  SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,		  1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,			  8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,			  8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,			  8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,			  8);

	if(Config::MULTISAMPLING_SAMPLES > 0)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, Config::MULTISAMPLING_SAMPLES);
	}

	SDL_Window* window = SDL_CreateWindow("OpenGL Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

	assert(window != nullptr);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	assert(context != nullptr);

	glewExperimental = GL_TRUE; // Required for some OpenGL features to be available
	const GLenum glewResult = glewInit();
	assert(glewResult == GLEW_OK);
	glGetError(); // For some reason an OpenGL error is emitted by calling glewInit()

	int versionMajor = 0;
	int versionMinor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
	cout << "OpenGL context version: " << versionMajor << '.' << versionMinor << '\n';

	int parameter;
	glGetIntegerv(GL_SAMPLES, &parameter);
	cout << "Multisampling samples: " << parameter << '\n';

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	loop(window);
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

static void loop(SDL_Window* window)
{
	bool exit = false;
	SDL_Event event;
	Application application;

	while (!exit)
	{
		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				exit = true;
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_UP)
					application.moveUp = true;
				if (event.key.keysym.sym == SDLK_DOWN)
					application.moveDown = true;
				if (event.key.keysym.sym == SDLK_LEFT)
					application.moveLeft = true;
				if (event.key.keysym.sym == SDLK_RIGHT)
					application.moveRight = true;
			}
			if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_UP)
					application.moveUp = false;
				if (event.key.keysym.sym == SDLK_DOWN)
					application.moveDown = false;
				if (event.key.keysym.sym == SDLK_LEFT)
					application.moveLeft = false;
				if (event.key.keysym.sym == SDLK_RIGHT)
					application.moveRight = false;
			}
		}

		application.update();
		SDL_GL_SwapWindow(window);
	}
}
