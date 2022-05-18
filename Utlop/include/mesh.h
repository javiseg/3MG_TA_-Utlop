#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glad/glad.h"
#include <vector>
#include "component.h"
#include "texture.h"

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
		std::vector<Texture> textures;

		GLuint normalMap;
		GLuint vao;
    GLuint n_indices;
		
		string path;

		Mesh(const std::vector<float> vertices_, const std::vector<uint32_t> indices_,
			std::vector<Texture> textures_, string geoPath);
		Mesh(){
			vao = 999;
		}
		~Mesh() {}
		
		const std::vector<Texture>& Mesh::getTextures() const
		{
			return textures;
		}

		void Draw(GLuint& shader, CameraComponent& cameracmp);
		void DrawMesh(Mesh m, GLuint& shader, CameraComponent& cameracmp, LocalTRComponent& localcmp);	
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