#include "window.h"
#include "GLFW/glfw3.h"

namespace Utlop
{
  Window::Window()
  {
    _window = nullptr;
  }

  Window::~Window()
  {
    
  }

  bool Window::isValid()
  {
    return _window != nullptr;
  }

  void Window::init(int width, int height, char* title)
  {
    _window = glfwCreateWindow(width, height, title, NULL, NULL);
		this->width = width;
		this->height = height;
		_window = _window;

  }

	GLFWwindow* Window::getWindow()
	{
		return _window;
	}

}