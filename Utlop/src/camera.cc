#include "camera.h"


namespace Utlop {

	

	Camera::Camera()
	{
		data_ = (CameraData*) malloc(sizeof(CameraData));
	}

	Camera::~Camera()
	{
	}

	void Camera::init()
	{
		data_->Right = Vector3(1.0f, 0.0f, 0.0f);
	}

	void Camera::update()
	{
		printf("Vector: %f, %f, %f\n", data_->Right.x, data_->Right.y, data_->Right.z);
	}

}

