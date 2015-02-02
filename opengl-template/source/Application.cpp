/**
 * @file Application.cpp
 *
 * OpenGL Template
 */

#include <Application.h>
#include <vector>
#include <iostream>
#include <ShaderSource.h>

Application::Application()
{
	// Initialisation

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	GLfloat vertexData[] = { 
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, 0.0f, };

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	program = glCreateProgram();
	program2 = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	GLint result = GL_FALSE;
	int infoLogLength;

	std::string shaderPath = ShaderSource::load("assets/shaders/trivial_vertex.glsl");
	const char* temp = shaderPath.c_str();

	glShaderSource(vertexShader, 1, &temp, nullptr);
	glCompileShader(vertexShader);

	shaderPath = ShaderSource::load("assets/shaders/trivial_fragment.glsl");
	temp = shaderPath.c_str();

	glShaderSource(fragmentShader, 1, &temp, nullptr);
	glCompileShader(fragmentShader);

	shaderPath = ShaderSource::load("assets/shaders/trivial_fragment2.glsl");
	temp = shaderPath.c_str();

	glShaderSource(fragmentShader2, 1, &temp, nullptr);
	glCompileShader(fragmentShader2);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glAttachShader(program2, vertexShader);
	glAttachShader(program2, fragmentShader2);
	glLinkProgram(program2);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

Application::~Application()
{
	// Deinitialisation
	glDisableVertexAttribArray(0);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	glDeleteProgram(program2);
	glDeleteProgram(program);
}

void Application::update()
{
	// Updating and drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(program2);

	glDrawArrays(GL_TRIANGLES, 3, 3);
}
