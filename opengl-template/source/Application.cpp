/**
 * @file Application.cpp
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

#include <iostream>
#include <string>
#include <vector>
#include <Application.h>
#include <lodepng.h>
#include <ShaderSource.h>

// Public

Application::Application()
{
	// Initialisation

	// Shader sources can be loaded from text files with static ShaderSource class:
	// std::string source;
	// const bool sourceResult = ShaderSource::load("assets/shaders/FILENAME", source);
	// sourceResult == true when the source is loaded successfully

	// Textures can be loaded from PNG files with LodePNG:
	// unsigned int imageWidth;
	// unsigned int imageHeight;
	// std::vector<unsigned char> imageData;
	// const unsigned int imageResult = lodepng::decode(imageData, imageWidth, imageHeight, "assets/FILENAME.png");
	// imageResult == 0 when the image is loaded and decoded successfully

	rotation = 0.0f;

	projection = glm::perspective(glm::radians(60.0f),
		static_cast<float>(Config::WINDOW_WIDTH) / Config::WINDOW_HEIGHT,
		0.1f, 100.0f);

	view = glm::lookAt(
		glm::vec3(2, 2, 2),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
		);

	model = glm::mat4(1.0f);

	glm::mat4 MVP = projection * view * model;

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	GLfloat vertexData[] = {
		-0.5f, -0.5f, -0.5f,		// Taka yl� vasen	0
		-0.5f, 0.5f, -0.5f,		// Taka ala vasen	1
		0.5f, 0.5f, -0.5f,		// Taka ala oikea	2
		0.5f, -0.5f, -0.5f,		// Taka yl� oikea	3

		-0.5f, -0.5f, 0.50f,		// Etu yl� vasen	4
		-0.5f, 0.5f, 0.5f,		// Etu ala vasen	5
		0.5f, 0.5f, 0.5f,		// Etu ala oikea	6
		0.5f, -0.5f, 0.5f,		// Etu yl� oikea	7
	};

	GLfloat colorData[] = {
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	GLfloat texcoords[2*4*6] = {
		0.0f,	0.0f,
		1.0f,	0.0f,
		1.0f,	1.0f,
		0.0f,	1.0f,
	};

	for (int i = 1; i < 6; i++)
		memcpy(&texcoords[i * 4 * 2], &texcoords[0], 2 * 4 * sizeof(GLfloat));

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(3);

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(5);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(5);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(5);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(2);

	indices.push_back(7);
	indices.push_back(2);
	indices.push_back(3);

	glGenBuffers(1, &texcoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	program = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string shaderPath;
	ShaderSource::load("assets/shaders/vertex.glsl", shaderPath);
	const char* temp = shaderPath.c_str();

	glShaderSource(vertexShader, 1, &temp, nullptr);
	glCompileShader(vertexShader);

	ShaderSource::load("assets/shaders/fragment.glsl", shaderPath);
	temp = shaderPath.c_str();

	glShaderSource(fragmentShader, 1, &temp, nullptr);
	glCompileShader(fragmentShader);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glUseProgram(program);
	GLuint loc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));

	modelIndex = glGetUniformLocation(program, "model");

	loadTexture("assets/textures/cube.png");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

Application::~Application()
{
	// Deinitialisation
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(program);
	glDeleteTextures(1, &textureID);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Application::update()

{
	// Updating and drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLint uniformMyTexture = glGetUniformLocation(program, "texture");
	glUniform1i(uniformMyTexture, 0);

	rotation += 0.02f;
	model = glm::rotate(rotation, glm::vec3(2.0f, 1.0f, 0.5f));
	glUniformMatrix4fv(modelIndex, 1, GL_FALSE, value_ptr(model));


	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}

void Application::loadTexture(const std::string& path)
{
	// T�� TOIMI
	std::vector<unsigned char> png;
	std::vector<unsigned char> pixels;

	lodepng::load_file(png, path);
	lodepng::decode(pixels, textureWidth, textureHeight, png.data(), png.size());

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

}