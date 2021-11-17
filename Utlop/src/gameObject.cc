#include "gameObject.h"
//#include <glad/glad.h>
#include <fstream>
#include <chrono>
#include <stdlib.h> 
#include <glm/gtc/matrix_transform.hpp>

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
    //float vertices[] = {
    //  -0.5f, -0.5f, 0.0f, 1.0f,
    //   0.5f, -0.5f, 0.0f, 1.0f,
    //   0.0f,  0.5f, 0.0f, 1.0f
    //};
    _vertices = (float*)calloc(12, sizeof(float));
    float vertices[] = {
      -0.05f, -0.05f, 0.0f, 0.5f,
       0.05f, -0.05f, 0.0f, 1.25f,
       0.0f,  0.05f, 0.0f, 0.5f
    };
    memcpy(_vertices, vertices, 12 * sizeof(float));

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), _vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    _material.init();
    _material.loadShader("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs.glsl");
		printf("Program: %d\n", _material.ProgramNumber());
		printf("Uniform Location: %d\n", glGetUniformLocation(_material.ProgramNumber(), "positions"));
		GLuint location = glGetUniformLocation(_material.ProgramNumber(), "positions");
		glUniform4f(location, _transform.Position().x, _transform.Position().y,
			_transform.Position().z, 3.0f);
		printf("X: %f \n Y: %f \n Z: %f \n", _transform.Position().x, _transform.Position().y, _transform.Position().z);
	}

  void GameObject::draw()
  {

    _material.draw();
  }

  void GameObject::start()
  {

  }

  void GameObject::update()
  {
		//printf("X: %f \n Y: %f \n Z: %f \n", _transform.Position().x, _transform.Position().y, _transform.Position().z);

		/*glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		vec = trans * vec;*/

    _vertices[0] = rand() % 2;
    _vertices[0] = fmodf(_vertices[0], 0.5f);
    _vertices[9] = 0.002f;
    _vertices[9] = fmodf(_vertices[9], 0.5f);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), _vertices, GL_STATIC_DRAW);
	}

  void GameObject::destroy()
  {

  }

  void GameObject::setMaterial(Material material)
  {
    _material = material;
  }

  void GameObject::setTransform(Transform transform)
  {
    _transform = transform;
  }
}