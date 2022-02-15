#include "camera.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

namespace Utlop {

	using namespace glm;

	Camera::Camera()
	{
		//data_ = (CameraData*) malloc(sizeof(CameraData));
		data_ = std::make_unique<CameraData>();
	}

	Camera::~Camera()
	{
	}

	void Camera::init()
	{
		data_->Right = vec3(1.0f, 0.0f, 0.0f);
		data_->Up = vec3(0.0f, 1.0f, 0.0f);
		data_->scale_ = vec3(1.0f, 1.0f, 1.0f);
		data_->position_ = vec3(0.0f, 0.0f, -5.0f);
		data_->rotation_ = vec3(0.0f, 1.0f, 0.0f);
		data_->target_ = vec3(0.0f, 0.0f, 0.0f);
		data_->rotation_angle_ = 0.0f;


		data_->projection_ = perspective(1.57f, 4.0f / 3.0f, 0.1f, 100.0f);
		data_->view_ = lookAt(data_->position_, data_->target_, data_->Up);

		UpdateData();

		//std::cout << glm::to_string(data_->projection_) << std::endl;

	}
	void Camera::update()
	{
	}

	glm::mat4 Camera::getViewProjection()
	{
		return data_->view_projection_;
	}

	void Camera::moveForward(float value)
	{
		data_->position_ = glm::vec3(data_->position_[0], data_->position_[1], data_->position_[2] + value);
		UpdateData();
	}
	void Camera::moveRight(float value)
	{
		data_->position_ = glm::vec3(data_->position_[0] + value, data_->position_[1], data_->position_[2]);
		UpdateData();
	}
	void Camera::moveUp(float value)
	{
		data_->position_ = glm::vec3(data_->position_[0], data_->position_[1] + value, data_->position_[2]);
		UpdateData();
	}

	void Camera::RotateCamera(float deltaTime, float speed)
	{
		//vec3 position_ = vec3(data_->position_);
		//vec3 target_ = vec3(data_->target_);
		////data_->direction_ = normalize(data_->position_ - data_->target_);
		//vec3 cameraRight = normalize(cross(data_->Up, data_->direction_));
		//vec3 cameraUp = cross(data_->direction_, cameraRight);

		//yaw_ -= deltaTime * speed;

		//vec3 direction;
		//direction.x = cos(glm::radians(yaw_)) * cos(radians(pitch_));
		//direction.z = sin(radians(yaw_)) * cos(radians(pitch_));;
		//direction.y = radians(pitch_);
		//
		//data_->direction_ = normalize(direction);
		//data_->view_ = lookAt(data_->position_, vec3(0.0f,0.0f,0.0f), data_->Up);
		//printf("%f \n", data_->position_ - data_->direction_);

		//data_->target_ = vec3(data_->target_[0] + 1.0f, data_->target_[1], data_->target_[2]);

		UpdateData();
	}
	void Camera::UpdateData()
	{
		data_->view_ = lookAt(data_->position_, data_->target_, data_->Up);
		data_->view_projection_ = data_->projection_ * data_->view_;
		printf("X: %f, Y: %f, Z: %f \n", data_->position_.x, data_->position_.y, data_->position_.z);
	}

}

