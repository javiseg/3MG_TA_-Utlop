#include "displaylist.h"

void Utlop::DisplayList::executeOnGPU(list<shared_ptr<Command>> cmdListToExecute)
{
	for (auto&& list : cmdListToExecute) {
		list->executeOnGPU();
	}
}

void Utlop::DisplayList::submit()
{
	executeOnGPU(move(cmdList));
}

Utlop::DisplayList& Utlop::addWindowClearCmd(DisplayList* dl, float r, float g, float b, float a)
{
	shared_ptr<WindowClearCmd> window_clear_cmd;
	window_clear_cmd = make_shared<WindowClearCmd>();
	window_clear_cmd->r = r;
	window_clear_cmd->g = g;
	window_clear_cmd->b = b;
	window_clear_cmd->a = a;

	//Add the callback function


	dl->cmdList.push_back(move(window_clear_cmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addInitMaterialCmd(DisplayList* dl, float r, float g, float b, float a)
{
	return *dl;
}

Utlop::DisplayList& Utlop::addDrawCmd(Utlop::DisplayList* dl, GLuint shaderId, GLuint materialID, GLuint vao, size_t size)
{
	shared_ptr<DrawMaterialCmd> draw_material_cmd;
	draw_material_cmd = make_shared<DrawMaterialCmd>();
	draw_material_cmd->shaderId = shaderId;
	draw_material_cmd->materialID = materialID;
	draw_material_cmd->vao = vao;
	draw_material_cmd->size = size;

	dl->cmdList.push_back(move(draw_material_cmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addSetPolygonCmd(Utlop::DisplayList* dl, uint on)
{
	shared_ptr<SetPolygonCmd> pol;
	pol = make_shared<SetPolygonCmd>();
	pol->value = on;

	dl->cmdList.push_back(move(pol));

	return *dl;
}

void Utlop::callback_WindowClearCmd(WindowClearCmd cmd)
{
	
}