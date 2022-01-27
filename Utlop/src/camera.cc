#include "camera.h"

void Utlop::camera::init()
{
	using glm::vec3;
	using glm::mat4;

	vec3 scaling(1.0f, 1.0f, 1.0f);
	vec3 translation(-0.5f, 0, 0);
	vec3 rotation_axis(0, 1.0f, 0);
	float rotation_angle = 0;
	mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
		mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);

	mat4 model_view_projection = model_matrix;
}
