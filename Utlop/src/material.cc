#pragma once

#include "material.h"
#include <fstream>
#include <math.h>
#include <vector>
#include "includes.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Utlop
{

	enum UniformType {
		U_POSITION = 0,
		U_COLOR = 2,
		U_MODEL = 3
	};


  Material::Material()
  {
		printf("Material\n");
  }

  Material::~Material()
  {
    glDeleteShader(_vertex_shader);
    glDeleteShader(_fragment_shader);
  }

  void Material::init()
  {
    _shader = glCreateProgram();
		glUseProgram(_shader);
		projection_mat_index = glGetUniformLocation(_shader, "u_vp_matrix");
		_transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
  }

  void Material::draw(int elements)
  {
		glUseProgram(_shader);
		

		//glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
		int size;  
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(int), GL_UNSIGNED_INT, 0);

		//DEBUG
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//
		
		//printf("\n%d\n",sizeof(int));
		/*int bufflen = 0;
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
			printf("Failed to compile fragment shader.\n");

		}*/
		
		
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

	void Material::setColor(glm::vec3 color)
	{
		setParameters(U_COLOR, T_FLOAT_3, &color[0]);
	}

	void Material::setPosition(glm::vec3 position)
	{
		_transform.setPosition(glm::vec3(position.x, position.y, position.z));
	}

	void Material::translate(glm::vec3 position, float speed)
	{
		_transform.setPosition(_transform.getPosition() + ( position * speed ) );
	}

	void Material::setTransform(Utlop::Transform transform)
	{
		_transform = transform;
	}

	void Material::update()
	{
		if (projection_mat_index == -1)
			projection_mat_index = glGetUniformLocation(_shader, "u_vp_matrix");

		glm::vec3 scaling(1.0f, 1.0f, 1.0f);
		//glm::vec3 translation(glm::vec3(0.0f, 0.0f, 0.0f));
		glm::vec3 rotation_axis(0.0f, 1.0f, 0.0f);
		float rotation_angle = 0.0f;
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), _transform.getScale()), rotation_angle, rotation_axis), _transform.getPosition());

		model_matrix *= Core::Instance()->getCamera()->getViewProjection();
		glUniformMatrix4fv(projection_mat_index, 1, GL_FALSE, &model_matrix[0][0]);
	}

	Material& Material::operator=(const Material& other)
	{
		_vertex_shader = other._vertex_shader;
		_fragment_shader = other._fragment_shader;
		_shader = other._shader;
		_transform = other._transform;

		return *this;
	}

	Material::Material(const Material& other)
	{
		_vertex_shader = other._vertex_shader;
		_fragment_shader = other._fragment_shader;
		_shader = other._shader;
		_transform = other._transform;
	}

	Material::Material(Material&& other)
	{
		_vertex_shader = other._vertex_shader;
		_fragment_shader = other._fragment_shader;
		_shader = other._shader;
		_transform = other._transform;
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