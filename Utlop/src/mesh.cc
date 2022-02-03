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
    _vertices = (float*)calloc(18, sizeof(float));

    float vertices[] = {
      // positions         // colors
       0.2f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
      -0.2f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
       0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

    };
    memcpy(_vertices, vertices, 18 * sizeof(float));

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), _vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

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
		printf("MESH X: %f, Y: %f, Z: %f\n", _material->_transform.getPosition().x,
			_material->_transform.getPosition().y, _material->_transform.getPosition().z);

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