/**
 * @file Application.h
 *
 * OpenGL Template
 */

#pragma once
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

	GLuint program;
	GLuint program2;
	GLuint fragmentShader2;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint vertexArrayID;
	GLuint vertexBuffer;
};
