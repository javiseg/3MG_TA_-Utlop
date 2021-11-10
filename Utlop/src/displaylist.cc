#include "displaylist.h"


/*
//Binding point 2

struct PerFrameData {
	float viewProjection[16];
};

//Binding point 1

struct PerModelData {
	float model[16];
};

//Binding point 0

struct PerApplicationData {
	int width;
	int height;
};

Luego en GPUState se añade una variable de cada struct
*/

struct GPUState {
	int program;
};

void Utlop::DisplayList::DrawGeometry::executeOnGPU(GPUState* gpu) {

}

Utlop::DisplayList::Command::~Command()
{
}
