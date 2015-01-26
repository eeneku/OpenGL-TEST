/**
 * @file Main.cpp
 *
 * OpenGL Template
 */

#include <cassert>
#include <iostream>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <Application.h>

using namespace std;

static void loop(SDL_Window* window);

int main(int argc, char** argv)
{
	argc;
	argv;

	int result = SDL_Init(SDL_INIT_VIDEO);
	assert(result == 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Application::WINDOW_WIDTH, Application::WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

	assert(window != nullptr);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	assert(context != nullptr);
	glewExperimental = GL_TRUE;
	const GLenum glewResult = glewInit();
	assert(glewResult == GLEW_OK);

	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MAJOR_VERSION, &versionMinor);
	cout << "OpenGL context version: " << versionMajor << '.' << versionMinor << '\n';

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
			if (event.type == SDL_QUIT)
				exit = true;
		}

		application.update();
		SDL_GL_SwapWindow(window);
	}
}
