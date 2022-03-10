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

		string path_;
	};

}