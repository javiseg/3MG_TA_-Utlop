#include "mesh.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include <chrono>

namespace Utlop
{
  Mesh::Mesh()
  {
		_vao = 0;
		_vbo = 0;
		_vertices = nullptr;
		
  }

  Mesh::~Mesh()
  {

  }

  void Mesh::init()
  {
  //  _vertices = (float*)calloc(18, sizeof(float));

  //  float vertices[] = {
  //    // positions         // colors
  //     0.5f, -0.5f, -0.5f,  // bottom right
  //    -0.5f, -0.5f, -0.5f,  // bottom left
  //     0.0f,  0.5f, -0.5f  // top 
  //  };
  //  memcpy(_vertices, vertices, 9 * sizeof(float));

  //  glGenVertexArrays(1, &_vao);
  //  glGenBuffers(1, &_vbo);
		//glBindVertexArray(_vao);
		//glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), _vertices, GL_STATIC_DRAW);

		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		float vertices[] = {
				-0.5f, -0.5f, 0.5f,        // 0
				0.5f, -0.5f, 0.5f,        // 1
				0.5f, 0.5f, 0.5f,            // 2
				-0.5f, 0.5f, 0.5f,         // 3

				0.5f, -0.5f, 0.5f,      // 4
				0.5f, -0.5f, -0.5f,        // 5
				0.5f, 0.5f, -0.5f,        // 6
				0.5f, 0.5f, 0.5f,        // 7

				0.5f, -0.5f, -0.5f,        // 8
				-0.5f, -0.5f, -0.5f,    // 9
				-0.5f, 0.5f, -0.5f,        // 10
				00.5f, 0.5f, -0.5f,        // 11

				-0.5f, -0.5f, -0.5f,    // 12
				-0.5f, -0.5f, 0.5f,      // 13
				-0.5f, 0.5f, 0.5f,        // 14
				-0.5f, 0.5f, -0.5f,        // 15

				0.5f, 0.5f, 0.5f,        // 16
				0.5f, 0.5f, -0.5f,        // 17
				-0.5f, 0.5f, -0.5f,      // 18
				-0.5f, 0.5f, 0.5f,    // 19

				-0.5f, -0.5f, -0.5f,    // 20
				-0.5f, -0.5f, 0.5f,        // 21
				0.5f, -0.5f, 0.5f,        // 22
				0.5f, -0.5f, -0.5f,        // 23
		};
		unsigned int indices[] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15,
				16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), &vertices, GL_STATIC_DRAW);
		
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(float), &indices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		_material = std::make_shared<Material>();
    _material->init();
    _material->loadShader("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs.glsl");
  }

  void Mesh::draw()
  {
    _material->draw();
  }

  void Mesh::start()
  {

  }

  void Mesh::update()
  {

  }

  void Mesh::destroy()
  {

  }

	void Mesh::setMaterialParameters(const int uniform_pos, const Utlop::Type uniform_type, const float* value)
	{
		_material->setParameters(uniform_pos, uniform_type, value);
	}

	void Mesh::setMaterial(Material material)
  {
    //_material = material;
  }

	void Mesh::setColor(glm::vec3 color) {
		_material->setColor(color);
	}

	void Mesh::translate(glm::vec3 position, float speed)
	{
		_material->translate(position, speed);
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		_vao = other._vao;
		_vbo = other._vbo;

		_material = other._material;
		_vertices = other._vertices;

		return *this;
	}

	Mesh::Mesh(std::shared_ptr<Mesh> other)
	{
		
	}

	Mesh::Mesh(const Mesh& other)
	{
		_vao = other._vao;
		_vbo = other._vbo;

		_material = _material = other._material;
		_vertices = other._vertices;
	}

	Mesh::Mesh(Mesh&& other)
	{
		_vao = other._vao;
		_vbo = other._vbo;

		_material = other._material;
		_vertices = other._vertices;
	}

	void Mesh::setPosition(glm::vec3 position)
	{
		_material->setPosition(position);
	}

}