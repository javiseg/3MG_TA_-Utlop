#include "shader.h"

void Utlop::Shader::setBool(const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(id_, name), (int)value);
}

void Utlop::Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(id_, name), value);
}

void Utlop::Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(id_, name), value);
}

void Utlop::Shader::setParameters(const int uniform_pos, const Utlop::Type uniform_type, const float* value)
{
	glUseProgram(id_);

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

void Utlop::Shader::loadShaderFiles(const char* vS, const char* fS)
{
	id_ = glCreateProgram();

	loadVertexShader(vS);
	loadFragmentShader(fS);

	glLinkProgram(id_);
}

void Utlop::Shader::loadVertexShader(const char* filename)
{
	std::ifstream inputVertexShader(filename);
	std::string vertexShader((std::istreambuf_iterator<char>(inputVertexShader)), (std::istreambuf_iterator<char>()));
	const char* vertexShaderString = vertexShader.c_str();

	unsigned int _vertex_shader;
	_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertex_shader, 1, &vertexShaderString, NULL);
	glCompileShader(_vertex_shader);

	glAttachShader(id_, _vertex_shader);

	//glDeleteShader(_vertex_shader);
}

void Utlop::Shader::loadFragmentShader(const char* filename)
{
	std::ifstream inputFragmentShader(filename);
	std::string fragmentShader((std::istreambuf_iterator<char>(inputFragmentShader)), (std::istreambuf_iterator<char>()));
	const char* fragmentShaderString = fragmentShader.c_str();
	
	unsigned int _fragment_shader;
	_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragment_shader, 1, &fragmentShaderString, NULL);
	glCompileShader(_fragment_shader);

	glAttachShader(id_, _fragment_shader);
	
	//glDeleteShader(_fragment_shader);
}

int Utlop::Shader::uniformLocation(char* src)
{
	return glGetUniformLocation(id_, src);
}

void Utlop::Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
