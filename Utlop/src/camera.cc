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
		glm::vec3 scaling(1.0f, 1.0f, 1.0f);
		glm::vec3 translation(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation_axis(0.0f, 1.0f, 0.0f);
		float rotation_angle = 0.0f;
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);

		data_->projection_ = glm::perspective(1.57f, 4.0f / 3.0f, 0.1f, 100.0f);
		data_->view_ = glm::lookAt(glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f));

		data_->view_projection_ = data_->projection_ * data_->view_ * model_matrix;

		//std::cout << glm::to_string(data_->projection_) << std::endl;

	}
	void keyPressed(unsigned char key, int x, int y) {
	}
	void Camera::update()
	{
	}

}

