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
    _window = _window;
  }

}