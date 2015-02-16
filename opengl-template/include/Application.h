/**
 * @file Application.h
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

#pragma once

#include <GLM.h>
#include <GL/glew.h>
#include <vector>

namespace Config
{
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const int MULTISAMPLING_SAMPLES = 16; // 0 = disabled
}

class Application
{
public:
	Application();
	~Application();

	void update();
	void loadTexture(const std::string& path);

	bool moveForward;
	bool moveBackward;
	bool turnLeft;
	bool turnRight;
private:

	GLuint program;
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint elementBuffer;
	
	float rotation;
	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 view;

	GLuint textureID;

	GLint textureIndex;
	GLint MVPIndex;

	GLint vertexSize;

	float moveSpeed;
	float turnSpeed;
};
