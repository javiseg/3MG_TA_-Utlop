#ifndef UTLOP_CORE
#define UTLOP_CORE 1
#pragma once
#include "window.h"

#include <list>
#include <vector>
#include "data.h"

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
			float getDeltaTime();

			void AddEntity(bool camera);
			void AddCameraEntity();

			void InitComponents();

			void InitSystems();
			void ExecSystems();

      static Core* Instance();
			
    protected:

    private:
      static Core* _instance;
      Utlop::Window _window;
      float _fps;
      long _frame_time_millis;
			float deltaTime_;

			RenderCtx* data;


  };


}

#endif // !UTLOP_WINDOW
