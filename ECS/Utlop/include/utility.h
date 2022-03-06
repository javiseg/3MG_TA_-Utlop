#pragma once
#include "geometry.h"
#include "component.h"
#include <vector>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float normalized(float value, float max) {
	return  (value / max);
}
bool loadOBJ(const char* path, Utlop::Geometry& geo) {

	std::vector<float> temp_vertices;


	float max = 1.0f, min = -1.0f;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	while (1) {

		char lineHeader[128];
		// Lee la primera palabra de la línea
		int res = fscanf(file, "%s\n", lineHeader);
		if (res == EOF)
			break;


		//printf("%s\n", lineHeader);
		if (strcmp(&lineHeader[0], "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (abs(vertex.x) > max || abs(vertex.y) > max || abs(vertex.z) > max) {
				max = abs(std::max(vertex.x, vertex.y));
				max = abs(std::max(max, vertex.z));
			}

			temp_vertices.push_back(vertex.x);
			temp_vertices.push_back(vertex.y);
			temp_vertices.push_back(vertex.z);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			geo.texCoords_.push_back(uv.x);
			geo.texCoords_.push_back(uv.y);

		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%03f %03f %03f\n", &normal.x, &normal.y, &normal.z);
			geo.normals_.push_back(normal.x);
			geo.normals_.push_back(normal.y);
			geo.normals_.push_back(normal.z);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			geo.verticesIndices_.push_back(vertexIndex[0] - 1);
			geo.verticesIndices_.push_back(vertexIndex[1] - 1);
			geo.verticesIndices_.push_back(vertexIndex[2] - 1);
			geo.texCoordsIndices_.push_back(uvIndex[0] - 1);
			geo.texCoordsIndices_.push_back(uvIndex[1] - 1);
			geo.texCoordsIndices_.push_back(uvIndex[2] - 1);
			geo.normalsIndices_.push_back(normalIndex[0] - 1);
			geo.normalsIndices_.push_back(normalIndex[1] - 1);
			geo.normalsIndices_.push_back(normalIndex[2] - 1);

		}
	}
	for (size_t i = 0; i < temp_vertices.size(); i++) {
		geo.vertices_.push_back(normalized(temp_vertices[i], max));
	}
	geo.path = path;

	return true;
}

void loadVertexShader(const char* filename, Utlop::RenderComponent& rc)
{
	std::ifstream inputVertexShader(filename);
	std::string vertexShader((std::istreambuf_iterator<char>(inputVertexShader)), (std::istreambuf_iterator<char>()));
	const char* vertexShaderString = vertexShader.c_str();

	unsigned int _vertex_shader;
	_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertex_shader, 1, &vertexShaderString, NULL);
	glCompileShader(_vertex_shader);

	glAttachShader(rc.shaderID_, _vertex_shader);

	//glDeleteShader(_vertex_shader);
}

void loadFragmentShader(const char* filename, Utlop::RenderComponent& rc)
{
	std::ifstream inputFragmentShader(filename);
	std::string fragmentShader((std::istreambuf_iterator<char>(inputFragmentShader)), (std::istreambuf_iterator<char>()));
	const char* fragmentShaderString = fragmentShader.c_str();

	unsigned int _fragment_shader;
	_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragment_shader, 1, &fragmentShaderString, NULL);
	glCompileShader(_fragment_shader);

	glAttachShader(rc.shaderID_, _fragment_shader);

	//glDeleteShader(_fragment_shader);
}

void checkCompileErrors(unsigned int shader, std::string type)
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

void setMat4fv(GLuint shader_id, glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE)
{
	glUseProgram(shader_id);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, name), 1, transpose, &value[0][0]);
	glUseProgram(0);
}