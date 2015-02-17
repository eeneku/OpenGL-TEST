#include <fstream>
#include "mesh.h"
#include "GLM.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::loadFromFile(const std::string& path)
{
	std::ifstream in(path);

	if (in.is_open())
	{
		std::vector<GLushort> vIndices, tIndices, nIndices;
		std::vector<glm::vec3> tmpVertices, tmpNormals;
		std::vector<glm::vec2> tmpUvs;

		while (!in.eof()) {
			std::string line;
			std::string prefix;
			std::stringstream ss;

			std::getline(in, line);
			ss << line;
			ss >> prefix >> std::ws;

			if (prefix == "v") {
				//vertex
				glm::vec3 v;
				ss >> v.x >> v.y >> v.z >> std::ws;
				tmpVertices.push_back(v);
			}
			else if (prefix == "vt") {
				// texture coord
				glm::vec2 uv;
				ss >> uv.x >> uv.y >> std::ws;
				tmpUvs.push_back(uv);
			}
			else if (prefix == "vn") {
				// normal
				glm::vec3 v;
				ss >> v.x >> v.y >> v.z >> std::ws;
				tmpNormals.push_back(v);
			}
			else if (prefix == "f") {
				// index
				for (size_t i = 0; i < 3; i++) {
					GLuint v, t, n;
					char c;

					ss >> v >> c >> t >> c >> n >> std::ws;
					vIndices.push_back(v);
					tIndices.push_back(t);
					nIndices.push_back(n);
				}
			}
		}

		// f = v/vt/vn
		for (size_t i = 0; i < vIndices.size(); i++) {
			glm::vec3& vertex = tmpVertices[vIndices[i] - 1];
			vertices.push_back(vertex.x);
			vertices.push_back(vertex.y);
			vertices.push_back(vertex.z);

			if (tmpUvs.size() > 0)
			{
				glm::vec2& uv = tmpUvs[tIndices[i] - 1];
				vertices.push_back(uv.x);
				vertices.push_back(uv.y);
			}

			if (tmpNormals.size() > 0)
			{ 
				glm::vec3& normal = tmpNormals[nIndices[i] - 1];
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			}
		}
	}
	else return false;

	return true;
}