/**
 * @file Application.cpp
 *
 * OpenGL Template
 */

#include <Application.h>
#include <iostream>
#include <ShaderSource.h>

Application::Application()
{
	// Initialisation

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	GLfloat vertexData[] = { 
		0.0f, 0.0f, 0.0f,
		0.50f, 0.0f, 0.0f,
		0.25f, 0.50f, 0.0f,
		0.75f, 0.50f, 0.0f, };

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	program = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

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
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(program);
}

void Application::update()
{
	// Updating and drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}
