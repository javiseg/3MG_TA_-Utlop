#pragma once
#include "glad/glad.h"

namespace Utlop {


	struct Shader {

		Shader(const char* vertexFile, const char* fragmentFile);
		
		void Activate();
		void Delete();

		void compileErrors(unsigned int shader, const char* type);

		GLuint id;

	};





}