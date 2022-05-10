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
#include "utility_cubemap.h"

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
			vector<Texture> InitMaterials(RenderCtx* data, vector<string> texturePaths);
			Geometry InitGeometry(RenderCtx* data, const char* path);
			void ChangeMesh(Entity& entity, RenderCtx* data, int option);

			void InitMesh(string geometryPath, vector<string> texturePath);

			void MoveCamera();

			void InitComponents();

			void InitSystems();
			void PreExecSystems();
			void PreExecSystem(Entity& entity);
			void ExecSystems();
			void ExecSystems2();

      static Core* Instance();
			RenderCtx* getData();
			DisplayList* getDisplayList();
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
			unique_ptr<CubeMap> cubemap;

	};


}

#endif // !UTLOP_WINDOW
