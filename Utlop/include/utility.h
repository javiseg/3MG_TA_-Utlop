#pragma once
#include "geometry.h"
#include "component.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "bitmap.h"
#include "utility_cubemap.h"
#include "tiny_obj_loader.h"
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

namespace std
{
	template <>
	struct hash<Utlop::Vertex>
	{
		size_t operator()(const Utlop::Vertex& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.positions) ^ (hash<glm::vec2>()(vertex.texCoords) << 1)) >> 1);
		}
	};
}
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class UtlopMap {
	std::map<glm::vec3, uint32_t> mymap;
};


float normalized(float value, float max) {
	return  (value / max);
}
bool loadOBJ2(const char* path, Utlop::Geometry& geo) {

		
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warning;
	std::string error;

	if (!tinyobj::LoadObj(
		&attributes,
		&shapes,
		&materials,
		&warning,
		&error,
		path))
	{
		throw std::runtime_error("ast::assets::loadOBJFile: Error: " + warning + error);
	}
	
	std::vector<Utlop::Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Utlop::Vertex> texVertices;
	std::vector<uint32_t> texIndices;
	std::unordered_map<Utlop::Vertex, uint32_t> uniqueVertices;

	for (int i = 0; i < shapes.size(); i++)
	{
		for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
		{
			
			geo.verticesIndices_.push_back(shapes[i].mesh.indices[j].vertex_index);
			geo.texCoordsIndices_.push_back(shapes[i].mesh.indices[j].texcoord_index);
		}
	}

	for (int i = 0; i < attributes.GetVertices().size(); i++) {
		geo.vertices_.push_back(attributes.vertices[i]);
	}
	for (int i = 0; i < attributes.texcoords.size(); i++) {
		geo.texCoords_.push_back(attributes.texcoords[i]);
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

	glDeleteShader(_vertex_shader);
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

	glDeleteShader(_fragment_shader);
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

void loadCubemap(const char* path, GLuint& texture)
{
	int w, h, comp;
	const float* img = stbi_loadf(path, &w, &h, &comp, 3);
	Utlop::Bitmap in(w, h, comp, Utlop::eBitmapFormat_Float, img);
	Utlop::Bitmap out = convertEquirectangularMapToVerticalCross(in);
	stbi_image_free((void*)img);

	stbi_write_hdr("screenshot.hdr", out.w_, out.h_, out.comp_, (const float*)out.data_.data());

	Utlop::Bitmap cubemap = convertVerticalCrossToCubeMapFaces(out);

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &texture);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(texture, 1, GL_RGB32F, cubemap.w_, cubemap.h_);
	const uint8_t* data = cubemap.data_.data();

	for (unsigned i = 0; i != 6; ++i)
	{
		glTextureSubImage3D(texture, 0, 0, 0, i, cubemap.w_, cubemap.h_, 1, GL_RGB, GL_FLOAT, data);
		data += cubemap.w_ * cubemap.h_ * cubemap.comp_ * Utlop::Bitmap::getBytesPerComponent(cubemap.fmt_);
	}
	glBindTextures(1, 1, &texture);

}