#pragma once
#include <string>

namespace Utlop {

	struct Geometry {
		vector<float> vertices_;
		vector<float> texCoords_;
		vector<float> normals_;
		vector<GLuint> verticesIndices_;
		vector<GLuint> texCoordsIndices_;
		vector<GLuint> normalsIndices_;
		string path;
	};

}