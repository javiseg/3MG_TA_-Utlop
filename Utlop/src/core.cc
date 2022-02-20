#include "core.h"

#include <chrono>
#include <thread>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "..\include\core.h"
#include <time.h>


namespace Utlop
{
  Core* Core::_instance = nullptr;

  Core::Core()
  {
    _fps = 0.0f;
    _frame_time_millis = 0;
    _instance = this;
		camera_ = std::make_unique<Camera>();
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
		//camera_.alloc();
		camera_->init();
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

      Utlop::GameScene::current_scene_->_start();
			camera_->update();
			glEnable(GL_DEPTH_TEST);
      glClearColor(160.0f/255.0f, 160.0f / 255.0f, 160.0f / 255.0f, 1.0f);
			GLint version_max, version_min;
			glGetIntegerv(GL_MAJOR_VERSION, &version_max);
			glGetIntegerv(GL_MINOR_VERSION, &version_min);
			printf("Version: %d.%d \n", version_max, version_min);

			float lastFrame = glfwGetTime();

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				deltaTime_ = (float)glfwGetTime() - lastFrame;
				lastFrame = (float)glfwGetTime();
				//double xpos, ypos;
				//glfwGetCursorPos(_window.getWindow(), &xpos, &ypos);
				//printf("Cursor Position at (%f, %f )\n",xpos, ypos);
				//Utlop::GameScene::current_scene_->_gameObjects[0].setPosition(glm::vec3(xpos, ypos, 0.0f));
				camera_->update();
				Utlop::GameScene::current_scene_->_update();

        Utlop::GameScene::current_scene_->draw();

        glfwSwapBuffers(_window._window);

				/*printf("Objects: %d\n", Utlop::GameScene::current_scene_->_gameObjects.size());*/

        glfwPollEvents();

        std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
        std::this_thread::sleep_until(start_time + std::chrono::milliseconds(_frame_time_millis));
				
				//long current_time = (long)(end_time.time_since_epoch().count() - start_time.time_since_epoch().count());
				//printf("%f\n", deltaTime_);
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

	Utlop::Camera* Core::getCamera()
	{
		return camera_.get();
	}

	float Core::getDeltaTime()
	{
		return deltaTime_;
	}


  Core* Core::Instance()
  {
    return _instance;
  }
}