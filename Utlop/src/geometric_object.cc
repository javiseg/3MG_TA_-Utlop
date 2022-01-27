#include "geometric_object.h"
#include <stdlib.h>
#include <stdio.h>

void Utlop::geometric_object::init(Utlop::Geo_Type type)
{
	g_type = type;
	switch (type) {
		case GEO_CUBE:
			vertices = (float*)calloc(72, sizeof(float));
				float positions[] = {
				-0.5f, 0.5f, 0.5f,        // 0
				-0.5f, -0.5f, 0.5f,        // 1
				0.5f, -0.5f, 0.5f,        // 2
				0.5f, 0.5f, 0.5f,         // 3

				0.5f, 0.5f, 0.5f,          // 4
				0.5f, -0.5f, 0.5f,        // 5
				0.5f, -0.5f, -0.5f,        // 6
				0.5f, 0.5f, -0.5f,    // 7

				-0.5f, 0.5f, -0.5f,        // 8
				-0.5f, 0.5f, 0.5f,        // 9
				0.5f, 0.5f, 0.5f,            // 10
				0.5f, 0.5f, -0.5f,        // 11

				-0.5f, 0.5f, -0.5f,        // 12
				-0.5f, -0.5f, -0.5f,    // 13
				0.5f, -0.5f, -0.5f,        // 14
				0.5f, 0.5f, -0.5f,         // 15

				-0.5f, 0.5f, 0.5f,      // 16
				-0.5f, -0.5f, 0.5f,        // 17
				-0.5f, -0.5f, -0.5f,    // 18
				-0.5f, 0.5f, -0.5f,   // 19

				-0.5f, -0.5f, -0.5f,    // 20
				-0.5f, -0.5f, 0.5f,        // 21
				0.5f, -0.5f, 0.5f,        // 22
				0.5f, -0.5f, -0.5f,        // 23
				};
				vertices = positions;
			break;
	}
}

void Utlop::geometric_object::showVertices()
{
	/*for (int i = 0; i < 72; i++) {
		printf(" %f \n", vertices[i]);
	}*/
}
