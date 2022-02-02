#pragma once
/*
struct RenderData {
	Matix4x4 model;
	const EDK3::Geometry* geometry;
	const EDK3::Material* material;
	const EDK3::MaterialSettings* mat_setting;
};
*/
#include <memory>
#include "matrix_4.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Utlop {
	
	struct CameraData
	{
		glm::vec3 position_;
		glm::vec3 rotation_;
		glm::vec3 scale_;

		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 target_;
		glm::vec3 direction_;

		float near_;
		float a_ratio_;
		float fov_;
		float far_;

		glm::mat4 transform_;
		glm::mat4 view_;
		glm::mat4 projection_;

		glm::mat4 view_projection_;
	};

	class Camera {

		public:
			Camera();
			~Camera();

			void init();
			void update();
		/*private:*/
			
			std::unique_ptr<CameraData> data_;
	};
}