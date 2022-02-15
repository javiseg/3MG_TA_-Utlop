#ifndef UTLOP_CORE
#define UTLOP_CORE 1
#pragma once
#include "window.h"

#include "includes.h"
#include <list>
#include <vector>
#include "TaskScheduler.h"
#include "gameScene.h"

namespace Utlop
{

  class Core
  {
    public:
      Core();
      ~Core();

      bool init(float fps);
      void start();
      void stop();

      Utlop::Window* getWindow();
      enki::TaskScheduler* getScheduler();
			Utlop::Camera* getCamera();
			float getDeltaTime();


      static Core* Instance();
			
    protected:

    private:
      static Core* _instance;
      Utlop::Window _window;
      float _fps;
      long _frame_time_millis;
			float deltaTime_;
      enki::TaskScheduler _scheduler;
			std::unique_ptr<Camera> camera_;
  };


}

#endif // !UTLOP_WINDOW
