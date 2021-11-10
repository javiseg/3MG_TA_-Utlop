//#include "GLFW/glfw3.h"
#include "window.h"
#include "core.h"
#include <vector>
/*
struct Command {
	explicit Command(float f) : f(f) {}
	Command(Command&&) = default;
	Command& operator=(Command&&) = delete;
	float f;
};

void test() {
	std::vector<Command> vector;
	Command cmd(5.0f);

	vector.push_back(std::move(cmd));
}
*/





int main()
{
	Utlop::Core core;
	core.init(1000.0f);
	core.getWindow()->init(1920, 1080, "Test Window");
	core.start();
	core.stop();

  return 0;
}

