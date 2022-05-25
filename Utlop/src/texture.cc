#include "..\include\texture.h"
#include "stb_image.h"

Utlop::Texture::Texture(const char* path, const char* texType, GLenum slot, GLenum format, GLenum pixelType)
{
  id = 0;
	type = texType;
	unit = slot;
	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	unsigned char* text_buffer_;
	text_buffer_ = stbi_load(path, &widthImg, &heightImg, &numColCh, 4);

	if (text_buffer_) {
		stbi_set_flip_vertically_on_load(false);

		glCreateTextures(GL_TEXTURE_2D, 1, &id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(id, 1, GL_RGBA8, widthImg, heightImg);
		glTextureSubImage2D(id, 0, 0, 0, widthImg, heightImg, GL_RGBA, GL_UNSIGNED_BYTE, text_buffer_);
		
		
		if (type == "normal") {
			glTextureStorage2D(id, 1, GL_RGB, widthImg, heightImg);
			glTextureSubImage2D(id, 0, 0, 0, widthImg, heightImg, GL_RGBA, GL_UNSIGNED_BYTE, text_buffer_);
		}
		else {
			glTextureStorage2D(id, 1, GL_RGBA8, widthImg, heightImg);
			glTextureSubImage2D(id, 0, 0, 0, widthImg, heightImg, GL_RGBA, GL_UNSIGNED_BYTE, text_buffer_);
		}
		
		
		
		
		glGenerateTextureMipmap(id);


		stbi_image_free(text_buffer_);
	}
	else {
		printf("\nError loading Texture");
	}
	
	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Utlop::Texture::texUnit(GLuint shader, const char* uniform, GLuint unit)
{
	// Shader needs to be activated before changing the value of a uniform
	glUseProgram(shader);
  GLuint texUni = glGetUniformLocation(shader, uniform);

  // Sets the value of the uniform
	glUniform1i(texUni, unit);
	//glUseProgram(0);
}

void Utlop::Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Utlop::Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Utlop::Texture::deleteTexture()
{
	glDeleteTextures(1, &id);
}
