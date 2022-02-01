#ifndef UTLOP_CORE
#define UTLOP_CORE 1

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
			Utlop::Camera getCamera();

      static Core* Instance();

    protected:

    private:
      static Core* _instance;
      Utlop::Window _window;
      float _fps;
      long _frame_time_millis;
      enki::TaskScheduler _scheduler;
			Camera camera_;
  };


}

#endif // !UTLOP_WINDOW
