#include "gameObject.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include <chrono>

//Testing
#include <time.h>
///


namespace Utlop
{
  GameObject::GameObject()
  {
		_mesh = std::make_unique<Mesh>();
  }

  GameObject::~GameObject()
  {

  }

  void GameObject::init()
  {
    _mesh->init();
		_mesh->setColor(glm::vec3(0.0f,0.0f,1.0f));
		_mesh->setPosition(glm::vec3(0.0f, 0.4f, 0.1f));
		printf("GameObjectInit\n");
  }

  void GameObject::draw()
  {
    _mesh->draw();
  }

  void GameObject::start()
  {
		
  }

  void GameObject::update()
  {
    _mesh->update();
  }

  void GameObject::destroy()
  {

  }

  void GameObject::setTransform(Transform transform)
  {
    _transform = transform;
		
  }
	void GameObject::setScale(glm::vec3 scale)
	{
		//_mesh.setMaterialParameters(Utlop::U_)
	}
	void GameObject::setRotation(glm::vec3 rotation)
	{
	}
	void GameObject::setPosition(glm::vec3 position)
	{
		_transform.setPosition(position);
		_mesh->setPosition(position);

	}
	void GameObject::setColor(glm::vec3 color)
	{
		_mesh->setColor(color);
	}
	void GameObject::translate(glm::vec3 position, float speed)
	{
		_transform.setPosition(_transform.getPosition() + (position * speed));

	}
}