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
		n_vertice_ = 0;
		
  }

  Mesh::~Mesh()
  {

  }

  void Mesh::init(unsigned int geoType)
  {

		switch (geoType) {
			case 0: {
				  float* _vertices = (float*)calloc(18, sizeof(float));

					float vertices[] = {
						// positions         // colors
						 0.5f, -0.5f, -0.5f,  // bottom right
						-0.5f, -0.5f, -0.5f,  // bottom left
						 0.0f,  0.5f, -0.5f  // top 
					};

					unsigned int indices[] = { 0, 1, 3,
					1, 2, 3};


					glGenVertexArrays(1, &_vao);
					glGenBuffers(1, &_vbo);
					glBindVertexArray(_vao);
					glBindBuffer(GL_ARRAY_BUFFER, _vbo);
					glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &_vertices, GL_STATIC_DRAW);

					glGenBuffers(1, &_ebo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);


					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);
					n_vertice_ = 6;
			}break;
			case 1: {
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
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

				glGenBuffers(1, &_ebo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				n_vertice_ = 36;

			}break;
		}
  
		

		_material = std::make_shared<Material>();
    _material->init();
    _material->loadShader("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs.glsl");
  }

  void Mesh::draw()
  {
    _material->draw(n_vertice_);
  }

  void Mesh::start()
  {

  }

  void Mesh::update()
  {
		_material->update();
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
	}

	Mesh::Mesh(Mesh&& other)
	{
		_vao = other._vao;
		_vbo = other._vbo;

		_material = other._material;
	}

	void Mesh::setPosition(glm::vec3 position)
	{
		_material->setPosition(position);
	}

}