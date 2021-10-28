#ifndef Utlop_GAMESCENE
#define Utlop_GAMESCENE 1

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
      void update();

    protected:
      std::vector<GameObject> _gameObjects;

    private:

  };


}

#endif // !Utlop_GAMESCENE
