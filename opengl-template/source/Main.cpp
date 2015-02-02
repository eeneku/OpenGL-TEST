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

	glewExperimental = GL_TRUE; // Required for some OpenGL features to be available
	const GLenum glewResult = glewInit();
	assert(glewResult == GLEW_OK);
	glGetError(); // For some reason an OpenGL error is emitted by calling glewInit()

	int versionMajor = 0;
	int versionMinor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
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