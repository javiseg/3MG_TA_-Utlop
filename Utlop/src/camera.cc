#include "camera.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace Utlop {

	

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
		data_->Right = glm::vec3(1.0f, 0.0f, 0.0f);
		data_->scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
		data_->position_ = glm::vec3(0.0f, 0.0f, -5.0f);
		data_->rotation_ = glm::vec3(0.0f, 1.0f, 0.0f);
		data_->rotation_angle_ = 0.0f;
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), data_->scale_), data_->rotation_angle_, data_->rotation_), data_->position_);

		data_->projection_ = glm::perspective(1.57f, 4.0f / 3.0f, 0.1f, 100.0f);
		data_->view_ = glm::lookAt(glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f));

		data_->view_projection_ = data_->projection_ * data_->view_ * model_matrix;

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
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), data_->scale_), data_->rotation_angle_, data_->rotation_), data_->position_);
		data_->view_projection_ = data_->projection_ * data_->view_ * model_matrix;
	}
	void Camera::moveRight(float value)
	{
		data_->position_ = glm::vec3(data_->position_[0] + value, data_->position_[1], data_->position_[2]);
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), data_->scale_), data_->rotation_angle_, data_->rotation_), data_->position_);
		data_->view_projection_ = data_->projection_ * data_->view_ * model_matrix;
	}
	void Camera::moveUp(float value)
	{
		data_->position_ = glm::vec3(data_->position_[0], data_->position_[1] + value, data_->position_[2]);
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), data_->scale_), data_->rotation_angle_, data_->rotation_), data_->position_);
		data_->view_projection_ = data_->projection_ * data_->view_ * model_matrix;
	}


}

