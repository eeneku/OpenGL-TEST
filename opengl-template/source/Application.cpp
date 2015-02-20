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
#define glAssert() assert(glGetError() == GL_NO_ERROR)
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

	mesh.loadFromFile("assets/meshes/cube.mesh");

	rotation = 0.0f;
	moveForward = false;
	moveBackward = false;
	turnLeft = false;
	turnRight = false;
	moveSpeed = 0.1f;
	turnSpeed = 0.05f;

	projection = glm::perspective(glm::radians(60.0f),
		static_cast<float>(Config::WINDOW_WIDTH) / Config::WINDOW_HEIGHT,
		0.1f, 100.0f);

	view = glm::lookAt(
		glm::vec3(0, 0, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
		);

	model = glm::mat4(1.0f);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(glm::vec3), mesh.getVertices().data(), GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.getUvs().size() * sizeof(glm::vec2), mesh.getUvs().data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.getNormals().size() * sizeof(glm::vec3), mesh.getNormals().data(), GL_STATIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices().size() * sizeof(GLushort), mesh.getIndices().data(), GL_STATIC_DRAW);

	program = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

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

	loadTexture("assets/textures/cube.png");

	MVPIndex = glGetUniformLocation(program, "MVP");
	textureIndex = glGetUniformLocation(program, "texture");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

Application::~Application()
{
	// Deinitialisation
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(program);
	glDeleteTextures(1, &textureID);
}

void Application::update()

{
	// Updating and drawing
	if (turnLeft)
		model = glm::rotate(model, turnSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
	if (turnRight)
		model = glm::rotate(model, turnSpeed, glm::vec3(0.0f, 0.0f, -1.0f));
	if (moveForward)
		model = glm::translate(model, glm::vec3(0.0f, moveSpeed, 0.0f));
	if (moveBackward)
		model = glm::translate(model, glm::vec3(0.0f, -moveSpeed, 0.0f));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Ololoo translate ja rotatee ja scalee
	//rotation += 0.0001f;
	

	//model = glm::scale(model, glm::vec3(1.0f + rotation, 1.0f + rotation, 1.0f + rotation));

	//model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 2.0f));
	glUniform1i(textureIndex, 0);
	glUniformMatrix4fv(MVPIndex, 1, GL_FALSE, glm::value_ptr(projection*view*model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_SHORT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Application::loadTexture(const std::string& path)
{
	// TÄÄ TOIMI
	std::vector<unsigned char> png;
	std::vector<unsigned char> pixels;
	GLuint width;
	GLuint height;

	lodepng::load_file(png, path);
	lodepng::decode(pixels, width, height, png.data(), png.size());

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

}