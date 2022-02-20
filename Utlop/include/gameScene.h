#ifndef UTLOP_GAMESCENE
#define UTLOP_GAMESCENE 1
#pragma once

#include "gameObject.h"
#include "geometry.h"
#include "texture.h"

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
			static GameScene* getCurrentScene();
			unsigned int CreateGeometry();
			std::vector<std::shared_ptr<Utlop::Geometry>> getGeometries();
			std::shared_ptr<Utlop::Geometry> getGeometryByID(unsigned int id);
			int getGeometryID(unsigned int id);
			int getGeometryByType(Geo type);
			
			void addGameObject(Utlop::GameObject gO);

			std::vector<std::shared_ptr<Utlop::GameObject>> gameObjects_;
    protected:
			

    private:
      static GameScene* current_scene_;
      static std::vector<GameScene*> scenes_;
			std::vector<std::shared_ptr<Utlop::Geometry>> geometryData_;
			std::vector<std::shared_ptr<Utlop::Texture>> textureData_;

      friend class Core;

      void _start();
      void _update();
  };


}

#endif // !UTLOP_GAMESCENE
