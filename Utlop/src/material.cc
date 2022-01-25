#include "material.h"
#include <fstream>
#include <math.h>
#include <vector>

namespace Utlop
{
  Material::Material()
  {

  }

  Material::~Material()
  {
    glDeleteShader(_vertex_shader);
    glDeleteShader(_fragment_shader);
  }

  void Material::init()
  {
    _shader = glCreateProgram();
  }

  void Material::draw()
  {
    glUseProgram(_shader);

    setParameters();

    glDrawArrays(GL_TRIANGLES, 0, 3);
		int bufflen = 0;
		glGetShaderiv(_vertex_shader, GL_COMPILE_STATUS, &bufflen);
		if (bufflen > 1)
		{
			GLchar* log_string = new char[bufflen + 1];
			glGetShaderInfoLog(_vertex_shader, bufflen, 0, log_string);
			printf("Log found for vert:\n%s", log_string);

			delete[] log_string;
		}

		glGetShaderiv(_fragment_shader, GL_COMPILE_STATUS, &bufflen);
		if (bufflen != GL_TRUE)
		{
			printf("Failed to compile fragment shader.");

		}
		
		
  }

  void Material::setParameters() {
	
		float color[3] = { 1.0f, 1.0f, 1.0f };

		GLuint matrix = glGetUniformLocation(_shader, "new_color");
		printf("%d\n", matrix);
		glUniform3fv(matrix, 1, color);
	
	}

  void Material::loadShader(char* vertexFilename, char* fragmentFilename)
  {
    loadVertexShader(vertexFilename);
    loadFragmentShader(fragmentFilename);

    glLinkProgram(_shader);
  }

  void Material::setBool(const char* name, bool value)
  {
    glUniform1i(glGetUniformLocation(_shader, name), (int)value);
  }

  void Material::setInt(const char* name, int value)
  {
    glUniform1i(glGetUniformLocation(_shader, name), value);
  }

  void Material::setFloat(const char* name, float value)
  {
    glUniform1f(glGetUniformLocation(_shader, name), value);
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