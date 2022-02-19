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
			std::shared_ptr<Utlop::Geometry> CreateGeometry();
			std::vector<std::shared_ptr<Utlop::Geometry>> getGeometries();
			std::vector<Utlop::GameObject> gameObjects_;
    protected:
			

    private:
      static GameScene* _current_scene;
      static std::vector<GameScene*> _scenes;
			std::vector<std::shared_ptr<Utlop::Geometry>> geometryData_;
			std::vector<std::shared_ptr<Utlop::Texture>> textureData_;

      friend class Core;

      void _start();
      void _update();
  };


}

#endif // !UTLOP_GAMESCENE
