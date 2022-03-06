#ifndef UTLOP_MATERIAL
#define UTLOP_MATERIAL 1

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "glm/glm.hpp"
#include "glm/glm.hpp"

namespace Utlop
{
	using namespace glm;

	struct Material {
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		float shininess;
	};

}

#endif // !UTLOP_MATERIAL
