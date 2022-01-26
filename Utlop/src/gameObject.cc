#include "gameObject.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include <chrono>

namespace Utlop
{
  GameObject::GameObject()
  {

  }

  GameObject::~GameObject()
  {

  }

  void GameObject::init()
  {
    _mesh.init();
  }

  void GameObject::draw()
  {
    _mesh.draw();
  }

  void GameObject::start()
  {

  }

  void GameObject::update()
  {
    _mesh.update();
  }

  void GameObject::destroy()
  {

  }

  void GameObject::setTransform(Transform transform)
  {
    _transform = transform;
  }
	void GameObject::setColor(float* color)
	{
		_mesh.setColor(color);
	}
}