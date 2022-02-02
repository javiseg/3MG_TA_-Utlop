#pragma once

#include "material.h"
#include <fstream>
#include <math.h>
#include <vector>
#include "matrix_4.h"
#include "includes.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Utlop
{

	enum UniformType {
		U_POSITION = 1,
		U_COLOR = 2,
		U_MODEL = 3
	};


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

    

		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		/*glm::mat4 final_matrix = glm::make_mat4(camera_mat_.m);
		Vector3 cam_pos = Vector3(view_.m + 12);*/

		int projection_mat = glGetUniformLocation(_shader, "u_vp_matrix");

		glm::vec3 scaling(1.0f, 1.0f, 1.0f);
		//glm::vec3 translation(glm::vec3(0.0f, 0.0f, 0.0f));
		glm::vec3 rotation_axis(0.0f, 1.0f, 0.0f);
		float rotation_angle = 0;
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), _transform.getPosition());
		model_matrix *= Core::Instance()->getCamera().data_->view_projection_;
		glUniformMatrix4fv(projection_mat, 1, GL_FALSE, &model_matrix[0][0]);

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

	void Material::setColor(glm::vec3 color)
	{
		setParameters(U_COLOR, T_FLOAT_3, &color[0]);
	}

	void Material::setPosition(glm::vec3 position)
	{
		_transform.setPosition(position);
	}

	void Material::translate(glm::vec3 position, float speed)
	{
		_transform.setPosition(_transform.getPosition() + ( position * speed ) );
	}

	void Material::setTransform(Utlop::Transform transform)
	{
		_transform = transform;
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