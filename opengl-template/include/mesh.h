#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "GL\glew.h"
#include "GLM.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;

	bool operator<(const Vertex& other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
	}
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool load(const std::string& path);
	bool getSimilarVertexIndex(Vertex& vertex, std::map<Vertex, GLushort>& vertexToOutIndex, GLushort& result);
	void indexVBO(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals);
	const std::vector<Vertex>& getVertices() const { return vertices; }
	const std::vector<GLushort>& getIndices() const { return indices; }

private:
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
};

#endif