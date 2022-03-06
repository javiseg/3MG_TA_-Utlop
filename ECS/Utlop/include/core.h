#ifndef UTLOP_CORE
#define UTLOP_CORE 1
#pragma once
#include "window.h"

#include <list>
#include <vector>
#include "data.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Utlop
{

  class Core
  {
    public:
      Core();
      ~Core();

      bool init(float fps);
      void start();

      Utlop::Window* getWindow();
			float getDeltaTime();

			void AddEntity(bool camera);
			
			void MoveCamera();

			void InitComponents();

			void InitSystems();
			void ExecSystems();

      static Core* Instance();
			RenderCtx* getData();
			float getCameraSpeed();
			

			void ImGUI();
			void InitImGUI();
			void DestroyImGUI();

			 
			bool polygon_;
			vec3 bg_color_;
    protected:

    private:
      static Core* _instance;
      Utlop::Window _window;
      float _fps;
      long _frame_time_millis;
			float deltaTime_;
			float camera_speed_;


			RenderCtx* data;
	};


}

#endif // !UTLOP_WINDOW