#include "geometry.h"
#include <iostream>
#include "gameScene.h"

float normalized(float value, float max, float min) {
	return (value - min) / (max - min);
}

bool loadOBJ(const char* path,
	std::vector <float>& out_vertices,
	std::vector < glm::vec2 >& out_uvs,
	std::vector < glm::vec3 >& out_normals,
	std::vector <unsigned int>& out_indices) {

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< float > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector< glm::vec3 > temp_indices;

	float max = 1.0f, min = -1.0f;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	while (1) {

		char lineHeader[128];
		// Lee la primera palabra de la línea
		int res = fscanf(file, "%s\n", lineHeader);
		if (res == EOF)
			break;

		//printf("%s\n", lineHeader);
		if (strcmp(&lineHeader[0], "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x > max || vertex.y > max || vertex.z > max) {
				max = std::max(vertex.x, vertex.y);
				max = std::max(max, vertex.z);
			}
			if (vertex.x < min || vertex.y < min || vertex.z < min) {
				min = std::min(vertex.x, vertex.y);
				min = std::min(min, vertex.z);
			}

			temp_vertices.push_back(vertex.x);
			temp_vertices.push_back(vertex.y);
			temp_vertices.push_back(vertex.z);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			out_uvs.push_back(uv);

		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);




			out_indices.push_back(vertexIndex[0] - 1);
			out_indices.push_back(vertexIndex[1] - 1);
			out_indices.push_back(vertexIndex[2] - 1);

		}
	}
	for (int i = 0; i < temp_vertices.size(); i++) {
		out_vertices.push_back(normalized(temp_vertices[i], max, min));
	}

	return true;
}


void Utlop::Geometry::newGeometryData(Geo type)
{
	type_ = type;
	switch (type_) {
		case kConst_Triangle: {
			vertices_.insert(vertices_.begin(), begin(TriangleArray), end(TriangleArray));
			indices_.insert(indices_.begin(), begin(TriangleArrayIndices), end(TriangleArrayIndices));
		}break;
		case kConst_Cube: {
			vertices_.insert(vertices_.begin(), begin(CubeArray), end(CubeArray));
			indices_.insert(indices_.begin(), begin(CubeArrayIndices), end(CubeArrayIndices));
		}break;
	}
	
}

void Utlop::Geometry::newGeometryData(char* src)
{
	type_ = Geo::kConst_OBJ;

	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	bool res = loadOBJ(src, vertices_, uvs, normals, indices_);
	printf("");
}
