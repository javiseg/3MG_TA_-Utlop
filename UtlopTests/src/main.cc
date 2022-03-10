//#include "GLFW/glfw3.h"
#include "core.h"

int main()
{
	Utlop::Core core;
	core.init(1000.0f);
	core.getWindow()->init(1380, 780, "Utlop");
	core.start();

  return 0;
}