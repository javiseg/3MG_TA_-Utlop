#include "core.h"

#include <chrono>
#include <thread>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Utlop
{
  Core* Core::_instance = nullptr;

  Core::Core()
  {
    _fps = 0.0f;
    _frame_time_millis = 0;
    _instance = this;
  }

  Core::~Core()
  {
    stop();
  }

  bool Core::init(float fps)
  {
    _fps = fps;
    _frame_time_millis = 1000 / (long)_fps;

    _scheduler.Initialize();

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

      Utlop::GameScene::_current_scene->_start();

      glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);

        glClear(GL_COLOR_BUFFER_BIT);

        Utlop::GameScene::_current_scene->draw();

        glfwSwapBuffers(_window._window);

        Utlop::GameScene::_current_scene->_update();

        glfwPollEvents();

        std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
        std::this_thread::sleep_until(start_time + std::chrono::milliseconds(_frame_time_millis));

        long current_time = (long)(end_time.time_since_epoch().count() - start_time.time_since_epoch().count());

#ifdef DEBUG
        printf("Desired FPS: %f - Current FPS: %f\n", _fps, current_time / 1000.0f);
#endif // DEBUG
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

  enki::TaskScheduler* Core::getScheduler()
  {
    return &_scheduler;
  }

  Core* Core::Instance()
  {
    return _instance;
  }
}