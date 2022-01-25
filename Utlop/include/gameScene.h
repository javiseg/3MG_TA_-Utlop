#ifndef UTLOP_GAMESCENE
#define UTLOP_GAMESCENE 1

#include "gameObject.h"

#include <vector>

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

    protected:
      std::vector<GameObject> _gameObjects;

    private:
      static GameScene* _current_scene;
      static std::vector<GameScene*> _scenes;
      friend class Core;

      void _start();
      void _update();
  };


}

#endif // !UTLOP_GAMESCENE
