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
	struct Mesh
	{
		GLuint normalMap;
		GLuint vao;
    GLuint n_indices;

    Geometry geo;
		
		string path;

		Mesh(const std::vector<float> vertices_, const std::vector<uint32_t> indices_,
      string geoPath, Geometry geo_);
		Mesh(){
			vao = 999;
		}
		~Mesh() {}
		
		void Draw(GLuint& shader, CameraComponent& cameracmp, int hasLightcomponent, Material mat);
	};

}