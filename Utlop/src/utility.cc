#include "utility.h"
#include "tiny_obj_loader.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "stb_image.h"

float normalized(float value, float max) {
	return  (value / max);
}
bool loadOBJ2(const char* path, Utlop::Geometry& geo) {


	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warning;
	std::string error;

	if (!tinyobj::LoadObj(
		&attributes,
		&shapes,
		&materials,
		&warning,
		&error,
		path))
	{
		throw std::runtime_error("ast::assets::loadOBJFile: Error: " + warning + error);
	}

	std::vector<Utlop::Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Utlop::Vertex> texVertices;
	std::vector<uint32_t> texIndices;
	std::unordered_map<Utlop::Vertex, uint32_t> uniqueVertices;

	// Loop through all the shapes that there found.
	for (const auto& shape : shapes)
	{
		// For each shape, loop through its indices.
		for (const auto& index : shape.mesh.indices)
		{
			// Construct a new (x, y, z) position for the current mesh index.
			glm::vec3 position{
					attributes.vertices[(int)3 * index.vertex_index + 0],
					attributes.vertices[(int)3 * index.vertex_index + 1],
					attributes.vertices[(int)3 * index.vertex_index + 2] };

			// Construct a new (u, v) texture coordinate for the current mesh index.
			glm::vec2 texCoord{
					attributes.texcoords[(int)2 * index.texcoord_index + 0],
					1.0f - attributes.texcoords[(int)2 * index.texcoord_index + 1] };

			glm::vec3 normals{
					attributes.normals[(int)3 * index.normal_index + 0],
					attributes.normals[(int)3 * index.normal_index + 1],
					attributes.normals[(int)3 * index.normal_index + 2] };


			// Construct a vertex with the extracted data.
			Utlop::Vertex vertex{ position, texCoord, normals };

			// This will help deduplicate vertices - we maintain a hash map where a
			// vertex is used as a unique key with its value being which index can
			// be used to locate the vertex. The vertex is only added if it has not
			// been added before.
			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			geo.totalIndices_.push_back(uniqueVertices[vertex]);
		}
	}

	for (const auto& vertex : vertices)
	{
		// Position
		geo.totalVertex_.push_back(vertex.positions.x);
		geo.totalVertex_.push_back(vertex.positions.y);
		geo.totalVertex_.push_back(vertex.positions.z);

		// Texture coordinate
		geo.totalVertex_.push_back(vertex.texCoords.x);
		geo.totalVertex_.push_back(vertex.texCoords.y);

		geo.totalVertex_.push_back(vertex.normals.x);
		geo.totalVertex_.push_back(vertex.normals.y);
		geo.totalVertex_.push_back(vertex.normals.z);
	}




	geo.path = path;

	return true;
}
