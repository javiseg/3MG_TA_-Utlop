#ifndef UTLOP_CORE
#define UTLOP_CORE 1
#pragma once
#include "window.h"

#include <list>
#include <vector>
#include "data.h"
#include "displaylist.h"
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

			int AddEntity();
			void createEntities(Core* cr);
			void AddComponent(Entity& entity, Utlop::ComponentID id);
			void AddCubeMap();
			void InitMaterials(RenderCtx* data, const char* path);
			void InitGeometry(RenderCtx* data, const char* path);
			void InitShaders(Entity& entity, RenderCtx* data, const char* path);

			void MoveCamera();

			void InitComponents();

			void InitSystems();
			void PreExecSystems();
			void PreExecSystem(Entity& entity);
			void ExecSystems();
			void ExecSystems2();

      static Core* Instance();
			RenderCtx* getData();
			float getCameraSpeed();
			

			void ImGUI();
			void InitImGUI();
			void DestroyImGUI();

			 
			bool polygon_;
			vec4 bg_color_;
    protected:

    private:
      static Core* _instance;
      Utlop::Window _window;
      float _fps;
      long _frame_time_millis;
			float deltaTime_;
			float camera_speed_;
			uint8 preExecDone_;

			RenderCtx* data;
			DisplayList* displayList;

	};


}

#endif // !UTLOP_WINDOW
