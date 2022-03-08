//#include "GLFW/glfw3.h"
#include "core.h"
#include "UGameScene.h"

int main()
{
	Utlop::Core core;
	Utlop::UGameScene scene;
	core.init(1000.0f);
	core.getWindow()->init(1380, 780, "Utlop");
	core.start();

  return 0;
}