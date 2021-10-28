#include "material.h"
#include <fstream>

namespace Utlop
{
  Material::Material()
  {

  }

  Material::~Material()
  {

  }

  void Material::init()
  {
    _shader = glCreateProgram();
  }

  void Material::draw()
  {
    glUseProgram(_shader);

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  void Material::loadShader(char* vertexFilename, char* fragmentFilename)
  {
    loadVertexShader(vertexFilename);
    loadFragmentShader(fragmentFilename);

    glLinkProgram(_shader);
  }

  void Material::loadVertexShader(char* filename)
  {
    std::ifstream inputVertexShader(filename);
    std::string vertexShader((std::istreambuf_iterator<char>(inputVertexShader)), (std::istreambuf_iterator<char>()));
    const char* vertexShaderString = vertexShader.c_str();

    _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertex_shader, 1, &vertexShaderString, NULL);
    glCompileShader(_vertex_shader);

    glAttachShader(_shader, _vertex_shader);
  }

  void Material::loadFragmentShader(char* filename)
  {
    std::ifstream inputFragmentShader(filename);
    std::string fragmentShader((std::istreambuf_iterator<char>(inputFragmentShader)), (std::istreambuf_iterator<char>()));
    const char* fragmentShaderString = fragmentShader.c_str();

    _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragment_shader, 1, &fragmentShaderString, NULL);
    glCompileShader(_fragment_shader);

    glAttachShader(_shader, _fragment_shader);
  }
}