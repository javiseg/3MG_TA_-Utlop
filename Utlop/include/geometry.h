#pragma once
#include <string>

namespace Utlop {
	struct Vertex {
		glm::vec3 positions;
		glm::vec2 texCoords;
		bool Vertex::operator==(const Vertex& other) const
		{
			return positions == other.positions && texCoords == other.texCoords;
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
		vector<uint32_t> verticesIndices_;
		vector<GLuint> texCoordsIndices_;
		vector<GLuint> normalsIndices_;
		string path;
	};

}