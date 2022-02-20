#include "material.h"
#include <fstream>
#include "..\include\transform.h"

namespace Utlop
{
	Transform::Transform()
	{
		position_ = glm::vec3(0.0f, 0.0f, 0.0f);
		scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
		rotation_ = glm::vec3(0.0f, 1.0f, 0.0f);
		rotation_angle_ = 0.0f;
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
	Transform& Transform::operator=(const Transform& other)
	{
		position_ = other.position_;
		scale_ = other.scale_;
		rotation_ = other.rotation_;
		rotation_angle_ = other.rotation_angle_;

		return *this;
	}
	Transform::Transform(const Transform& other)
	{
		position_ = other.position_;
		scale_ = other.scale_;
		rotation_ = other.rotation_;
		rotation_angle_ = other.rotation_angle_;
	}
	Transform::Transform(Transform&& other)
	{
		position_ = other.position_;
		scale_ = other.scale_;
		rotation_ = other.rotation_;
		rotation_angle_ = other.rotation_angle_;
	}
}