#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <sstream>

#include "GL\glew.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool loadFromFile(const std::string& path);
	const std::vector<GLfloat>& getVertices() const { return vertices; }

private:
	std::vector<GLfloat> vertices;
};

#endif