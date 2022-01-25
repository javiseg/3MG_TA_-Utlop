#include "mesh.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include <chrono>

namespace Utlop
{
  Mesh::Mesh()
  {

  }

  Mesh::~Mesh()
  {

  }

  void Mesh::init()
  {
    _vertices = (float*)calloc(18, sizeof(float));

    float vertices[] = {
      // positions         // colors
       0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
       0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

    };
    memcpy(_vertices, vertices, 18 * sizeof(float));

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);


    _material.init();
    _material.loadShader("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs.glsl");
  }

  void Mesh::draw()
  {
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), _vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    _material.draw();
  }

  void Mesh::start()
  {

  }

  void Mesh::update()
  {
    //_vertices[0] += 0.002f;
    //_vertices[0] = fmodf(_vertices[0], 0.5f);
    //_vertices[9] -= 0.002f;
    //_vertices[9] = fmodf(_vertices[9], 0.5f);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), _vertices, GL_STATIC_DRAW);
  }

  void Mesh::destroy()
  {

  }

  void Mesh::setMaterial(Material material)
  {
    _material = material;
  }
}