#ifndef UTLOP_GAMESCENE
#define UTLOP_GAMESCENE 1
#pragma once

#include "gameObject.h"
#include "texture.h"
#include "mesh.h"
#include "imgui.h"

namespace Utlop
{

  class GameScene
  {
    public:
      GameScene();
      ~GameScene();

      void init();
      void draw();
      virtual void start();
      virtual void update();
			void destroy();
			static GameScene* getCurrentScene();
			void addGameObject(Utlop::GameObject gO);
			void addDefaultGameObject();

			int CreateMesh(Geo type);
			int CreateMesh(Geo type, char* src);
			int getMeshIndexByType(Geo type);
			int getMeshIndexByType(Geo type, const char* src);
			std::vector<std::shared_ptr<Utlop::Mesh>> getMeshes();

			void ImGUI();
			void InitImGUI();
			void DestroyImGUI();

			std::vector<std::shared_ptr<Utlop::GameObject>> gameObjects_;
			bool polygon_;
    protected:
			

    private:
      static GameScene* current_scene_;
      static std::vector<GameScene*> scenes_;
			std::vector<std::shared_ptr<Utlop::Texture>> textureData_;
			std::vector<std::shared_ptr<Utlop::Mesh>> meshes_;

      friend class Core;

			
      void _start();
      void _update();
  };


}

#endif // !UTLOP_GAMESCENE
