#include "material.h"
#include <fstream>
#include <math.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Utlop
{

	enum UniformType {
		U_COLOR = 2
	};


  Material::Material()
  {
		_color = (GLfloat*)malloc(3 * sizeof(GLfloat));
		_color[0] = 1.0f;
		_color[1] = 1.0f;
		_color[2] = 1.0f;
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

    setParameters(U_COLOR, T_FLOAT_3, _color);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
		mvp_uniform_attribute = glGetUniformLocation(_shader, "mvp");
		glm::vec3 scaling(1.0f, 1.0f, 1.0f);
		glm::vec3 translation(-0.5f, 0, 0);
		glm::vec3 rotation_axis(0, 1.0f, 0);
		float rotation_angle = 0;
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);

		glm::mat4 model_view_projection = model_matrix;

		glUniformMatrix4fv(mvp_uniform_attribute, 1, GL_FALSE, &model_view_projection[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		translation = glm::vec3(+0.5f, 0, 0);
		model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);

		model_view_projection = model_matrix;

		glUniformMatrix4fv(mvp_uniform_attribute, 1, GL_FALSE, &model_view_projection[0][0]);

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

  void Material::setParameters(const int uniform_pos, const Utlop::Type uniform_type, const float* value) {
	

		glUseProgram(_shader);

		switch (uniform_type)
		{
			case Utlop::T_FLOAT_1:
				glUniform1f(uniform_pos, value[0]); break;
			case Utlop::T_FLOAT_2:
				glUniform2f(uniform_pos, value[0], value[1]); break;
			case Utlop::T_FLOAT_3:
				glUniform3f(uniform_pos, value[0], value[1], value[2]); break;
			case Utlop::T_FLOAT_4:
				glUniform4f(uniform_pos, value[0], value[1], value[2], value[3]); break;
			case Utlop::T_MAT_4x4:
				glUniformMatrix4fv(uniform_pos, 1, GL_FALSE, value); break;
			default: break;
		}
	
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

	void Material::setColor(float* color)
	{
		_color = color;
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