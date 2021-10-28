#ifndef Utlop_CORE
#define Utlop_CORE 1

#include "window.h"

#include <list>
#include <vector>

#include "gameScene.h"

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

    protected:

    private:

      Utlop::Window _window;
      float _fps;
      long _frame_time_millis;
      std::vector<GameScene> _scenes;
  };


}

#endif // !Utlop_WINDOW
