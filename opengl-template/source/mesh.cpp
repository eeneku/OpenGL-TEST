#include <fstream>
#include "mesh.h"
#include "GLM.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::getSimilarVertexIndex(Vertex& vertex, std::map<Vertex, GLushort>& vertexToOutIndex, GLushort& result)
{
	std::map<Vertex, GLushort>::iterator it = vertexToOutIndex.find(vertex);

	if (it == vertexToOutIndex.end())
		return false;
	else
	{
		result = it->second;
		return true;
	}
}

void Mesh::indexVBO(std::vector<glm::vec3>& inVertices, std::vector<glm::vec2>& inUvs, std::vector<glm::vec3>& inNormals)
{
	std::map<Vertex, GLushort> vertexToOutIndex;

	for (unsigned int i = 0; i < inVertices.size(); i++)
	{
		Vertex vertex = { inVertices[i], inUvs[i], inNormals[i] };

		GLushort index;
		bool found = getSimilarVertexIndex(vertex, vertexToOutIndex, index);

		if (found)
			indices.push_back(index);
		else
		{
			vertices.push_back(inVertices[i]);
			uvs.push_back(inUvs[i]);
			normals.push_back(inNormals[i]);

			GLushort newIndex = (GLushort)vertices.size() - 1;
			indices.push_back(newIndex);
			vertexToOutIndex[vertex] = newIndex;
		}
	}
}

bool Mesh::loadFromFile(const std::string& path)
{
	std::ifstream file(path);

	if (file.is_open())
	{
		std::vector<GLushort> vIndices, tIndices, nIndices;
		std::vector<glm::vec3> inVertices, inNormals;
		std::vector<glm::vec2> inUvs;

		while (!file.eof()) 
		{
			std::string line;
			std::string prefix;
			std::stringstream ss;

			std::getline(file, line);
			ss << line;
			ss >> prefix >> std::ws;

			if (prefix == "v") 
			{
				glm::vec3 v;
				ss >> v.x >> v.y >> v.z >> std::ws;
				inVertices.push_back(v);
			}
			else if (prefix == "vt") 
			{
				glm::vec2 uv;
				ss >> uv.x >> uv.y >> std::ws;
				inUvs.push_back(uv);
			}
			else if (prefix == "vn") 
			{
				glm::vec3 v;
				ss >> v.x >> v.y >> v.z >> std::ws;
				inNormals.push_back(v);
			}
			else if (prefix == "f") 
			{
				for (size_t i = 0; i < 3; i++) 
				{
					GLuint v, t, n;
					char c;

					ss >> v >> c >> t >> c >> n >> std::ws;
					vIndices.push_back(v);
					tIndices.push_back(t);
					nIndices.push_back(n);
				}
			}
		}

		std::vector <glm::vec3> tVertices, tNormals;
		std::vector <glm::vec2> tUvs;

		for (size_t i = 0; i < vIndices.size(); i++) 
		{
			glm::vec3& vertex = inVertices[vIndices[i] - 1];
			tVertices.push_back(vertex);

			if (inUvs.size() > 0)
			{
				glm::vec2& uv = inUvs[tIndices[i] - 1];
				tUvs.push_back(uv);
			}

			if (inNormals.size() > 0)
			{ 
				glm::vec3& normal = inNormals[nIndices[i] - 1];
				tNormals.push_back(normal);
			}
		}

		indexVBO(tVertices, tUvs, tNormals);
	}
	else return false;

	return true;
}
