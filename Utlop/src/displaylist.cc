#include "displaylist.h"

void Utlop::DisplayList::addWindowClearCmd(float r, float g, float b, float a)
{
	shared_ptr<WindowClearCmd> window_clear_cmd;
	window_clear_cmd = make_shared<WindowClearCmd>();
	window_clear_cmd->r = r;
	window_clear_cmd->g = g;
	window_clear_cmd->b = b;
	window_clear_cmd->a = a;

	//Add the callback function


	cmdList.push_back(window_clear_cmd);

}

void Utlop::DisplayList::addInitMaterialCmd(float r, float g, float b, float a)
{
}

void Utlop::callback_WindowClearCmd(WindowClearCmd cmd)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(cmd.r, cmd.g, cmd.b, cmd.a);

}
