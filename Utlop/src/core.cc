#include "core.h"

#include <chrono>
#include <thread>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Utlop
{
  Core::Core()
  {
    _fps = 0.0f;
    _frame_time_millis = 0;
  }

  Core::~Core()
  {
    stop();
  }

  bool Core::init(float fps)
  {
    _fps = fps;
    _frame_time_millis = 1000 / (long)_fps;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return glfwInit();
  }

  void Core::start()
  {
    if (_window.isValid())
    {
      glfwMakeContextCurrent(_window._window);

      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
        printf("Failed to initialize GLAD");
      }

      Utlop::GameScene scene;
      scene.init();
      //_scenes.push_front(scene);
      _scenes.push_back(scene);

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);

        glClear(GL_COLOR_BUFFER_BIT);

        _scenes[0].draw();

        glfwSwapBuffers(_window._window);

        _scenes[0].update();

        glfwPollEvents();

        std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
        std::this_thread::sleep_until(start_time + std::chrono::milliseconds(_frame_time_millis));

        long current_time = (long)(end_time.time_since_epoch().count() - start_time.time_since_epoch().count());
        printf("Desired FPS: %f - Current FPS: %f\n", _fps, current_time / 1000.0f);
      }
    }
  }

  void Core::stop()
  {
    glfwTerminate();
  }

  Utlop::Window* Core::getWindow()
  {
    return &_window;
  }
}