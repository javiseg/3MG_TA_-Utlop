#include "utility_cubemap.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "tools.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using glm::vec3;
using glm::vec4;
using glm::ivec2;

void loadVertexShader(const char* filename, GLuint& vertShader);
void loadFragmentShader(const char* filename, GLuint& fragShader);
void checkCompileErrors(unsigned int shader, std::string type);

vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize)
{
	const float A = 2.0f * float(i) / faceSize;
	const float B = 2.0f * float(j) / faceSize;

	if (faceID == 0) return vec3(-1.0f, A - 1.0f, B - 1.0f);
	if (faceID == 1) return vec3(A - 1.0f, -1.0f, 1.0f - B);
	if (faceID == 2) return vec3(1.0f, A - 1.0f, 1.0f - B);
	if (faceID == 3) return vec3(1.0f - A, 1.0f, 1.0f - B);
	if (faceID == 4) return vec3(B - 1.0f, A - 1.0f, 1.0f);
	if (faceID == 5) return vec3(1.0f - B, A - 1.0f, -1.0f);

	return vec3();
}

void Utlop::CubeMap::createBuffers()
{

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Utlop::CubeMap::loadTextures(std::string faces[6])
{

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
}

void Utlop::CubeMap::loadShaders(const char* vertPath, const char* fragPath)
{
	shaderID = glCreateProgram();
	
	GLuint vert, frag;
	loadVertexShader(vertPath, vert);
	loadFragmentShader(fragPath, frag);

	glUseProgram(shaderID);
	glAttachShader(shaderID, vert);
	glAttachShader(shaderID, frag);

	glLinkProgram(shaderID);
	checkCompileErrors(shaderID, "PROGRAM");
	checkCompileErrors(shaderID, "LINK");
	
	glUseProgram(0);


}

