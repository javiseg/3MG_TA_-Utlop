#include "gameObject.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "geometry.h"
#include <fstream>
#include <vector>
#include <chrono>

//Testing
#include <time.h>
///


namespace Utlop
{
  GameObject::GameObject()
  {
		
  }

  GameObject::~GameObject()
  {

  }

  void GameObject::init()
  {
		_mesh = std::make_unique<Mesh>();
    _mesh->init();
		_mesh->setColor(glm::vec3(0.0f,0.0f,1.0f));
		_mesh->setPosition(glm::vec3(0.0f, 0.4f, 0.1f));
		printf("GameObjectInit\n");
  }

  void GameObject::draw()
  {
    _mesh->draw();
  }

  void GameObject::start()
  {
		
  }

  void GameObject::update()
  {
    _mesh->update();
  }

  void GameObject::destroy()
  {

  }

  void GameObject::setTransform(Transform transform)
  {
    _transform = transform;
		
  }
	void GameObject::setScale(glm::vec3 scale)
	{
		//_mesh.setMaterialParameters(Utlop::U_)
	}
	void GameObject::setRotation(glm::vec3 rotation)
	{
	}
	void GameObject::setPosition(glm::vec3 position)
	{
		_transform.setPosition(position);
		_mesh->setPosition(position);

	}
	void GameObject::setColor(glm::vec3 color)
	{
		_mesh->setColor(color);
	}
	void GameObject::translate(glm::vec3 position, float speed)
	{
		_transform.setPosition(_transform.getPosition() + (position * speed));

	}

	void GameObject::setGeometry(Utlop::Geo geometry)
	{
		switch (geometry) {
			case 0: {
				_mesh->createTriangle();
			}break;	
			case 1: {
				_mesh->createCube();
			}break;
		}
	}

	float normalized(float value, float max, float min) {
		return (value - min) / (max - min);
	}

	bool loadOBJ(const char* path,
		std::vector <float>& out_vertices,
		std::vector < glm::vec2 >& out_uvs,
		std::vector < glm::vec3 >& out_normals,
		std::vector <unsigned int>& out_indices) {

		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< float > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;
		std::vector< glm::vec3 > temp_indices;

		float max = 1.0f, min = -1.0f;

		FILE* file = fopen(path, "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return false;
		}
		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
		while (1) {

			char lineHeader[128];
			// Lee la primera palabra de la línea
			int res = fscanf(file, "%s\n", lineHeader);
			if (res == EOF)
				break;

			//printf("%s\n", lineHeader);
			if (strcmp(&lineHeader[0], "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

				if (vertex.x > max || vertex.y > max || vertex.z > max) {
					max = std::max(vertex.x, vertex.y);
					max = std::max(max, vertex.z);
				}
				if (vertex.x < min || vertex.y < min || vertex.z < min) {
					min = std::min(vertex.x, vertex.y);
					min = std::min(min, vertex.z);
				}

				temp_vertices.push_back(vertex.x);
				temp_vertices.push_back(vertex.y);
				temp_vertices.push_back(vertex.z);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);

			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;

				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);




				out_indices.push_back(vertexIndex[0] - 1);
				out_indices.push_back(vertexIndex[1] - 1);
				out_indices.push_back(vertexIndex[2] - 1);

			}
		}
		for (int i = 0; i < temp_vertices.size(); i++) {
			out_vertices.push_back(normalized(temp_vertices[i], max, min));
		}

		return true;
	}

	void GameObject::setGeometry(char* src)
	{
		std::vector<float> vertices;
		std::vector< glm::vec2 > uvs;
		std::vector< glm::vec3 > normals;
		std::vector<unsigned int> indices;

		bool res = loadOBJ(src, vertices, uvs, normals, indices);
		// "../UtlopTests/src/shaders/vs.glsl"

		_mesh->createObject(vertices, indices);
	}

	GameObject& GameObject::operator=(const GameObject& other)
	{
		_vao = other._vao;
		_vbo = other._vbo;
		_transform = other._transform;
		_mesh = other._mesh;
		_vertices = other._vertices;

		return *this;
	}
	// TEXTURES STB _IMAGE
	GameObject::GameObject(const GameObject& other) {
		_vao = other._vao;
		_vbo = other._vbo;
		_transform = other._transform;
		_mesh = other._mesh;
		_vertices = other._vertices;
	}
	GameObject::GameObject(GameObject&& other) {
		_vao = other._vao;
		_vbo = other._vbo;
		_transform = other._transform;
		_mesh = other._mesh;
		_vertices = other._vertices;
	}

}