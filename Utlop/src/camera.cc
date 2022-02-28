#include "camera.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "core.h"
#include "GLFW/glfw3.h"

namespace Utlop {

	using namespace glm;

	Camera::Camera()
	{
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
		data_->position_ = vec3(0.0f, 0.0f, 15.0f);
		data_->rotation_ = vec3(0.0f, 1.0f, 0.0f);
		data_->target_ = vec3(0.0f, 0.0f, -1.0f);
		data_->direction_ = vec3(0.0f, 0.0f, -1.0f);
		data_->rotation_angle_ = 0.0f;

		WorldUp = vec3(0.0f, 1.0f, 0.0f);
		WorldRight = vec3(1.0f, 0.0f, 0.0f);

		velocity_ = 10.0f;

		//data_->projection_ = perspective(1.57f, 1.0f, 0.1f, 100.0f);
		data_->projection_ = glm::perspective(glm::radians(45.0f), (float)1380 / (float)780, 0.1f, 100.0f);
		yaw_ = -90.0f;
		pitch_ = 0.0f;

		UpdateData();

		//std::cout << glm::to_string(data_->projection_) << std::endl;

	}
	void Camera::update()
	{
		
	}

	glm::mat4 Camera::getViewProjection()
	{
		UpdateData();
		this->data_->view_ = glm::lookAt(this->data_->position_, this->data_->position_ + this->data_->direction_, this->data_->Up);

		return data_->projection_ * data_->view_;
	}

  glm::mat4 Camera::getView()
  {
		UpdateData();
		this->data_->view_ = glm::lookAt(this->data_->position_, this->data_->position_ + this->data_->direction_, this->data_->Up);
    return data_->view_;
  }

	glm::mat4 Camera::getProjection()
	{
		return data_->projection_;
	}

	void Camera::moveForward(float value)
	{
		if(value > 0)
			data_->position_ += data_->direction_ * velocity_ * Core::Instance()->getDeltaTime();
		
		if(value < 0)
			data_->position_ -= data_->direction_ * velocity_ * Core::Instance()->getDeltaTime();
		
		UpdateData();
	}
	void Camera::moveRight(float value)
	{
		if (value > 0)
			data_->position_ -= data_->Right * velocity_ * Core::Instance()->getDeltaTime();

		if (value < 0)
			data_->position_ += data_->Right * velocity_ * Core::Instance()->getDeltaTime();
		
		UpdateData();
	}
	void Camera::moveUp(float value)
	{
		if (value > 0)
			data_->position_ -= data_->Up * velocity_ * Core::Instance()->getDeltaTime();

		if (value < 0)
			data_->position_ += data_->Up * velocity_ * Core::Instance()->getDeltaTime(); 
		
		UpdateData();
	}

	void Camera::RotateCamera(float deltaTime, float speed, int direction)
	{

		switch (direction) {
			case 0: {
				yaw_ -= deltaTime * speed;
			}break;
			case 1: {
				yaw_ += deltaTime * speed;
			}break;
			case 2: {
				pitch_ += deltaTime * speed;
			}break;
			case 3: {
				pitch_ -= deltaTime * speed;
			}break;
		}

		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		if (pitch_ < -89.0f)
			pitch_ = -89.0f;

		UpdateData();
	}
	void Camera::UpdateData()
	{
		//data_->direction_ = normalize(data_->position_ - data_->target_);

		this->data_->direction_.x = cos(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_));
		this->data_->direction_.y = sin(glm::radians(this->pitch_));
		this->data_->direction_.z = sin(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_));

		this->data_->direction_ = glm::normalize(this->data_->direction_);
		this->data_->Right = glm::normalize(glm::cross(this->data_->direction_, this->WorldUp));
		this->data_->Up = glm::normalize(glm::cross(this->data_->Right, this->data_->direction_));
	}
}

