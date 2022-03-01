#ifndef UTLOPmesh_
#define UTLOPmesh_ 1
#pragma once
#include <memory>
#include "shader.h"
#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "transform.h"

namespace Utlop
{
	enum Geo {
		kConst_Triangle,
		kConst_Cube,
		kConst_OBJ
	};
	const float TriangleArray[] = {
		// first triangle
		 0.0f,  0.5f, 0.0f,  // top 
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  -0.5f, 0.0f,  // bottom left 
	};
	const unsigned int TriangleArrayIndices[] = {
		0, 1, 2
	};
	const float CubeArray[] = {
				-1, -1,  1, //0
				 1, -1,  1, //1
				-1,  1,  1, //2
				 1,  1,  1, //3
				-1, -1, -1, //4
				 1, -1, -1, //5
				-1,  1, -1, //6
				 1,  1, -1  //7
	};
	const float CubeArrayIndices[] = {
		//Top
		2, 6, 7,
		2, 3, 7,

		//Bottom
		0, 4, 5,
		0, 1, 5,

		//Left
		0, 2, 6,
		0, 4, 6,

		//Right
		1, 3, 7,
		1, 5, 7,

		//Front
		0, 2, 3,
		0, 1, 3,

		//Back
		4, 6, 7,
		4, 5, 7
	};

	using namespace glm;
	using namespace std;

	struct Vertex {
		vec3 vertices;
		vec2 texCoords;
		vec3 normals;
	};


  class Mesh
  {
    public:
      Mesh();
			Mesh(char* src, Geo type, vector<float> vertices, vector<GLuint> verticesIndices,
			vector<float> texCoords,vector<GLuint> texIndices,
			vector<float> normals,vector<GLuint> normalsIndices);
      ~Mesh();

      void draw(Shader& shader);
      void start();
      void update();
      void destroy();

			void setupMesh();
			void UpdateModelMatrix();
			void UpdateUniforms(Shader& shader);
			void SetTransform(Transform tr);
		
			Mesh& operator=(const Mesh& other);
			Mesh(std::shared_ptr<Mesh> other);
			Mesh(const Mesh& other);
			Mesh(Mesh&& other);

			char* objPath_;
			Geo type_;

			vector<float> vertices_;
			vector<GLuint> verticesIndices_;

			vector<float> texCoords_;
			vector<GLuint> texIndices_;

			vector<float> normals_;
			vector<GLuint> normalsIndices_;

			Transform transform_;



			mat4 ModelMatrix;
			vec3 origin_;

    protected:

    private:
      GLuint vao_;
      GLuint vbo_;
			GLuint ebo_;
			GLuint tbo_;
			GLuint etbo_;
			
	};

	Utlop::Mesh* loadMeshFromOBJ(char* path);
}

#endif // !UTLOPmesh_
