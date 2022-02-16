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
		glm::vec3 direction_; // Front

		float near_;
		float a_ratio_;
		float fov_;
		float far_;
		float rotation_angle_;
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

			glm::mat4 getViewProjection();
			
			void moveForward(float value);
			void moveRight(float value);
			void moveUp(float value);
			void RotateCamera(float deltaTime, float speed, int direction);

		private:

			void UpdateData();

			std::unique_ptr<CameraData> data_;
			float yaw_ = 90.0f;
			float pitch_ = 0.0f;
			float velocity_;
			glm::vec3 WorldUp;
			glm::vec3 WorldRight;
	};
}