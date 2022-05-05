#pragma once
#include "glad/glad.h"
#include <string>

namespace Utlop {

	struct Material {
		GLuint diff_ = 999;
		GLuint spec_ = 999;
		GLuint alb_ = 999;

		//Debug purposes:
		GLint width_;
		GLint height_;
		GLint bpp_;

		std::string path_;
	};

	struct Texture {
		GLuint id;
		const char* type;
		GLuint unit;
		Texture(const char* path, const char* texType, GLenum slot, GLenum format, GLenum pixelType);

		void texUnit(GLuint shader, const char* uniform, GLuint unit);
		void bind();
		void unbind();
		void deleteTexture();
	};

}