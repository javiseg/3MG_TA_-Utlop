#include "material.h"
#include <fstream>
#include "..\include\transform.h"

namespace Utlop
{
  Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
  {
    position_ = position;
    scale_ = scale;
    rotation_ = rotation;
  }

  Transform::~Transform()
  {

  }

  void Transform::setPosition(glm::vec3 position)
  {
    position_ = position;
  }

  void Transform::setScale(glm::vec3 scale)
  {
		scale_ = scale;
  }

  void Transform::setRotation(glm::vec3 rotation)
  {
		rotation_ = rotation;
  }

  void Transform::translate(glm::vec3 translation)
  {
		position_ += translation;
  }

  void Transform::translate(glm::vec3 translation, float speed)
  {
		position_ += (translation * speed);
  }

  void Transform::scale(glm::vec3 scale)
  {
		scale_ *= scale;
  }

  void Transform::rotate(glm::vec3 rotation)
  {
		rotation_ += rotation;
  }
  void Transform::rotation_angle(float rot_ang)
  {
		rotation_angle_ = rot_ang;
  }
  glm::vec3 Transform::getPosition()
  {
		return position_;
  }
  glm::vec3 Transform::getScale()
  {
		return scale_;
  }
  glm::vec3 Transform::getRotation()
  {
		return rotation_;
  }
	float Transform::getRotationAngle()
	{
		return rotation_angle_;
	}
}