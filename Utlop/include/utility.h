#pragma once
#include "mesh.h"
#include <map>
#include "glm/gtx/hash.hpp"

namespace std
{
	template <>
	struct hash<Utlop::Vertex>
	{
		size_t operator()(const Utlop::Vertex& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.positions) ^ (hash<glm::vec2>()(vertex.texCoords) << 1)) >> 1);
		}
	};
}


