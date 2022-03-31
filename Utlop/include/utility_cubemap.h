#pragma once

#include "bitmap.h"
namespace Utlop {

	Bitmap convertEquirectangularMapToVerticalCross(const Bitmap& b);
	Bitmap convertVerticalCrossToCubeMapFaces(const Bitmap& b);
}