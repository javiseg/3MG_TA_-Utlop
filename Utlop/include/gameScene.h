#ifndef UTLOP_GAMESCENE
#define UTLOP_GAMESCENE 1
#pragma once

#include "gameObject.h"
#include "geometry.h"

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

			std::vector<Utlop::GameObject> gameObjects_;
    protected:
			

    private:
      static GameScene* _current_scene;
      static std::vector<GameScene*> _scenes;
			std::unique_ptr<std::vector<Utlop::Geometry>> geometryData_;

      friend class Core;

      void _start();
      void _update();
  };


}

#endif // !UTLOP_GAMESCENE
