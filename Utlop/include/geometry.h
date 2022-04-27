#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glad/glad.h"
#include <vector>

namespace Utlop {

	using namespace std;

	struct Vertex {
		glm::vec3 positions;
		glm::vec2 texCoords;
		glm::vec3 normals;
		bool Vertex::operator==(const Vertex& other) const
		{
			return positions == other.positions && texCoords == other.texCoords && normals == other.normals;
		}
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		Mesh(const std::vector<Vertex> vertices_, const std::vector<uint32_t> indices_) { vertices = vertices_; indices = indices_; }
		const std::vector<Vertex>& Mesh::getVertices() const
		{
			return vertices;
		}
		const std::vector<uint32_t>& Mesh::getIndices() const
		{
			return indices;
		}

	};
	struct Geometry {
		vector<float> vertices_;
		vector<float> texCoords_;
		vector<float> normals_;
		vector<GLuint> verticesIndices_;
		vector<GLuint> texCoordsIndices_;
		vector<GLuint> normalsIndices_;
		vector<float> totalVertex_;
		vector<GLuint> totalIndices_;
		GLuint vao_ = 999;
		GLuint vbo_ = 999;
		GLuint ebo_ = 999;
		string path;
	};

}