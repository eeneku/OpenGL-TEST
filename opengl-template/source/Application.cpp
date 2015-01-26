/**
 * @file Application.cpp
 *
 * OpenGL Template
 */

#include <Application.h>
#include <vector>
#include <iostream>

static const char* VERTEX_SOURCE =
"#version 330\n"

"in vec3 inPosition;\n"

"void main()\n"
"{\n"
"	gl_Position = vec4(inPosition, 1.0);\n"
"}\n";

static const char* FRAGMENT_SOURCE =
"#version 330\n"

"out vec4 outColour;\n"

"void main()\n"
"{\n"
"	outColour = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";

Application::Application()
{
	// Initialisation

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	GLfloat vertexData[] = { -1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, };

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	program = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint result = GL_FALSE;
	int infoLogLength;

	glShaderSource(vertexShader, 1, &VERTEX_SOURCE, nullptr);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &FRAGMENT_SOURCE, nullptr);
	glCompileShader(fragmentShader);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
}

Application::~Application()
{
	// Deinitialisation
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(program);
}

void Application::update()
{
	// Updating and drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
}
