//#include "GLFW/glfw3.h"
#include "window.h"
#include "core.h"
#include <vector>


int main()
{
	Utlop::Core core;
	core.init(1000.0f);
	core.getWindow()->init(820, 680, "Test Window");
	core.start();
	core.stop();

  return 0;
}

