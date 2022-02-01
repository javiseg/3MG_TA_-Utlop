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
		Vector3 position_;
		Vector3 rotation_;
		Vector3 scale_;

		Vector3 Up;
		Vector3 Right;
		Vector3 target_;
		Vector3 direction_;

		float near_;
		float a_ratio_;
		float fov_;
		float far_;

		Matix4x4 transform_;
		Matix4x4 view_;
		Matix4x4 projection_;

		Matix4x4 view_projection_;
	};

	class Camera {

		public:
			Camera();
			~Camera();

			void init();
			void update();
		/*private:*/
			
			CameraData* data_;
	};
}