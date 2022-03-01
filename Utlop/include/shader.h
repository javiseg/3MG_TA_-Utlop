#pragma once
#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include <iostream>
#include <fstream>

namespace Utlop {
	enum Type {
		T_NONE = 0x0,
		T_FLOAT_1,
		T_FLOAT_2,
		T_FLOAT_3,
		T_FLOAT_4,

		T_DOUBLE_1,
		T_DOUBLE_2,
		T_DOUBLE_3,
		T_DOUBLE_4,

		T_INT_1,
		T_INT_2,
		T_INT_3,
		T_INT_4,

		T_UINT_1,
		T_UINT_2,
		T_UINT_3,
		T_UINT_4,

		T_BYTE_1,
		T_BYTE_2,
		T_BYTE_3,
		T_BYTE_4,

		T_UBYTE_1,
		T_UBYTE_2,
		T_UBYTE_3,
		T_UBYTE_4,

		T_SHORT_1,
		T_SHORT_2,
		T_SHORT_3,
		T_SHORT_4,

		T_USHORT_1,
		T_USHORT_2,
		T_USHORT_3,
		T_USHORT_4,

		T_MAT_4x4,
		T_MAT_3x3,
		T_MAT_2x2,

		T_SAMPLER_1D,
		T_SAMPLER_2D,
		T_SAMPLER_3D,

		// aliases
		T_FLOAT = T_FLOAT_1,
		T_DOUBLE = T_DOUBLE_1,
		T_INT = T_INT_1,
		T_UINT = T_UINT_1,
		T_BYTE = T_BYTE_1,
		T_UBYTE = T_UBYTE_1,
		T_SHORT = T_SHORT_1,
		T_USHORT = T_USHORT_1,

		//--------------------------
		T_MAX_DEFINED // max value of defined types
	};

	enum UniformType {
		U_POSITION = 0,
		U_COLOR = 1,
		U_MODEL = 2,
		U_TEXTURE = 3,
		U_NORMALS = 4
	};


	class Shader
	{
	public:
		Shader() {
			id_ = 0;
		}
		Shader::Shader(const Shader& other) {
			id_ = other.id_;
		}
		Shader(const char* vertexPath, const char* fragmentPath)
		{
			id_ = glCreateProgram();
			loadVertexShader(vertexPath);
			loadFragmentShader(fragmentPath);

			glLinkProgram(id_);
		}
		Shader& Shader::operator=(const Shader& other)
		{
			id_ = other.id_;

			return *this;
		}
		Shader::Shader(std::shared_ptr<Shader> other)
		{
			
		}

		Shader::Shader(Shader&& other)
		{
			id_ = other.id_;
		}
		~Shader() {}
		unsigned int ID() { return id_; }

		void use() { glUseProgram(id_); }

		void setBool(const char* name, bool value);
		void setInt(const char* name, int value);
		void setFloat(const char* name, float value);
		void setParameters(const int uniform_pos, const Utlop::Type uniform_type, const float* value);
		void loadShaderFiles(const char* vS, const char* fS);
		void loadVertexShader(const char* filename);
		void loadFragmentShader(const char* filename);

		int uniformLocation(const GLchar* src);
		void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

	private:
		unsigned int id_;

		void checkCompileErrors(unsigned int shader, std::string type);
	};
}



