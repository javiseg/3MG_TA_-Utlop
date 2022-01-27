#pragma once
/*
struct RenderData {
	Matix4x4 model;
	const EDK3::Geometry* geometry;
	const EDK3::Material* material;
	const EDK3::MaterialSettings* mat_setting;
};
*/
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Utlop {

	class camera
	{
		public:
			void init();
	};

}
