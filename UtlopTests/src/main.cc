//#include "GLFW/glfw3.h"
#include "window.h"
#include "core.h"
#include "matrixThreadScene.h"

int main()
{
	Utlop::Core core;
	core.init(1000.0f);
	core.getWindow()->init(1380, 780, "Utlop");
	MatrixThreadScene scene;
	scene.init();
	core.start();

  return 0;
}