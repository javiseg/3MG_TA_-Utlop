#include "material.h"
#include <fstream>
#include "..\include\transform.h"

namespace Utlop
{
  Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation)
  {
    _position = position;
    _scale = scale;
    _rotation = rotation;
  }

  Transform::~Transform()
  {

  }

  void Transform::setPosition(glm::vec3 position)
  {
    _position = position;
  }

  void Transform::setScale(glm::vec3 scale)
  {
    _scale = scale;
  }

  void Transform::setRotation(glm::vec4 rotation)
  {
    _rotation = rotation;
  }

  void Transform::translate(glm::vec3 translation)
  {
    _position += translation;
  }

  void Transform::translate(glm::vec3 translation, float speed)
  {
    _position += (translation * speed);
  }

  void Transform::scale(glm::vec3 scale)
  {
    _scale *= scale;
  }

  void Transform::rotate(glm::vec4 rotation)
  {
    _rotation += rotation;
  }
  glm::vec3 Transform::getPosition()
  {
		return _position;
  }
  glm::vec3 Transform::getScale()
  {
		return _scale;
  }
  glm::vec3 Transform::getRotation()
  {
		return _rotation;
  }
}