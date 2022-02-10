#pragma once
#include <memory>
#include <stdlib.h>
#include "glm/glm.hpp"

namespace Utlop {

	enum Geo {
		kConst_Triangle,
		kConst_Cube
	};

	
	const float TriangleArray[9] = {
		0.2f, -0.5f, 0.0f, 
		-0.2f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};
	const float CubeArray[24] = {
		1,1,1,   1,1,-1,   1,-1,-1,   1,-1,1,
		-1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1
	};
	const float ColorCubeArray[24] = {
		1,1,1,   1,0,0,   1,1,0,   0,1,0,
	  0,0,1,   1,0,1,   0,0,0,   0,1,1
	};

}

