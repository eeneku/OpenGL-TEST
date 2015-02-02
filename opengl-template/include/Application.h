/**
 * @file Application.h
 *
 * OpenGL Template
 */

#pragma once
#include <vector>
#include "GL/glew.h"

class Application
{
public:

	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

	Application();

	~Application();

	void update();

private:

	std::vector<unsigned short> indices;

	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint vertexArrayID;
	GLuint vertexBuffer;
};
