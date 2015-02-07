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
	GLuint make_texture(const char *filename);
	short le_short(unsigned char *bytes);
	void* read_tga(const char *filename, int *width, int *height);
	void show_info_log(
		GLuint object,
		PFNGLGETSHADERIVPROC glGet__iv,
		PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
		);

private:

	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint elementBuffer;
	GLuint textures[2];
	
	struct {
		GLint fade_factor;
		GLint textures[2];
	} uniforms;

	struct {
		GLint position;
	} attributes;

	GLfloat fade_factor;
};
