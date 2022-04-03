#pragma once
#include "command.h"
#include <list>
#include <memory>

namespace Utlop {

	using namespace std;

	void callback_WindowClearCmd(WindowClearCmd cmd);

	class DisplayList {
		list<shared_ptr<Command>> cmdList;

		void addWindowClearCmd(float r, float g, float b, float a);
		void addInitMaterialCmd(float r, float g, float b, float a);
	};

}